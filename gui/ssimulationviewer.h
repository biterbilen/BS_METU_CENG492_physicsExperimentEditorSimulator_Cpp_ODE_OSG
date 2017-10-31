#ifndef _SSIMULATIONVIEWER_H
#define _SSIMULATIONVIEWER_H 1

#include "uiSimulationViewer.h"

#include "qtviewer.h"
#include "senvironment.h"
#include "ssimulation.h"

class SSimulationViewer : public UiSimulationViewer
{
    Q_OBJECT

public:
    SSimulationViewer(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SSimulationViewer();
    
    void setEnvironment(SEnvironment * env);

public slots:

    virtual void stopSimulation();
    virtual void pauseSimulation();
    virtual void startSimulation();
    virtual void viewStats();
    virtual void selectCharts();
    virtual void viewFullScreen();
    virtual void fileExit();
    virtual void fileOpen();

protected slots:
    /**
    * This timerEvent function will be called automatically
    * for each timer_interval milliseconds
    * To start timerEvent's we need to call startTimer(timer_interval)
    */
    void timerEvent( QTimerEvent * );
private:
 
    osg::ref_ptr<SEnvironment> mEnv;
    SSimulation * mSim;
    int mTimerInterval; // milliseconds
    
    bool mStarted;
    bool mPaused;
};

#endif


