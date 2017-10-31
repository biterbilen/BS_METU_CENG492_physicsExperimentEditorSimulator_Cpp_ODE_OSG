#ifndef SBALLSOCKETJOINTWIDGET_H
#define SBALLSOCKETJOINTWIDGET_H

#include "sguiproperties.h"
#include "uiBallSocketJointWidget.h"
#include "sballsocket.h"

class SBallSocketJointWidget : public UiBallSocketJointWidget, public SGuiProperties 
{
  Q_OBJECT
public:
  SBallSocketJointWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    void setObject(SJoint * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SBallSocket> mJoint;
};

#endif

