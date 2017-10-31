#ifndef __TOOL_PICK_H__
#define __TOOL_PICK_H__

#include "drawingareatool.h"
#include <osg/MatrixTransform>


class ToolPick: public DrawingAreaTool {
    osg::Vec2 mouse;
public:
    ToolPick(): DrawingAreaTool("pick") { }
    virtual void mousePressed(float x, float y, CameraView * camView);
    virtual Tool* clone() { return new ToolPick(); }
};

#endif
