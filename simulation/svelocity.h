#ifndef _S_VELOCITY_H
#define _S_VELOCITY_H 1

#include <osg/Referenced>
#include <osg/Vec3>
#include <ode/ode.h>

/**@class SVelocity
 *@brief Base Class for Linear and Angular Velocity
 *@author Sourcerer
 */
class SVelocity : public osg::Referenced
{
public:
    typedef enum {
        LINEAR,
        ANGULAR
    }VelocityTypes;

    SVelocity();
    SVelocity(osg::Vec3 f);
    
    VelocityTypes getType() const;
    
    void setVelocity(osg::Vec3 f);
    osg::Vec3 getVelocity() const;
   
    virtual void createSimulationVelocity(dBodyID)=0;
    
protected:
    ~SVelocity() {};
    
    VelocityTypes mType;
    osg::Vec3 mVelocity;
};

#endif
