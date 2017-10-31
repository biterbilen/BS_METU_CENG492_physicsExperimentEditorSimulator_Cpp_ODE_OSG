#include "comrenameobject.h"

ComRenameObject::ComRenameObject(SObject * obj, string name) 
{
    mObject = obj;
        
    newName = name;
    oldName = mObject->getName();
}

void ComRenameObject::realExecute() 
{    
    mObject->setName(newName);
}

void ComRenameObject::undo()
{
    mObject->setName(oldName);
}
