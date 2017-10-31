#include "tooladdforce.h"

#include <osg/LineSegment>
#include <osgUtil/IntersectVisitor>

#include <qmessagebox.h>

void ToolAddForce::mousePressed(float x, float y, CameraView * camView)
{
    mMouse[0] = (x - 0.5f)*2;
    mMouse[1] = (y - 0.5f)*2;

    osg::ref_ptr<osgUtil::SceneView> sceneView= camView->getSceneView();
    osg::Vec3 near_point,far_point;
    if (!sceneView->projectWindowXYIntoObject((int)mMouse[0],(int)mMouse[1],near_point,far_point))
    {
        return;
    }

    osg::Matrix matrix = sceneView->getViewMatrix();
    osg::Matrix matrix2 = sceneView->getProjectionMatrix();

    osg::Matrix inv = osg::Matrix::inverse(matrix*matrix2*osg::Matrix::translate(0,0,1)*osg::Matrix::scale(1,1,0.5f));
    near_point=osg::Vec3(mMouse[0],mMouse[1],0.0f)*inv;
    far_point=osg::Vec3(mMouse[0],mMouse[1],1.0f)*inv;
    osg::ref_ptr<osg::LineSegment> lineSegment = new osg::LineSegment;
    lineSegment->set(near_point,far_point);

    osgUtil::IntersectVisitor iv;
    iv.addLineSegment(lineSegment.get());

    editor->getSceneGraph()->getScene()->accept(iv);
    if (iv.hits())
    {
        osgUtil::IntersectVisitor::HitList& hitList = iv.getHitList(lineSegment.get());
        osgUtil::IntersectVisitor::HitList::iterator hitr;

        hitr=hitList.begin();
        osg::Geode* geode = hitr->_geode.get();

        if (geode)
        {
            SOsgGeode * sogeode = dynamic_cast<SOsgGeode*>(geode);
            if (sogeode != 0)
            {
                /** select the object in the editor */
                mObj = sogeode->getSObject();
                
                /** this one selects the object that the force will be added to */
                // editor->selectNode(hitr->_geode.get(), hitr->_nodePath);
                
                if(!mObj->canAddForce())
                {
                        QMessageBox::warning(0, "3DPhySims", "Cannot add force to this object!\n\n", "Ok");
                        return;
                }
                
                /** add the force */
                SForce * force = makeForce();
        
                osg::ref_ptr<ComAddForce> cmd=new ComAddForce(mObj.get(), force, editor.get());
                cmd->execute();
            }
        }
    }
}


