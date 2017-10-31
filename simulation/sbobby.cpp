#include "sbobby.h"

SBobby::SBobby()
{
    mWaterNormal.set(0.0, 0.0, 1.0); 
    mCenterDepth = 10000;
    mDragCoefficient = 1.0;
    mDragForce.set(0, 0, 0);
    mBuoyancyForce.set(0, 0, 0);
    mWorldPosition.set(-100000, -100000, -100000);
    mOldWorldPosition.set(-100000, -100000, -100000);    
}

SBobby::~SBobby()
{

}

SObject* SBobby::getBody()
{
    return mBody;
}

void SBobby::update(float waterLevel, float liquidDensity)
{
    // If the body that the bobby is connected to is disabled, then we can jump out here    
    if (dBodyIsEnabled(mAttachedBodyID) == 0)            
        return;

    mWaterLevel = waterLevel;
    mLiquidDensity = liquidDensity;    

    // * Update bobby world position    
    updateWorldPosition();   
    
    // * Update bobby speed    
    updateSpeed();
    
    // * Call GetDepthAndNormal with each bobby    
    getDepth();
    
    // * Calculate the buoyancy forces    
    calcBuoyForce();
    
    // * Calculate the drag forces    
    //     calcDragForce();                   CURRENTLY HAS SOME ERRORS. USE WITH CAUTION!!!!!

    // * Add the forces to the bodies      
    applyForces();
}

void SBobby::getDepth()
{
    mCenterDepth = mWorldPosition.z() - mWaterLevel;    
}

void SBobby::updateWorldPosition()
{       
    dReal pos[3];
    
    mOldWorldPosition = mWorldPosition;
    
    dBodyGetRelPointPos(mAttachedBodyID, mPosition.x(), mPosition.y(), mPosition.z(), pos);    
    
    mWorldPosition.set(pos[0],
                       pos[1],
                       pos[2]);   
}

void SBobby::updateSpeed()
{     
    mBobbySpeed = (mWorldPosition - mOldWorldPosition) / mTime;    
}

void SBobby::calcBuoyForce()
{
   
    float displacementMass;
    float depth;
    
    depth = submergedSphereCapCentroidHeight(mRadius, mCenterDepth);

    mBuoyancyForceCenter[0] = mWorldPosition[0] + depth * mWaterNormal[0];
    mBuoyancyForceCenter[1] = mWorldPosition[1] + depth * mWaterNormal[1];
    mBuoyancyForceCenter[2] = mWorldPosition[2] + depth * mWaterNormal[2];

    // Calculate displaced volume
    mSubmergedVolume = submergedSphereCapVolume(mRadius, mCenterDepth);

    if (mSubmergedVolume == 0) 
        mSubmergedSurfaceArea = 0;
    else if (mCenterDepth < -mRadius)
        mSubmergedSurfaceArea = mSurfaceArea;
    else
        mSubmergedSurfaceArea = mSurfaceArea * (mRadius * 2 - mCenterDepth);
            
    // Calculate displacement mass
    displacementMass = mSubmergedVolume * mLiquidDensity;    
    
    // The lifting force is always opposing gravity    
              
    mBuoyancyForce[0] = -mGravity.x() * displacementMass;
    mBuoyancyForce[1] = -mGravity.y() * displacementMass;
    mBuoyancyForce[2] = -mGravity.z() * displacementMass;

    // The sideways moving force is proportional to the slope of the water normal         
}

void SBobby::calcDragForce()
{
    float forceMagnitude;
    float speed;
    float speed2;
    
    // If this is the first time, ignore drag!
    if (mOldWorldPosition[0] == -100000)
        return;

    // If the bobby isn't submerged, the force is zero
    if (mSubmergedVolume == 0)
    {
        mDragForce.set(0, 0, 0);        
        return;
    }
  
    speed2 = mBobbySpeed.length2();
    speed  = sqrt(speed2);
 
    forceMagnitude = - mDragCoefficient / 2 * mLiquidDensity * speed2 * mSubmergedSurfaceArea;
  
    mDragForce[0] = mBobbySpeed[0] * forceMagnitude / speed;
    mDragForce[1] = mBobbySpeed[1] * forceMagnitude / speed;
    mDragForce[2] = mBobbySpeed[2] * forceMagnitude / speed;
}

void SBobby::applyForces()
{    
    osg::Vec3 totalForce = mBuoyancyForce + mDragForce;
    
    dBodyAddForceAtPos(mAttachedBodyID,
        totalForce.x(), 
        totalForce.y(), 
        totalForce.z(),      mBuoyancyForceCenter.x(), 
                             mBuoyancyForceCenter.y(), 
                             mBuoyancyForceCenter.z());
}

float SBobby::getTotalVolume()
{
    return mTotalVolume;
}
    
float SBobby::getSubmergedVolume()
{
    return mSubmergedVolume;    
}

float SBobby::getCenterDepth()
{
    return mCenterDepth;
}

float SBobby::getRadius()
{
    return mRadius;
}

float SBobby::getWaterLevel()
{
    return mWaterLevel;
}

osg::Vec3 SBobby::getWaterNormal()
{
    return mWaterNormal;
}

osg::Vec3 SBobby::getBuoyancyForce()
{
    return mBuoyancyForce;
}

osg::Vec3 SBobby::getBuoyancyForceCenter()
{
    return mBuoyancyForceCenter;
}

osg::Vec3 SBobby::getWorldPosition()
{
    return mWorldPosition;
}

osg::Vec3 SBobby::getPosition()
{
    return mPosition;
}

dBodyID SBobby::getAttachedBodyID()
{
    return mAttachedBodyID;
}

void SBobby::setSubmergedVolume(float f)
{
    mSubmergedVolume = f;    
}

void SBobby::setCenterDepth(float f)
{
    mCenterDepth = f;    
}

void SBobby::setRadius(float f)
{
    mRadius = f;        
    mTotalVolume = 4.0 * M_PI * f * f * f / 3.0;
    mSurfaceArea = M_PI * f * f;
}

void SBobby::setWaterLevel(float f)
{
    mWaterLevel = f;    
}

void SBobby::setBuoyancyForce(osg::Vec3 f)
{
    mBuoyancyForce = f;
}

void SBobby::setLiquidDensity(float f)
{
    mLiquidDensity = f;
}

void SBobby::setBuoyancyForceCenter(osg::Vec3 f)
{
    mBuoyancyForceCenter = f;    
}

void SBobby::setWorldPosition(osg::Vec3 f)
{
    mWorldPosition = f;    
}

void SBobby::setPosition(osg::Vec3 f)
{
    mPosition = f;    
}

void SBobby::setBody(SObject* f)
{
    mBody = f;    
}

void SBobby::setAttachedBodyID(dBodyID f)
{
    mAttachedBodyID = f;
}   
