#ifndef _S_POOL_H
#define _S_POOL_H 1

#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <ssimcallback.h>

#include <sobject.h>

/**@class SPool
 *@brief class for pools to simulate buoyancy
 */
class SPool : public SObject
{
public:
    SPool();
    SPool(osg::Vec3 pos, osg::Vec3 dim);
    
    void setWaterLevel(float f);
    void setLiquidDensity(float f);
    void setDimensions(osg::Vec3 v);
    float getWaterLevel();
    float getLiquidDensity();
    osg::Vec3 getDimensions();
    
    float getDepth() { return mDepth; }
    void setDepth(float d) { mDepth = d; }
    
    string getCounter();
    void setCounter(string);
    
    /** Create the dynamics object in ODE */
    void createSimObject(dWorldID world, dSpaceID space);
    
    /** Create the visual object for simulation */
    osg::Node * createVisualObject();
    
    /** Create the visual object for the editor */
    void createEditorObject();   
    
protected:
    ~SPool() {}

    float mDepth;
    float mWaterLevel;
    float mLiquidDensity;
    osg::Vec3 mDimensions;
    
    static string mCounter;
};

#endif
