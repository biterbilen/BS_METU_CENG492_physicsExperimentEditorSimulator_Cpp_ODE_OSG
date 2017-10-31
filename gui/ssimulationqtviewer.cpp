#include <ssimulationqtviewer.h>
#include <schartview.h>

SSimulationQtViewer::SSimulationQtViewer(SSimulation* s, QWidget* parent, const char* name)
        : QtViewer(parent, name)
{
    mSim = s;
    setViewport(mSim->getSceneData());

    mTimerInterval = 5; // milliseconds  can be set to 33 for 30 fps
    mSim->setTimeStep(mTimerInterval);

    startTimer( mTimerInterval );
};

SSimulationQtViewer::~SSimulationQtViewer()
{
}

void SSimulationQtViewer::closeEvent(QCloseEvent * e)
{
    killTimers();
//     e->accept();
    emit simulationClosed();
}

void SSimulationQtViewer::timerEvent( QTimerEvent * )
{
    // take a step in the simulation
    mSim->worldStep();

//     osg::ref_ptr<QtEventAdapter> ea = new QtEventAdapter();
//     ea->adaptFrame(/*mFrameStamp->getReferenceTime()*/);
// 
//     if(mViewportList[mFocusedViewport].mMatrixManipulator->handle(*ea, mActAdp))
//         handleGUIActionUpdate(mFocusedViewport);

    updateGL();
}


