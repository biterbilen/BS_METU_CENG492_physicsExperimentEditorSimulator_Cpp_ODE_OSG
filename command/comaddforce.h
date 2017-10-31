#ifndef _COM_ADD_FORCE_H
#define _COM_ADD_FORCE_H 1

#include <osg/Node>

#include "command.h"
#include "editor.h"
#include "sobject.h"
#include "sforce.h"

class ComAddForce: public Command {
    osg::ref_ptr<SForce> mForce;
    osg::ref_ptr<SObject> mObj;
    osg::ref_ptr<Editor> mEditor;
public:
    ComAddForce(SObject * obj, SForce * force, Editor * ed);
    virtual ~ComAddForce() {}
    virtual void realExecute();
    virtual void undo();
};

#endif

