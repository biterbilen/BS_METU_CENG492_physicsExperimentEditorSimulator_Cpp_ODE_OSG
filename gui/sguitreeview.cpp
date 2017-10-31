#include "sguitreeview.h"

#include <qpopupmenu.h>
#include <qlabel.h>
#include <qcursor.h>

SGuiTreeView::SGuiTreeView( SEnvironment * env, QWidget* parent, const char* name ) : QListView(parent, name)
{
    addColumn( "Objects" );
    header()->hide();
    setRootIsDecorated( true );
    setSorting(-1);
    setMinimumWidth(180);

    createTreeView(env);
}

void SGuiTreeView::addObject(SObject * obj)
{
    SObjectItem * select = new SObjectItem(obj, mObjects);
    
    if(select)
    {
        this->setSelected(select, true);
        this->ensureItemVisible(select);
    }
}

void SGuiTreeView::addForce(SObject * obj, SForce * force)
{
    SObjectItem * tmp = findObjectItem(obj);
    
    if(tmp)
    {
        tmp->addForce(force);
        setSelectedItem(obj, force);
    }
}

void SGuiTreeView::addJoint(SObject * obj, SJoint * joint)
{
    SObjectItem * tmp = findObjectItem(obj);
    
    if(tmp)
    {
        tmp->addJoint(joint);
        setSelectedItem(obj, joint);
    }
}

void SGuiTreeView::removeObject(SObject * obj)
{
    SObjectItem * tmp = findObjectItem(obj);

    if(tmp)
        delete tmp;
}

void SGuiTreeView::removeForce(SObject * obj, SForce * force)
{
    SForceItem * tmp = findForceItem(obj, force);

    if(tmp)
        delete tmp;
}

void SGuiTreeView::removeJoint(SJoint * joint)
{
    SJointItem * tmp1 = findJointItem(joint->getBody1(), joint);
    SJointItem * tmp2 = findJointItem(joint->getBody2(), joint);

    if(tmp1)
        delete tmp1;
    
    if(tmp2)
        delete tmp2;
}

void SGuiTreeView::setSelectedItem(SObject * obj)
{
    SObjectItem * select = this->findObjectItem(obj);

    if(select)
    {
        this->setSelected(select, true);
        this->ensureItemVisible(select);
    }
}

void SGuiTreeView::setSelectedItem(SObject * obj, SForce * force)
{
    SForceItem * select = this->findForceItem(obj, force);

    if(select)
    {
        this->setSelected(select, true);
        this->ensureItemVisible(select);
    }
}

void SGuiTreeView::setSelectedItem(SObject * obj, SJoint * joint)
{
    SJointItem * select = this->findJointItem(obj, joint);

    this->setSelected(select, true);
    this->ensureItemVisible(select);
}

void SGuiTreeView::clearObjects()
{
    if(mObjects)
    {
        while( mObjects->firstChild() )
            delete mObjects->firstChild();
    }
}

void SGuiTreeView::setEnvironment(SEnvironment * env)
{
    if(mEnvItem)
    {
        mEnvItem->setEnvironment(env);
        
        this->setSelected(mEnvItem, true);
        this->ensureItemVisible(mEnvItem);
    }
}

void SGuiTreeView::createTreeView(SEnvironment * env)
{
    if(env != NULL)
    {
        mObjects = new QListViewItem(this, "Objects");
        mEnvItem = new SEnvItem(env, this);
    }
}

SObjectItem * SGuiTreeView::findObjectItem(SObject * obj)
{
    QListViewItemIterator it( mObjects );
    while ( it.current() )
    {
        SGuiTreeViewItem * sgtvi = dynamic_cast<SGuiTreeViewItem *>(it.current());

        if(sgtvi != 0L)
        {
            if(sgtvi->getItemType() == SGuiTreeViewItem::SOBJECT)
            {
                SObject * tmp = 0;
                if(sgtvi->getItem(tmp) == obj)
                {
                    return (SObjectItem *)sgtvi;
                }
            }
        }
        ++it;
    }
    
    return 0;
}

SForceItem * SGuiTreeView::findForceItem(SObject * obj, SForce * force)
{
    SGuiTreeViewItem * tmp = findObjectItem(obj);
    
    if(tmp)
    {
        QListViewItemIterator it( mObjects );
        while ( it.current() )
        {
            SGuiTreeViewItem * sgtvi = dynamic_cast<SGuiTreeViewItem *>(it.current());
    
            if(sgtvi != 0L)
            {
                if(sgtvi->getItemType() == SGuiTreeViewItem::FORCE)
                {
                    SForce * tmp = 0;
                    if(sgtvi->getItem(tmp) == force)
                    {
                        return (SForceItem *)sgtvi;
                    }
                }
            }
            ++it;
        }
    }
    
    return 0;
}

SJointItem * SGuiTreeView::findJointItem(SObject * obj, SJoint * joint)
{
    SGuiTreeViewItem * tmp = findObjectItem(obj);
    
    if(tmp)
    {
        QListViewItemIterator it( tmp );
        while ( it.current() )
        {
            SGuiTreeViewItem * sgtvi = dynamic_cast<SGuiTreeViewItem *>(it.current());
    
            if(sgtvi != 0L)
            {
                if(sgtvi->getItemType() == SGuiTreeViewItem::JOINT)
                {
                    SJoint * tmp = 0;
                    if(sgtvi->getItem(tmp) == joint)
                    {
                        return (SJointItem *)sgtvi;
                    }
                }
            }
            ++it;
        }
    }
    
    return 0;
}

void SGuiTreeView::deleteItem()
{
    if(mDeleteItem)
    {
        emit deletingItem(mDeleteItem);
        
//         if(mDeleteItem->getItemType() == SGuiTreeViewItem::JOINT)
//         {
//             std::cout << "sguitreeview, deleteItem: removing joint" << std::endl;
//             SJoint * j = 0;
//             removeJoint(mDeleteItem->getItem(j));
//         }
//         else
//         {
//             delete mDeleteItem;
//         }
    }
}

void SGuiTreeView::contextMenuEvent(QContextMenuEvent * e)
{
    mDeleteItem = dynamic_cast<SGuiTreeViewItem*>(this->itemAt(e->pos()));
    
    if(mDeleteItem == 0)
    {
        /** This is not an SGuiTreeViewItem */
        return;
    }
    
    if(mDeleteItem->getItemType() == SGuiTreeViewItem::ENVIRONMENT ||
        mDeleteItem->getItemType() == SGuiTreeViewItem::VELOCITY )
    {
        return;
    }

    QPopupMenu* contextMenu = new QPopupMenu( this );
    Q_CHECK_PTR( contextMenu );
    QLabel *caption = new QLabel( "<b>Actions</b>", this );
    caption->setAlignment( Qt::AlignCenter );
    contextMenu->insertItem( caption );
    contextMenu->insertItem( "&Delete",  this, SLOT(deleteItem()));
    
    contextMenu->exec( QCursor::pos() );

    delete contextMenu;
}

