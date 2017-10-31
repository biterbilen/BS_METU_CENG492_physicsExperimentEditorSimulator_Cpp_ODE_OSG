#ifndef __TOOL_MAKE_INCLINED_PLANE_H__
#define __TOOL_MAKE_INCLINED_PLANE_H__

#include <osg/Vec3>
#include "toolmakeshape.h"
#include "sinclinedplane.h"

class ToolMakeInclinedPlane: public ToolMakeShape {
public:
    ToolMakeInclinedPlane(): ToolMakeShape("make-inclined-plane") {}
    virtual SObject *makeShape() { return new SInclinedPlane(osg::DegreesToRadians(30.0), 3, 3, getPos()); }
    virtual Tool* clone() { return new ToolMakeInclinedPlane(); }
};

#endif
