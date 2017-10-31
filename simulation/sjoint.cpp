#include "sjoint.h"

// constructors
SJoint::SJoint()
{
    mName = "joint";    
    mBody1Index = -1;
    mBody2Index = -1;
   
    mLoStopBool = false;
    mHiStopBool = false;
    mVelBool = false;
    mFMaxBool = false;    
    
    mPosAttTransNode = new osg::PositionAttitudeTransform();
}

void SJoint::jointAttach(dBodyID b1, dBodyID b2) 
{
    dJointAttach ((dJointID)this->mJointID, b1, b2);
}

void SJoint::setJointID(dJointID id)
{
    mJointID = id;    
}

void SJoint::setName(string name)        
{    
    mName = name; 
}

string SJoint::getName()                 
{    
    return mName; 
}

SJoint::JointTypes SJoint::getJointType()        
{    
    return mType; 
}

dJointID SJoint::getJointID()
{
    return mJointID;
}

void SJoint::setBody1ID(dBodyID id)
{
    mBody1ID = id;    
}

void SJoint::setBody2ID(dBodyID id)
{
    mBody2ID = id;    
}

void SJoint::setBody1(SObject* obj)
{
    mBody1 = obj;    
}

void SJoint::setBody2(SObject* obj)
{
    mBody2 = obj;    
}

SObject* SJoint::getBody1() 
{
    return mBody1;
}

SObject* SJoint::getBody2() 
{
    return mBody2;
}

int SJoint::getBody1Index() const
{
    return mBody1Index;
}

int SJoint::getBody2Index() const
{
    return mBody2Index;
}

void SJoint::setBodyIndex(int i)
{
    if(mBody1Index == -1)
        mBody1Index = i;
    else
        mBody2Index = i;    
}

void SJoint::resetJoint()
{
    mBody1Index = -1;
    mBody2Index = -1;    
}

string SJoint::incrementString(string s, int n)
{
    if(s[n] == '9')
    {
        s[n] = '0';
        if(n == 0)
            s = "1" + s;
        else
            s = incrementString(s, n-1);    
    }
    else
    {
        switch(s[n])
            {
                case '0' : s[n] = '1'; break;
                case '1' : s[n] = '2'; break;
                case '2' : s[n] = '3'; break;
                case '3' : s[n] = '4'; break;
                case '4' : s[n] = '5'; break;
                case '5' : s[n] = '6'; break;
                case '6' : s[n] = '7'; break;
                case '7' : s[n] = '8'; break;
                default  : s[n] = '9'; break;            
            }    
    }    
    return s;
}


