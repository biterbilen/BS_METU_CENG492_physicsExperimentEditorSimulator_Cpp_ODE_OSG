#ifndef _TOOL_ADD_NORMAL_FORCE_H
#define _TOOL_ADD_NORMAL_FORCE_H 1

#include "tooladdforce.h"
#include "snormalforce.h"

class ToolAddNormalForce: public ToolAddForce {
public:
    ToolAddNormalForce(): ToolAddForce("add-normal-force") {}
    virtual SForce *makeForce() { return new SNormalForce(osg::Vec3(0, 0, 1)); }
    virtual Tool* clone() { return new ToolAddNormalForce(); }
};

#endif

