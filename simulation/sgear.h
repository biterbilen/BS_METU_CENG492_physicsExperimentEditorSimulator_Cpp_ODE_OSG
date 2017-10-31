#ifndef _S_GEAR_H
#define _S_GEAR_H 1

#include <sobject.h>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/Geode>
#include "sosggeode.h"
#include "ssimcallback.h"
#include "seditorcallback.h"

/**@class SGear
*@brief class for gear objects
*/
class SGear : public SObject
{
    
public:
    enum GearTypes {
        BOXGEAR,       // box at (mRadius,0,0) is 0 degrees inclined
        PRISMGEAR      // box at (mRadius,0,0) is 45 degrees inclined
    };
    SGear(GearTypes g);
    SGear(float r, int t, osg::Vec3 p);
    
    string getCounter();
    void setCounter(string);
    
    void setGearType(GearTypes t) { mGearType = t; }

    void setRadius(float);
    float getRadius() const;
    GearTypes getGearType() const;
    void setToothNumber(int t); 
    int getToothNumber() const;    
        
    void setMass(float); /////TODO No mass usage ?????
            
    /** Create the dynamics object in ODE */
    void createSimObject(dWorldID world, dSpaceID space);
    
    /** Create the visual object for simulation */
    osg::Node * createVisualObject();
    
    /** Create the visual object for the editor */
    void createEditorObject();
    
    void init();            //    sets private members below
    
protected:
    ~SGear() {}
    GearTypes mGearType;
    float mRadius;
    int mToothNumber;
    vector<dGeomID> mGeoms;
    
private:    
    
    float mBoxDim;          //Cube dimensions to place mToothNumber around mRadius Cylinder
    osg::Vec3 mBoxCenter;   //Cube center at (mRadius,0,z) for to place mToothNumber around mRadius Cylinder
    //Angle between two tooth centers in PRISMGEAR and 1/2 times that for BOXGEAR 
    float mInitAngle;
    float mAngle;
    osg::Matrixf mBoxCenterOrientationMatrix; // mBoxCenterOrientationMatrix*mBoxCenter

    static string mCounter;
};

#endif
