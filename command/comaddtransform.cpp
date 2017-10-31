#include "comaddtransform.h"

ComAddTransform::ComAddTransform(osg::Node *node,
				 osg::Group *parent,
				 SceneGraph *sg) {
    this->affected=node;
    this->parent=parent;
    this->transform=new osg::MatrixTransform();
    this->sceneGraph=sg;
}

void ComAddTransform::realExecute() {
    transform->addChild(affected.get());
    if (parent == 0) {
        sceneGraph->replaceScenegraph(transform.get());
    } else {
        sceneGraph->replaceChild(parent.get(),affected.get(),transform.get());
    }
}

void ComAddTransform::undo() {
    transform->removeChild(affected.get());
    if (parent == 0) {
        sceneGraph->replaceScenegraph(affected.get());
    } else {
        sceneGraph->replaceChild(parent.get(),transform.get(),affected.get());
    }
}
