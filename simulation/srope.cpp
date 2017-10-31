#include "srope.h"

string SRope::mCounter = "1";

SRope::SRope() : SObject(SObject::DYNAMIC)
{
    mObjectType = SObject::ROPE;        
    mName = "Rope" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);    
    mStartObject = 0;
    mEndObject = 0;
    mStartJointID = 0;
    mEndJointID = 0;
    mStart.set(0.5, 0, 3);
    mEnd.set(0, 0, 1);
    mRadius = 0.05;  
    mPosition = mStart;
    mDirection = mEnd - mStart;
    mLength = mDirection.length();      
    mNumberOfSpheres = ceilf((0.75*mLength) / mRadius);
}

string SRope::getCounter()
{
    return mCounter;
}

void SRope::setCounter(string counter)
{
    mCounter = counter;
} 

void SRope::setEndObject(SObject* o)
{
    mEndObject = o;
}    

void SRope::setStartObject(SObject* o)
{
    mStartObject = o;
}

void SRope::setEnd(osg::Vec3 v) 
{     
    mEnd = v;     
    mDirection = mEnd - mStart;
    mLength = mDirection.length();    
    mNumberOfSpheres = ceilf((0.75*mLength) / mRadius);
}

void SRope::setStart(osg::Vec3 v) 
{ 
    mStart = v;     
    mPosition = mStart;     
    mDirection = mEnd - mStart;
    mLength = mDirection.length();    
    mNumberOfSpheres = ceilf((0.75*mLength) / mRadius);
}

void SRope::setRadius(float rad)
{
    mRadius = rad; 
    mDirection = mEnd - mStart;
    mLength = mDirection.length();    
    mNumberOfSpheres = ceilf((0.75*mLength) / mRadius);
}

float SRope::getRadius()
{
    return mRadius;
}
void SRope::init()
{
    mGeoms.resize(mNumberOfSpheres);
    mBodies.resize(mNumberOfSpheres);
    mJoints.resize(mNumberOfSpheres-1);
}
   
void SRope::setMass(float m) 
{
    ;
}
    
void SRope::createSimObject(dWorldID world, dSpaceID space) 
{    
    osg::Vec3 u = mDirection;    
    u /= mNumberOfSpheres;    
    
    dMassSetSphere(&mMass, 0.01, mRadius);      
    
    osg::Vec3 p = mStart + u/2;    

    for (int i = 0 ; i < mNumberOfSpheres ; i++) 
    {
        mBodies[i] = dBodyCreate(world);
        dBodySetPosition(mBodies[i], p.x(), p.y(), p.z());
        dBodySetMass(mBodies[i], &mMass);
        mGeoms[i] = dCreateSphere (space, mRadius);
        dGeomSetBody(mGeoms[i], mBodies[i]);        
        p += u;
    }
    mGeomID = mGeoms[0];
    mBodyID = mBodies[0];
    mEndGeomID = mGeoms[mNumberOfSpheres-1];
    mEndBodyID = mBodies[mNumberOfSpheres-1];
    
    u = mDirection;
    u /= mNumberOfSpheres;
    
    p = mPosition + u;    
      
    for (int i = 0 ; i < mNumberOfSpheres - 1 ; i++) 
    {
        mJoints[i] = dJointCreateBall(world, 0);
        dJointAttach(mJoints[i], mBodies[i], mBodies[i+1]);        
        dJointSetBallAnchor(mJoints[i], p.x(), p.y(), p.z());    
        p += u;        
    }    
    
    osg::Vec3 pos;
    
    if(mStartObject != 0)
    {        
        mStartJointID = dJointCreateBall(world, 0);
        dJointAttach(mStartJointID, mBodyID, mStartObject->getBodyID());
        pos = (mStart + mStartObject->getPosition()) / 2;
        dJointSetBallAnchor(mStartJointID, pos.x(), pos.y(), pos.z());    
    }
    
    if(mEndObject != 0)
    {        
        mEndJointID = dJointCreateBall(world, 0);
        dJointAttach(mEndJointID, mEndBodyID, mEndObject->getBodyID());
        pos = (mEnd + mEndObject->getPosition()) / 2;
        dJointSetBallAnchor(mEndJointID, pos.x(), pos.y(), pos.z());  
    }
     
    dQuaternion quat;
    quat[0] = mOrientation[3];
    quat[1] = mOrientation[0];
    quat[2] = mOrientation[1];
    quat[3] = mOrientation[2];
    dGeomSetQuaternion(mGeomID, quat);
}
osg::Node * SRope::createVisualObject()
{    
    // OSG related stuff
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet
    osg::StateSet * ropeState = new osg::StateSet();
    osg::Material * redMaterial = new osg::Material();
    osg::Vec4 color( 1.0f, 0.2f, 0.6f, 1.0f );
    redMaterial->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    ropeState->setAttribute( redMaterial);

     /** Get necessary osg nodes */
    vector< osg::Sphere * > sphere;
    vector< osg::ShapeDrawable * > ropeDraw;
    vector<osg::Geode * > geode;
    vector<osg::Transform * > transNode;
    osg::Group * group = new osg::Group();
    
    sphere.resize(mNumberOfSpheres*2-1);
    ropeDraw.resize(mNumberOfSpheres*2-1);
    geode.resize(mNumberOfSpheres*2-1);
    transNode.resize(mNumberOfSpheres*2-1);    
  
    for (int i = 0 ; i < mNumberOfSpheres ; i++) 
    {       
        sphere[i] = new osg::Sphere(osg::Vec3(0, 0, 0), mRadius);        
        ropeDraw[i] = new osg::ShapeDrawable(sphere[i], hints);
        ropeDraw[i]->setStateSet( ropeState );

        geode[i] = new osg::Geode();
        geode[i]->addDrawable(ropeDraw[i]);

        transNode[i] = new osg::PositionAttitudeTransform();
        transNode[i]->addChild(geode[i]);
        
        /** Create a callback for transform node with the corresponding geomID */
        SRopeSimCallback * odeCallBack = new SRopeSimCallback(mGeoms[i], mGeoms[i]);
        transNode[i]->setUpdateCallback( odeCallBack );
        
        /** Add the transform node to the upper most group node */
        group->addChild(transNode[i]);      
    } 
    
    for (int i = 0 ; i < mNumberOfSpheres-1 ; i++) 
    {       
        sphere[i+mNumberOfSpheres] = new osg::Sphere(osg::Vec3(0, 0, 0), mRadius);        
        ropeDraw[i+mNumberOfSpheres] = new osg::ShapeDrawable(sphere[i+mNumberOfSpheres], hints);
        ropeDraw[i+mNumberOfSpheres]->setStateSet( ropeState );

        geode[i+mNumberOfSpheres] = new osg::Geode();
        geode[i+mNumberOfSpheres]->addDrawable(ropeDraw[i+mNumberOfSpheres]);

        transNode[i+mNumberOfSpheres] = new osg::PositionAttitudeTransform();
        transNode[i+mNumberOfSpheres]->addChild(geode[i+mNumberOfSpheres]);
        
        /** Create a callback for transform node with the corresponding geomID */
        SRopeSimCallback * odeCallBack = new SRopeSimCallback(mGeoms[i], mGeoms[i+1]);
        transNode[i+mNumberOfSpheres]->setUpdateCallback( odeCallBack );
        
        /** Add the transform node to the upper most group node */
        group->addChild(transNode[i+mNumberOfSpheres]);        
    } 
    
    return group;
}

void SRope::createEditorObject()
{       

}

