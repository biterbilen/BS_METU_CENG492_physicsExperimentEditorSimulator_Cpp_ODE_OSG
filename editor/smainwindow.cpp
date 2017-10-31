#include "smainwindow.h"

#include <osgGA/MatrixManipulator>
#include <osgGA/TrackballManipulator>

#include "sosggeode.h"
#include "comaddchild.h"

class NullManipulator: public osgGA::MatrixManipulator
{
private:
    float ancho, alto;
    SMainWindow * mMainWindow;
    osg::ref_ptr<CameraView> cameraView;
public:
    NullManipulator(SMainWindow *p, CameraView *cam) { mMainWindow=p; cameraView=cam; }
    /** set the position of the matrix manipulator using a 4x4 Matrix.*/
    virtual void setByMatrix(const osg::Matrix& /*matrix*/) {}

    /** set the position of the matrix manipulator using a 4x4 Matrix.*/
    virtual void setByInverseMatrix(const osg::Matrix& /*matrix*/) {}

    /** get the position of the manipulator as 4x4 Matrix.*/
    virtual osg::Matrix getMatrix() const
    {
        return cameraView->getViewMatrix();
    }

    /** get the position of the manipulator as a inverse matrix of the manipulator,
    * typically used as a model view matrix.*/
    virtual osg::Matrix getInverseMatrix() const
    {
        osg::Matrix m = cameraView->getViewMatrix();
        return osg::Matrix::inverse(m);
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
    {
        switch (ea.getEventType())
        {
        case osgGA::GUIEventAdapter::RESIZE:
            {
                ancho=ea.getXmax();
                alto=ea.getYmax();
                cameraView->setAspectRatio((float)ancho / (float)alto);
                cameraView->applyViewType();
                us.requestRedraw();
                return true;
            }
            break;
        case osgGA::GUIEventAdapter::PUSH:
            {
                float px=ea.getX()/(float)ancho;
                float py=1-ea.getY()/(float)alto;
                us.requestRedraw();
                mMainWindow->mousePressed(px, py, cameraView.get());
                return true;
            }
            break;
        case osgGA::GUIEventAdapter::RELEASE:
            {
                float px=ea.getX()/(float)ancho;
                float py=1-ea.getY()/(float)alto;
                mMainWindow->mouseReleased(px, py, cameraView.get());
                us.requestRedraw();
                return true;
            }
            break;
        case osgGA::GUIEventAdapter::MOVE:
        case osgGA::GUIEventAdapter::DRAG:
            {
                float px=ea.getX()/(float)ancho;
                float py=1-ea.getY()/(float)alto;
                mMainWindow->mouseMoved(px,py, cameraView.get());
                us.requestRedraw();
                return true;
            }
            break;
        default:
            return false;
        }
        return false;
    }
};

SMainWindow::SMainWindow(QWidget* parent, const char* name, WFlags fl)
        : UiMainWindow(parent,name,fl)
{
    // initialization of some variables
    mSimQtViewer = 0;
    mSimulation = 0;

    mTreeViewDock = new QDockWindow(this, "Objects");
    mTreeView = new SGuiTreeView(mEnv.get(), mTreeViewDock);

    connect(mTreeView, SIGNAL (deletingItem(SGuiTreeViewItem *)), this, SLOT (deleteItem(SGuiTreeViewItem *)));

//     mTreeViewDock->setCloseMode(QDockWindow::Always);
    mTreeViewDock->setResizeEnabled(true);
    mTreeViewDock->setHorizontalStretchable(true);
    mTreeViewDock->setVerticalStretchable(true);
    mTreeViewDock->setWidget(mTreeView);
    moveDockWindow(mTreeViewDock, Qt::Left );

    mPropertiesViewDock = new QDockWindow(this, "Properties");
    mPropertiesView = new SGuiPropertiesView(mPropertiesViewDock);

    connect(mPropertiesView, SIGNAL(applyClicked()), this, SLOT(applyPropertiesView()));
    connect(mPropertiesView, SIGNAL(cancelClicked()), this, SLOT(cancelPropertiesView()));
    
    connect(mTreeView, SIGNAL (selectionChanged(QListViewItem *)), 
                     mPropertiesView, SLOT (treeViewSelectionChanged(QListViewItem *)));

//     mPropertiesViewDock->setCloseMode(QDockWindow::Always);
    mPropertiesViewDock->setResizeEnabled(true);
    mPropertiesViewDock->setHorizontalStretchable(true);
    mPropertiesViewDock->setVerticalStretchable(true);
    mPropertiesViewDock->setWidget(mPropertiesView);
    moveDockWindow(mPropertiesViewDock, Qt::Right );

    this->topDock()->setAcceptDockWindow ( mTreeViewDock, false );
    this->bottomDock()->setAcceptDockWindow ( mTreeViewDock, false );
    this->topDock()->setAcceptDockWindow ( mPropertiesViewDock, false );
    this->bottomDock()->setAcceptDockWindow ( mPropertiesViewDock, false );

    mContextIDcounter = 0;
    getCameraView(0)->setData(sceneGraph->getRealRoot(0));
    getCameraView(0)->getSceneView()->getState()->setContextID(mContextIDcounter++);
    getCameraView(0)->setViewType(CameraView::TOP);

    getCameraView(1)->setData(sceneGraph->getRealRoot(1));
    getCameraView(1)->getSceneView()->getState()->setContextID(mContextIDcounter++);
    getCameraView(1)->setViewType(CameraView::RIGHT);

    getCameraView(2)->setData(sceneGraph->getRealRoot(2));
    getCameraView(2)->getSceneView()->getState()->setContextID(mContextIDcounter++);
    getCameraView(2)->setViewType(CameraView::FRONT);

    getCameraView(3)->setData(sceneGraph->getRealRoot(3));
    getCameraView(3)->getSceneView()->getState()->setContextID(mContextIDcounter++);
    getCameraView(3)->setViewType(CameraView::USER);

    qtViewer1->setCameraView(getCameraView(0));
    qtViewer2->setCameraView(getCameraView(1));
    qtViewer3->setCameraView(getCameraView(2));
    qtViewer4->setCameraView(getCameraView(3));

    qtViewer1->registerCameraManipulator(new NullManipulator(this,getCameraView(0)));
    qtViewer2->registerCameraManipulator(new NullManipulator(this,getCameraView(1)));
    qtViewer3->registerCameraManipulator(new NullManipulator(this,getCameraView(2)));
    qtViewer4->registerCameraManipulator(new NullManipulator(this,getCameraView(3)));
}

SMainWindow::~SMainWindow()
{}

void SMainWindow::fileExit()
{
    QApplication::exit(0);
}

void SMainWindow::runSimulation()
{
    if(mSimQtViewer != 0)
        delete mSimQtViewer;
    if(mSimulation != 0)
        delete mSimulation;

    mSimulation = new SSimulation(mEnv.get());
    mSimQtViewer = new SSimulationQtViewer(mSimulation);
    mSimQtViewer->getSceneView()->getState()->setContextID(mContextIDcounter++);

    connect(mSimQtViewer, SIGNAL(simulationClosed()), this, SLOT(stopSimulation()));

    this->setDisabled(true);

    mSimQtViewer->resize(640, 480);
    mSimQtViewer->setCaption( "Simulation Running..." );
    mSimQtViewer->show();
}

void SMainWindow::stopSimulation()
{
    if(mSimQtViewer != 0)
        delete mSimQtViewer;
    if(mSimulation != 0)
        delete mSimulation;

    mSimulation = 0;
    mSimQtViewer = 0;

    mEnv->reset();
    this->setEnabled(true);
}

void SMainWindow::addCylinder() {  useTool("make-ccylinder"); }
void SMainWindow::addSphere()  { useTool("make-sphere"); }
void SMainWindow::addBox() { useTool("make-box"); }
void SMainWindow::addPool() { useTool("make-pool"); }
void SMainWindow::addGear() { useTool("make-gear"); }
void SMainWindow::addInclinedPlane() { useTool("make-inclined-plane"); }
void SMainWindow::addTriMesh() 
{ 
    QString fName = QFileDialog::getOpenFileName("./", "3DS File (*.3ds)", this);

    if(!fName.isNull())
    {
        STrimesh * triMesh = new STrimesh(SObject::STATIC, fName.ascii());
        triMesh->createEditorObject();

        osg::Node * node = triMesh->getTransformNode();
                
        osg::ref_ptr<ComAddChild> com = new ComAddChild(node, this);
        com->execute();
    }
}

void SMainWindow::addLightSource() { useTool("make-light-source"); }
void SMainWindow::addFlatMirror() { useTool("make-flat-mirror"); }
void SMainWindow::addConMirror() {useTool("make-con-mirror");}
void SMainWindow::addConvexLens() { useTool("make-convex-lens"); }
void SMainWindow::addConcavLens() { useTool("make-concav-lens"); }

// Add Joints
void SMainWindow::addBallSocketJoint() { useTool("add-ball-socket-joint");}
void SMainWindow::addSliderJoint() { useTool("add-slider-joint");}
void SMainWindow::addUniversalJoint() { useTool("add-universal-joint");}
void SMainWindow::addHinge2Joint() { useTool("add-hinge2-joint");}
void SMainWindow::addHingeJoint() { useTool("add-hinge-joint");}

// add Normal Forces
void SMainWindow::addNormalForce() { useTool("add-normal-force"); }
void SMainWindow::addNormalForceAtPos() { useTool("add-normal-force-at-pos"); }
void SMainWindow::addNormalForceAtRelPos() { useTool("add-normal-force-at-rel-pos"); }
void SMainWindow::addNormalTurque() { useTool("add-normal-torque"); }

// Add Relative Forces
void SMainWindow::addRelativeForce() { useTool("add-relative-force"); }
void SMainWindow::addRelativeForceAtPos() { useTool("add-relative-force-at-pos"); }
void SMainWindow::addRelativeForceAtRelPos() { useTool("add-relative-force-at-rel-pos"); }
void SMainWindow::addRelativeTorque() { useTool("add-relative-torque"); }

void SMainWindow::toolMove() { useTool("move"); }
void SMainWindow::toolRotate() { useTool("rotate"); }

void SMainWindow::toolZoom() { useTool("zoom"); }
void SMainWindow::toolSelect() { useTool("pick"); }

void SMainWindow::toolOrbit() { useTool("orbit"); }
void SMainWindow::toolPan() { useTool("pan"); }

void SMainWindow::toolUndo() { useTool("undo"); }
void SMainWindow::toolRedo() { useTool("redo"); }

void SMainWindow::helpAbout() {}

void SMainWindow::fileSaveAs()
{
    QString fName = QFileDialog::getSaveFileName("./", "Sourcerer File Format (*.sff)", this);

    if(!fName.isNull())
    {
        this->saveFileAs(fName.ascii());
    }
}

void SMainWindow::fileSave()
{
    if(this->getLoadedFile() == "")
    {
        fileSaveAs();
    }
    else
    {
        this->saveFile();
    }
}

void SMainWindow::fileNew()
{
    this->setEnvironment(new SEnvironment());
    mLoadedFileName = "";
    updateViews();
}

void SMainWindow::fileOpen()
{
    QString fName = QFileDialog::getOpenFileName("./", "Sourcerer File Format (*.sff)", this);

    if(!fName.isNull())
    {
        this->loadFile(fName.ascii());
        updateViews();
    }
}

void SMainWindow::deleteItem(SGuiTreeViewItem * sgtvi)
{
        switch(sgtvi->getItemType())
        {
        case SGuiTreeViewItem::SOBJECT:
            {
                std::cout << "smainwindow: remove object" << std::endl;
                SObject * obj = sgtvi->getItem(obj);
                this->getSceneGraph()->removeChild(obj->getTransformNode());
                
                for(list<SForce*>::iterator i = obj->getForceBegin() ; i != obj->getForceEnd() ; i++)
                {
                    this->getSceneGraph()->removeChild( (*i)->getTransformNode());
                    mTreeView->removeForce(obj, (*i));
                }
                
                for(list<SJoint*>::iterator i = obj->getJointBegin() ; i != obj->getJointEnd() ; i++)
                {
                    this->getSceneGraph()->removeChild( (*i)->getTransformNode());
                    mTreeView->removeJoint((*i));
                }
                
                this->getEnvironment()->removeObjectJoint(obj);
                mTreeView->removeObject(obj);
                updateViews();
            }
            break;
        case SGuiTreeViewItem::FORCE:
            {
                std::cout << "smainwindow: remove force" << std::endl;
                SForce * obj = sgtvi->getItem(obj);
                this->getSceneGraph()->removeChild(obj->getTransformNode());
                this->getEnvironment()->removeForce(obj);
                mTreeView->removeForce(obj->getObject(), obj);
                updateViews();
            }
            break;
        case SGuiTreeViewItem::JOINT:
            {
                std::cout << "smainwindow: remove joint" << std::endl;
                SJoint * obj = sgtvi->getItem(obj);
                this->getSceneGraph()->removeChild(obj->getTransformNode());
                this->getEnvironment()->removeJoint(obj);
                mTreeView->removeJoint(obj);
                updateViews();
            }
            break;
        default: break;
        }
}


void SMainWindow::applyPropertiesView()
{
    if(mPropertiesView->getProperties())
    {
        mPropertiesView->getProperties()->save();
        mPropertiesView->getProperties()->reset();

        if(mPropertiesView->getProperties()->needsRedraw())
        {
            updateViews();
        }
    }
}

void SMainWindow::cancelPropertiesView()
{
    if(mPropertiesView->getProperties())
    {
        mPropertiesView->getProperties()->reset();
    }
}

void SMainWindow::selected(osg::Node *node)
{
    SOsgGeode * sgeode = dynamic_cast<SOsgGeode *>(node);

    if( sgeode == 0)
    {
        std::cout << "FATAL ERROR: cannot convert osg::Node* to SOsgGeode*" << std::endl;
        return;
    }

    // We are getting the object data from the geode
    SObject * obj = sgeode->getSObject();

    mTreeView->setSelectedItem(obj);
}

void SMainWindow::executed(Command * com)
{
    mPropertiesView->updateProperties();
    if(com->needsRedraw())
    {
        updateViews();
    }
}

void SMainWindow::unexecuted(Command * com)
{
    mPropertiesView->updateProperties();
    //mTreeView->repaint();

    if(com->needsRedraw())
    {
        updateViews();
    }
}

void SMainWindow::registerDrawingAreaView(DrawingAreaView *v)
{
    mDrawingAreaView=v;
}

bool SMainWindow::mouseMoved(float x, float y, CameraView * camView)
{
    if (mDrawingAreaView.valid())
    {
        mDrawingAreaView->mouseMoved(x, y, camView);
        updateViews();

        return true;
    }

    return false;
}

bool SMainWindow::mousePressed(float x, float y, CameraView * camView)
{
    if (mDrawingAreaView.valid())
    {
        mDrawingAreaView->mousePressed(x, y, camView);
        updateViews();

        return true;
    }

    return false;
}

bool SMainWindow::mouseReleased(float x, float y, CameraView * camView)
{
    if (mDrawingAreaView.valid())
    {
        mDrawingAreaView->mouseReleased(x, y, camView);
        updateViews();

        return true;
    }

    return false;
}

void SMainWindow::updateViews()
{
    qtViewer1->updateGL();
    qtViewer2->updateGL();
    qtViewer3->updateGL();
    qtViewer4->updateGL();
}

