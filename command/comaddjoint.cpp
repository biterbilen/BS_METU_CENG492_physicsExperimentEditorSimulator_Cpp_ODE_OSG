#include "comaddjoint.h"

ComAddJoint::ComAddJoint(SObject * obj1, SObject * obj2, SJoint * joint, Editor * ed) 
{
    mObj1 = obj1;
    mObj2 = obj2;
    
    mJoint = joint;
    mEditor = ed;
    
    mNeedsRedraw = false;
}

void ComAddJoint::realExecute() 
{
    mJoint->setBody1(mObj1.get());
    mJoint->setBody2(mObj2.get());
    
    mJoint->createEditorJoint();
    osg::Node * node = mJoint->getTransformNode();

    mEditor->getSceneGraph()->addChild(node);

    mEditor->getTreeView()->addJoint(mObj1.get(),  mJoint.get());
    mEditor->getTreeView()->addJoint(mObj2.get(),  mJoint.get());
    mEditor->getEnvironment()->addJoint(mJoint.get());
}

void ComAddJoint::undo() 
{
    mEditor->getSceneGraph()->removeChild(mJoint->getTransformNode());
    mEditor->getEnvironment()->removeJoint(mJoint.get());
}

