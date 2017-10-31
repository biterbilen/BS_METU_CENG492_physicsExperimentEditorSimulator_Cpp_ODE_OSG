#ifndef _TOOL_ADD_RELATIVE_FORCE_H
#define _TOOL_ADD_RELATIVE_FORCE_H 1

#include "tooladdforce.h"
#include "srelforce.h"

class ToolAddRelativeForce: public ToolAddForce {
public:
    ToolAddRelativeForce(): ToolAddForce("add-relative-force") {}
    virtual SForce *makeForce() { return new SRelForce(osg::Vec3(0, 0, 1)); }
    virtual Tool* clone() { return new ToolAddRelativeForce(); }
};

#endif
