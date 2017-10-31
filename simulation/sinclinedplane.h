#ifndef _S_INCLINED_PLANE_H
#define _S_INCLINED_PLANE_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <cmath>

#include "sosggeode.h"

/**@class SInclinedPlane
 *@brief class for inclined plane objects
 * consist of two planes
 * position is the intersection point of inclined and horizantal planes
 * orientation is the orientation of the inclined plane constant in z axis 
 */
class SInclinedPlane : public SObject
{
public:
    SInclinedPlane();
    SInclinedPlane(float angle, float height, float width);
    SInclinedPlane(float angle, float height, float width, osg::Vec3 pos);
    
    void setAngle(float a) { mAngle = a; }
    float getAngle() const { return mAngle; }
    void setHeight(float h) { mHeight = h; }
    float getHeight() const { return mHeight; }
    void setWidth(float w) { mWidth = w; }
    float getWidth() const { return mWidth; }
    
    string getCounter();
    void setCounter(string);
    
    void init();     
    /** Create the dynamics object in ODE */
    void createSimObject(dWorldID world, dSpaceID space);
    
    /** Create the visual object for simulation */
    osg::Node * createVisualObject();
    
    /** Create the visual object for the editor */
   void createEditorObject();

protected:
    /** 
    * protect the destructor, because this object is inherited 
    * directly or indirectly from osg::Referenced
    */
    ~SInclinedPlane() {}
    
    float mHeight;
    float mWidth;
    float mAngle;
    dGeomID geom2;
    
    static string mCounter;
    
private:
    float mLen;         // inclined plane length 
    float mProjXLen;     // mLen projection on x axis 
};

#endif
