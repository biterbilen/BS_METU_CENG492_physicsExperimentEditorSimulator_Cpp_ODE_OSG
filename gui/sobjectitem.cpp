#include "sobjectitem.h"

#include <qstring.h>

#include "sforceitem.h"
#include "sjointitem.h"

SObjectItem::SObjectItem(SObject * obj, QListViewItem* parent)
        : SGuiTreeViewItem(SGuiTreeViewItem::SOBJECT, parent)
{
    mObject = obj;
    this->setText(0, QString(mObject->getName()));
    setRenameEnabled(0, true);
    setDragEnabled(true);

    /** If object is not of some specific types add Forces and Joints childs */
    if(obj->getObjectType() != SObject::POOL &&
        obj->getObjectType() != SObject::GEAR &&
        obj->getObjectType() != SObject::LIGHT &&
        obj->getObjectType() != SObject::FLATMIRROR &&
        obj->getObjectType() != SObject::CONMIRROR &&
        obj->getObjectType() != SObject::CONVEXLENS &&
        obj->getObjectType() != SObject::INCLINEDPLANE)
    {
        mForces = new QListViewItem(this, "Forces");
        mJoints = new QListViewItem(this, "Joints");
        mVelocity = new SVelocityItem(mObject.get(), this);

        /** Add the forces of the object, if there are any */
        list<SForce*>::iterator i;
        for(i = obj->getForceBegin() ; i != obj->getForceEnd() ; i++)
       {
           this->addForce((*i));
       }
       
        /** Add the joints of the object, if there are any */
        list<SJoint*>::iterator j;
        for(j = obj->getJointBegin() ; j != obj->getJointEnd() ; j++)
       {
           this->addJoint((*j));
       }
    }
}

void SObjectItem::addForce(SForce * force)
{
    SForceItem * a = new SForceItem(force, mForces);
}

void SObjectItem::removeForce(SForce *)
{

}

void SObjectItem::addJoint(SJoint * joint)
{
    SJointItem * a = new SJointItem(joint, mJoints);
}

void SObjectItem::removeJoint(SJoint *)
{

}

void SObjectItem::okRename(int col)
{
    if(col ==0)
    {
        QListViewItem::okRename(col);
    
        mObject->setName(this->text(0));
    }
}


