#ifndef _S_CAPPED_CYLINDER_WIDGET_H
#define _S_CAPPED_CYLINDER_WIDGET_H 1

#include "uiCappedCylinderWidget.h"
#include "sguiproperties.h"
#include "scappedcylinder.h"

class SCappedCylinderWidget : public UiCappedCylinderWidget,  public SGuiProperties
{
    //Q_OBJECT
        
    public:
        SCappedCylinderWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SCappedCylinderWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    protected slots:
        
    private:
        osg::ref_ptr<SCappedCylinder> mObject;
};

#endif

