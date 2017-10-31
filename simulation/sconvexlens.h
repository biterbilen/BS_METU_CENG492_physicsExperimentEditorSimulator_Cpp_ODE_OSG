#ifndef _S_CONVEX_LENS_H
#define _S_CONVEX_LENS_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <ssimcallback.h>
#include "sosggeode.h"
#include "seditorcallback.h"

/**@class SConvexLens
 *@brief class for lens objects
 */
class SConvexLens : public SObject
{
public:
    SConvexLens();
    SConvexLens(osg::Vec3 pos, float foc, float thickness);

    void setFocalLength(float len);
    float getFocalLength() const;

    void setThickness(float len);
    float getThickness() const;

    void setRefractionIndex(float ri);
    float getRefractionIndex() const;

    void init();      
    /**
    *given the intersection point p of the ray and the whole sphere 
    *returns valid if the point is between elevation and azimuth ranges 
    */
    bool isValidPoint(osg::Vec3 p);  
    
    /** Create the dynamics object in ODE */
    void createSimObject(dWorldID world, dSpaceID space);
    
    /** Create the visual object for simulation */
    osg::Node * createVisualObject();
    
    /** Create the visual object for the editor */
    void createEditorObject();
    
    string getCounter();
    void setCounter(string);
    
    void calculateAzElevMinMax();
    
    dGeomID getGeomID2();
    
    //TODO erase this function
    void print();

protected:
    /** 
    * protect the destructor, because this object is inherited 
    * directly or indirectly from osg::Referenced
    */
    ~SConvexLens() {}
        
    dGeomID mGeomID2;
    float mRefractionIndex;
    
    static string mCounter;    
    float mFocalLength;
    float mThickness;
                
private:
    
    void recalculateParamaters();
    osg::Vec3 mDirection;    
    float mRadius;
    float mScale;          
    float mLensRadius;
    int mDensity;
    osg::Vec3 mLensCenter1;
    osg::Vec3 mLensCenter2;
    
    /**
    azimuth and elevation range in radians
    */    
    float mAzRange;
    float mElevRange;
    
    float mAzMin;
    float mAzMax;    
    float mElevMin;
    float mElevMax; 
};

#endif

