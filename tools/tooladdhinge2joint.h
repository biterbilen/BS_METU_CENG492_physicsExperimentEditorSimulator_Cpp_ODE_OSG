#ifndef _TOOL_ADD_HINGE2_JOINT_H
#define _TOOL_ADD_HINGE2_JOINT_H 1

#include "tooladdjoint.h"
#include "shinge2.h"

class ToolAddHinge2Joint: public ToolAddJoint {
public:
    ToolAddHinge2Joint(): ToolAddJoint("add-hinge2-joint") {}
    virtual SJoint *makeJoint() { 
        SHinge2 * hinge2 = new SHinge2(); 
        
        osg::Vec3 anchor = (mObj1->getPosition()+mObj2->getPosition()) / 2;
        anchor[2] = mObj2->getPosition().z();
        osg::Vec3 axis1 = osg::Vec3(0, 0, mObj1->getPosition().z()-mObj2->getPosition().z());
        osg::Vec3 axis2 = anchor - mObj2->getPosition();
        
        hinge2->setAnchor(anchor);
        hinge2->setAxis1(axis1);
        hinge2->setAxis2(axis2);
        
        return hinge2;        
    }
    virtual Tool* clone() { return new ToolAddHinge2Joint(); }
};

#endif


