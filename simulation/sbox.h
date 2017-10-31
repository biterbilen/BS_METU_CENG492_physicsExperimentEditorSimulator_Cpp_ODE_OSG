#ifndef _S_BOX_H
#define _S_BOX_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include <ssimcallback.h>
#include <sbobby.h>

class SBobby;

/**@class SBox
 *@brief class for box objects
 */
class SBox : public SObject
{
public:
    SBox(SObject::Types t);
    SBox(float len, osg::Vec3 pos);
    SBox(osg::Vec3 len, osg::Vec3 pos);

    void setDimensions(osg::Vec3 dim);
    osg::Vec3 getDimensions() const;
    
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
    
    friend ostream& operator<<(ostream& out, const SBox& box);
    friend istream& operator>>(istream& in, SBox& box);
    
    string getCounter();
    void setCounter(string);

protected:
    /** 
    * protect the destructor, because this object is inherited 
    * directly or indirectly from osg::Referenced
    */
    ~SBox() {}
    
    void createBobbies();

    vector<osg::Vec4> swap(vector<osg::Vec4>& v, int i, int j);
    vector<osg::Vec4> calculatePositionRadius(float x, float y, float z);
    vector<osg::Vec4> cubic(vector<osg::Vec4>& v, float x, float displacement);
    vector<osg::Vec4> cubic(vector<osg::Vec4>& v, float x, float disY, float disZ);
    vector<osg::Vec4> prism(vector<osg::Vec4>& v, float x, float y, float displacement);
    vector<osg::Vec4> stickZ(vector<osg::Vec4>& v, float n, float volume, float disZ);
    vector<osg::Vec4> stickY(vector<osg::Vec4>& v, float m, float volume, float disY);
    bool isEven(int n);
    
    static string mCounter;
    
    osg::Vec3 mDimensions;
};

#endif

