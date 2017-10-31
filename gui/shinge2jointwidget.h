#ifndef SHINGE2ALJOINTWIDGET_H
#define SHINGE2JOINTWIDGET_H

#include "sguiproperties.h"
#include "uiHinge2JointWidget.h"
#include "shinge2.h"

class SHinge2JointWidget : public UiHinge2JointWidget, public SGuiProperties 
{
  Q_OBJECT

public:
    SHinge2JointWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    void setObject(SJoint * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SHinge2> mJoint;
};

#endif

