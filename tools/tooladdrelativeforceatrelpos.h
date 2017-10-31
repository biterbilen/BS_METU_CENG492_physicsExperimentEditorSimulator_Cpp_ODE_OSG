#ifndef _TOOL_ADD_RELATIVE_FORCE_AT_REL_POS_H
#define _TOOL_ADD_RELATIVE_FORCE_AT_REL_POS_H 1

#include "tooladdforce.h"
#include "srelforceatrelpos.h"

class ToolAddRelativeForceAtRelPos: public ToolAddForce {
public:
    ToolAddRelativeForceAtRelPos(): ToolAddForce("add-relative-force-at-rel-pos") {}
    virtual SForce *makeForce() { return new SRelForceAtRelPos(osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 0)); }
    virtual Tool* clone() { return new ToolAddRelativeForceAtRelPos(); }
};

#endif
