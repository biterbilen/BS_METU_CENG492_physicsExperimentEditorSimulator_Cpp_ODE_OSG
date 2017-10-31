#ifndef _S_RAY_H
#define _S_RAY_H 1

#include <iostream>
#include <string>
#include <vector>

#include <ode/ode.h>

#include <osg/Vec3>
#include <osg/Node>
#include <osg/Referenced>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geometry>

#include "sobject.h"
#include "slight.h"
#include "sconmirror.h"
#include "sconvexlens.h"

/**@class SRay
 *@brief class for ray objects
 */
class SRay : public SObject
{
public:
    
    SRay(osg::Vec3 pos);
    SRay(osg::Vec3 pos, osg::Vec3 dir);
    
    void setNormal(osg::Vec3 normal);
    osg::Vec3 getNormal();
    void setEndPoint(osg::Vec3 end);
    osg::Vec3 getEndPoint();
    void setCollidedObjectType(SObject::ObjTypes type);
    SObject::ObjTypes getCollidedObjectType();
    void setCollidedObjectGeom(dGeomID geom);
    dGeomID getCollidedObjectGeom();
    void setLength(float len);
    float getLength();
    void setDirection(osg::Vec3 dir);
    osg::Vec3 getDirection(); 
    
    osg::Geode * getGeode() { return mGeode.get(); }
    
    bool areStartEndEqual(SRay *ray);
    //TODO THESE SHOULD BE WRIITEN AS OPTICAL OBJECT FUNCTIONS
    void calculateDirection(osg::Vec3 sourceDirection, osg::Vec3 surfaceNormal);
    void calculateDirection(SRay * ray,SObject *collObj);
    void calculateDirection(SRay * ray, float refIndex1, float refIndex2,SObject *collObj);
    void geomDestroy();
    void setInfiniteEnd();

    
    //TODO erase this function
    void print();
    
    /** Create the dynamics object in ODE */
    void createSimObject(dWorldID world, dSpaceID space);
    
    /** Create the visual object for simulation */
    osg::Node * createVisualObject();
    
    string getCounter();
    void setCounter(string);
          
protected:
    ~SRay();
   
    float mLength;
    osg::Vec3 mEnd;
    osg::Vec3 mNormal;
    osg::Vec3 mDirection;
    dGeomID mCollidedObjectGeom;
    SObject::ObjTypes mCollidedObject;
    
    osg::ref_ptr<osg::Geode> mGeode;
    
    static string mCounter;
};

#endif

