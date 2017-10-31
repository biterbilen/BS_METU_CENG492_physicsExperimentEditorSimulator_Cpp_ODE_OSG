#include "sflatmirror.h"
#include "sosggeode.h"
#include "seditorcallback.h"

string SFlatMirror::mCounter = "1";

// SFlatMirror::SFlatMirror(SObject::Types t) : SObject(t, "Box")
SFlatMirror::SFlatMirror() : SObject(SObject::STATIC)
{    
    mObjectType = SObject::FLATMIRROR;
    mPosition.set(0,0,0);
    mThickness = 0.1;
    mWidth = 30;
    mLength = 50;    
    mCatBits = 1;    
    mColBits = 2;    
    
    mName = "FlatMirror" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

SFlatMirror::SFlatMirror(osg::Vec3 pos, float wid, float len) : SObject(SObject::STATIC)
{
    mObjectType = SObject::FLATMIRROR;
    mPosition = pos;
    mWidth = wid;
    mLength = len;
    mThickness = 0.1;
    mCatBits = 1;    
    mColBits = 2;    
    
    mName = "FlatMirror" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

string SFlatMirror::getCounter()
{
    return mCounter;
}

void SFlatMirror::setCounter(string counter)
{
    mCounter = counter;
}       
 
void SFlatMirror::setLength(float len)
{
    mLength = len;
}

float SFlatMirror::getLength() const
{
    return mLength;
}

void SFlatMirror::setWidth(float wid)
{
    mWidth = wid;
}

float SFlatMirror::getWidth() const
{
    return mWidth;
}

void SFlatMirror::getOffPosition()
{
    
}

void SFlatMirror::getOnPosition()
{
    
}

void SFlatMirror::getOffDimension()
{
    
}

void SFlatMirror::getOnDimension()
{
    
}
       
void SFlatMirror::createSimObject(dWorldID world, dSpaceID space)
{
    mBodyID = 0;
    mGeomID = dCreateBox(space, mThickness, mWidth, mLength);
    dGeomSetPosition(mGeomID, mPosition.x(), mPosition.y(), mPosition.z());
    dGeomSetBody(mGeomID, mBodyID);      

    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(mGeomID, quat);
}

osg::Node * SFlatMirror::createVisualObject()
{
    // Set up a StateSet to make the mirror transparent
    osg::StateSet * flatMirrorState = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 1.0f, 1.0f, 1.0f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    flatMirrorState->setAttribute( material );

    osg::ShapeDrawable *flatMirrorDraw;
    osg::Box * flatMirror = new osg::Box(mPosition, mThickness, mWidth, mLength);
    flatMirror->setRotation(mOrientation);
    flatMirrorDraw = new osg::ShapeDrawable(flatMirror);    
    flatMirrorDraw->setStateSet(flatMirrorState);

    osg::Geode *geode = new osg::Geode();
    geode->addDrawable(flatMirrorDraw);
    
    return geode;
    
}

void SFlatMirror::createEditorObject() 
{
    init();
    
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
    
    // Set up a StateSet to make the mirror transparent
    osg::StateSet * flatMirrorState = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 1.0f, 1.0f, 1.0f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    flatMirrorState->setAttribute( material );

    /**
    * Create a drawable shape
    */
    osg::ShapeDrawable *flatMirrorDraw;
    osg::Box * flatMirror = new osg::Box(osg::Vec3(0,0,0), mThickness, mWidth, mLength);
    flatMirror->setRotation(mOrientation);
    flatMirrorDraw = new osg::ShapeDrawable(flatMirror);
    flatMirrorDraw->setStateSet(flatMirrorState);

    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(flatMirrorDraw);
    geode->setSObject(this);
        
    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}
