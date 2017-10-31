#include "schartselectwidget.h"

#include <qlistview.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qmessagebox.h>

#include "schartobjectitem.h"
#include "schartitem.h"

SChartSelectWidget::SChartSelectWidget( SEnvironment * env, QWidget* parent, const char* name, WFlags fl )
        : UiChartSelectWidget( parent, name, fl )

{
    setEnvironment( env );
}

void SChartSelectWidget::setEnvironment( SEnvironment * env )
{
    mEnv = env;

    for ( list<SObject*>::iterator i = mEnv->getObjectBegin() ; i != mEnv->getObjectEnd() ; i++ )
    {
        if ( ( *i ) ->canHaveChart() )
        {
            new SChartObjectItem( (*i), mObjectsListView1);
            new SChartObjectItem( (*i), mObjectsListView2);
        }
    }
    
    for ( list<SChart*>::iterator i = mEnv->getChartBegin() ; i != mEnv->getChartEnd() ; i++ )
    {
        new SChartItem((*i), mChartsListView);
    }
}

void SChartSelectWidget::addChart()
{
    SChart::AxisType dataType1, dataType2;
    string dataName1, dataName2;
    
    switch(mDataTypeComboBox1->currentItem())
    {
        case 0:
            dataType1 = SChart::VEL;
            dataName1 = "Velocity";
            break;
        case 1:
            dataType1 = SChart::POSX;
            dataName1 = "Position X";
            break;
        case 2:
            dataType1 = SChart::POSY;
            dataName1 = "Position Y";
            break;
        case 3:
            dataType1 = SChart::POSZ;
            dataName1 = "Position Z";
            break;
        case 4:
            dataType1 = SChart::ACC;
            dataName1 = "Acceleration";
            break;
        case 5:
            dataType1 = SChart::ANGVEL;
            dataName1 = "Angular Velocity";
            break;
        case 6:
            dataType1 = SChart::FORCE;
            dataName1 = "Force";
            break;
        case 7:
            dataType1 = SChart::TORQUE;
            dataName1 = "Torque";
            break;
    }

    switch(mDataTypeComboBox2->currentItem())
    {
        case 0:
            dataType2 = SChart::TIME;
            dataName2 = "Time";
            break;
        case 1:
            dataType2 = SChart::VEL;
            dataName2 = "Velocity";
            break;
        case 2:
            dataType2 = SChart::POSX;
             dataName2 = "Position X";
            break;
        case 3:
            dataType2 = SChart::POSY;
            dataName2 = "Position Y";
            break;
        case 4:
            dataType2 = SChart::POSZ;
            dataName2 = "Position Z";
            break;
        case 5:
            dataType2 = SChart::ACC;
            dataName2 = "Acceleration";
            break;
        case 6:
            dataType2 = SChart::ANGVEL;
            dataName2 = "Angular Velocity";
            break;
        case 7:
            dataType2 = SChart::FORCE;
            dataName2 = "Force";
            break;
        case 8:
            dataType2 = SChart::TORQUE;
            dataName2 = "Torque";
            break;
    }
    
    SChart * chart = new SChart(dataType1, dataType2, mSecondsSpinBox->value());

    SChartObjectItem * item = (SChartObjectItem *) mObjectsListView1->selectedItem();
    
    if(item == 0)
    {
        string msg = "Please select an object of which " + dataName1 + " data will be collected!\n\n";
        QMessageBox::warning(this, "3DPhySims", msg, "Ok");
        return;
    }

    chart->addObj1(item->getObject());
    chart->setYAxisName(dataName1);
    chart->setXAxisName(dataName2);
    
    if(dataType2 != SChart::TIME)
    {
        item = (SChartObjectItem *) mObjectsListView2->selectedItem();
        
        if(item == 0)
        {
            string msg = "Please select an object of which " + dataName2 + " data will be collected!\n\n";
            QMessageBox::warning(this, "3DPhySims", msg, "Ok");
            return;
        }
    
        chart->addObj2(item->getObject());
    }

    new SChartItem(chart, mChartsListView);
    mEnv->addChart(chart);
}

void SChartSelectWidget::removeChart()
{
    SChartItem * item = (SChartItem *)mChartsListView->selectedItem();
    
    if( item != 0)
    {
        mEnv->removeChart(item->getChart());
        delete item;
    }
}

void SChartSelectWidget::closeButtonClicked()
{
    this->close();
}

