#ifndef _S_CON_MIRROR_H
#define _S_CON_MIRROR_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <osg/Quat>
#include <ssimcallback.h>
#include "sosggeode.h"

/**@class SConMirror
 *@brief class for con mirror objects
 */
class SConMirror : public SObject
{
public:
    SConMirror();
    SConMirror(osg::Vec3 pos, float foc);
    void init();

    void setFocalLength(float len);
    float getFocalLength() const;

    float getRadius() const;
   
    void setAzRange(float az);
    float getAzRange() const;
    
    void setElevRange(float elev);
    float getElevRange() const;      
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

protected:
    /** 
    * protect the destructor, because this object is inherited 
    * directly or indirectly from osg::Referenced
    */
    ~SConMirror() {}
    void calculateAzElevMinMax();  
    
    static string mCounter;    
    float mFocalLength;

    /**
    azimuth and elevation range in radians
    */
    float mAzRange;
    float mElevRange;
    
private:
    float mRadius;
    int mDensity;
    float mAzMin;
    float mAzMax;
    float mElevMin;
    float mElevMax;    
    osg::Vec3 mDirection;
            
};

#endif

