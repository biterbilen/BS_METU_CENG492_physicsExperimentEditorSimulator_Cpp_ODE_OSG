#include "schartview.h"

#include "sobject.h"

SChartView::SChartView(QWidget* parent, const char* name) : QwtPlot(parent, name)
{
    mColors[0] = blue;
    mColors[1] = red;
    mColors[2] = yellow;
    mColors[3] = white;
    mColors[4] = black;
    mChart  = 0;
}

SChartView::SChartView(SChart *chart, QWidget* parent, const char* name) : QwtPlot(parent, name)
{
    mColors[0] = blue;
    mColors[1] = red;
    mColors[2] = yellow;
    mColors[3] = white;
    mColors[4] = black;
    mChart  = chart;
}

void SChartView::Plot()
{
       //legend set
      setAutoLegend(TRUE); // We want a legend
            
        //set legend pos
       setLegendPos(Qwt::Right);
    
    
    if(mChart->getChartScope() == SChart::OBJVSTIME)
    {
                std::cout << "vs time" << std::endl;
                
                setTitle("Object Property vs. Time Chart");
                //legend set
                setAutoLegend(TRUE); // We want a legend
            
                //set legend pos
                setLegendPos(Qwt::Right);
                
                switch(mChart->getYAxisType())
                {
                    case SChart::VEL:
                        setAxisTitle(yLeft, "Velocity");
                        mCurve = insertCurve("y = vel(" + mChart->getObject1()->getName() + ")" );
                        break;
                    case SChart::ACC:
                        setAxisTitle(yLeft, "Acceleration");
                        mCurve = insertCurve("y = acc(" + mChart->getObject1()->getName() + ")" );
                        break;
                    case SChart::POSX:
                        setAxisTitle(yLeft, "Position X");
                         mCurve = insertCurve("y = posX(" + mChart->getObject1()->getName() + ")" );
                        break;
                    case SChart::POSY:
                        setAxisTitle(yLeft, "Position Y");
                         mCurve = insertCurve("y = posY(" + mChart->getObject1()->getName() + ")" );
                        break;
                    case SChart::POSZ:
                        setAxisTitle(yLeft, "Position Z");
                         mCurve = insertCurve("y = posZ(" + mChart->getObject1()->getName() + ")" );
                        break;
                    case SChart::FORCE:
                        setAxisTitle(yLeft, "Force");
                         mCurve = insertCurve("y = for(" + mChart->getObject1()->getName() + ")" );
                        break;
                    case SChart::TORQUE:
                        setAxisTitle(yLeft, "Torque");
                         mCurve = insertCurve("y = tor(" + mChart->getObject1()->getName() + ")" );
                        break;
                   case SChart::ANGVEL:
                        setAxisTitle(yLeft, "Angular Velocity");
                         mCurve = insertCurve("y = angvel(" + mChart->getObject1()->getName() + ")" );
                        break;
                    default:
                    break;
                }
                
                setCurvePen(mCurve,QPen(mColors[0]));
                            
                //set time axis title
                setAxisTitle(xBottom, "Time(in seconds)");
                 
                for(int j = 0 ; j < mChart->getDataCount(); j++)
                {
                    mTimeData[j] = j*mChart->getSeconds();
                }
                
                setCurveData( mCurve,mTimeData,mChart->getData(0), mChart->getDataCount() );
    }
    //OBJ vs. OBJ
    else
    {    
                setTitle("Object Property vs. Object Property Chart");
                //legend set
                setAutoLegend(TRUE); // We want a legend
            
                //set legend pos
                setLegendPos(Qwt::Right);
                mCurve = insertCurve("y ="+ mChart->getObject1()->getName() +" vs. " + mChart->getObject2()->getName());
                 
                switch(mChart->getYAxisType())
                {
                    case SChart::VEL:
                        setAxisTitle(yLeft, "Velocity");
                        break;
                    case SChart::ACC:
                        setAxisTitle(yLeft, "Acceleration");
                        break;
                    case SChart::POSX:
                        setAxisTitle(yLeft, "Position X");
                        break;
                    case SChart::POSY:
                        setAxisTitle(yLeft, "Position Y");
                        break;
                    case SChart::POSZ:
                        setAxisTitle(yLeft, "Position Z");
                        break;
                    case SChart::FORCE:
                        setAxisTitle(yLeft, "Force");
                        break;
                    case SChart::TORQUE:
                        setAxisTitle(yLeft, "Torque");
                        break;
                    case SChart::ANGVEL:
                        setAxisTitle(yLeft, "Angular Velocity");
                        break;
                      default:
                        break;
                }
                
                 switch(mChart->getXAxisType())
                {
                    case SChart::VEL:
                         setAxisTitle(xBottom, "Velocity");
                        break;
                    case SChart::ACC:
                      setAxisTitle(xBottom, "Acceleration");
                        break;
                    case SChart::POSX:
                        setAxisTitle(xBottom, "Position X");
                        break;
                    case SChart::POSY:
                       setAxisTitle(xBottom, "Position Y");
                        break;
                    case SChart::POSZ:
                       setAxisTitle(xBottom, "Position Z");
                        break;
                    case SChart::FORCE:
                        setAxisTitle(xBottom, "Force");
                        break;
                    case SChart::TORQUE:
                       setAxisTitle(xBottom, "Torque");
                        break;
                   case SChart::ANGVEL:
                       setAxisTitle(xBottom, "Angular Velocity");
                        break;
                        
                  default:
                       break;
                }
                
                setCurvePen(mCurve,QPen(mColors[0]));
                setCurveData( mCurve,mChart->getData(1), mChart->getData(0), mChart->getDataCount() );
                
    
    }
    
    replot();

}
