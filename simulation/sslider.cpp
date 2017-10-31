#include "sslider.h"

#include "ssliderjointsimcallback.h"
#include "ssliderjointeditorcallback.h"

string SSlider::mCounter = "1";

SSlider::SSlider(): SJoint()
{
    mType = SLIDER;
    mAxis.set(1, 0, 0);
    
    mName = "Slider" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void SSlider::createSimulationJoint(dWorldID worldid, dJointGroupID jointgroupid)
{       
    mJointID = dJointCreateSlider(worldid, 0);
    dJointAttach(mJointID, mBody1->getBodyID(), mBody2->getBodyID());
    dJointSetSliderAxis(mJointID, mAxis.x(), mAxis.y(), mAxis.z());
    
    //set the parameters
    if(mLoStopBool)
        dJointSetSliderParam(mJointID, dParamLoStop, mLoStop);
    if(mHiStopBool)
        dJointSetSliderParam(mJointID, dParamHiStop, mHiStop);
    if(mVelBool)
        dJointSetSliderParam(mJointID, dParamVel, mVel);
    if(mFMaxBool)
        dJointSetSliderParam(mJointID, dParamFMax, mFMax);    
}    

void SSlider::setAxis(osg::Vec3 a)
{
	mAxis = a;    
}

osg::Vec3 SSlider::getAxis() 
{
	return mAxis;
}

string SSlider::getCounter(){
    return mCounter;
}

void SSlider::setCounter(string s){
    mCounter = s;
}

osg::Node * SSlider::createVisualJoint() 
{
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 0.2f, 1.0f, 0.2f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    state->setAttribute( material );

    // vertical axis        
    osg::Cylinder * cylinder1 = new osg::Cylinder(osg::Vec3(0, 0, 0), 0.2, 1);    
    
    osg::ShapeDrawable* drawable1 = new osg::ShapeDrawable(cylinder1);
    drawable1->setStateSet( state );
    
    osg::Geode * geode1 = new osg::Geode();        
    geode1->addDrawable(drawable1);    
    
    osg::Transform *transNode1 = new osg::MatrixTransform();
    transNode1->addChild(geode1);
    SSliderJointSimCallback *jointSimCallback1 = new SSliderJointSimCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    // horizontal axis    
    osg::Cylinder * cylinder2 = new osg::Cylinder(osg::Vec3(0, 0, 0), 0.1, 1);    
    
    osg::ShapeDrawable* drawable2 = new osg::ShapeDrawable(cylinder2);
    drawable2->setStateSet( state );    
    
    osg::Geode * geode2 = new osg::Geode();
    geode2->addDrawable(drawable2);        

    osg::Transform *transNode2 = new osg::MatrixTransform();
    transNode2->addChild(geode2);
    SSliderJointSimCallback *jointSimCallback2 = new SSliderJointSimCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);

    // set the transNode to return    
    osg::Transform *transNode3 = new osg::PositionAttitudeTransform();
    transNode3->addChild(transNode1);
    transNode3->addChild(transNode2);
        
    return transNode3;   
}

void SSlider::createEditorJoint() 
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

    // vertical axis        
    osg::Cylinder * cylinder1 = new osg::Cylinder(osg::Vec3(0, 0, 0), 0.2 , 1);    
    
    osg::ShapeDrawable* drawable1 = new osg::ShapeDrawable(cylinder1);
    drawable1->setStateSet( state );
    
    osg::Geode * geode1 = new osg::Geode();        
    geode1->addDrawable(drawable1);    
    
    osg::Transform *transNode1 = new osg::MatrixTransform();
    transNode1->addChild(geode1);
    SSliderJointEditorCallback *jointSimCallback1 = new SSliderJointEditorCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    // horizontal axis    
    osg::Cylinder * cylinder2 = new osg::Cylinder(osg::Vec3(0, 0, 0), 0.1 ,1);    
    
    osg::ShapeDrawable* drawable2 = new osg::ShapeDrawable(cylinder2);
    drawable2->setStateSet( state );    
    
    osg::Geode * geode2 = new osg::Geode();
    geode2->addDrawable(drawable2);        

    osg::Transform *transNode2 = new osg::MatrixTransform();
    transNode2->addChild(geode2);
    SSliderJointEditorCallback *jointSimCallback2 = new SSliderJointEditorCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);

    mPosAttTransNode->addChild(transNode1);
    mPosAttTransNode->addChild(transNode2);
}



