#ifndef _SMAINWINDOW_H
#define _SMAINWINDOW_H 1

#include "editor.h"
#include "uiMainWindow.h"

#include <qmessagebox.h>
#include <qlistbox.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <qdockwindow.h>
#include <qscrollview.h>
#include <qlayout.h>
#include <qapplication.h>
#include <qlabel.h>
#include <qdockarea.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qpopupmenu.h>
#include <qcursor.h>

#include "seditorqtviewer.h"
#include "ssimulation.h"
#include "ssimulationqtviewer.h"

#include "sguipropertiesview.h"
#include "sguitreeview.h"

#include "drawingareaview.h"

class DrawingAreaView;

class SMainWindow : public UiMainWindow, public Editor
{
    Q_OBJECT

public:
    SMainWindow(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    
    void executed(Command *);
    void unexecuted(Command *);

    void registerDrawingAreaView(DrawingAreaView *v);
    bool mouseMoved(float x, float y, CameraView * c);
    bool mousePressed(float x, float y, CameraView * c);
    bool mouseReleased(float x, float y, CameraView * c);
    
    /** update openGL editor views */
    void updateViews();

public slots:
    virtual void runSimulation();    
    
    // Add Objects
    virtual void addCylinder();
    virtual void addSphere();
    virtual void addBox();
    virtual void addPool();
    virtual void addGear();
    virtual void addInclinedPlane();
    virtual void addTriMesh();
    
    virtual void addLightSource();
    virtual void addFlatMirror();
    virtual void addConMirror();
    virtual void addConvexLens();
    virtual void addConcavLens();
    
    // Add Joints
    virtual void addBallSocketJoint();
    virtual void addSliderJoint();
    virtual void addUniversalJoint();
    virtual void addHinge2Joint();
    virtual void addHingeJoint();
    
    // add Normal Forces
    virtual void addNormalForce();
    virtual void addNormalForceAtPos();
    virtual void addNormalForceAtRelPos();
    virtual void addNormalTurque();
    
    // Add Relative Forces
    virtual void addRelativeForce();
    virtual void addRelativeForceAtPos();
    virtual void addRelativeForceAtRelPos();
    virtual void addRelativeTorque();
    
    virtual void toolMove();
    virtual void toolRotate();
    virtual void toolZoom();
    virtual void toolSelect();
    virtual void toolOrbit();
    virtual void toolPan();
    virtual void toolRedo();
    virtual void toolUndo();
    
    virtual void helpAbout();
    virtual void fileExit();
    virtual void fileSaveAs();
    virtual void fileSave();
    virtual void fileOpen();
    virtual void fileNew();

protected slots:
    void deleteItem(SGuiTreeViewItem *);
    void stopSimulation();
    void applyPropertiesView();
    void cancelPropertiesView();

protected:
    ~SMainWindow();

    virtual void selected(osg::Node *node);

private:

     /** We need to notify this for clicks and mouse movements */
    osg::ref_ptr<DrawingAreaView> mDrawingAreaView;

    QDockWindow * mTreeViewDock;
    
    QDockWindow * mPropertiesViewDock;
    SGuiPropertiesView * mPropertiesView;
    
    SSimulation * mSimulation;
    SSimulationQtViewer * mSimQtViewer;
    
    unsigned int mContextIDcounter;
};

#endif

