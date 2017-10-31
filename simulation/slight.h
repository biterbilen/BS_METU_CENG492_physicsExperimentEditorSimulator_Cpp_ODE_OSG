#ifndef _S_LIGHT_H
#define _S_LIGHT_H 1

#include <string>
#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <sosggeode.h>
#include <ssimcallback.h>
#include <sray.h>
#include "sosggeode.h"

class SRay;

/**@class SLight
 *@brief class for light objects
 */
class SLight : public SObject
{
public:    
    SLight();
    SLight(osg::Vec3 pos);
    
    float getLength();
    void setLength(float len);
    osg::Vec3 getDirection();

    void init();
    void pushRay(SRay *ray);
    int getRayVectorSize();
    void resizeRayVector();
    void setSceneNode(osg::Group * sceneData) { mLightSceneData = sceneData; }
    void setObjectMap(map<dGeomID, SObject*> *mObjectMap);
    //TODO erase this function
    void print();
    /**
    *After the deletion of the invalid rays we have to create o VISUAL ray for the last valid ray in the mRays array 
    *and                                                          SIM  ray for a new ray that is reflected of refracted from the 
    *                                                                  collided object if it is OPTICAL
    */
    void createNewRays(dWorldID worldID, dSpaceID spaceID);    
    
    void clearRays();
        
    /** Create the visual object as a sphere for the light source */
    osg::Node * createVisualObject();
    
    /** Create the visual object as a sphere and a line for the editor */
    void createEditorObject();
    
    string getCounter();
    void setCounter(string);
    
protected:
    /** 
    * protect the destructor, because this object is inherited 
    * directly or indirectly from osg::Referenced
    */    
    ~SLight() {}
    vector<osg::ref_ptr<SRay> > mRays;
    float mLength; 
    static string mCounter;
    float mRadius;
    osg::Vec3 mDirection;
    osg::Group * mLightSceneData;
    /**
    *    pointer to the mObjectMap Structure in Simulation class
    */
    map<dGeomID, SObject*> *mLightObjectMap;
};

#endif


