#include "shinge.h"

#include "shingejointsimcallback.h"
#include "shingejointeditorcallback.h"

// constructors

string SHinge::mCounter = "1";

SHinge::SHinge(): SJoint::SJoint()
{
    mType = HINGE;
    mAnchor.set(0,0,0);
    mAxis.set(1, 0, 0);
    mName = "Hinge-" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void SHinge::setAnchor(osg::Vec3 a)
{
	mAnchor = a; 
}
void SHinge::setAxis(osg::Vec3 a)
{
	mAxis = a; 
}
osg::Vec3 SHinge::getAnchor()
{
	return mAnchor; 
}

osg::Vec3 SHinge::getAxis()			
{
	return mAxis; 
}

void SHinge::createSimulationJoint(dWorldID worldid, dJointGroupID jointgroupid)
{
    
    mJointID = dJointCreateHinge(worldid, 0);
    dJointAttach(mJointID, mBody1->getBodyID(), mBody2->getBodyID());
    dJointSetHingeAnchor(mJointID, mAnchor.x(), mAnchor.y(), mAnchor.z());
    dJointSetHingeAxis(mJointID, mAxis.x(), mAxis.y(), mAxis.z());        
    
    //set the parameters
    if(mLoStopBool)
        dJointSetHingeParam(mJointID, dParamLoStop, mLoStop);
    if(mHiStopBool)
        dJointSetHingeParam(mJointID, dParamHiStop, mHiStop);
    if(mVelBool)
        dJointSetHingeParam(mJointID, dParamVel, mVel);
    if(mFMaxBool)
        dJointSetHingeParam(mJointID, dParamFMax, mFMax);
}

osg::Node * SHinge::createVisualJoint() 
{   
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 0.2f, 1.0f, 0.2f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    state->setAttribute( material );
    
    // vertical axis body1   
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
    SHingeJointSimCallback *jointSimCallback1 = new SHingeJointSimCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    // horizontal axis
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
    SHingeJointSimCallback *jointSimCallback2 = new SHingeJointSimCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);
        
    osg::Transform *transNode3 = new osg::PositionAttitudeTransform();
    transNode3->addChild(transNode1);
    transNode3->addChild(transNode2);
    
    return transNode3;       
    
}

string SHinge::getCounter(){
    return mCounter;
}

void SHinge::setCounter(string s){
    mCounter = s;    
}

void SHinge::createEditorJoint() 
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
    
    // vertical axis body1   
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
    SHingeJointEditorCallback *jointSimCallback1 = new SHingeJointEditorCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    // horizontal axis
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
    SHingeJointEditorCallback *jointSimCallback2 = new SHingeJointEditorCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);
    
    mPosAttTransNode->addChild(transNode1);
    mPosAttTransNode->addChild(transNode2);
}
