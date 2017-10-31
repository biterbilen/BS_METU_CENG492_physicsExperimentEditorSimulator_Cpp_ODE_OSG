#ifndef _S_VELOCITY_ITEM_H
#define _S_VELOCITY_ITEM_H 1

#include "sguitreeviewitem.h"
#include "sobject.h"

/**@class SVelocityItem
 */
class SVelocityItem : public SGuiTreeViewItem
{
public:
     
     /** Constructs a new velocity list view item which is a child of parent and
     * first in the parent's list of children. */
    SVelocityItem(SObject * obj, QListViewItem* parent);

    /** Return the SObject this list view item represents */
    SObject * getItem(SObject * obj) { return obj = mObject.get(); }
    
protected:

private:

    osg::ref_ptr<SObject>  mObject;
};

#endif

