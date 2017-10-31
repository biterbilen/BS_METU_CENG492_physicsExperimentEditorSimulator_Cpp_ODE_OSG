#include "shinge2.h"

#include "shinge2jointeditorcallback.h"
#include "shinge2jointsimcallback.h"

string SHinge2::mCounter = "1";
// constructors
SHinge2::SHinge2(): SJoint::SJoint() 
{
    mType = HINGE2;
    
    mName = "Hinge2-" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    
    mLoStopBoolAxis2 = false;
    mHiStopBoolAxis2 = false;
    mVelBoolAxis2 = false;
    mFMaxBoolAxis2 = false;
}
void SHinge2::setAnchor(osg::Vec3 a)
{
	mAnchor = a; 
}

osg::Vec3 SHinge2::getAnchor()
{
	return mAnchor; 
}

void SHinge2::setAxis1(osg::Vec3 a)
{
	mAxis1 = a;     
}

osg::Vec3 SHinge2::getAxis1()			
{
	return mAxis1; 
}

void SHinge2::setAxis2(osg::Vec3 a)
{
	mAxis2 = a;     
}

osg::Vec3 SHinge2::getAxis2()			
{
	return this->mAxis2; 
}

void SHinge2::createSimulationJoint(dWorldID worldid, dJointGroupID jointgroupid)
{    
    mJointID = dJointCreateHinge2(worldid, 0);   
    dJointAttach(mJointID, mBody1->getBodyID(), mBody2->getBodyID());   
    dJointSetHinge2Anchor(mJointID, mAnchor.x(), mAnchor.y(), mAnchor.z());   
    dJointSetHinge2Axis1(mJointID, mAxis1.x(), mAxis1.y(), mAxis1.z());   
    dJointSetHinge2Axis2(mJointID, mAxis2.x(), mAxis2.y(), mAxis2.z());
    
    //set the parameters    
    if(mLoStopBool)
        dJointSetHinge2Param(mJointID, dParamLoStop, mLoStop);
    if(mHiStopBool)
        dJointSetHinge2Param(mJointID, dParamHiStop, mHiStop);
    if(mVelBool)
        dJointSetHinge2Param(mJointID, dParamVel, mVel);
    if(mFMaxBool)
        dJointSetHinge2Param(mJointID, dParamFMax, mFMax);
    if(mLoStopBoolAxis2)
        dJointSetUniversalParam(mJointID, dParamLoStop2, mLoStopAxis2);
    if(mHiStopBoolAxis2)
        dJointSetUniversalParam(mJointID, dParamHiStop2, mHiStopAxis2);
    if(mVelBoolAxis2)
        dJointSetUniversalParam(mJointID, dParamVel2, mVelAxis2);
    if(mFMaxBoolAxis2)
        dJointSetUniversalParam(mJointID, dParamFMax2, mFMaxAxis2);
}
    
string SHinge2::getCounter(){
    return mCounter;
}

void SHinge2::setCounter(string s){
    mCounter = s;
}

osg::Node * SHinge2::createVisualJoint() 
{
    osg::Vec3 body1Pos = mBody1->getPosition();
    float A = mAxis1.x();    
    float B = mAxis1.y();    
    float C = mAxis1.z();    
    float D = -(A*body1Pos.x() + B*body1Pos.y() + C*body1Pos.z());        
    float f = A*mAnchor.x() + B*mAnchor.y() + C*mAnchor.z() + D;       
    float u = -f/(A*mAxis1.x() + B*mAxis1.y() + C*mAxis1.z());        
    osg::Vec3 p = mAnchor + mAxis1 * u;        
    float len0 = (body1Pos - p).length();
    float len1 = abs(f)/mAxis1.length();    
    float len2 = (mAnchor - mBody2->getPosition()).length();    
    
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 0.6f, 0.8f, 0.2f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    state->setAttribute( material );
    
    osg::StateSet * state2 = new osg::StateSet();
    osg::Material * material2 = new osg::Material();
    osg::Vec4 color2( 0.5f, 0.5f, 0.5f, 1.0f );
    material2->setDiffuse( osg::Material::FRONT_AND_BACK, color2 );
    state2->setAttribute( material2 );
    
    // vertical axis    
    osg::Geometry *geom1=new osg::Geometry();
    osg::Vec3Array *vertices1=new osg::Vec3Array();

    vertices1->push_back(osg::Vec3(0,0,len1));
    vertices1->push_back(osg::Vec3(len0,0,len1));   
    
    geom1->setVertexArray(vertices1);
    geom1->setStateSet(state2);
        
    osg::Vec4Array *colorArray = new osg::Vec4Array();
    colorArray->push_back(osg::Vec4( 0.5f, 0.5f, 0.5f, 1.0f ));    
    geom1->setColorArray(colorArray);
    geom1->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    
    osg::Cylinder * cylinder1 = new osg::Cylinder(osg::Vec3(0, 0, len1/2), 0.1 ,len1);        
    osg::ShapeDrawable* drawable1 = new osg::ShapeDrawable(cylinder1);
    drawable1->setStateSet( state );
    
    osg::Sphere * sphere = new osg::Sphere(osg::Vec3(0, 0, 0), 0.2);    
    osg::ShapeDrawable* drawable3 = new osg::ShapeDrawable(sphere);
    drawable3->setStateSet( state );
    
    osg::Sphere * sphere2= new osg::Sphere(osg::Vec3(0, 0, len1), 0.2);    
    osg::ShapeDrawable* drawable4 = new osg::ShapeDrawable(sphere2);
    drawable4->setStateSet( state );

    osg::Geode * geode1 = new osg::Geode();    
    geode1->addDrawable(drawable1);
    geode1->addDrawable(drawable3);
    geode1->addDrawable(drawable4);
    geode1->addDrawable(geom1);

    osg::Transform *transNode1 = new osg::PositionAttitudeTransform();
    transNode1->addChild(geode1);
    SHinge2JointSimCallback *jointSimCallback1 = new SHinge2JointSimCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    // horizontal axis
    osg::Cylinder * cylinder2 = new osg::Cylinder(osg::Vec3(0,0,0), 0.1 ,len2);
        
    osg::ShapeDrawable* drawable2 = new osg::ShapeDrawable(cylinder2);
    drawable2->setStateSet( state );    

    osg::Geode * geode2 = new osg::Geode();
    geode2->addDrawable(drawable2);    
    
    osg::Transform *transNode2 = new osg::PositionAttitudeTransform();
    transNode2->addChild(geode2);
    SHinge2JointSimCallback *jointSimCallback2 = new SHinge2JointSimCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);
        
    osg::Transform *transNode3 = new osg::PositionAttitudeTransform();
    transNode3->addChild(transNode1);
    transNode3->addChild(transNode2);
    
    return transNode3;
}

void SHinge2::createEditorJoint() 
{
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
    
     osg::Vec3 body1Pos = mBody1->getPosition();
    float A = mAxis1.x();    
    float B = mAxis1.y();    
    float C = mAxis1.z();    
    float D = -(A*body1Pos.x() + B*body1Pos.y() + C*body1Pos.z());        
    float f = A*mAnchor.x() + B*mAnchor.y() + C*mAnchor.z() + D;       
    float u = -f/(A*mAxis1.x() + B*mAxis1.y() + C*mAxis1.z());        
    osg::Vec3 p = mAnchor + mAxis1 * u;        
    float len0 = (body1Pos - p).length();
    float len1 = abs(f)/mAxis1.length();    
    float len2 = (mAnchor - mBody2->getPosition()).length();    
    
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 0.6f, 0.8f, 0.2f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    state->setAttribute( material );
    
    osg::StateSet * state2 = new osg::StateSet();
    osg::Material * material2 = new osg::Material();
    osg::Vec4 color2( 0.5f, 0.5f, 0.5f, 1.0f );
    material2->setDiffuse( osg::Material::FRONT_AND_BACK, color2 );
    state2->setAttribute( material2 );
    
    // vertical axis    
    osg::Geometry *geom1=new osg::Geometry();
    osg::Vec3Array *vertices1=new osg::Vec3Array();

    vertices1->push_back(osg::Vec3(0,0,len1));
    vertices1->push_back(osg::Vec3(len0,0,len1));   
    
    geom1->setVertexArray(vertices1);
    geom1->setStateSet(state2);
        
    osg::Vec4Array *colorArray = new osg::Vec4Array();
    colorArray->push_back(osg::Vec4( 0.5f, 0.5f, 0.5f, 1.0f ));    
    geom1->setColorArray(colorArray);
    geom1->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    
    osg::Cylinder * cylinder1 = new osg::Cylinder(osg::Vec3(0, 0, len1/2), 0.1 ,len1);        
    osg::ShapeDrawable* drawable1 = new osg::ShapeDrawable(cylinder1);
    drawable1->setStateSet( state );
    
    osg::Sphere * sphere = new osg::Sphere(osg::Vec3(0, 0, 0), 0.2);    
    osg::ShapeDrawable* drawable3 = new osg::ShapeDrawable(sphere);
    drawable3->setStateSet( state );
    
    osg::Sphere * sphere2= new osg::Sphere(osg::Vec3(0, 0, len1), 0.2);    
    osg::ShapeDrawable* drawable4 = new osg::ShapeDrawable(sphere2);
    drawable4->setStateSet( state );

    osg::Geode * geode1 = new osg::Geode();    
    geode1->addDrawable(drawable1);
    geode1->addDrawable(drawable3);
    geode1->addDrawable(drawable4);
    geode1->addDrawable(geom1);

    osg::Transform *transNode1 = new osg::PositionAttitudeTransform();
    transNode1->addChild(geode1);
    SHinge2JointEditorCallback *jointSimCallback1 = new SHinge2JointEditorCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    // horizontal axis
    osg::Cylinder * cylinder2 = new osg::Cylinder(osg::Vec3(0,0,0), 0.1 ,len2);
        
    osg::ShapeDrawable* drawable2 = new osg::ShapeDrawable(cylinder2);
    drawable2->setStateSet( state );    

    osg::Geode * geode2 = new osg::Geode();
    geode2->addDrawable(drawable2);    
    
    osg::Transform *transNode2 = new osg::PositionAttitudeTransform();
    transNode2->addChild(geode2);
    SHinge2JointEditorCallback *jointSimCallback2 = new SHinge2JointEditorCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);
        
    mPosAttTransNode->addChild(transNode1);
    mPosAttTransNode->addChild(transNode2);    
}
