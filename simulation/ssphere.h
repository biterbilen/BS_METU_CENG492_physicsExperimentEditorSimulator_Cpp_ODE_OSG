#ifndef _S_SPHERE_H
#define _S_SPHERE_H 1

#include <sobject.h>
#include <sbobby.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <ssimcallback.h>

class SBobby;

/**@class SSphere
*@brief class for sphere objects
*/
class SSphere : public SObject
{
public:
    SSphere(SObject::Types t);
    SSphere(float r, osg::Vec3 pos);
    
    float getRadius() const;
    void setRadius(float r);
    
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
    void  createEditorObject();
    
protected:
    /** 
    * protect the destructor, because this object is inherited 
    * directly or indirectly from osg::Referenced
    */
    
    void createBobbies();
    
    static string mCounter;
    
    ~SSphere() {}
    float mRadius;
};

#endif
