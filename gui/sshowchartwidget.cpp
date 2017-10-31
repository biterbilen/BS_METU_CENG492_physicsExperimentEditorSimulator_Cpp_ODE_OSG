#include "sshowchartwidget.h"

#include "schartitem.h"
#include "schartview.h"

#include <qlayout.h>

SShowChartWidget::SShowChartWidget( SEnvironment * env, QWidget* parent, const char* name, WFlags fl )
        : UiShowChartWidget( parent, name, fl )
{
    setEnvironment(env);
}

SShowChartWidget::~SShowChartWidget()
{}

void SShowChartWidget::setEnvironment( SEnvironment * env )
{
    mEnv = env;
    
    if(mEnv.valid())
    {
          for ( list<SChart*>::iterator i = mEnv->getChartBegin() ; i != mEnv->getChartEnd() ; i++ )
        {
            new SChartItem((*i), mChartsListView);
        }    
    }
}

void SShowChartWidget::showChart()
{
    SChartItem * item = (SChartItem *)mChartsListView->selectedItem();
    
    if( item != 0)
    {
        if(mChartView)
            delete mChartView;
        
        mChartView = new SChartView(this);
        mChartView = new SChartView( this, "mChartView" );
        mChartView->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, mChartView->sizePolicy().hasHeightForWidth() ) );
        UiShowChartWidgetLayout->addWidget( mChartView, 1, 0 );

        mChartView->setChart(item->getChart());
        mChartView->Plot();
        mChartView->show();
    }
}



