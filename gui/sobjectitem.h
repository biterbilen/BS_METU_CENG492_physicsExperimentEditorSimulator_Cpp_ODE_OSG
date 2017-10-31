#ifndef _S_OBJECT_ITEM_H
#define _S_OBJECT_ITEM_H 1

#include "sguitreeviewitem.h"
#include "sobject.h"
#include "sforce.h"
#include "sjoint.h"
#include "svelocityitem.h"

/**@class SObjectItem
 *@brief Represents a list view item for an SObject
 */
class SObjectItem : public SGuiTreeViewItem
{
public:
     
     /** Constructs a new SObject list view item which is a child of parent and
     * first in the parent's list of children. */
    SObjectItem(SObject * obj, QListViewItem* parent);

    /** Return the SObject this list view item represents */
    SObject * getItem(SObject * obj) { return obj = mObject.get(); }

    void addForce(SForce *);
    void removeForce(SForce *);
    
    void addJoint(SJoint *);
    void removeJoint(SJoint *);
    
protected:
    void okRename(int col);

private:

    osg::ref_ptr<SObject>  mObject;

    SVelocityItem * mVelocity;
    QListViewItem * mForces;
    QListViewItem * mJoints;
};

#endif

