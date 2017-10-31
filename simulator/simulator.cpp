#include <qapplication.h>

#include <senvironment.h>
#include <ssimulationviewer.h>

int main(int argc, char **argv)
{

    QApplication::setColorSpec(QApplication::CustomColor);
    QApplication a(argc, argv);

    if (!QGLFormat::hasOpenGL())
    {
        qWarning("This system has no OpenGL support. Exiting.");
        return -1;
    }
    
    SEnvironment* env = new SEnvironment;
    env->setGravity(osg::Vec3(0, 0, -9.8));
    env->setCFM(1e-5);
    env->setMu(0.4);
    env->setStepSize(0.005);
    
    SSimulationViewer * simViewer = new SSimulationViewer();
    simViewer->setEnvironment(env);
    
    a.setMainWidget(simViewer);
    simViewer->show();
    return a.exec();
}
