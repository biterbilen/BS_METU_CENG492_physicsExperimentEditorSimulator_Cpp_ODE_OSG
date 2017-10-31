#include "schartobjectitem.h"

SChartObjectItem::SChartObjectItem(SObject * obj, QListView* parent ) 
                : QListViewItem ( parent )
{
    mObject = obj;
    this->setText(0, mObject->getName());
}

