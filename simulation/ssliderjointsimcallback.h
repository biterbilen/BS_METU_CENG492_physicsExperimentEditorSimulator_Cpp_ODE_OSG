#ifndef _S_SLIDER_JOINT_SIM_CALLBACK_H
#define _S_SLIDER_JOINT_SIM_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include <ode/ode.h>

#include "sjoint.h"

class SSliderJointSimCallback : public osg::NodeCallback
{
public:
    SSliderJointSimCallback(SJoint * j, int type)
    {
        mJoint = j;
        mType = type;
    }

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {           
        dReal *geomPosOde1;
        osg::Vec3 geomPos1;              
        
        dReal *geomPosOde2;
        osg::Vec3 geomPos2;
        
        dVector3 axisOde;
        osg::Vec3 axis;        
        
        osg::Vec3 diff;
        
        dJointID jointID = mJoint->getJointID();
        dGeomID geomID1 = mJoint->getBody1()->getGeomID();
        dGeomID geomID2 = mJoint->getBody2()->getGeomID();
        dReal real = dJointGetSliderPosition(jointID);
        
        geomPosOde1 = const_cast<dReal*>(dGeomGetPosition(geomID1));
        geomPos1.set(geomPosOde1[0],geomPosOde1[1],geomPosOde1[2]);
        
        geomPosOde2 = const_cast<dReal*>(dGeomGetPosition(geomID2));
        geomPos2.set(geomPosOde2[0],geomPosOde2[1],geomPosOde2[2]);
        
        diff = (geomPos2-geomPos1);
                
        dJointGetSliderAxis(jointID,axisOde);
        axis.set(axisOde[0],axisOde[1],axisOde[2]);
        
        osg::Matrix m;
        m.makeScale(osg::Vec3(1, 1, diff.length()/2));
        osg::Matrix m2;
        m2.makeRotate(osg::Vec3(0, 0, 1), axis);

        osg::Matrix m3;
        if(mType == 0)
        {
            m3.makeTranslate(geomPos1+diff * 1/4);
        }
        else {
            m3.makeTranslate(geomPos1+diff * 3/4);
        }
        
        osg::MatrixTransform *pat = dynamic_cast<osg::MatrixTransform *>(node);
        if( pat != 0L )        
            pat->setMatrix(m * m2 * m3);        

        traverse(node,nv);
    }

private:

    osg::ref_ptr<SJoint> mJoint;
    int mType;
};

#endif


