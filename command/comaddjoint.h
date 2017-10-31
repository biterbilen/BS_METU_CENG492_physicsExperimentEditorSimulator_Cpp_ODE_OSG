#ifndef _COM_ADD_JOINT_H
#define _COM_ADD_JOINT_H 1

#include <osg/Node>

#include "command.h"
#include "editor.h"
#include "sobject.h"
#include "sjoint.h"

class ComAddJoint: public Command {
    osg::ref_ptr<SJoint> mJoint;
    osg::ref_ptr<SObject> mObj1;
    osg::ref_ptr<SObject> mObj2;
    osg::ref_ptr<Editor> mEditor;
public:
    ComAddJoint(SObject * obj1, SObject * obj2, SJoint * joint, Editor * ed);
    virtual ~ComAddJoint() {}
    virtual void realExecute();
    virtual void undo();
};

#endif

