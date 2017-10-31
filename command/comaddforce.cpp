#include "comaddforce.h"

ComAddForce::ComAddForce(SObject * obj, SForce * force, Editor * ed) 
{
    mObj = obj;
    mForce = force;
    mEditor = ed;
    
    mNeedsRedraw = false;
}

void ComAddForce::realExecute() 
{
    mForce->setObject(mObj.get());
    
    mForce->createEditorForce();
    osg::Node * node = mForce->getTransformNode();
    
    mEditor->getSceneGraph()->addChild(node);
    
    mEditor->getTreeView()->addForce(mObj.get(),  mForce.get());
    mEditor->getEnvironment()->addForce(mForce.get());
}

void ComAddForce::undo() 
{
    mEditor->getSceneGraph()->removeChild(mForce->getTransformNode());
    mEditor->getEnvironment()->removeForce(mForce.get());
//     mEditor->getTreeView()->removeObject( obj );
}

