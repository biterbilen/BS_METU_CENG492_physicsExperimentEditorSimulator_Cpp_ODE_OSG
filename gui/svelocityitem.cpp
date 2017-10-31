#include "svelocityitem.h"

SVelocityItem::SVelocityItem(SObject * obj, QListViewItem* parent)
        : SGuiTreeViewItem(SGuiTreeViewItem::VELOCITY, parent)
{
    mObject = obj;
    this->setText(0,"Velocity");
}

