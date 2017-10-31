#ifndef _QT_VIEWER_H
#define _QT_VIEWER_H 1

#include <osgGA/GUIEventAdapter>
#include <osgGA/MatrixManipulator>
#include <osgUtil/SceneView>
#include <osg/Timer>

#include <qgl.h>

#include <qteventadapter.h>
#include <qtactionadapter.h>

class QtViewer: public QGLWidget
{
    Q_OBJECT

public:
    QtViewer(QWidget *parent=0, const char *name=0);
    virtual ~QtViewer();

    virtual void setViewport(osgUtil::SceneView* sv, float x = 0.0, float y = 0.0, float width = 1.0, float height = 1.0);

    virtual void setViewport(osg::Node*, float x = 0.0, float y = 0.0, float width = 1.0, float height = 1.0);

    osgUtil::SceneView* getSceneView() { return mSceneView.get(); }

    // called on each frame redraw..return the time in ms for each operation.
    virtual float app();
    virtual float cull();
    virtual float draw();

    void registerCameraManipulator(osgGA::MatrixManipulator* cm);
    void selectCameraManipulator();

    /** Handle request from QtActionAdapter */
    void handleGUIActionUpdate();
            
    float clockSeconds() { return mTimer.delta_s(mInitialTime, mTimer.tick()); }

    void setUpdateCameraLens(bool v) { mUpdateCameraLens=v; }

protected:

    // Event handlers (Qt overrides)
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void keyReleaseEvent(QKeyEvent * e);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

    void initializeGL();
    void resizeGL( int, int );
    void paintGL();

    QtEventAdapter *makeEvent();
    
    // Qt implementation of GUIActionAdapter
    QtActionAdapter mActAdp;
        
     osg::ref_ptr<osgUtil::SceneView> mSceneView;
     float mViewport[4];  // Win-size-relative [0,1]
     osg::ref_ptr<osgGA::MatrixManipulator> mMatrixManipulator;
     
    osg::ref_ptr<osg::FrameStamp> mFrameStamp;
    osg::Timer   mTimer;
    osg::Timer_t mInitialTime;

    osg::ref_ptr<osg::DisplaySettings>  mDisplaySettings;

    bool mUpdateCameraLens; /// True by default, means that this class will update camera perspective
};

#endif
