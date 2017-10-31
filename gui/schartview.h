#ifndef _S_CHART_VIEW_H
#define _S_CHART_VIEW_H 1

#include <string>
#include <osg/Vec3>
#include <qwt/qwt_plot.h>
#include "schart.h"

/**
 * @class SChartView
 * By using the qwt plot class the collected data are shown 
 *as graphical view 
 */
class SChartView: public QwtPlot
{
   public:
    SChartView (QWidget* parent = 0, const char* name = 0);
    SChartView (SChart * chart, QWidget* parent = 0, const char* name = 0);
    void setChart(SChart * c) { mChart = c; }
    void Plot();
  
  private:  
    osg::ref_ptr<SChart> mChart;
    long int mCurve; 
    QColor mColors[5];
    double  mTimeData[1000];
};
#endif
