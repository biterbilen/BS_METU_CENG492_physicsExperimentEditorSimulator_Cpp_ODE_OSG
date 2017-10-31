#ifndef _S_HINGE2_JOINT_SIM_CALLBACK_H
#define _S_HINGE2_JOINT_SIM_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include <ode/ode.h>

#include "sjoint.h"

class SHinge2JointSimCallback : public osg::NodeCallback
{
public:
    SHinge2JointSimCallback(SJoint * j, int type)
    {
        mJoint = j;
        mType = type;
    }

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {   
        dJointID jointID = mJoint->getJointID();
        dGeomID geomID; 
        
        dReal *geomPosOde;
        osg::Vec3 geomPos;
        dVector3 anchorPosOde;
        osg::Vec3 anchorPos;        
        dVector3 axisOde;
        osg::Vec3 axis;        
        
        osg::Vec3 finalPos;
                
        osg::Quat quat;
        
        //get position of body 1
        if (mType == 0) 
        {
            geomID = mJoint->getBody1()->getGeomID();
            dJointGetHinge2Anchor(jointID, anchorPosOde);            
            dJointGetHinge2Axis1(jointID, axisOde);
            geomPosOde = const_cast<dReal*>(dGeomGetPosition(geomID));
            geomPos.set(geomPosOde[0],geomPosOde[1],geomPosOde[2]);
            anchorPos.set(anchorPosOde[0],anchorPosOde[1],anchorPosOde[2]);   
            axis.set(axisOde[0], axisOde[1], axisOde[2]);
            
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
            geomID = mJoint->getBody2()->getGeomID();
            dJointGetHinge2Anchor2(jointID,anchorPosOde);            
            dJointGetHinge2Axis2(jointID, axisOde);
            geomPosOde = const_cast<dReal*>(dGeomGetPosition(geomID));
            geomPos.set(geomPosOde[0],geomPosOde[1],geomPosOde[2]);
            anchorPos.set(anchorPosOde[0],anchorPosOde[1],anchorPosOde[2]);            
            axis.set(axisOde[0], axisOde[1], axisOde[2]);
            
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

    osg::ref_ptr<SJoint> mJoint;
    int mType;
};

#endif


