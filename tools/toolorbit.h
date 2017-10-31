#ifndef __TOOL_ORBIT_H__
#define __TOOL_ORBIT_H__

#include "drawingareatool.h"
#include <osg/MatrixTransform>

/** @class ToolOrbit
 * @brief Makes the camera orbit in a trackball-like manner using the mouse
 */
class ToolOrbit: public DrawingAreaTool {
    osg::Vec2 source;
    osg::Vec2 target;
public:
    ToolOrbit(): DrawingAreaTool("orbit") { }
    virtual void mouseMoved(float x, float y, CameraView * camView);
    virtual void mousePressed(float x, float y, CameraView * camView);
    virtual void mouseReleased(float x, float y, CameraView * camView);
    virtual Tool* clone() { return new ToolOrbit(); }


};

#endif
