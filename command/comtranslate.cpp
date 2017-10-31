#include "comtranslate.h"
#include <editor.h>

ComTranslate::ComTranslate(SObject * obj, osg::Vec3 newPos) {
    affected=obj;
    original=affected->getPosition();
    target=newPos;
    
    mNeedsRedraw = true;
}

void ComTranslate::realExecute() {
    affected->setPosition(target);
    
    list<SJoint*>::iterator j;
    for(j = affected->getJointBegin() ; j != affected->getJointEnd() ; j++)
    {
        (*j)->createEditorJoint();
    }
}

void ComTranslate::undo() {
    affected->setPosition(original);
}
