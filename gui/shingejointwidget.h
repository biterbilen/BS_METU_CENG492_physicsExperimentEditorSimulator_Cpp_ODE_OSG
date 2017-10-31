#ifndef SHINGEJOINTWIDGET_H
#define SHINGEJOINTWIDGET_H

#include "sguiproperties.h"
#include "uiHingeJointWidget.h"
#include "shinge.h"

class SHingeJointWidget : public UiHingeJointWidget, public SGuiProperties 
{
  Q_OBJECT

public:
    SHingeJointWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    void setObject(SJoint * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SHinge> mJoint;
};

#endif

