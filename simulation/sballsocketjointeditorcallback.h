#ifndef _S_BALL_SOCKET_JOINT_EDITOR_CALLBACK_H
#define _S_BALL_SOCKET_JOINT_EDITOR_CALLBACK_H 1

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>

#include "sballsocket.h"

class SBallSocket;

class SBallSocketJointEditorCallback : public osg::NodeCallback
{
public:
    SBallSocketJointEditorCallback(SBallSocket * j, int type)
    {
        mJoint = j;
        mType = type;
    }

    virtual void operator () (osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::Vec3 anchorPos;
        osg::Vec3 geomPos;

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
        m.makeRotate(osg::Vec3(0, 0, 1), geomPos-anchorPos);
        osg::Quat quat;
        quat.set(m);
        
        osg::PositionAttitudeTransform *pat = dynamic_cast<osg::PositionAttitudeTransform *>(node);
        if( pat != 0L )
        {
            pat->setPosition(anchorPos);
            pat->setAttitude(quat);
        }

        traverse(node,nv);
    }

private:

    osg::ref_ptr<SBallSocket> mJoint;
    int mType;
};

#endif

