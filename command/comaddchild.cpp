#include "comaddchild.h"
#include "seditorcallback.h"

ComAddChild::ComAddChild(osg::Node *node, Editor * ed) 
{
    this->child=node;
    this->mEditor=ed;
    
    mNeedsRedraw = true;    
}

void ComAddChild::realExecute() {
    
    SObject * obj = ((SEditorCallback *)child->getUpdateCallback())->getObject();
    
    mEditor->getSceneGraph()->addChild(child.get());
    mEditor->getEnvironment()->addObject( obj );
    mEditor->getTreeView()->addObject( obj );
}

void ComAddChild::undo() {
    
    SObject * obj = ((SEditorCallback *)child->getUpdateCallback())->getObject();
    
    mEditor->getTreeView()->removeObject( obj );
    mEditor->getEnvironment()->removeObject( obj );
    
    mEditor->getSceneGraph()->removeChild(child.get());
}
