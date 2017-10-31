#ifndef __TOOL_MOVE_H__
#define __TOOL_MOVE_H__

#include "drawingareatool.h"
#include "sobject.h"

class ToolMove: public DrawingAreaTool {
    osg::ref_ptr<SObject> mObj;
    osg::Vec2 mMouse;
    osg::Vec3 source;
    osg::Vec3 target;
    osg::Vec3 original;
    osg::Matrix transform; // camera transformation matrix ;D
public:
    ToolMove(): DrawingAreaTool("move") {}
    virtual void mouseMoved(float, float, CameraView *);
    virtual void mousePressed(float, float, CameraView *);
    virtual void mouseReleased(float, float, CameraView *);

    virtual Tool* clone() { return new ToolMove(); }
};

#endif
