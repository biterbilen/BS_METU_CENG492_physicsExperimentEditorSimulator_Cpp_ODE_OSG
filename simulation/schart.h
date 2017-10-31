#ifndef S_CHART_H
#define S_CHART_H 1

#include <string>
#include <osg/Vec3>
#include <osg/Referenced>
#include "sobject.h"

class SObject;

/**
 * @class Schart
 * @brief chart class for showing collected data graphically
 */
 class SChart : public osg::Referenced
{
public:
     enum ChartScope{
            OBJVSOBJ,
            OBJVSTIME,
     };
     
     enum  AxisType{
        TIME,
        VEL,
        ACC,
        POSX,
        POSY,
        POSZ,
        FORCE,
        TORQUE,
        ANGVEL,
     };
     
     /**@Constructor SChart(AxisType yType, AxisType xType, double sec)
     *If the xType is Time a ObjvsTime graph  is generated and
     *the data is collected according to the sec variable
     *At each sec seconds the data is collected.Min and max values of sec is
     *1 and 60
     */
      
     SChart(AxisType yType, AxisType xType, double sec);
     
     /**@function    string getName()
     *For getting the name of the chart
     */  
     string getName() { return mName; }
     
     /**@function    string getName()
     *For setting the name of the chart
     */       
     void setName(string n) { mName = n; }
     
     /**@function  double*  getData(int which) 
     *This function collects the wanted data of the object at the given time
     *intervals
     */
     
     double*  getData(int which) const { return mData[which]; }
          
     int getDataCount();
     
     /**@function      AxisType getXAxisType()
     *Gets the X axis' type
     */      
     AxisType getXAxisType();
     /**@function      AxisType getYAxisType()
     *Gets the Y axis' type
     */  
     AxisType getYAxisType();
     
     void setXAxisName(string n) { mXAxisName = n; }
     void setYAxisName(string n) { mYAxisName = n; }
     string getXAxisName() { return mXAxisName; }
     string getYAxisName() { return mYAxisName; }
     
     /**@function void resetChart()
     *When restarting the simulation this function resets all the variables
     */
     void resetChart();
     
     float getSeconds();
     void addData();
     ChartScope  getChartScope();
     SObject * getObject1() { return mObj1.get(); }
     SObject * getObject2() { return mObj2.get(); }
     
     
     void addObj1(SObject * obj);
     void addObj2(SObject * obj);
     
    string getCounter();
    void setCounter(string);     
     string incrementString(string s, int n);
protected:
    ~SChart();

private:
     double* mData[2];
     osg::ref_ptr<SObject> mObj1;
     osg::ref_ptr<SObject> mObj2;
     int mDataCount;
     AxisType mYType;
     AxisType mXType;
     string mXAxisName;
     string mYAxisName;
     float mSeconds;
     string mName;
     ChartScope mChartScope;
     
     static string mCounter;
};

#endif

