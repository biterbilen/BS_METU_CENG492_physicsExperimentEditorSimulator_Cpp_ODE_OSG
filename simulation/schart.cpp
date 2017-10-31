#include "schart.h"

string SChart::mCounter = "1";

SChart::SChart(AxisType yType, AxisType xType,double sec)
{
     mName = "Chart" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);   
    
    mYType = yType;
    mXType = xType;

    mData[0] = 0;
    mData[1] = 0;
            
    if(xType == TIME)
    {
        mChartScope = OBJVSTIME;
        mData[0] = new double[1000]; 
    }
    else
    {
       mChartScope = OBJVSOBJ;
       mData[0] = new double[1000]; 
       mData[1] = new double[1000];
    }
     mSeconds = sec;
     mDataCount = 0;
}

SChart::~SChart()
{
    if(mData[0])
        delete [] mData[0];
    if(mData[1])
        delete [] mData[1];        
}

 int  SChart::getDataCount(){ return mDataCount;}
 
SChart::AxisType SChart::getXAxisType(){
    return mXType;
}

SChart::AxisType SChart::getYAxisType(){
    return mYType;
}
void SChart::resetChart(){ mDataCount = 0;}
float SChart::getSeconds(){return mSeconds;}
void SChart::addObj1(SObject * obj){mObj1 = obj;}
void SChart::addObj2(SObject * obj){mObj2 = obj;}

void SChart::addData(){

        switch(mYType){
        case VEL:
           
            (mData[0])[mDataCount] = mObj1->getObjectLinearVel();
        break;
        case ACC:
             (mData[0])[mDataCount] = mObj1->getObjectAcc();
        break;
        case FORCE:
             (mData[0])[mDataCount] = mObj1->getObjectForce();
        break;
        case ANGVEL:
             (mData[0])[mDataCount] = mObj1->getObjectAngularVel();
        break;
        case POSX:
             (mData[0])[mDataCount] = mObj1->getObjectPositionX();   
        break;    
        case POSY:
             (mData[0])[mDataCount] = mObj1->getObjectPositionY();
        break;
        case POSZ:
            (mData[0])[mDataCount] = mObj1->getObjectPositionZ();
        break;
        case TORQUE:
            (mData[0])[mDataCount] = mObj1->getObjectTorque();

        break;
        default:
        break;
       }  
    if(mChartScope == OBJVSOBJ)
    {
        switch(mXType){
        case VEL:
            (mData[1])[mDataCount] = mObj2->getObjectLinearVel();
        break;
        case ACC:
             (mData[1])[mDataCount] = mObj2->getObjectAcc();
        break;
        case FORCE:
             (mData[1])[mDataCount] = mObj2->getObjectForce();
        break;
        case ANGVEL:
             (mData[1])[mDataCount] = mObj2->getObjectAngularVel();
        break;
        case POSX:
             (mData[1])[mDataCount] = mObj2->getObjectPositionX();   
        break;    
        case POSY:
             (mData[1])[mDataCount] = mObj2->getObjectPositionY();
        break;
        case POSZ:
            (mData[1])[mDataCount] = mObj2->getObjectPositionZ();
        break;
        case TORQUE:
            (mData[1])[mDataCount] = mObj2->getObjectTorque();

        break;
        default:
        break;
       }  
    }
    mDataCount++;
}

SChart::ChartScope  SChart::getChartScope(){return mChartScope;}

string SChart::incrementString(string s, int n)
{
    if(s[n] == '9')
    {
        s[n] = '0';
        if(n == 0)
            s = "1" + s;
        else
            s = incrementString(s, n-1);    
    }
    else
    {
        switch(s[n])
            {
                case '0' : s[n] = '1'; break;
                case '1' : s[n] = '2'; break;
                case '2' : s[n] = '3'; break;
                case '3' : s[n] = '4'; break;
                case '4' : s[n] = '5'; break;
                case '5' : s[n] = '6'; break;
                case '6' : s[n] = '7'; break;
                case '7' : s[n] = '8'; break;
                default  : s[n] = '9'; break;            
            }    
    }    
    return s;
}

string SChart::getCounter(){return mCounter;}

void SChart::setCounter(string s) {
    
        mCounter = s;
}
