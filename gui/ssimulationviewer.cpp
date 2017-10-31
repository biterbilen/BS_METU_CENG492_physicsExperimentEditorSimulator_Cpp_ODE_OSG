#include "ssimulationviewer.h"

#include <qapplication.h>
#include <qfiledialog.h>

#include "sfileinputoutput.h"
#include "sshowchartwidget.h"

#include "schartselectwidget.h"

SSimulationViewer::SSimulationViewer(QWidget* parent, const char* name, WFlags fl)
        : UiSimulationViewer(parent,name,fl)
{
    mSim = 0;
    mEnv = 0;
    
    mStarted = false;
    mPaused = true;
}

SSimulationViewer::~SSimulationViewer()
{
    killTimers();
    
    if(mSim != 0)
        delete mSim;
}

void SSimulationViewer::setEnvironment(SEnvironment * env)
{
    mEnv = env;
    mStarted = false;
    mPaused = true;

    if(mSim != 0)
        delete mSim;

    mSim = new SSimulation(mEnv.get());
    qtViewer->setViewport(mSim->getSceneData());
    
    qtViewer->updateGL();
}

void SSimulationViewer::startSimulation()
{
    if(!mStarted || mPaused)
    {
        mTimerInterval = 5; // milliseconds  can be set to 33 for 30 fps
        mSim->setTimeStep(mTimerInterval);
    
        startTimer( mTimerInterval );
        
        mStarted = true;
        mPaused = false;
    }
}

void SSimulationViewer::pauseSimulation()
{
    killTimers();
    mPaused = true;
}

void SSimulationViewer::stopSimulation()
{
    // FIXME if SEnvironment keeps info from the simulation this will be very dangerous!!!
    // check if this is true... schart??
               
    killTimers();
    mStarted = false;

    if(mSim != 0)
        delete mSim;
    
    mEnv->reset();

    mSim = new SSimulation(mEnv.get());
    qtViewer->setViewport(mSim->getSceneData());

    qtViewer->updateGL();
}

void SSimulationViewer::viewStats()
{
    SShowChartWidget * w = new SShowChartWidget(mEnv.get());
    w->show();
}

void SSimulationViewer::selectCharts()
{
    SChartSelectWidget * w = new SChartSelectWidget(mEnv.get());
    w->show();
}

void SSimulationViewer::viewFullScreen()
{
    if(!isFullScreen())
    {
        showFullScreen();
    }
    else {
        showNormal();
    }
}

void SSimulationViewer::fileExit()
{
    if(mSim != 0)
        delete mSim;

    QApplication::exit();
}

void SSimulationViewer::fileOpen()
{
    if(mStarted)
        stopSimulation();

    QString fileName = QFileDialog::getOpenFileName("./", "Sourcerer File Format (*.sff)", this);

    if(!fileName.isNull())
    {
        SEnvironment * env = new SEnvironment();
        SFileInputOutput * sfin = new SFileInputOutput();

        env = sfin->readFile(fileName, env);

        setEnvironment(env);

        if(sfin)
            delete sfin;
    }
}


void SSimulationViewer::timerEvent( QTimerEvent * )
{
    // take a step in the simulation
    mSim->worldStep();
    qtViewer->updateGL();
}

