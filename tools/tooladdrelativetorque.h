#ifndef _TOOL_ADD_RELATIVE_TORQUE_H
#define _TOOL_ADD_RELATIVE_TORQUE_H 1

#include "tooladdforce.h"
#include "sreltorque.h"

class ToolAddRelativeTorque: public ToolAddForce {
public:
    ToolAddRelativeTorque(): ToolAddForce("add-relative-torque") {}
    virtual SForce *makeForce() { return new SRelTorque(osg::Vec3(0, 0, 0)); }
    virtual Tool* clone() { return new ToolAddRelativeTorque(); }
};

#endif

