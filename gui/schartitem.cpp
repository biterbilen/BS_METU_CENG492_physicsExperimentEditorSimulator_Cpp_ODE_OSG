#include "schartitem.h"

SChartItem::SChartItem(SChart * c, QListView* parent ) 
                : QListViewItem ( parent )
{
    mChart = c;
    setRenameEnabled(0, true);
    
    this->setText(0, mChart->getName());
    this->setText(1, mChart->getYAxisName());
    this->setText(2, mChart->getXAxisName());
    this->setText(3, mChart->getObject1()->getName());
    
    if(mChart->getXAxisType() != SChart::TIME)
    {
        this->setText(4, mChart->getObject2()->getName());
    } else this->setText(4, "Seconds");
}

void SChartItem::okRename(int col)
{
    QListViewItem::okRename(col);
    
    if(col == 0)
    {
        mChart->setName(this->text(0));
    }
}

