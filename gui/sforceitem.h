#ifndef _S_FORCE_ITEM_H
#define _S_FORCE_ITEM_H 1

#include "sguitreeviewitem.h"
#include "sforce.h"

/**@class SForceItem
 *@brief Represents a list view item for an SForce
 */
class SForceItem : public SGuiTreeViewItem
{
public:
     
     /** Constructs a new SForce list view item which is a child of parent */
    SForceItem(SForce * force, QListViewItem* parent);

    /** Return the SForce this list view item represents */
    SForce * getItem(SForce * force) { return force = mForce.get(); }

protected:
    void okRename(int);

private:

    osg::ref_ptr<SForce>  mForce;
};

#endif

