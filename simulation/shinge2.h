#ifndef _HINGE2_H
#define _HINGE2_H 1

#include <string>

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>
#include <osg/NodeCallback>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include <ode/ode.h>

#include "sjoint.h"


/**
 * @class SHinge2
 * @brief Ode's hinge2 type joint
 */
class SHinge2:public SJoint
{
public:
	SHinge2();

	void setAnchor(osg::Vec3 a);
	osg::Vec3 getAnchor();
	void setAxis1(osg::Vec3 a);
	osg::Vec3 getAxis1();
	void setAxis2(osg::Vec3 a);
	osg::Vec3 getAxis2();
 
    void setLoStopAxis2(float f)          { mLoStopAxis2 = f;        mLoStopBoolAxis2 = true;        }
    void setHiStopAxis2(float f)          { mHiStopAxis2 = f;        mHiStopBoolAxis2 = true;        }
    void setVelAxis2(float f)             { mVelAxis2 = f;           mVelBoolAxis2 = true;           }
    void setFMaxAxis2(float f)            { mFMaxAxis2 = f;          mFMaxBoolAxis2 = true;          }
    
    float getLoStopAxis2()                { return mLoStopAxis2;        }
    float getHiStopAxis2()                { return mHiStopAxis2;        }    
    float getVelAxis2()                   { return mVelAxis2;           }
    float getFMaxAxis2()                  { return mFMaxAxis2;          }
    
    bool isLoStopSetAxis2()                { return mLoStopBoolAxis2;        }
    bool isHiStopSetAxis2()                { return mHiStopBoolAxis2;        }    
    bool isVelSetAxis2()                   { return mVelBoolAxis2;           }
    bool isFMaxSetAxis2()                  { return mFMaxBoolAxis2;          }
  
 
    void createSimulationJoint(dWorldID worldid, dJointGroupID jointgroupid);
    osg::Node * createVisualJoint();
    
    void createEditorJoint();
    
    string getCounter();
    void setCounter(string);            

protected:
	~SHinge2() {}
 
    static string mCounter;
     
     osg::Vec3 mAnchor;	
	osg::Vec3 mAxis1;
    osg::Vec3 mAxis2; 	
 
    float mLoStopAxis2;
    float mHiStopAxis2;
    float mVelAxis2;
    float mFMaxAxis2;
    
    bool mLoStopBoolAxis2;
    bool mHiStopBoolAxis2;
    bool mVelBoolAxis2;
    bool mFMaxBoolAxis2;
};

#endif
