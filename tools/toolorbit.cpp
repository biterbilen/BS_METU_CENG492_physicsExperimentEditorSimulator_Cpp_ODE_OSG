#include "toolorbit.h"
#include <editor.h>
#include <comtranslate.h>

#define SPEED_ORBIT 5.0f

void ToolOrbit::mouseMoved(float x, float y, CameraView * camView)
{
    target=osg::Vec2(x,y);
    osg::Vec2 displ=target-source;

    camView->rotateCamera(-displ[1]*SPEED_ORBIT, -displ[0]*SPEED_ORBIT);

    source=osg::Vec2(x,y);
}

void ToolOrbit::mousePressed(float x, float y, CameraView *)
{
    source=osg::Vec2(x,y);
}

void ToolOrbit::mouseReleased(float, float, CameraView *)
{
}

