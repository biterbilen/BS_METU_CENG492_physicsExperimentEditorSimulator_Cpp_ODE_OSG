#ifndef _S_ROPE_H
#define _S_ROPE_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include "sosggeode.h"
#include "sropesimcallback.h"

/**@class SRope
 *@brief class for rope objects
 */
 
class SRope : public SObject
{
public:
    SRope();     
    void setRadius(float rad);
        
    string getCounter();
    void setCounter(string);
    
    void setEnd(osg::Vec3);
    osg::Vec3 getEnd() { return mEnd; }
    
    void setStart(osg::Vec3);
    osg::Vec3 getStart() { return mStart; }
    
    SObject* getStartObject() { return mStartObject; }
    SObject* getEndObject() { return mEndObject; }
    void setStartObject(SObject*);
    void setEndObject(SObject*);
    
    float getRadius();
    void init();
    void setMass(float m); // mMass takes the value of m/mSphNum
        
    void createSimObject(dWorldID world, dSpaceID space);
    osg::Node * createVisualObject();
    void createEditorObject();

protected:
    ~SRope() {}
        
    vector<dBodyID> mBodies;
    vector<dGeomID> mGeoms;
    vector<dJointID> mJoints;
    
    SObject* mStartObject;
    SObject* mEndObject;
    
    osg::Vec3 mEnd;
    osg::Vec3 mStart;
    
    dJointID mStartJointID;
    dJointID mEndJointID;
    
    dGeomID mEndGeomID;
    dBodyID mEndBodyID;
    
    static string mCounter;
    
private:
    
    float mLength;
    int mNumberOfSpheres;       // number of spheres to represent as ode object
    float mRadius;
    osg::Vec3 mDirection;
      
};

#endif
