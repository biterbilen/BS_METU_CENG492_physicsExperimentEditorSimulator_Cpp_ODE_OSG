#include "qtviewer.h"
#include "qteventadapter.h"

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgUtil/DisplayRequirementsVisitor>


QtViewer::QtViewer(QWidget *parent, const char *name) : QGLWidget( parent, name)
{
    mDisplaySettings=new osg::DisplaySettings();
    mFrameStamp=new osg::FrameStamp();
    mInitialTime= mTimer.tick();
    mUpdateCameraLens=true;
}

QtViewer::~QtViewer()
{}

void QtViewer::setViewport(osgUtil::SceneView* sv, float x, float y, float width, float height )
{
    mSceneView   = sv;
    mViewport[0] = x;
    mViewport[1] = y;
    mViewport[2] = width;
    mViewport[3] = height;
}

void QtViewer::setViewport(osg::Node* rootnode, float x, float y, float width, float height )
{
    osgUtil::SceneView *sceneView = new osgUtil::SceneView(mDisplaySettings.get());
    sceneView->setDefaults();
    sceneView->setSceneData(rootnode);

    setViewport( sceneView, x, y, width, height );
}


void QtViewer::initializeGL()
{
    if ( !mSceneView.valid() )
    {
        osg::notify(osg::FATAL)<<"QtViewer::open() called with no Viewports registered."<< std::endl;
        return;
    }

    if (!mMatrixManipulator.valid())
    {
        osg::notify(osg::INFO)<<"QtViewer::open() called without any camara manipulator registered,"<< std::endl;
        osg::notify(osg::INFO)<<"automatically registering trackball manipulator"<< std::endl;

        registerCameraManipulator(new osgGA::TrackballManipulator);
        selectCameraManipulator();
    }

    bool needQuadBufferStereo = false;

    // do we need quad buffer stereo?

    // Set the absolute viewport for each SceneView based on the
    //   relative viewport coordinates given to us
    int view[4] = { int(mViewport[0]*this->width()), int(mViewport[1]*this->height()),
                    int(mViewport[2]*this->width()), int(mViewport[3]*this->height()) };

    mSceneView->setViewport(view[0], view[1], view[2], view[3]);
    if (view[3] > 0)
    {
        if (mUpdateCameraLens)
        {
            mSceneView->setProjectionMatrixAsPerspective(40.0, view[2]/(double)view[3], 1, 1000);
        }
    }

    const osg::DisplaySettings* vs = mSceneView->getDisplaySettings();
    if (vs)
    {
        mDisplaySettings->merge(*vs);
    }
    else
    {
        // one does not already exist so attach the viewers visualsSettins to the SceneView
        mSceneView->setDisplaySettings(mDisplaySettings.get());
    }


    if (mDisplaySettings->getStereo() &&
            mDisplaySettings->getStereoMode()==osg::DisplaySettings::QUAD_BUFFER) needQuadBufferStereo = true;

    // traverse the scene graphs gathering the requirements of the OpenGL buffers.
    osgUtil::DisplayRequirementsVisitor drv;
    drv.setDisplaySettings(mDisplaySettings.get());

    osg::Node* node = mSceneView->getSceneData();
    if (node) node->accept(drv);


    // set up each render stage to clear the appropriate buffers.
    GLbitfield clear_mask=0;
    if (mDisplaySettings->getRGB())              clear_mask |= GL_COLOR_BUFFER_BIT;
    if (mDisplaySettings->getDepthBuffer())      clear_mask |= GL_DEPTH_BUFFER_BIT;
    if (mDisplaySettings->getStencilBuffer())    clear_mask |= GL_STENCIL_BUFFER_BIT;


    osgUtil::RenderStage *stage = mSceneView->getRenderStage();
    stage->setClearMask(clear_mask);

    // Reset the views of all of SceneViews
    osg::ref_ptr<QtEventAdapter> ea = new QtEventAdapter;
    mMatrixManipulator->home(*ea, mActAdp);
}

// called on each frame redraw..return the time in ms for each operation.
float QtViewer::app()
{
    //     /*FIXME
    //     * the calls below will be performed on the inherited
    //     *
    //     */
    //     osg::ref_ptr<QtEventAdapter> ea = new QtEventAdapter();
    //     ea->adaptFrame(/*mFrameStamp->getReferenceTime()*/);
    //     if(mViewportList[viewport].mMatrixManipulator->handle(*ea, mActAdp))
    //         handleGUIActionUpdate(viewport);

    mSceneView->setFrameStamp(mFrameStamp.get());
    mSceneView->setViewMatrix(mMatrixManipulator->getInverseMatrix());
    mSceneView->update();

    return 0.0f;
}

float QtViewer::cull()
{
    mSceneView->cull();
    return 0.0f;
}

float QtViewer::draw()
{
    mSceneView->draw();
    return 0.0f;
}

void QtViewer::registerCameraManipulator(osgGA::MatrixManipulator* cm)
{
    mMatrixManipulator = cm;
}

void QtViewer::selectCameraManipulator()
{
    mMatrixManipulator->setNode(mSceneView->getSceneData());

    osg::ref_ptr<QtEventAdapter> ea = new QtEventAdapter;
    mMatrixManipulator->init(*ea, mActAdp);
}

QtEventAdapter *QtViewer::makeEvent()
{
    int view[4] = { int(mViewport[0]*this->width()), int(mViewport[1]*this->height()),
                    int(mViewport[2]*this->width()), int(mViewport[3]*this->height()) };

    QtEventAdapter *ea = new QtEventAdapter();
    ea->adaptResize(view[0], view[1], view[0]+view[2], view[1]+view[3]);
    return ea;
}

void QtViewer::keyPressEvent(QKeyEvent */*e*/)
{
    /*TODO have to control this if it works */

    //     osg::ref_ptr<QtEventAdapter> ea = makeEvent();
    //     ea->adaptKeyPressEvent(e);
    //     if(mViewportList[mFocusedViewport].mMatrixManipulator->handle(*ea, mActAdp))
    //         handleGUIActionUpdate(mFocusedViewport);
}

void QtViewer::keyReleaseEvent(QKeyEvent */*e*/)
{
    /*TODO have to control this if it works */

    //     osg::ref_ptr<QtEventAdapter> ea = makeEvent();
    //     ea->adaptKeyReleaseEvent(e);
    //     if(mViewportList[mFocusedViewport].mMatrixManipulator->handle(*ea, mActAdp))
    //         handleGUIActionUpdate(mFocusedViewport);
}

void QtViewer::mouseMoveEvent(QMouseEvent *e)
{
    osg::ref_ptr<QtEventAdapter> ea = makeEvent();
    ea->adaptMouseMoveEvent(e);

    if(mMatrixManipulator->handle(*ea, mActAdp))
        handleGUIActionUpdate();
}

void QtViewer::mousePressEvent(QMouseEvent *e)
{
    osg::ref_ptr<QtEventAdapter> ea = makeEvent();
    ea->adaptMousePressEvent(e);

    if(mMatrixManipulator->handle(*ea, mActAdp))
        handleGUIActionUpdate();
}

void QtViewer::mouseReleaseEvent(QMouseEvent *e)
{
    osg::ref_ptr<QtEventAdapter> ea = makeEvent();
    ea->adaptMouseReleaseEvent(e);

    if(mMatrixManipulator->handle(*ea, mActAdp))
        handleGUIActionUpdate();
}

void QtViewer::resizeGL(int width, int height)
{
    // Propagate new window size
    int view[4] = { int(mViewport[0]*this->width()), int(mViewport[1]*this->height()),
                    int(mViewport[2]*this->width()), int(mViewport[3]*this->height()) };

    mSceneView->setViewport(view[0], view[1], view[2], view[3]);
    if (mViewport[3] > 0)
    {
        if (mUpdateCameraLens)
        {
            mSceneView->setProjectionMatrixAsPerspective(40.0, view[2]/(double)view[3], 1, 1000);
        }
    }

    osg::ref_ptr<QtEventAdapter> ea = new QtEventAdapter;
    ea->adaptResize(view[0], view[1], view[0]+view[2], view[1]+view[3]);

    mMatrixManipulator->handle(*ea, mActAdp);
}

void QtViewer::paintGL()
{
    mFrameStamp->setFrameNumber(mFrameStamp->getFrameNumber()+1);
    mFrameStamp->setReferenceTime(clockSeconds());


    // application traversal.
    app();

    // cull traversal.
    cull();

    // draw traversal.
    draw();
}

void QtViewer::handleGUIActionUpdate()
{
    if(/*mActAdp.getNeedRedraw() && */mSceneView->getSceneData())
    {
        updateGL();
    }

    /*
        if(redrawPolicy_==AS_NEEDED){
            if(aa_.getNeedContinuousUpdate()){
        
                if( !timer()->isActive() ) timer()->start(0);
        
            }else{
     
                if( timer()->isActive() ) timer()->stop();
     
            }
        } // else we're in CONSTANT redraw mode, and may ignore the aa wrt to continuous update.
      
        float x,y;
        if(aa_.getNeedWarpPointer(x,y)){
            QCursor::setPos(mapToGlobal(QPoint(static_cast<int>(x),static_cast<int>(y))));
            aa_.clearWarpRequest();
        }
    */
}


