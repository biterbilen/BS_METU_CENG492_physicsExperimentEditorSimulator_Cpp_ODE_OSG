#ifndef __COM_DELETE_H__
#define __COM_DELETE_H__

#include "command.h"
#include <osg/Node>
#include <osg/Group>
#include <scenegraph.h>

class ComDelete: public Command {
    osg::ref_ptr<osg::Node> toDelete;
    osg::ref_ptr<osg::Group> parent;
    osg::ref_ptr<SceneGraph> sceneGraph;
public:
    ComDelete(osg::Node *node, osg::Group *parent, SceneGraph*);
    virtual ~ComDelete() {}
    virtual void realExecute();
    virtual void undo();

};

#endif
