#ifndef _S_FLATMIRROR_H
#define _S_FLATMIRROR_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <ssimcallback.h>

/**@class SFlatMirror
 *@brief Class for Flat Mirror
 */
class SFlatMirror : public SObject
{
public:
    SFlatMirror();
    SFlatMirror(osg::Vec3 pos, float wid,float len);

    void setLength(float len);
    float getLength() const;
    
    void setWidth(float len);
    float getWidth() const;  
    
    void getOffPosition(); 
    void getOnPosition();
    
    void getOffDimension(); 
    void getOnDimension();
        
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
    ~SFlatMirror() {}
    
    static string mCounter;    
    float mWidth;
    float mLength;
    float mThickness;
};

#endif

