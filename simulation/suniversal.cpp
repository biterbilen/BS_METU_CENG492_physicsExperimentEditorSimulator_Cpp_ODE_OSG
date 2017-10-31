#include "suniversal.h"

#include "suniversaljointsimcallback.h"
#include "suniversaljointeditorcallback.h"

string SUniversal::mCounter = "1"; 
// constructors

SUniversal::SUniversal(): SJoint()
{
	mType = UNIVERSAL;
    mName = "Universal" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    
    mAnchor.set(0,0,0);
    mAxis1.set(1,0,0);
    mAxis2.set(0,1,0);

    mLoStopBoolAxis2 = false;
    mHiStopBoolAxis2 = false;
    mVelBoolAxis2 = false;
    mFMaxBoolAxis2 = false;    
}

void SUniversal::setAxis1(osg::Vec3 a)
{
	mAxis1 = a;     
}

void SUniversal::setAxis2(osg::Vec3 a)
{	
	mAxis2 = a;     
}

void SUniversal::setAnchor(osg::Vec3 a)
{	
	mAnchor = a; 
}

osg::Vec3 SUniversal::getAxis1()
{	
	return mAxis1; 
}

osg::Vec3 SUniversal::getAxis2()
{	
	return mAxis2; 
}

osg::Vec3 SUniversal::getAnchor()
{	
	return mAnchor; 
}

void SUniversal::createSimulationJoint(dWorldID worldid, dJointGroupID jointgroupid) 
{
    mJointID = dJointCreateUniversal(worldid, 0);
    dJointAttach(mJointID, mBody1->getBodyID(), mBody2->getBodyID());
    dJointSetUniversalAnchor(mJointID, mAnchor.x(), mAnchor.y(), mAnchor.z());
    dJointSetUniversalAxis1(mJointID, mAxis1.x(), mAxis1.y(), mAxis1.z());
    dJointSetUniversalAxis2(mJointID, mAxis2.x(), mAxis2.y(), mAxis2.z());
    
    //set the parameters
    if(mLoStopBool)
        dJointSetUniversalParam(mJointID, dParamLoStop, mLoStop);
    if(mHiStopBool)
        dJointSetUniversalParam(mJointID, dParamHiStop, mHiStop);
    if(mVelBool)
        dJointSetUniversalParam(mJointID, dParamVel, mVel);
    if(mFMaxBool)
        dJointSetUniversalParam(mJointID, dParamFMax, mFMax);
    if(mLoStopBoolAxis2)
        dJointSetUniversalParam(mJointID, dParamLoStop2, mLoStopAxis2);
    if(mHiStopBoolAxis2)
        dJointSetUniversalParam(mJointID, dParamHiStop2, mHiStopAxis2);
    if(mVelBoolAxis2)
        dJointSetUniversalParam(mJointID, dParamVel2, mVelAxis2);
    if(mFMaxBoolAxis2)
        dJointSetUniversalParam(mJointID, dParamFMax2, mFMaxAxis2);
}

string SUniversal::getCounter(){
        return mCounter;
}
void SUniversal::setCounter(string s){
        mCounter = s;
}

osg::Node * SUniversal::createVisualJoint() 
{
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 0.2f, 1.0f, 0.2f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    state->setAttribute( material );
    
    // horizantal axis    
    osg::Geometry *geom1=new osg::Geometry();
    osg::Vec3Array *vertices1=new osg::Vec3Array();

    osg::Vec3 end1 = getBody1()->getPosition();    
    float len1 = (end1-mAnchor).length();    
    
    vertices1->push_back(osg::Vec3(0,0,0));
    vertices1->push_back(osg::Vec3(len1,0,0));
    geom1->setVertexArray(vertices1);
        
    osg::Vec4Array *colorArray = new osg::Vec4Array();
    colorArray->push_back(osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ));
    geom1->setColorArray(colorArray);
    geom1->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    
    osg::Cylinder * cylinder1 = new osg::Cylinder(osg::Vec3(0,0,0), 0.05 ,0.4);
    
    osg::ShapeDrawable* drawable1 = new osg::ShapeDrawable(cylinder1);
    drawable1->setStateSet( state );

    osg::Geode * geode1 = new osg::Geode();    
    geode1->addDrawable(drawable1);
    geode1->addDrawable(geom1);

    osg::Transform *transNode1 = new osg::PositionAttitudeTransform();
    transNode1->addChild(geode1);
    SUniversalJointSimCallback *jointSimCallback1 = new SUniversalJointSimCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    // vertical axis
    osg::Geometry *geom2=new osg::Geometry();
    osg::Vec3Array *vertices2=new osg::Vec3Array();   
   
    osg::Vec3 end2 = getBody2()->getPosition();    
    float len2 = (end2-mAnchor).length();    
    
    vertices2->push_back(osg::Vec3(0,0,0));
    vertices2->push_back(osg::Vec3(len2,0,0));
    geom2->setVertexArray(vertices2);
        
    geom2->setColorArray(colorArray);
    geom2->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    
    osg::Cylinder * cylinder2 = new osg::Cylinder(osg::Vec3(0,0,0), 0.05 ,0.4);
        
    osg::ShapeDrawable* drawable2 = new osg::ShapeDrawable(cylinder2);
    drawable2->setStateSet( state );    

    osg::Geode * geode2 = new osg::Geode();
    geode2->addDrawable(drawable2);
    geode2->addDrawable(geom2);    
    
    osg::Transform *transNode2 = new osg::PositionAttitudeTransform();
    transNode2->addChild(geode2);
    SUniversalJointSimCallback *jointSimCallback2 = new SUniversalJointSimCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);
        
    osg::Transform *transNode3 = new osg::PositionAttitudeTransform();
    transNode3->addChild(transNode1);
    transNode3->addChild(transNode2);
    
    return transNode3;    
}

void SUniversal::createEditorJoint() 
{
    /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
    
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 0.2f, 1.0f, 0.2f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    state->setAttribute( material );
    
    // horizantal axis    
    osg::Geometry *geom1=new osg::Geometry();
    osg::Vec3Array *vertices1=new osg::Vec3Array();

    float len1 = (mBody1->getPosition()-mAnchor).length();
    
    vertices1->push_back(osg::Vec3(0,0,0));
    vertices1->push_back(osg::Vec3(len1,0,0));
    geom1->setVertexArray(vertices1);
        
    osg::Vec4Array *colorArray = new osg::Vec4Array();
    colorArray->push_back(osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ));
    geom1->setColorArray(colorArray);
    geom1->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    
    osg::Cylinder * cylinder1 = new osg::Cylinder(osg::Vec3(0,0,0), 0.05 ,0.4);
    
    osg::ShapeDrawable* drawable1 = new osg::ShapeDrawable(cylinder1);
    drawable1->setStateSet( state );

    osg::Geode * geode1 = new osg::Geode();    
    geode1->addDrawable(drawable1);
    geode1->addDrawable(geom1);

    osg::Transform *transNode1 = new osg::PositionAttitudeTransform();
    transNode1->addChild(geode1);
    SUniversalJointEditorCallback *jointSimCallback1 = new SUniversalJointEditorCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    // vertical axis
    osg::Geometry *geom2=new osg::Geometry();
    osg::Vec3Array *vertices2=new osg::Vec3Array();   
   
    float len2 = (mBody2->getPosition()-mAnchor).length();    
    
    vertices2->push_back(osg::Vec3(0,0,0));
    vertices2->push_back(osg::Vec3(len2,0,0));
    geom2->setVertexArray(vertices2);
        
    geom2->setColorArray(colorArray);
    geom2->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    
    osg::Cylinder * cylinder2 = new osg::Cylinder(osg::Vec3(0,0,0), 0.05 ,0.4);
        
    osg::ShapeDrawable* drawable2 = new osg::ShapeDrawable(cylinder2);
    drawable2->setStateSet( state );    

    osg::Geode * geode2 = new osg::Geode();
    geode2->addDrawable(drawable2);
    geode2->addDrawable(geom2);    
    
    osg::Transform *transNode2 = new osg::PositionAttitudeTransform();
    transNode2->addChild(geode2);
    SUniversalJointEditorCallback *jointSimCallback2 = new SUniversalJointEditorCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);
        
    mPosAttTransNode->addChild(transNode1);
    mPosAttTransNode->addChild(transNode2);
}



