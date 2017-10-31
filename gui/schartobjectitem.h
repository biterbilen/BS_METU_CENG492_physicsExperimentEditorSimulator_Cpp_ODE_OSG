#ifndef _S_CHART_OBJECT_ITEM_H
#define _S_CHART_OBJECT_ITEM_H 1

#include <qlistview.h>

#include "sobject.h"

class SChartObjectItem : public QListViewItem
{
public:

    SChartObjectItem(SObject *obj, QListView* parent);
        
    virtual SObject * getObject() { return mObject.get(); }

protected:

private:
    osg::ref_ptr<SObject> mObject;
};

#endif
