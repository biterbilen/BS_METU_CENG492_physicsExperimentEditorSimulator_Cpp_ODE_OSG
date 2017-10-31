#ifndef _TOOL_ADD_RELATIVE_FORCE_AT_POS_H
#define _TOOL_ADD_RELATIVE_FORCE_AT_POS_H 1

#include "tooladdforce.h"
#include "srelforceatpos.h"

class ToolAddRelativeForceAtPos: public ToolAddForce {
public:
    ToolAddRelativeForceAtPos(): ToolAddForce("add-relative-force-at-pos") {}
    virtual SForce *makeForce() { return new SRelForceAtPos(osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 0)); }
    virtual Tool* clone() { return new ToolAddRelativeForceAtPos(); }
};

#endif
