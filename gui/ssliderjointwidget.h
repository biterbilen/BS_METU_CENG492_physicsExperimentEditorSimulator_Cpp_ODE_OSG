#ifndef SSLIDERJOINTWIDGET_H
#define SSLIDERJOINTWIDGET_H

#include "sguiproperties.h"
#include "uiSliderJointWidget.h"
#include "sslider.h"

class SSliderJointWidget : public UiSliderJointWidget, public SGuiProperties 
{
  Q_OBJECT

public:
    SSliderJointWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

    void setObject(SJoint * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SSlider> mJoint;
};

#endif

