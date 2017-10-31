#ifndef _S_FORCE_H
#define _S_FORCE_H 1

#include <iostream>
#include <string>
#include <osg/Vec3>
#include <osg/Referenced>
#include <osg/PositionAttitudeTransform>
#include <sobject.h>

using namespace std;

class SObject;

/**@class SForce
*@brief Class for forces
*/
class SForce : public osg::Referenced
{

public:
    enum ForceTypes {
        NORMALFORCE,
        TORQUE,
        RELFORCE,
        RELTORQUE,
        FORCEATPOS,
        FORCEATRELPOS,
        RELFORCEATPOS,
        RELFORCEATRELPOS
    };

    SForce(osg::Vec3 f);
    SForce();
    
    void setName(string s);
    string getName() const;
    
    float getDuration() const;
    void setDuration(float d);
    
    float getStart() const;
    void setStart(float s);
    
    SForce::ForceTypes getType() const;
    
    void setForce(osg::Vec3 f);   
    osg::Vec3 getForce() const;    
    void setObject(SObject* obj);
    
    virtual osg::Vec3 getPosition() const { return osg::Vec3(0,0,0); }
    
    SObject* getObject();
    void setBodyIndex(int i);
    int getBodyIndex() const;

    virtual void createSimulationForce()=0;
        
    /** Get this objects transform node. Used only for the editor. */
    osg::PositionAttitudeTransform * getTransformNode() { return mPosAttTransNode.get(); }
    void createEditorForce();
    
protected:
    ~SForce() {};
    ForceTypes mType;
    
    osg::ref_ptr<SObject> mObject;
    
    string mName;
    osg::Vec3 mForce;
    int mBodyIndex;
    float mDuration;// -1 for infinite duration
    float mStart;// 0 to start with the simulation
    
    osg::ref_ptr<osg::PositionAttitudeTransform> mPosAttTransNode;
    
    virtual string getCounter(){return "";}
    virtual void setCounter(string) {}
    
    string incrementString(string s, int n);
};


#endif
