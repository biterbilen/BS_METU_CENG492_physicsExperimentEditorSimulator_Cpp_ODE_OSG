#include "sguitreeviewitem.h"

SGuiTreeViewItem::SGuiTreeViewItem(ItemType it, QListView* parent ) 
                : QListViewItem ( parent )
{
    mItemType = it;
}

SGuiTreeViewItem::SGuiTreeViewItem(ItemType it, QListViewItem* parent )
                : QListViewItem( parent )
{
    mItemType = it;
}


