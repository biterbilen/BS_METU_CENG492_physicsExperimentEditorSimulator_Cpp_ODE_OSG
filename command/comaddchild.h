#ifndef __COM_ADD_CHILD_H__
#define __COM_ADD_CHILD_H__

#include <osg/Node>

#include "command.h"
#include "editor.h"

class ComAddChild: public Command {
    osg::ref_ptr<osg::Node> child;
    osg::ref_ptr<Editor> mEditor;
public:
    ComAddChild(osg::Node *node, Editor * ed);
    virtual ~ComAddChild() {}
    virtual void realExecute();
    virtual void undo();
};

#endif

