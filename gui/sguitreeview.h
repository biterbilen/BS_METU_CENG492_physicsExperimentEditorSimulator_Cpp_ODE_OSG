#ifndef _S_GUI_TREE_VIEW_H
#define _S_GUI_TREE_VIEW_H 1

#include <qlistview.h>
#include <qheader.h>

#include "sobjectitem.h"
#include "sforceitem.h"
#include "sjointitem.h"
#include "senvitem.h"

#include "sjoint.h"
#include "sobject.h"
#include "sbox.h"

class SGuiTreeView : public QListView
{
   Q_OBJECT
public:
   SGuiTreeView( SEnvironment * env, QWidget* parent, const char* name = 0 );
   
   void addJoint(SObject *, SJoint *);
   void addObject(SObject *);
   void addForce(SObject *, SForce *);
   
   void removeJoint(SJoint *);
   void removeObject(SObject *);
   void removeForce(SObject *, SForce *);
   
   void setSelectedItem(SObject *);
   void setSelectedItem(SObject *, SJoint *);
   void setSelectedItem(SObject *, SForce *);
   
   /** Delete contents of items under objects */
   void clearObjects();
   
   /** set the environment which this tree view represents */
   void setEnvironment(SEnvironment * env);
   
protected slots:
    void deleteItem();
    
signals:
    void deletingItem(SGuiTreeViewItem *);

protected:
    virtual void contextMenuEvent( QContextMenuEvent * );
    
    void createTreeView(SEnvironment * env);
    SObjectItem * findObjectItem(SObject *);
    SForceItem * findForceItem(SObject * obj, SForce *);
    SJointItem * findJointItem(SObject * obj, SJoint *);

private:

    SEnvItem  * mEnvItem;
    QListViewItem * mObjects;
    
    SGuiTreeViewItem * mDeleteItem;
};

#endif

