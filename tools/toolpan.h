#ifndef __TOOL_PAN_H__
#define __TOOL_PAN_H__

#include "drawingareatool.h"
#include <osg/MatrixTransform>

/** @class ToolPan
 * @brief Moves the camera without rotating it
 */
class ToolPan: public DrawingAreaTool {
    osg::Vec3 source;
    osg::Vec3 target;
    osg::Matrix transform; // camera transformation matrix ;D
public:
    ToolPan(): DrawingAreaTool("pan") {}
    virtual void mouseMoved(float, float, CameraView *);
    virtual void mousePressed(float, float, CameraView *);
    virtual Tool* clone() { return new ToolPan(); }


};

#endif
