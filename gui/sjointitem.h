#ifndef _S_JOINT_ITEM_H
#define _S_JOINT_ITEM_H 1

#include "sguitreeviewitem.h"
#include "sjoint.h"

/**@class SJointItem
 *@brief Represents a list view item for an SJoint
 */
class SJointItem : public SGuiTreeViewItem
{
public:
     /** Constructs a new SJoint list view item which is a child of parent */
    SJointItem(SJoint * joint, QListViewItem* parent);

    /** Return the SJoint this list view item represents */
    SJoint * getItem(SJoint * joint) { return joint = mJoint.get(); }

protected:
    void okRename(int);

private:

    osg::ref_ptr<SJoint>  mJoint;
};

#endif

