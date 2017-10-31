#include "toolmove.h"
#include "editor.h"
#include "comtranslate.h"
#include "sosggeode.h"

#include <osg/LineSegment>
#include <osgUtil/IntersectVisitor>

void ToolMove::mouseMoved(float x, float y, CameraView *)
{
    if (mObj.valid())
    {
        target=osg::Vec3(2*(x-0.5),2*(y-0.5), -transform(3, 2)/transform(2,2))*transform;
        mObj->setPosition(original+target-source);
    }
}

void ToolMove::mousePressed(float x, float y, CameraView * camView)
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
                mObj = sogeode->getSObject();
                editor->selectNode(hitr->_geode.get(), hitr->_nodePath);
            }
        }
    }

    if(mObj.valid())
    {
        osg::Matrix matrix = camView->getSceneView()->getViewMatrix();
        osg::Matrix matrix2 = camView->getSceneView()->getProjectionMatrix();
        transform = osg::Matrix::inverse(matrix2) * osg::Matrix::inverse(matrix);

        source=osg::Vec3(2*(x-0.5),2*(y-0.5), -transform(3, 2)/transform(2,2))*transform;
        target=source;

        original=mObj->getPosition();
    }
}

void ToolMove::mouseReleased(float, float, CameraView *)
{
    /* Build a command and apply it! */
    if (mObj.valid())
    {
        mObj->setPosition(original);
        osg::ref_ptr<ComTranslate> cmd=new ComTranslate(mObj.get(), original+target-source);
        cmd->execute();
        source=target;
    }
}


