#ifndef COM_RENAME_OBJECT_H
#define COM_RENAME_OBJECT_H 1

#include <string.h>

#include "command.h"
#include "editor.h"

class ComRenameObject: public Command {
    osg::ref_ptr<SObject> mObject;
    string oldName;
    string newName;
public:
    ComRenameObject(SObject *, string);
    virtual ~ComRenameObject() {}
    virtual void realExecute();
    virtual void undo();
};

#endif

