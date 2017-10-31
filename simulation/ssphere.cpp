#include "ssphere.h"
#include "sosggeode.h"
#include "seditorcallback.h"
#include <cmath>

string SSphere::mCounter = "1";

SSphere::SSphere(SObject::Types t) : SObject(t)
{
    mObjectType = SObject::SPHERE;
    mName = "Sphere" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}


SSphere::SSphere(float r, osg::Vec3 pos) : SObject(SObject::DYNAMIC)
{
    mObjectType = SObject::SPHERE;
    mPosition = pos;
    mOrientation = osg::Quat(0, osg::Vec3(0, 0, 1));
    mRadius = r;
    mName = "Sphere" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    setMass(1);
}

string SSphere::getCounter()
{
    return mCounter;
}

void SSphere::setCounter(string counter)
{
    mCounter = counter;
}

float SSphere::getRadius() const
{
	return mRadius;    
}

void SSphere::setRadius(float r)
{
	mRadius = r;      
}
 
void SSphere::setMass(float m)
{
    // Fill the mMass structure with corresponding mass information
    dMassSetSphereTotal(&mMass, m, mRadius);    
}

float SSphere::getVolume()
{
    return 4/3 * M_PI * mRadius * mRadius * mRadius;
}
    
float SSphere::getDensity()
{
    return (this->getMass() / this->getVolume());
}

void SSphere::createSimObject(dWorldID worldid, dSpaceID spaceid)
{
    // ODE related stuff
    if (this->isDynamic()) 
    {
        mBodyID = dBodyCreate(worldid);
        dBodySetPosition(mBodyID, mPosition.x(), mPosition.y(), mPosition.z());
        dBodySetMass(mBodyID, &mMass);        
        
        mGeomID = dCreateSphere(spaceid, mRadius);        
        dGeomSetBody(mGeomID, mBodyID);
    }
    else 
    {
        mBodyID = 0;
        mGeomID = dCreateSphere(spaceid, mRadius);
        dGeomSetPosition(mGeomID, mPosition.x(), mPosition.y(), mPosition.z());
        dGeomSetBody(mGeomID, 0);      
    }

    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(mGeomID, quat);
    
    if(mBobby)
        createBobbies();
}

void SSphere::createEditorObject() 
{
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
    
    /** Below we create the new child nodes and add them to the transform node */
        
    // OSG related stuff
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the cube color
    osg::StateSet * sphereState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 color( 1.0f, 0.2f, 0.2f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    sphereState->setAttribute( redMaterial );

    // Create a drawable shape
    osg::ShapeDrawable * sphereDraw;
    sphereDraw = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), mRadius), hints);
    sphereDraw->setStateSet( sphereState );

    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(sphereDraw);
    geode->setSObject(this);
        
    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

osg::Node * SSphere::createVisualObject()
{
    // OSG related stuff    
    osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the sphere red
    osg::ref_ptr<osg::StateSet> sphereState = new osg::StateSet();
    osg::ref_ptr<osg::Material> redMaterial = new osg::Material();
    osg::Vec4 red( 1.0f, 0.2f, 0.2f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, red );
    sphereState->setAttribute( redMaterial.get() );
    
    /*
    * Create a drawable shape
    * Note that it is created in coordinates mPosition
    * That's not so important because it will get it's position from ODE
    * with the help of the updateCallback we will set below...
    */
    osg::ref_ptr<osg:: ShapeDrawable> sphereDraw;
    sphereDraw = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), mRadius), hints.get());
    sphereDraw->setStateSet( sphereState.get() );

    // Create a Geometry Node, and add the drawable to that node
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(sphereDraw.get());
    
    /** 
    * Create a Transform Node, so that we can set up a NodeCallback which will
    * transform and rotate the child node in each drawing of the scene
    */
    osg::ref_ptr<osg::Transform> transNode = new osg::PositionAttitudeTransform();
    transNode->addChild(geode.get());

    osg::ref_ptr<SSimCallback> odeCallBack = new SSimCallback(mGeomID);
    transNode->setUpdateCallback( odeCallBack.get() );
   
    return transNode.take();     
}

void SSphere::createBobbies()
{       
    SBobby* bobby = new SBobby();
    bobby->setBody(this);
    bobby->setRadius(mRadius);
    bobby->setPosition(osg::Vec3(0, 0, 0));    
    bobby->setAttachedBodyID(mBodyID);
    mBobbies.push_back(bobby);    
}
