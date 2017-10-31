#ifndef _S_BOBBY_H
#define _S_BOBBY_H 1

#include <osg/Referenced>
#include <sobject.h>
#include <sbobbymath.h>

/**@class SBobby
 *@brief class that handles the buoyancy of objects by spherical particles
 */
class SBobby : public osg::Referenced
{
public:
    SBobby();
   
    float getTotalVolume();
    float getSubmergedVolume();
    float getCenterDepth();
    float getRadius();
    float getWaterLevel();
    float getMass() const;
    float getSubmergedAmount();
    osg::Vec3 getWaterNormal();
    osg::Vec3 getBuoyancyForce();
    osg::Vec3 getBuoyancyForceCenter();
    osg::Vec3 getWorldPosition();
    osg::Vec3 getPosition();
    SObject* getBody();
    dBodyID getAttachedBodyID();
    
    void setSubmergedVolume(float);
    void setCenterDepth(float);
    void setRadius(float);
    void setWaterLevel(float);    
    void setBuoyancyForce(osg::Vec3);
    void setBuoyancyForceCenter(osg::Vec3);
    void setWorldPosition(osg::Vec3);
    void setPosition(osg::Vec3);
    void setBody(SObject*);
    void setAttachedBodyID(dBodyID);   
    void setMass(float m);
    void update(float, float);
    void updateWorldPosition();
    void calcBuoyForce();
    void applyForces();
    void calcDragForce();
    void updateSpeed();
    void getDepth();
    void setLiquidDensity(float);
    
    void setGravity(osg::Vec3 g) { mGravity = g; }
    void setTime(float t) { mTime = t; }
    
protected:
    ~SBobby();

    SObject * mBody;
    dBodyID mAttachedBodyID;    
    
    float mTotalVolume;
    float mSubmergedVolume;        
    float mCenterDepth;
    float mRadius;
    
    osg::Vec3 mPosition;
    osg::Vec3 mWaterNormal;
    osg::Vec3 mBuoyancyForce;
    osg::Vec3 mBuoyancyForceCenter;
    osg::Vec3 mWorldPosition;  
    osg::Vec3 mOldWorldPosition;
    osg::Vec3 mDragForce;
    osg::Vec3 mBobbySpeed;
    float mDragCoefficient;
    float mSurfaceArea;
    float mSubmergedSurfaceArea;
    
    float mTime;
    
private:
    
    float mLiquidDensity;
    float mWaterLevel;
    osg::Vec3 mGravity;
};

#endif
