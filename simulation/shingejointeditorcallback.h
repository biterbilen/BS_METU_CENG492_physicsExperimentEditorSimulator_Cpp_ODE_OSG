#ifndef _S_HINGE_JOINT_EDITOR_CALLBACK_H
#define _S_HINGE_JOINT_EDITOR_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include "shinge.h"

class SHingeJointEditorCallback : public osg::NodeCallback
{
public:
    SHingeJointEditorCallback(SHinge * j, int type)
    {
        mJoint = j;
        mType = type;
    }

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::Vec3 geomPos;
        osg::Vec3 anchorPos;        
        osg::Vec3 axis;
        
        axis = mJoint->getAxis();
        anchorPos = mJoint->getAnchor();
        
        //get position of body 1
        if (mType == 0) 
        {
            geomPos = mJoint->getBody1()->getPosition();
        }
        else if(mType == 1)
        {  
            geomPos = mJoint->getBody2()->getPosition();
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

    osg::ref_ptr<SHinge> mJoint;
    int mType;
};

#endif


