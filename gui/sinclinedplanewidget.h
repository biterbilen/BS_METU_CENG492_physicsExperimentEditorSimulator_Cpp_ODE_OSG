#ifndef _S_INPLANE_WIDGET_H
#define _S_INPLANE_WIDGET_H 1

#include "sguiproperties.h"
#include "uiInclinedPlaneWidget.h"
#include "sinclinedplane.h"

class SInclinedPlaneWidget : public UiInclinedPlaneWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SInclinedPlaneWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SInclinedPlaneWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SInclinedPlane> mObject;
};

#endif

