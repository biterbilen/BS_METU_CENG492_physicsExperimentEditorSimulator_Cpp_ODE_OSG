#ifndef __COM_ADD_TRANSFORM_H__
#define __COM_ADD_TRANSFORM_H__

#include "command.h"
#include <osg/Node>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <scenegraph.h>

class ComAddTransform: public Command {
    osg::ref_ptr<osg::Node> affected;
    osg::ref_ptr<osg::Group> parent;
    osg::ref_ptr<osg::MatrixTransform> transform;
    osg::ref_ptr<SceneGraph> sceneGraph;
public:
    ComAddTransform(osg::Node *node, osg::Group *parent, SceneGraph*);
    virtual ~ComAddTransform() {}
    virtual void realExecute();
    virtual void undo();
};

#endif
