#ifndef _S_GUI_TREE_VIEW_ITEM_H
#define _S_GUI_TREE_VIEW_ITEM_H 1

#include <qlistview.h>

#include "sobject.h"
#include "sjoint.h"
#include "sforce.h"
#include "senvironment.h"

/**@class SGuiTreeViewItem
 *@brief Represents a tree view item, which will hold the 
 * 
 */
class SGuiTreeViewItem : public QListViewItem
{
public:

    typedef enum {
        SOBJECT,
        JOINT,
        FORCE,
        ENVIRONMENT,
        VELOCITY
    } ItemType;

    /** Constructs a new top-level list view item in the QListView parent. */
    SGuiTreeViewItem(ItemType it, QListView* parent);
    /** Constructs a new list view item which is a child of parent and
     * first in the parent's list of children. */
    SGuiTreeViewItem(ItemType it, QListViewItem* parent );
    
    /** Return this List View Item's type */
    ItemType getItemType() { return mItemType; }
    
    /** Returns the item (object, force, joint....) this
      * list view item corresponds to */
    virtual SObject * getItem(SObject * it) { return it; }
    virtual SJoint * getItem(SJoint * it) { return it; }
    virtual SForce * getItem(SForce * it) { return it; }
    virtual SEnvironment * getItem(SEnvironment * it) { return it; }

protected:

private:

    ItemType mItemType;
    
};

#endif
