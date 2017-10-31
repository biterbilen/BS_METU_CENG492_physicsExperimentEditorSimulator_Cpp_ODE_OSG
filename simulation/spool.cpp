#include "spool.h"
#include "sosggeode.h"
#include "seditorcallback.h"

string SPool::mCounter = "1";

SPool::SPool() : SObject(SObject::STATIC)
{
    mObjectType = SObject::POOL;
    mDimensions.set(0,0,0);
    mPosition.set(0,0,0);
    mDepth = -1;
    mWaterLevel = -1;
    mLiquidDensity = 0.0;
    mName = "Pool" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}
SPool::SPool(osg::Vec3 pos, osg::Vec3 dim) : SObject(SObject::STATIC)
{
    mObjectType = SObject::POOL;

    mDimensions = dim;
    mPosition = pos;

    mDepth = dim.z()*80.0/100.0;
    mWaterLevel = dim.z()*80.0/100.0;
    mLiquidDensity = 1.0;
    
    mName = "Pool" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

string SPool::getCounter()
{
    return mCounter;
}

void SPool::setCounter(string counter)
{
    mCounter = counter;
}

/** Create the dynamics object in ODE */
void SPool::createSimObject(dWorldID world, dSpaceID space)
{
    mWaterLevel = mDepth + mPosition.z();
    
    osg::Vec3 pos;

    // Left wall
    pos = mPosition + osg::Vec3(-(mDimensions[1]/2)+0.05, 0, mDimensions.z()/2 +0.05);
    dGeomID geomID1 = dCreateBox(space, 0.1, mDimensions.y()-0.2, mDimensions.z());
    dGeomSetPosition(geomID1, pos.x(), pos.y(), pos.z());
    dGeomSetBody(geomID1, 0);

    // Front wall
    pos = mPosition + osg::Vec3(0, -(mDimensions[1]/2)+0.05 ,mDimensions.z()/2 +0.05);
    dGeomID geomID2 = dCreateBox(space, mDimensions.x(), 0.1, mDimensions.z());
    dGeomSetPosition(geomID2, pos.x(), pos.y(), pos.z());
    dGeomSetBody(geomID2, 0);

    // Right wall
    pos = mPosition + osg::Vec3((mDimensions[1]/2)-0.05, 0, mDimensions.z()/2 +0.05);
    dGeomID geomID3= dCreateBox(space, 0.1, mDimensions.y()-0.2, mDimensions.z());
    dGeomSetPosition(geomID3, pos.x(), pos.y(), pos.z());
    dGeomSetBody(geomID3, 0);

    // Back wall
    pos = mPosition + osg::Vec3(0, (mDimensions[1]/2 - 0.05), mDimensions.z()/2 +0.05);
    dGeomID geomID4 = dCreateBox(space, mDimensions.x(), 0.1, mDimensions.z());
    dGeomSetPosition(geomID4, pos.x(), pos.y(), pos.z());
    dGeomSetBody(geomID4, 0);

    // Ground
    pos = mPosition + osg::Vec3(0, 0, 0.06);
    dGeomID geomID5 = dCreateBox(space, mDimensions.x()-0.1, mDimensions.y()-0.1, 0.1);
    dGeomSetPosition(geomID5, pos.x(), pos.y(), pos.z());
    dGeomSetBody(geomID5, 0);

    // Liquid
    pos = mPosition + osg::Vec3(0, 0, mDepth/2+0.05);
    mGeomID = dCreateBox(space, mDimensions.x()-0.05, mDimensions.y()-0.05, mDepth-0.06);
    dGeomSetPosition(mGeomID, pos.x(), pos.y(), pos.z());
    dGeomSetBody(mGeomID, 0);
}

/** Create the visual object for simulation */
osg::Node * SPool::createVisualObject()
{
    mWaterLevel = mDepth + mPosition.z();
    
    osg::Vec3 pos;

    // OSG related stuff
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
    
    // Set up a StateSet to make the cube red
    osg::StateSet * groundState = new osg::StateSet();
    osg::Material * groundMaterial = new osg::Material();
    osg::Vec4 ground( 0.5f, 0.5f, 0.5f, 1.0f );
    groundMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, ground );
    groundState->setAttribute( groundMaterial );

    // Ground
    pos = mPosition + osg::Vec3(0, 0, 0.06);
    osg::ShapeDrawable * boxDraw5;
    boxDraw5 = new osg::ShapeDrawable(new osg::Box(pos, mDimensions.x()-0.1, mDimensions.y()-0.1, 0.1), hints);
    boxDraw5->setStateSet( groundState );


    // Set up a StateSet to make the water transparent
    osg::StateSet * waterState = new osg::StateSet();
    osg::Material * blueMaterial = new osg::Material();
    osg::Vec4 blue( 0.5f, 0.5f, 1.0f, 1.0f );
    blueMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, blue );
    blueMaterial->setTransparency(osg::Material::FRONT_AND_BACK, 0.5);
    waterState->setRenderBinDetails( 11, "DepthSortedBin");
    waterState->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    waterState->setMode(GL_BLEND, osg::StateAttribute::ON);
    waterState->setAttribute( blueMaterial );

    // Liquid
    pos = mPosition +osg::Vec3(0, 0, mDepth/2+0.05);
    osg::ShapeDrawable * boxDraw6;
    boxDraw6 = new osg::ShapeDrawable(new osg::Box(pos, mDimensions.x()-0.05, mDimensions.y()-0.05, mDepth-0.06), hints);
    boxDraw6->setStateSet( waterState );

    // Set up a StateSet
    osg::StateSet * boxState = new osg::StateSet();
    osg::Material * grayMaterial = new osg::Material();
    osg::Vec4 gray( 0.8f, 0.8f, 1.0f, 1.0f );
    grayMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, gray );
    grayMaterial->setTransparency(osg::Material::FRONT, 0.4);
    boxState->setRenderBinDetails( 11, "DepthSortedBin");
    boxState->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    boxState->setMode(GL_BLEND, osg::StateAttribute::ON);
    boxState->setAttribute( grayMaterial );

    // Left wall
    pos = mPosition + osg::Vec3(-(mDimensions.x()/2)+0.05, 0, mDimensions.z()/2 +0.05);
    osg::ShapeDrawable * boxDraw1;
    boxDraw1 = new osg::ShapeDrawable(new osg::Box(pos, 0.1, mDimensions.y()-0.2, mDimensions.z()), hints);
    boxDraw1->setStateSet( boxState );

    // Front wall
    pos = mPosition +osg::Vec3(0, -(mDimensions.y()/2)+0.05 ,mDimensions.z()/2 +0.05);
    osg::ShapeDrawable * boxDraw2;
    boxDraw2 = new osg::ShapeDrawable(new osg::Box(pos, mDimensions.x(), 0.1, mDimensions.z()), hints);
    boxDraw2->setStateSet( boxState );

    // Right wall
    pos = mPosition + osg::Vec3((mDimensions.x()/2)-0.05, 0, mDimensions.z()/2 +0.05);
    osg::ShapeDrawable * boxDraw3;
    boxDraw3 = new osg::ShapeDrawable(new osg::Box(pos, 0.1, mDimensions.y()-0.2, mDimensions.z()), hints);
    boxDraw3->setStateSet( boxState );

    // Back wall
    pos = mPosition + osg::Vec3(0, (mDimensions.y()/2 - 0.05), mDimensions.z()/2 +0.05);
    osg::ShapeDrawable * boxDraw4;
    boxDraw4 = new osg::ShapeDrawable(new osg::Box(pos, mDimensions.x(), 0.1, mDimensions.z()), hints);
    boxDraw4->setStateSet( boxState );

    // Create a Geometry Node, and add the drawable to that node
    osg::Geode * geode = new osg::Geode();
    geode->addDrawable(boxDraw1);
    geode->addDrawable(boxDraw2);
    geode->addDrawable(boxDraw3);
    geode->addDrawable(boxDraw4);
    geode->addDrawable(boxDraw5);
    geode->addDrawable(boxDraw6);

    /**
    * Create a Transform Node, so that we can set up a NodeCallback which will
    * transform and rotate the child node in each drawing of the scene
    */
    osg::PositionAttitudeTransform * transNode = new osg::PositionAttitudeTransform();
    transNode->addChild(geode);
//     
//     transNode->setAttitude(mOrientation);
//     transNode->setPosition(mPosition);

        SSimCallback * odeCallBack = new SSimCallback(mGeomID);
        transNode->setUpdateCallback( odeCallBack );

    return geode;
}

/** Create the visual object for the editor */
void SPool::createEditorObject()
{
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }

    /** Below we create the new child nodes and add them to the transform node */
 
    osg::Vec3 pos;
    
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);
    
    // Set up a StateSet for the ground
    osg::StateSet * groundState = new osg::StateSet();
    osg::Material * groundMaterial = new osg::Material();
    osg::Vec4 ground( 0.5f, 0.5f, 0.5f, 1.0f );
    groundMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, ground );
    groundState->setAttribute( groundMaterial );

    // Ground
    pos = osg::Vec3(0, 0, 0.06);
    osg::ShapeDrawable * boxDraw5;
    boxDraw5 = new osg::ShapeDrawable(new osg::Box(pos, mDimensions.x()-0.1, mDimensions.y()-0.1, 0.1), hints);
    boxDraw5->setStateSet( groundState );


    // Set up a StateSet to make the water transparent
    osg::StateSet * waterState = new osg::StateSet();
    osg::Material * blueMaterial = new osg::Material();
    osg::Vec4 blue( 0.5f, 0.5f, 1.0f, 1.0f );
    blueMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, blue );
    blueMaterial->setTransparency(osg::Material::FRONT_AND_BACK, 0.5);
    waterState->setMode(GL_BLEND, osg::StateAttribute::ON);
    waterState->setAttribute( blueMaterial );

    // Liquid
    pos = osg::Vec3(0, 0, mDepth/2+0.05);
    osg::ShapeDrawable * boxDraw6;
    boxDraw6 = new osg::ShapeDrawable(new osg::Box(pos, mDimensions.x()-0.05, mDimensions.y()-0.05, mDepth-0.06), hints);
    boxDraw6->setStateSet( waterState );

    // Set up a StateSet
    osg::StateSet * boxState = new osg::StateSet();
    osg::Material * grayMaterial = new osg::Material();
    osg::Vec4 gray( 0.8f, 0.8f, 1.0f, 1.0f );
    grayMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, gray );
    grayMaterial->setTransparency(osg::Material::FRONT, 0.4);
    boxState->setRenderBinDetails( 11, "DepthSortedBin");
    boxState->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    boxState->setMode(GL_BLEND, osg::StateAttribute::ON);
    boxState->setAttribute( grayMaterial );

    // Left wall
    pos = osg::Vec3(-(mDimensions.x()/2)+0.05, 0, mDimensions.z()/2 +0.05);
    osg::ShapeDrawable * boxDraw1;
    boxDraw1 = new osg::ShapeDrawable(new osg::Box(pos, 0.1, mDimensions.y()-0.2, mDimensions.z()), hints);
    boxDraw1->setStateSet( boxState );

    // Front wall
    pos = osg::Vec3(0, -(mDimensions.y()/2)+0.05 ,mDimensions.z()/2 +0.05);
    osg::ShapeDrawable * boxDraw2;
    boxDraw2 = new osg::ShapeDrawable(new osg::Box(pos, mDimensions.x(), 0.1, mDimensions.z()), hints);
    boxDraw2->setStateSet( boxState );

    // Right wall
    pos = osg::Vec3((mDimensions.x()/2)-0.05, 0, mDimensions.z()/2 +0.05);
    osg::ShapeDrawable * boxDraw3;
    boxDraw3 = new osg::ShapeDrawable(new osg::Box(pos, 0.1, mDimensions.y()-0.2, mDimensions.z()), hints);
    boxDraw3->setStateSet( boxState );

    // Back wall
    pos = osg::Vec3(0, (mDimensions.y()/2 - 0.05), mDimensions.z()/2 +0.05);
    osg::ShapeDrawable * boxDraw4;
    boxDraw4 = new osg::ShapeDrawable(new osg::Box(pos, mDimensions.x(), 0.1, mDimensions.z()), hints);
    boxDraw4->setStateSet( boxState );

    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(boxDraw1);
    geode->addDrawable(boxDraw2);
    geode->addDrawable(boxDraw3);
    geode->addDrawable(boxDraw4);
    geode->addDrawable(boxDraw5);
    geode->addDrawable(boxDraw6);
    geode->setSObject(this);

    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);

    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

void SPool::setWaterLevel(float f)
{
    mWaterLevel = f;
}

void SPool::setLiquidDensity(float f)
{
    mLiquidDensity = f;
}

void SPool::setDimensions(osg::Vec3 v)
{
    mDimensions = v;
}

float SPool::getWaterLevel()
{
    return mWaterLevel;
}

float SPool::getLiquidDensity()
{
    return mLiquidDensity;
}

osg::Vec3 SPool::getDimensions()
{
    return mDimensions;
}
