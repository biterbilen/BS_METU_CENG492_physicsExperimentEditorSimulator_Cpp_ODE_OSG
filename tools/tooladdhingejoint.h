#ifndef _TOOL_ADD_HINGE_JOINT_H
#define _TOOL_ADD_HINGE_JOINT_H 1

#include "tooladdjoint.h"
#include "shinge.h"

class ToolAddHingeJoint: public ToolAddJoint {
public:
    ToolAddHingeJoint(): ToolAddJoint("add-hinge-joint") {}
    virtual SJoint *makeJoint() { 
        SHinge * hinge = new SHinge();
        
        hinge->setAnchor((mObj1->getPosition()+mObj2->getPosition())/2);
        
        osg::Vec3 v = mObj2->getPosition()-mObj1->getPosition();
        
        osg::Vec3 axis;
        if( (v ^ osg::Vec3(1,1,1)) == osg::Vec3(0,0,0))
            axis =  v ^ osg::Vec3(1,1,2);
         else axis = v ^ osg::Vec3(1,1,1);
        
        hinge->setAxis(axis);
        
        return hinge; 
    }
    virtual Tool* clone() { return new ToolAddHingeJoint(); }
};

#endif

