#ifndef _TOOL_ADD_NORMAL_FORCE_AT_REL_POS_H
#define _TOOL_ADD_NORMAL_FORCE_AT_REL_POS_H 1

#include "tooladdforce.h"
#include "sforceatrelpos.h"

class ToolAddNormalForceAtRelPos: public ToolAddForce {
public:
    ToolAddNormalForceAtRelPos(): ToolAddForce("add-normal-force-at-rel-pos") {}
    virtual SForce *makeForce() { return new SForceAtRelPos(osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 0)); }
    virtual Tool* clone() { return new ToolAddNormalForceAtRelPos(); }
};

#endif
