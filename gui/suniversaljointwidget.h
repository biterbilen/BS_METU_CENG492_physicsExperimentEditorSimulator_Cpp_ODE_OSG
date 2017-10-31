#ifndef SUNIVERSALJOINTWIDGET_H
#define SUNIVERSALJOINTWIDGET_H

#include "sguiproperties.h"
#include "uiUniversalJointWidget.h"
#include "suniversal.h"

class SUniversalJointWidget : public UiUniversalJointWidget, public SGuiProperties 
{
  Q_OBJECT

public:
    SUniversalJointWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    void setObject(SJoint * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SUniversal> mJoint;
};

#endif

