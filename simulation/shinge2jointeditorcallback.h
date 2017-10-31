#ifndef _S_HINGE2_JOINT_EDITOR_CALLBACK_H
#define _S_HINGE2_JOINT_EDITOR_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include "shinge2.h"

class SHinge2JointEditorCallback : public osg::NodeCallback
{
public:
    SHinge2JointEditorCallback(SHinge2 * j, int type)
    {
        mJoint = j;
        mType = type;
    }

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {   
        osg::Vec3 geomPos;
        osg::Vec3 anchorPos;        
        osg::Vec3 axis;        
        
        osg::Vec3 finalPos;
                
        osg::Quat quat;

        anchorPos = mJoint->getAnchor();        
       
        //get position of body 1
        if (mType == 0) 
        {
            geomPos = mJoint->getBody1()->getPosition();
            axis = mJoint->getAxis1();
            
            float A = axis.x();    
            float B = axis.y();    
            float C = axis.z();    
            float D = -(A*geomPos.x() + B*geomPos.y() + C*geomPos.z());            
            float f = A*anchorPos.x() + B*anchorPos.y() + C*anchorPos.z() + D;   
            float u = -f/(A*axis.x() + B*axis.y() + C*axis.z());
            osg::Vec3 p = anchorPos + axis * u;

            osg::Matrix m;     
            m.makeRotate(osg::Vec3(1, 0, 0), geomPos - p);
            osg::Vec3 vec = osg::Matrix::inverse(m) * osg::Vec3(0, 0, 1);
            osg::Matrix m2;            
            m2.makeRotate(vec, axis);

            finalPos = anchorPos;
            quat.set(m2*m);            
        }
        else if(mType == 1)
        {  
            geomPos = mJoint->getBody2()->getPosition();
            axis = mJoint->getAxis2();
            
            finalPos = (anchorPos + geomPos) / 2;
            quat.makeRotate(osg::Vec3(0, 0, 1), axis);
        }        
        
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
        if( pat != 0L )
        {         
            pat->setPosition(finalPos);
            pat->setAttitude(quat);
        }

        traverse(node,nv);
    }

private:

    osg::ref_ptr<SHinge2> mJoint;
    int mType;
};

#endif
