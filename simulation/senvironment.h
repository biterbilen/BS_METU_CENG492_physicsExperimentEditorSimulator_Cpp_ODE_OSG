#ifndef _S_ENVIRONMENT_H
#define _S_ENVIRONMENT_H 1

#include <list>

#include <osg/Referenced>
#include <sjoint.h>
#include <sbobby.h>
#include "schart.h"

/**@class SEnvironment
 *@brief class for the environment
 */
class SEnvironment : public osg::Referenced
{
public:
    
    typedef enum {
        NORMAL,
        FASTSTEP,
        QUICKSTEP
    }SolverType;
    
    SEnvironment();
    
    void setGravity(osg::Vec3 gra);
    void setMu(float m);
    void setStepSize(float ss);
    void setIterations(unsigned int iter);
    void setERP(float e);
    void setCFM(float c);
    void setSolverType(SolverType st);
    
    osg::Vec3 getGravity() const { return mGravity; }
    float getMu() const { return mMu; }
    float getStepSize() const { return mStepSize; }
    unsigned int getIterations() const { return mIterations; }
    float getCFM() const { return mCFM; }
    float getERP() const { return mERP; }
    SolverType getSolverType() const { return mSolverType; }
    
    void reset();

    //TODO when you remove an object, make sure you remove all forces, bobbies and velocities on the object

    void addObject(SObject* obj);
    /** try to remove object. return false if it has joints. otherwise return true */
    bool removeObject(SObject* obj);
    void removeObjectJoint(SObject* obj);

    void addJoint(SJoint* joint);
    void removeJoint(SJoint* joint);

    void addForce(SForce* force);
    void removeForce(SForce* force);

    void resetIndices();
    void setIndices();
    
    bool nameExists(string name);

    list<SObject*>::iterator getObjectBegin();
    list<SObject*>::iterator getObjectEnd();
    list<SJoint*>::iterator  getJointBegin();
    list<SJoint*>::iterator  getJointEnd();
    list<SForce*>::iterator  getForceBegin();
    list<SForce*>::iterator  getForceEnd();
    
    //Chart Related
    list<SChart*>::iterator getChartBegin();
    list<SChart*>::iterator getChartEnd();
    void addChart(SChart * chart);
    void removeChart(SChart * chart);

protected:
    ~SEnvironment();

    list<SObject*> mObjects;
    list<SJoint*> mJoints;
    list<SForce*> mForces;
    list<SChart*>    mCharts; 
    
    
    osg::Vec3 mGravity;
    float mCFM;
    float mERP;
    float mMu;
    float mStepSize;
    unsigned int mIterations;
    
    SolverType mSolverType;
};

#endif
