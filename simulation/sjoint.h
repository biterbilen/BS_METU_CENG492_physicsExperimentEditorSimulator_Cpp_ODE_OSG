#ifndef _JOINT_H
#define _JOINT_H 1

#include <osg/Referenced>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <ode/ode.h>

#include <string>
#include <sobject.h>


using namespace osg;
using namespace std;

/**
 * @class SJoint
 * @brief base class for joint types
 *
 * This class must be subclassed in each joint class (for example, SJointBall,SJointSlider,SJointHinge)
 *
 */
class SObject;

class SJoint : public osg::Referenced
{
public:
    /** 
     * An enum.
     * JointType
     */
    enum JointTypes {
        BALLSOCKET,
        HINGE,
        HINGE2,
        SLIDER,
        UNIVERSAL,        
    };

    SJoint();

    void jointAttach(dBodyID, dBodyID);
    void setName(string name);
    string getName();
    SObject* getBody1();
    SObject* getBody2();
    JointTypes getJointType();
    
    
    void setLoStop(float f)          { mLoStop = f;        mLoStopBool = true;        }
    void setHiStop(float f)          { mHiStop = f;        mHiStopBool = true;        }
    void setVel(float f)             { mVel = f;           mVelBool = true;           }
    void setFMax(float f)            { mFMax = f;          mFMaxBool = true;          }
    
    float getLoStop()                { return mLoStop;        }
    float getHiStop()                { return mHiStop;        }    
    float getVel()                   { return mVel;           }
    float getFMax()                  { return mFMax;          }
    
    bool isLoStopSet()                { return mLoStopBool;        }
    bool isHiStopSet()                { return mHiStopBool;        }    
    bool isVelSet()                   { return mVelBool;           }
    bool isFMaxSet()                  { return mFMaxBool;          }

    
    
    dJointID getJointID();
    void setJointID(dJointID);
    void setBody1ID(dBodyID id);
    void setBody2ID(dBodyID id);
    void setBody1(SObject* obj);
    void setBody2(SObject* obj);
    
    int getBody1Index() const;
    int getBody2Index() const;
    void setBodyIndex(int i);
    void resetJoint();


    virtual string getCounter(){return "";}
    virtual void setCounter(string) {}
    
      string incrementString(string s, int n);
    
    virtual void createSimulationJoint(dWorldID world, dJointGroupID jointgroup)=0;
    virtual osg::Node * createVisualJoint() {return 0;}
    
    osg::PositionAttitudeTransform * getTransformNode() { return mPosAttTransNode.get(); }
    virtual void createEditorJoint() {}
    
protected:
    ~SJoint() {}
    
    string mName;
    JointTypes mType; //ball hinge(2) slider or universal
    SObject* mBody1;
    SObject* mBody2;
    dBodyID mBody1ID;
    dBodyID mBody2ID;    
    dJointID mJointID;
   
    int mBody1Index;
    int mBody2Index;   
    
    float mLoStop;
    float mHiStop;
    float mVel;
    float mFMax;
    
    bool mLoStopBool;
    bool mHiStopBool;
    bool mVelBool;
    bool mFMaxBool;
    
    osg::ref_ptr<osg::PositionAttitudeTransform> mPosAttTransNode;
};

#endif

