#include "toolpan.h"
#include <editor.h>
#include <comtranslate.h>

void ToolPan::mouseMoved(float x, float y, CameraView * camView)
{
    x=-x+0.5;
    y=-y+0.5;
    
    target=osg::Vec3(2*x, 2*y, 0)*transform;
    osg::Vec3 displ=(target-source);
    camView->setCenter(displ[0],displ[1],displ[2]);
}

void ToolPan::mousePressed(float x, float y, CameraView * camView)
{
    x=-x+0.5;
    y=-y+0.5;
    
    osg::Matrix matrix = camView->getSceneView()->getViewMatrix();
    osg::Matrix matrix2 = camView->getSceneView()->getProjectionMatrix();
    transform = osg::Matrix::inverse(matrix2) * osg::Matrix::inverse(matrix);
    
    source = osg::Vec3(2*x, 2*y,0)*transform;
}



