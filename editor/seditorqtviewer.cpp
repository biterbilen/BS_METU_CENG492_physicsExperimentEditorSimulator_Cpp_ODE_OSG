#include "seditorqtviewer.h"

#include <qpopupmenu.h>
#include <qlabel.h>
#include <qcursor.h>

SEditorQtViewer::SEditorQtViewer(QWidget* parent, const char* name)
        : QtViewer(parent, name)
{
    mMenuBar = new QMenuBar(this);

    QPopupMenu *viewMenu = new QPopupMenu();
    viewMenu->insertItem( "Top",  this, SLOT(setTopView()));
    viewMenu->insertItem( "Left", this, SLOT(setLeftView()));
    viewMenu->insertItem( "Right", this, SLOT(setRightView()));
    viewMenu->insertItem( "Front", this, SLOT(setFrontView()));
    viewMenu->insertItem( "Back", this, SLOT(setBackView()));
    viewMenu->insertItem( "User", this, SLOT(setUserView()));
    mMenuBar->insertItem( "View", viewMenu );

    viewMenu->insertSeparator();

    QPopupMenu *polygonMenu = new QPopupMenu();
    polygonMenu->insertItem( "Solid",  this, SLOT(setSolid()));
    polygonMenu->insertItem( "Wireframe", this, SLOT(setWireframe()));
    viewMenu->insertItem( "Polygon Mode", polygonMenu );

    QPopupMenu * shadeMenu = new QPopupMenu();
    shadeMenu->insertItem( "Flat",  this, SLOT(setFlat()));
    shadeMenu->insertItem( "Smooth", this, SLOT(setSmooth()));
    viewMenu->insertItem( "Shade Mode", shadeMenu );
}

SEditorQtViewer::~SEditorQtViewer()
{}

void SEditorQtViewer::setCameraView(CameraView * cam)
{
    mCamView = cam;
    this->setViewport(mCamView->getSceneView());
}

void SEditorQtViewer::setTopView()
{
    mCamView->setViewType(CameraView::TOP);
}

void SEditorQtViewer::setLeftView()
{
    mCamView->setViewType(CameraView::LEFT);
}

void SEditorQtViewer::setRightView()
{
    mCamView->setViewType(CameraView::RIGHT);
}

void SEditorQtViewer::setFrontView()
{
    mCamView->setViewType(CameraView::FRONT);
}

void SEditorQtViewer::setBackView()
{
    mCamView->setViewType(CameraView::BACK);
}

void SEditorQtViewer::setUserView()
{
    mCamView->setViewType(CameraView::USER);
}

void  SEditorQtViewer::setSolid()
{
    mCamView->setPolygonMode(osg::PolygonMode::FILL);
}

void  SEditorQtViewer::setWireframe()
{
    mCamView->setPolygonMode(osg::PolygonMode::LINE);
}

void  SEditorQtViewer::setFlat()
{
    mCamView->setShadeModel(osg::ShadeModel::FLAT);
}

void  SEditorQtViewer::setSmooth()
{
    mCamView->setShadeModel(osg::ShadeModel::SMOOTH);
}
