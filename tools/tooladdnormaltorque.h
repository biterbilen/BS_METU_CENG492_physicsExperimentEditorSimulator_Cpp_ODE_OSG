#ifndef _TOOL_ADD_NORMAL_TORQUE_H
#define _TOOL_ADD_NORMAL_TORQUE_H 1

#include "tooladdforce.h"
#include "storque.h"

class ToolAddNormalTorque: public ToolAddForce {
public:
    ToolAddNormalTorque(): ToolAddForce("add-normal-torque") {}
    virtual SForce *makeForce() { return new STorque(osg::Vec3(0, 0, 0)); }
    virtual Tool* clone() { return new ToolAddNormalTorque(); }
};

#endif

