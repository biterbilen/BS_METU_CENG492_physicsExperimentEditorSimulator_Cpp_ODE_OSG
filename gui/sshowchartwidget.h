#ifndef _S_SHOW_CHART_WIDGET_H
#define _S_SHOW_CHART_WIDGET_H 1

#include "uiShowChart.h"
#include "senvironment.h"

class SShowChartWidget : public UiShowChartWidget
{
    Q_OBJECT

public:
    SShowChartWidget(SEnvironment * env, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SShowChartWidget();

    void setEnvironment( SEnvironment * env );

public slots:
    virtual void showChart();
    
private:
    
    osg::ref_ptr<SEnvironment> mEnv;
    
};

#endif

