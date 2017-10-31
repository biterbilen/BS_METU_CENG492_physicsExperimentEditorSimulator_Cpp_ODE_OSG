#ifndef _S_CHART_ITEM_H
#define _S_CHART_ITEM_H 1

#include <qlistview.h>

#include "schart.h"

class SChartItem : public QListViewItem
{
public:
    SChartItem(SChart *obj, QListView* parent);
        
    virtual SChart * getChart() { return mChart.get(); }

protected :
    void okRename(int);

private:
    osg::ref_ptr<SChart> mChart;
};

#endif
