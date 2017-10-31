#include "scylinder.h"
#include "sosggeode.h"
#include "seditorcallback.h"

string SCylinder::mCounter = "1";

SCylinder::SCylinder(SObject::Types t) : SObject::SObject(t)
{
    mObjectType = SObject::CYLINDER;
    mName = "Cylinder" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}


float SCylinder::getRadius() const
{
    return mRadius;
}

void SCylinder::setRadius(float r)
{
    mRadius = r;    
}

string SCylinder::getCounter()
{
    return mCounter;
}

void SCylinder::setCounter(string counter)
{
    mCounter = counter;
} 

void SCylinder::setHeight(float h)
{
    mHeight = h;    
}

float SCylinder::getHeight() const
{
    return mHeight;
}

void SCylinder::setMass(float m)
{
    // Fill the mMass structure with corresponding mass information
//void dMassSetCylinderTotal (dMass *, dReal total_mass, int direction,
                       //dReal radius, dReal length);    
    // FIXME DIRECTION SHOULD BE REGULATED AcCORDING TO THE ORIENTATION BUT HOW? direction (1=x, 2=y, 3=z). ???????????
    dMassSetCylinderTotal(&mMass, m, 3, mRadius, mHeight);
    
}

float SCylinder::getVolume()
{
    return (M_PI * mRadius * mRadius * mHeight);
}
    
float SCylinder::getDensity()
{
    return (this->getMass() / this->getVolume());
}

void SCylinder::createSimObject(dWorldID worldid, dSpaceID spaceid)
{
    // ODE related stuff
    if (this->isDynamic()) 
    {
        mBodyID = dBodyCreate(worldid);
        dBodySetPosition(mBodyID, mPosition.x(), mPosition.y(), mPosition.z());
        dBodySetMass(mBodyID, &mMass);
        //TODO not capped 
        mGeomID = dCreateCCylinder(spaceid, mRadius, mHeight);
        dGeomSetBody(mGeomID, mBodyID);
    }
    else 
    {
        mBodyID = 0;
        //TODO not capped 
        mGeomID = dCreateCCylinder(spaceid, mRadius, mHeight);
        dGeomSetPosition(mGeomID, mPosition.x(), mPosition.y(), mPosition.z());
        dGeomSetBody(mGeomID, 0);      
    }
      
    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(mGeomID, quat);    
    
}

void SCylinder::createEditorObject() 
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

    // Set up a StateSet to make the cylinder color
    osg::StateSet * cylinderState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 color( 0.2f, 1.0f, 0.2f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    cylinderState->setAttribute( redMaterial );

    // Create a drawable shape
    osg:: ShapeDrawable * cylinderDraw;
    cylinderDraw = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0, 0, 0), mRadius, mHeight), hints);
    cylinderDraw->setStateSet( cylinderState );
    
    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(cylinderDraw);
    geode->setSObject(this);
    
    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

osg::Node * SCylinder::createVisualObject()
{

    // OSG related stuff
    osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
        
    // Set up a StateSet to make the CCylinder blue
    osg::ref_ptr<osg::StateSet> cylinderState = new osg::StateSet();
    osg::ref_ptr<osg::Material> blueMaterial = new osg::Material();
    osg::Vec4 blue( 0.2f, 0.2f, 1.0f, 1.0f );
    blueMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, blue );
    cylinderState->setAttribute( blueMaterial.get() );
    
    /*
    * Create a drawable shape of cylinder
    * Note that it is created in coordinates (0, 0, 0) that is the center of the cylinder
    * That's not so important because it will get it's position from ODE
    * with the help of the updateCallback we will set below...
    */
    osg::ref_ptr<osg:: ShapeDrawable> cylinderDraw;
    cylinderDraw = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0, 0, 0), mRadius, mHeight), hints.get());
    cylinderDraw->setStateSet( cylinderState.get() );
    
    // Create a Geometry node, and add the drawable to that node
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(cylinderDraw.get());
   
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
