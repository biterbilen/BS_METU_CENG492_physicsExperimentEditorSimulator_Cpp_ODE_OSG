#ifndef _TOOL_ADD_UNIVERSAL_JOINT_H
#define _TOOL_ADD_UNIVERSAL_OINT_H 1

#include "tooladdjoint.h"
#include "suniversal.h"

class ToolAddUniversalJoint: public ToolAddJoint {
public:
    ToolAddUniversalJoint(): ToolAddJoint("add-universal-joint") {}
    virtual SJoint *makeJoint() { 
        SUniversal * universal = new SUniversal();
        
        universal->setAnchor((mObj1->getPosition()+mObj2->getPosition())/2);
        
        osg::Vec3 v = mObj2->getPosition()-mObj1->getPosition();
        
        osg::Vec3 axis;
        if( (v ^ osg::Vec3(1,1,1)) == osg::Vec3(0,0,0))
            axis =  v ^ osg::Vec3(1,1,2);
         else axis = v ^ osg::Vec3(1,1,1);
        
        universal->setAxis1(axis);
        universal->setAxis2(axis ^ v);
        
        return universal; 
    }
    virtual Tool* clone() { return new ToolAddUniversalJoint(); }
};

#endif

