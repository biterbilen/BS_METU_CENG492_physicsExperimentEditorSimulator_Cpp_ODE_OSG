#ifndef _S_HINGE_JOINT_SIM_CALLBACK_H
#define _S_HINGE_JOINT_SIM_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include <ode/ode.h>

#include "sjoint.h"

class SHingeJointSimCallback : public osg::NodeCallback
{
public:
    SHingeJointSimCallback(SJoint * j, int type)
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
        
        dJointGetHingeAxis(jointID,axisOde);
        axis.set(axisOde[0],axisOde[1],axisOde[2]);
        
        //get position of body 1
        if (mType == 0) 
        {
            geomID = mJoint->getBody1()->getGeomID();
            dJointGetHingeAnchor(jointID,anchorPosOde);            
            geomPosOde = const_cast<dReal*>(dGeomGetPosition(geomID));
            geomPos.set(geomPosOde[0],geomPosOde[1],geomPosOde[2]);
            anchorPos.set(anchorPosOde[0],anchorPosOde[1],anchorPosOde[2]);            
        }
        else if(mType == 1)
        {  
            geomID = mJoint->getBody2()->getGeomID();
            dJointGetHingeAnchor2(jointID,anchorPosOde);            
            geomPosOde = const_cast<dReal*>(dGeomGetPosition(geomID));
            geomPos.set(geomPosOde[0],geomPosOde[1],geomPosOde[2]);
            anchorPos.set(anchorPosOde[0],anchorPosOde[1],anchorPosOde[2]);            
        }        
   
        osg::Matrix m;     
        m.makeRotate(osg::Vec3(1, 0, 0), geomPos-anchorPos);
        osg::Vec3 vec = osg::Matrix::inverse(m) * osg::Vec3(0, 0, 1);
        osg::Matrix m2;
        m2.makeRotate(vec, axis);
        osg::Quat quat;
        quat.set(m * m2);

        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
        if( pat != 0L )
        {
            pat->setPosition(anchorPos);
            pat->setAttitude(quat);
        }

        traverse(node,nv);
    }

private:

    osg::ref_ptr<SJoint> mJoint;
    int mType;
};

#endif


