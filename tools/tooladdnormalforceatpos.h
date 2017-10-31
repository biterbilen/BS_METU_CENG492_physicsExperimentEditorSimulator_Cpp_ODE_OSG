#ifndef _TOOL_ADD_NORMAL_FORCE_AT_POS_H
#define _TOOL_ADD_NORMAL_FORCE_AT_POS_H 1

#include "tooladdforce.h"
#include "sforceatpos.h"

class ToolAddNormalForceAtPos: public ToolAddForce {
public:
    ToolAddNormalForceAtPos(): ToolAddForce("add-normal-force-at-pos") {}
    virtual SForce *makeForce() { return new SForceAtPos(osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 0)); }
    virtual Tool* clone() { return new ToolAddNormalForceAtPos(); }
};

#endif

