#ifndef _S_EDITOR_QTVIEWER_H
#define _S_EDITOR_QTVIEWER_H

#include <qmenubar.h>

#include "qtviewer.h"
#include "cameraview.h"

class SEditorQtViewer : public QtViewer
{
Q_OBJECT
public:
    SEditorQtViewer(QWidget* parent = 0, const char* name = 0);
    ~SEditorQtViewer();
    
    void setCameraView(CameraView * cam);

protected slots:
    void setTopView();
    void setLeftView();
    void setRightView();
    void setFrontView();
    void setBackView();
    void setUserView();

    void setSolid();
    void setWireframe();
    void setFlat();
    void setSmooth();

private:
    CameraView * mCamView;
    
    QMenuBar * mMenuBar;
};
#endif

