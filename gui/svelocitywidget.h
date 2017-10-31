#ifndef _S_VELOCITY_WIDGET_H
#define _S_VELOCITY_WIDGET_H 1

#include "sguiproperties.h"
#include "uiVelocityWidget.h"
#include "sobject.h"

class SVelocityWidget : public UiVelocityWidget, public SGuiProperties 
{
//     Q_OBJECT
    public:
        SVelocityWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SVelocityWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();
        
    private:
        osg::ref_ptr<SObject> mObject;
};

#endif

