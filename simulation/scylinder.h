#ifndef _S_CYLINDER_H
#define _S_CYLINDER_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <ssimcallback.h>

/**@class SCylinder
@brief class for cylinder objects
*/
class SCylinder : public SObject
{
public:
    SCylinder(SObject::Types t);

    float getRadius() const;
    void setRadius(float r);
       
    float getHeight() const;
    void setHeight(float h);
    
    string getCounter();
    void setCounter(string);
    
    void setMass(float m);
    
    /** Get volume of this object */    
    float getVolume();
    /** Get density of this object */    
    float getDensity();
    
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
    ~SCylinder() {}
    float mRadius;
    float mHeight;
    
    static string mCounter;
};

#endif
