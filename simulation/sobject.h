#ifndef _S_OBJECT_H
#define _S_OBJECT_H 1

#include <iostream>
#include <string>
#include <list>
#include <sforce.h>
#include <sjoint.h>
#include <sangvelocity.h>
#include <slinvelocity.h>
#include <osg/Vec3>
#include <osg/Referenced>
#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <ode/ode.h>
#include <sbobby.h>

using namespace std;

/**@class SObject
*@brief base class for all objects
*/
class SJoint;
class SForce;
class SAngVelocity;
class SLinVelocity;
class SBobby;


class SObject : public osg::Referenced
{
public:

    enum Types {
        STATIC,// Object is STATIC. Does collide, but not move
        DYNAMIC// Object is DYNAMIC. Collide and move
    };

    enum ObjTypes {
        BOX,
        SPHERE,
        CAPPEDCYLINDER,
        CYLINDER,
        GEAR,
        INCLINEDPLANE,
        ROPE,
        RAY,
        POOL,
        LIGHT,
        FLATMIRROR,
        CONMIRROR,
        CONCAVELENS,
        CONVEXLENS,
        TRIMESH,
        ON,
        OFF
    };

    SObject(Types t);

    osg::Vec3 getPosition() const;
    float getMass() const;
    virtual float getVolume() { return 1; }
    virtual float getDensity() { return 0; }
    string getName() const;
    Types getType() const;
    ObjTypes getObjectType() const;
    osg::Quat getOrientation() const;
    osg::Vec3 getEulerAngles();
    dBodyID getBodyID() const;
    dGeomID getGeomID() const;
    bool isStatic() const;
    bool isDynamic() const;
    SForce* getForce(int i);
    int getForceCount() const;

    void setStatic() { mType = SObject::STATIC; }
    void setDynamic() { mType = SObject::DYNAMIC; }

    list<SJoint*>::iterator getJointBegin();
    list<SJoint*>::iterator getJointEnd();
    list<SForce*>::iterator getForceBegin();
    list<SForce*>::iterator getForceEnd();

    void setPosition(osg::Vec3 pos);
    void setPosition(float x, float y, float z);
    virtual void setMass(float) { }
    void setName(string s);
    void setType(Types t);
    void setObjectType(ObjTypes obj);
    void setOrientation(osg::Quat ori);
    void setBodyID(dBodyID id);
    
    //TODO these can be protected later
    void setCollideBits(unsigned long col);
    void setCategoryBits(unsigned long cat);
    unsigned long getCollideBits();
    unsigned long getCategoryBits();
    void setOdeCollideBits();
    void setOdeCategoryBits();    

    bool canHaveChart();
    
    bool canAddForce();
    void addForce(SForce* f);
    void removeForce(SForce* f);

    void setAngVelocity(osg::Vec3 v);
    osg::Vec3 getAngVelocity() { return mAngVelocity->getVelocity(); }
    void setLinVelocity(osg::Vec3 v);
    osg::Vec3 getLinVelocity() { return mLinVelocity->getVelocity(); }

    bool canAddJoint();
    void addJoint(SJoint* j);
    void removeJoint(SJoint* j);

    void writeIndices(int index);
    void removeConnections();
    void removeConnection(SJoint* joint);
    void prepareForces();    

    void clearBobbies();

    bool zeroJoints();
    bool hasBobbies();
    bool isOptical(ObjTypes t);
    bool isMirror();
    bool isLens();
    float getSubmergedAmount();
    void updateBobbies(float, float);
    void setBobby(bool b);

    list<osg::ref_ptr<SBobby> >::iterator  getBobbyBegin();
    list<osg::ref_ptr<SBobby> >::iterator  getBobbyEnd();

    // Statistics Functions
    float getObjectLinearVel();
    float getObjectAngularVel();
    float getObjectPositionX();
    float getObjectPositionY();
    float getObjectPositionZ();
    float getObjectForce();
    float getObjectTorque();
    float getObjectAcc();

    /** An initialization function. Each object may have it's own initialization function */
    virtual void init() {}
    
    /** Create the dynamics object in ODE */
    virtual void createSimObject(dWorldID, dSpaceID) {}

    /** Create the visual object for simulation */
    virtual osg::Node * createVisualObject() { return 0; }

    /** Get this objects transform node. Used only for the editor. */
    osg::PositionAttitudeTransform * getTransformNode() { return mPosAttTransNode.get(); }
    /** Create the visual object for the editor */
    virtual void createEditorObject() { }
    
    virtual string getCounter(){return "";}
    virtual void setCounter(string) {}

protected:
    /**
    * protect the destructor, because this object is inherited 
    * directly or indirectly from osg::Referenced
    */
    ~SObject() {}   
    
    string incrementString(string s, int n);

    Types mType;
    string mName;
    ObjTypes mObjectType;
    osg::Vec3 mPosition;
    osg::Quat mOrientation;

    dBodyID mBodyID;
    dGeomID mGeomID;
    dMass mMass;    
    bool mBobby;
    unsigned long mColBits;
    unsigned long mCatBits;

    osg::ref_ptr<SLinVelocity> mLinVelocity;
    osg::ref_ptr<SAngVelocity> mAngVelocity;

    list<SForce* > mForces;
    list<SJoint*> mJoints;
    list<osg::ref_ptr<SBobby> > mBobbies;
    
    /** This one is used only for the editor. All editor visuals are created as a child to this
    * Set Update Callbacks to this node.
    */
    osg::ref_ptr<osg::PositionAttitudeTransform> mPosAttTransNode;
};

#endif

