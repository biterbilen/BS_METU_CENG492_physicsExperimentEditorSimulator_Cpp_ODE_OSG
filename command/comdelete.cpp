#include "comdelete.h"

ComDelete::ComDelete(osg::Node *node, osg::Group *parent, SceneGraph *sg) {
    toDelete=node;
    this->parent=parent;
    this->sceneGraph=sg;
    
    mNeedsRedraw = true;
}

void ComDelete::realExecute() {
    if (parent == 0) {
	std::cerr << "Trying to delete root node!!!!" << std::endl;
    } else {
        sceneGraph->removeChild(toDelete.get());
    }
}
void ComDelete::undo() {
    if (parent == 0) {
	std::cerr << "Trying to undelete root node ?????" << std::endl;
    } else {
        sceneGraph->addChild(toDelete.get());
    }
}
