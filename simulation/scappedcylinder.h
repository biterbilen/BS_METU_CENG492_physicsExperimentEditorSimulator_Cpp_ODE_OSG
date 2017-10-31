#ifndef _S_CAPPED_CYLINDER_H
#define _S_CAPPED_CYLINDER_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <ssimcallback.h>

/**@class SCappedCylinder
 *@brief class for capped cylinder objects
 */
class SCappedCylinder : public SObject
{
public:
    SCappedCylinder(SObject::Types t);
    SCappedCylinder(float radius, float height, osg::Vec3 pos);

    float getRadius() const;
    void setRadius(float r);
    
    void setHeight(float h);
    float getHeight() const;
    
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
    ~SCappedCylinder() {}
    
    void createBobbies();
    vector<osg::Vec4> calculatePositionRadius(float r, float h);
    bool isEven(int n);
    
    static string mCounter;
    
    float mRadius;
    float mHeight;
};

#endif
