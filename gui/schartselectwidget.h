#ifndef _S_CHART_SELECT_WIDGET_H
#define _S_CHART_SELECT_WIDGET_H 1

#include "uiChartSelectWidget.h"
#include "senvironment.h"

class SChartSelectWidget : public UiChartSelectWidget
{
    Q_OBJECT
    public:
        SChartSelectWidget(SEnvironment * env, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SChartSelectWidget() {}
        
        void setEnvironment(SEnvironment * env);

    public slots:
        void addChart();
        void removeChart();
        void closeButtonClicked();
        
    private:
        osg::ref_ptr<SEnvironment> mEnv;
};

#endif
