#include "sinclinedplane.h"

#include "seditorcallback.h"

#define INCLINEDPLANETHICKNESS 0.01
#define DIRX 0
#define DIRY 1
#define DIRZ 0

string SInclinedPlane::mCounter = "1";
    
//TODO angle should be equal to or less than 90 degrees
SInclinedPlane::SInclinedPlane() : SObject(SObject::STATIC)
{
    mObjectType = SObject::INCLINEDPLANE;
    mPosition.set(0,0,0);
    mAngle = M_PI/6;
    mHeight = 3;
    mWidth = 5;
    
    mName = "InclinedPlane" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}
SInclinedPlane::SInclinedPlane(float a, float h, float w) : SObject(SObject::STATIC)
{
    mObjectType = SObject::INCLINEDPLANE;
    mPosition.set(0,0,0);
    mAngle = a;
    mHeight = h;
    mWidth = w;
    
    mName = "InclinedPlane" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}


SInclinedPlane::SInclinedPlane(float a, float h, float w, osg::Vec3 pos) : SObject(SObject::STATIC)
{
    mObjectType = SObject::INCLINEDPLANE;
    mPosition = pos;
    mAngle = a;
    mHeight = h;
    mWidth = w;
    
    mName = "InclinedPlane" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

string SInclinedPlane::getCounter()
{
    return mCounter;
}

void SInclinedPlane::setCounter(string counter)
{
    mCounter = counter;
} 

void SInclinedPlane::init()
{
    //FIXME control condition for angles to be in RADIANs
    mLen = mHeight / sin(mAngle);
    mProjXLen = mHeight / tan(mAngle);
    
    std::cout <<"mAngle:"<< mAngle <<" mHeight" << mHeight<<" mwid:"<<mWidth << " mlen:"<<mLen << " mProjXLen:"<< mProjXLen<< std::endl;
}

void SInclinedPlane::createSimObject(dWorldID world, dSpaceID space)
{
    osg::Vec3 pos;

    //ODE related stuff
    mBodyID = 0;

    //INCLINED ONE
    mGeomID = dCreateBox(space, mLen, mWidth, INCLINEDPLANETHICKNESS);

    pos = mPosition + osg::Vec3(mProjXLen/2, 0, mHeight/2);
    dGeomSetPosition(mGeomID, pos.x(), pos.y(), pos.z());

    osg::Quat q = osg::Quat(-mAngle, osg::Vec3(DIRX, DIRY, DIRZ));
    q = q * mOrientation;

    dQuaternion quato;
    quato[0] = q[3];
    quato[1] = q[0];
    quato[2] = q[1];
    quato[3] = q[2];
    dGeomSetQuaternion(mGeomID, quato);

    dGeomSetBody(mGeomID, mBodyID);

    //VERTICAL PLANE
    dGeomID b2 = dCreateBox(space, INCLINEDPLANETHICKNESS, mWidth, mHeight);

    pos = mPosition + osg::Vec3(mProjXLen, 0, mHeight/2);
    dGeomSetPosition(b2, mPosition.x() + mProjXLen , mPosition.y() , mPosition.z() + mHeight/2);

    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(b2, quat);

    dGeomSetBody(b2, mBodyID);
}

osg::Node * SInclinedPlane::createVisualObject()
{
    osg::Vec3 pos;

    // Set up a StateSet to make the capped cylinder color
    osg::StateSet * inclinedPlaneState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 color( 1.0f, 0.0f, 0.0f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    inclinedPlaneState->setAttribute( redMaterial );

    /** composite shape object with two boxes */
    osg::CompositeShape * compositeInclinedPlane;
    compositeInclinedPlane = new osg::CompositeShape();

    // INCLINED ONE
    pos = mPosition + osg::Vec3(mProjXLen/2.0, 0, mHeight/2.0);
    osg::Box *boxInc = new osg::Box(pos, mLen, mWidth, INCLINEDPLANETHICKNESS);

    osg::Quat quatr(-mAngle,osg::Vec3(DIRX,DIRY,DIRZ));
    boxInc->setRotation(quatr * mOrientation);
    compositeInclinedPlane->addChild(boxInc);

    // VERTICAL ONE
    pos =mPosition + osg::Vec3(mProjXLen, 0, mHeight/2.0);
    osg::Box *boxVer = new osg::Box(pos, INCLINEDPLANETHICKNESS, mWidth, mHeight);
    boxVer->setRotation(mOrientation);
    compositeInclinedPlane->addChild(boxVer);

    osg:: ShapeDrawable * inclinedPlaneDraw;
    inclinedPlaneDraw = new osg::ShapeDrawable(compositeInclinedPlane );
    inclinedPlaneDraw->setStateSet( inclinedPlaneState );

    // Create a Geometry Node, and add the drawable to that node
    osg::Geode * geode = new Geode();
    geode->addDrawable(inclinedPlaneDraw);

    osg::Transform * transNode = new osg::PositionAttitudeTransform();
    transNode->addChild(geode);

    return transNode;
}

void SInclinedPlane::createEditorObject()
{
    init();
    
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
        
    /** Below we create the new child nodes and add them to the transform node */

    osg::Vec3 pos;

    // Set up a StateSet to make the capped cylinder color
    osg::StateSet * inclinedPlaneState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 color( 1.0f, 0.0f, 0.0f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    inclinedPlaneState->setAttribute( redMaterial );

    /** composite shape object with two boxes */
    osg::CompositeShape * compositeInclinedPlane;
    compositeInclinedPlane = new osg::CompositeShape();

    // INCLINED ONE
    pos =osg::Vec3(mProjXLen/2.0, 0, mHeight/2.0);
    osg::Box *boxInc = new osg::Box(pos, mLen, mWidth, INCLINEDPLANETHICKNESS);

    osg::Quat quatr(-mAngle,osg::Vec3(DIRX,DIRY,DIRZ));
    boxInc->setRotation(quatr * mOrientation);
    compositeInclinedPlane->addChild(boxInc);

    // VERTICAL ONE
    pos =osg::Vec3(mProjXLen, 0, mHeight/2.0);
    osg::Box *boxVer = new osg::Box(pos, INCLINEDPLANETHICKNESS, mWidth, mHeight);
    boxVer->setRotation(mOrientation);
    compositeInclinedPlane->addChild(boxVer);

    osg:: ShapeDrawable * inclinedPlaneDraw;
    inclinedPlaneDraw = new osg::ShapeDrawable(compositeInclinedPlane );
    inclinedPlaneDraw->setStateSet( inclinedPlaneState );

    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(inclinedPlaneDraw);
    geode->setSObject(this);

    /** Add the geom into the transform node of this object. */
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}

