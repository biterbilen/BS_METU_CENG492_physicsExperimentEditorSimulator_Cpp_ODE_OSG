#include "sballsocket.h"

#include "sballsocketjointsimcallback.h"
#include "sballsocketjointeditorcallback.h"

string SBallSocket::mCounter = "1";

// constructors
SBallSocket::SBallSocket(): SJoint()
{
	mType = BALLSOCKET;
    mName = "BallSocket" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
}

void SBallSocket::setAnchor(osg::Vec3 a)	
{	
	mAnchor = a;     
}

osg::Vec3 SBallSocket::getAnchor()
{
	return mAnchor; 
}

void SBallSocket::createSimulationJoint(dWorldID world, dJointGroupID)
{        
    mJointID = dJointCreateBall(world, 0);
    dJointAttach(mJointID, mBody1->getBodyID(), mBody2->getBodyID());
    dJointSetBallAnchor(mJointID, mAnchor.x(), mAnchor.y(), mAnchor.z());    
}

string SBallSocket::getCounter(){
    return mCounter;
}

void SBallSocket::setCounter(string s){
    mCounter = s;
}

osg::Node * SBallSocket::createVisualJoint() 
{
    osg::StateSet * state = new osg::StateSet();
    osg::Material * material = new osg::Material();
    osg::Vec4 color( 0.2f, 1.0f, 0.2f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, color );
    state->setAttribute( material );
    
    osg::Sphere * anchor = new osg::Sphere(osg::Vec3(0,0,0), 0.2);
    
    osg::ShapeDrawable* drawable = new osg::ShapeDrawable(anchor);
    drawable->setStateSet( state );

    osg::StateSet * state2 = new osg::StateSet();
    osg::Material * material2 = new osg::Material();
    osg::Vec4 color2( 0.5f, 0.5f, 0.5f, 1.0f );
    material2->setDiffuse( osg::Material::FRONT_AND_BACK, color2 );
    state2->setAttribute( material2 );
    
    osg::Geometry *geom1=new osg::Geometry();
    osg::Vec3Array *vertices1=new osg::Vec3Array();

    osg::Vec3 end1 = getBody1()->getPosition();    
    float len1 = (end1-mAnchor).length();    
    
    vertices1->push_back(osg::Vec3(0,0,0));
    vertices1->push_back(osg::Vec3(0,0,len1));
    geom1->setVertexArray(vertices1);    
    geom1->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
    geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    geom1->setStateSet(state2);
    
    osg::Geode * geode1 = new osg::Geode();
    geode1->addDrawable(geom1);
    geode1->addDrawable(drawable);

    osg::Transform *transNode1 = new osg::PositionAttitudeTransform();
    transNode1->addChild(geode1);
    SBallSocketJointSimCallback *jointSimCallback1 = new SBallSocketJointSimCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    osg::Geometry *geom2=new osg::Geometry();
    osg::Vec3Array *vertices2=new osg::Vec3Array();   
   
    osg::Vec3 end2 = getBody2()->getPosition();    
    float len2 = (end2-mAnchor).length();    
    
    vertices2->push_back(osg::Vec3(0,0,0));
    vertices2->push_back(osg::Vec3(0,0,len2));
    geom2->setVertexArray(vertices2);
    geom2->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
    geom2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    geom2->setStateSet(state2);
        
    osg::Geode * geode2 = new osg::Geode();
    geode2->addDrawable(geom2);
    geode2->addDrawable(drawable);
    
    osg::Transform *transNode2 = new osg::PositionAttitudeTransform();
    transNode2->addChild(geode2);
    SBallSocketJointSimCallback *jointSimCallback2 = new SBallSocketJointSimCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);
        
    osg::Transform *transNode3 = new osg::PositionAttitudeTransform();
    transNode3->addChild(transNode1);
    transNode3->addChild(transNode2);
        
    return transNode3;
    
}

void SBallSocket::createEditorJoint() 
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

    osg::Sphere * anchor = new osg::Sphere(osg::Vec3(0,0,0), 0.2);

    osg::ShapeDrawable* drawable = new osg::ShapeDrawable(anchor);
    drawable->setStateSet( state );

    osg::Geometry *geom1=new osg::Geometry();
    osg::Vec3Array *vertices1=new osg::Vec3Array();

    float len1 = (mBody1->getPosition()-mAnchor).length();
    
    vertices1->push_back(osg::Vec3(0,0,0));
    vertices1->push_back(osg::Vec3(0,0,len1));
    geom1->setVertexArray(vertices1);

    osg::Vec4Array *colorArray = new osg::Vec4Array();
    colorArray->push_back(osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ));
    geom1->setColorArray(colorArray);
    geom1->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom1->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));

    osg::Geode * geode1 = new osg::Geode();
    geode1->addDrawable(geom1);
    geode1->addDrawable(drawable);

    osg::Transform *transNode1 = new osg::PositionAttitudeTransform();
    transNode1->addChild(geode1);
    SBallSocketJointEditorCallback *jointSimCallback1 = new SBallSocketJointEditorCallback(this,0);
    transNode1->setUpdateCallback(jointSimCallback1);
    
    osg::Geometry *geom2=new osg::Geometry();
    osg::Vec3Array *vertices2=new osg::Vec3Array();
       
    float len2 = (mBody2->getPosition()-mAnchor).length();    
    
    vertices2->push_back(osg::Vec3(0,0,0));
    vertices2->push_back(osg::Vec3(0,0,len2));
    geom2->setVertexArray(vertices2);
        
    geom2->setColorArray(colorArray);
    geom2->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom2->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
        
    osg::Geode * geode2 = new osg::Geode();
    geode2->addDrawable(geom2);
    geode2->addDrawable(drawable);
    
    osg::Transform *transNode2 = new osg::PositionAttitudeTransform();
    transNode2->addChild(geode2);
    SBallSocketJointEditorCallback *jointSimCallback2 = new SBallSocketJointEditorCallback(this,1);
    transNode2->setUpdateCallback(jointSimCallback2);
    
    mPosAttTransNode->addChild(transNode1);
    mPosAttTransNode->addChild(transNode2);
}
