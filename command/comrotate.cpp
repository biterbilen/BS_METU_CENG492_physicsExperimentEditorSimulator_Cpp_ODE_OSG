#include "comrotate.h"
#include <editor.h>

ComRotate::ComRotate(SObject *node, osg::Vec3 quat_vec, float angle) {
    affected=node;
    original=affected->getOrientation();

    osg::Quat q;
    q.makeRotate(angle, quat_vec);
 
    if(original.zeroRotation())
    {
        target= q;
    }
    else {
        target = original * q;
    }
    
    mNeedsRedraw = true;
}

void ComRotate::realExecute() {
    affected->setOrientation(target);
}

void ComRotate::undo() {
    affected->setOrientation(original);
}

