#ifndef _S_ENV_ITEM_H
#define _S_ENV_ITEM_H 1

#include "sguitreeviewitem.h"
#include "senvironment.h"

/**@class SEnvItem
 *@brief Represents a list view item for an SEnvironment
 */
class SEnvItem : public SGuiTreeViewItem
{
public:
     
     /** Constructs a new SEnvironment list view item which is a child of parent and
     * first in the parent's list of children. */
    SEnvItem(SEnvironment * env, QListView* parent);
    
    /** Return the SEnvironment this list view item represents */
    SEnvironment * getItem(SEnvironment * obj) { return obj = mEnvironment.get(); }
    
   /** set the environment which this tree view item represents */
   void setEnvironment(SEnvironment * env);
    
protected:

private:

    osg::ref_ptr<SEnvironment>  mEnvironment;

};

#endif
