#ifndef _S_SPHERE_WIDGET_H
#define _S_SPHERE_WIDGET_H 1

#include "uiSphereWidget.h"
#include "sguiproperties.h"
#include "ssphere.h"

class SSphereWidget : public UiSphereWidget,  public SGuiProperties
{
    //Q_OBJECT
        
    public:
        SSphereWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SSphereWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    protected slots:
        
    private:
        osg::ref_ptr<SSphere> mObject;
};

#endif

