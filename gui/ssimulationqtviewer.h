#ifndef _S_SIMULATION_QTVIEWER_H
#define _S_SIMULATION_QTVIEWER_H

#include "qtviewer.h"
#include <ssimulation.h>

class SSimulation;

/**
*/
class SSimulationQtViewer : public QtViewer
{
    Q_OBJECT
public:
    SSimulationQtViewer(SSimulation* s, QWidget* parent = 0, const char* name = 0);
    ~SSimulationQtViewer();

protected slots:
    virtual void closeEvent(QCloseEvent * e);
    
    /**
    * This timerEvent function will be called automatically
    * for each timer_interval milliseconds
    * To start timerEvent's we need to call startTimer(timer_interval)
    */
    void timerEvent( QTimerEvent * );
    
signals:
    void simulationClosed();

private:
    SSimulation * mSim;
    int mTimerInterval; // milliseconds
};
#endif
