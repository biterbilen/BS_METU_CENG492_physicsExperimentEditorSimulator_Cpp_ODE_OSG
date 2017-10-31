#include "toolpick.h"
#include <osg/NodeVisitor>
#include <osgUtil/IntersectVisitor>

void ToolPick::mousePressed(float x, float y, CameraView * camView)
{
    mouse[0] = (x - 0.5f)*2;
    mouse[1] = (y - 0.5f)*2;

    osg::ref_ptr<osgUtil::SceneView> sceneView= camView->getSceneView();
    osg::Vec3 near_point,far_point;
    if (!sceneView->projectWindowXYIntoObject((int)mouse[0],(int)mouse[1],near_point,far_point))
    {
        return;
    }

    osg::Matrix matrix = sceneView->getViewMatrix();
    osg::Matrix matrix2 = sceneView->getProjectionMatrix();

    osg::Matrix inv = osg::Matrix::inverse(matrix*matrix2*osg::Matrix::translate(0,0,1)*osg::Matrix::scale(1,1,0.5f));
    near_point=osg::Vec3(mouse[0],mouse[1],0.0f)*inv;
    far_point=osg::Vec3(mouse[0],mouse[1],1.0f)*inv;
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
            editor->selectNode(hitr->_geode.get(), hitr->_nodePath);
            return;
        }
    }

}
