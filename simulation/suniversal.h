#ifndef _UNIVERSAL_H
#define _UNIVERSAL_H 1

#include <string>
#include <ode/ode.h>
#include <osg/Vec3>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include "sjoint.h"

/**
 * @class SUniversal
 * @brief Ode's universal type joint
 */
class SUniversal:public SJoint
{
public:
	SUniversal();

	void setType();
    void setAnchor(osg::Vec3 a);
	void setAxis1(osg::Vec3 a);
	void setAxis2(osg::Vec3 a);
	osg::Vec3 getAnchor();
    osg::Vec3 getAxis1();
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
	~SUniversal() {}
    
    static string mCounter;
 	
    osg::Vec3 mAxis1;		// axis directions
    osg::Vec3 mAxis2;		// axis directions
    osg::Vec3 mAnchor;
    
 
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
