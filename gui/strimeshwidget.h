#ifndef _S_TRIMESH_WIDGET_H
#define _S_TRIMESH_WIDGET_H 1

#include "sguiproperties.h"
#include "uiTrimeshWidget.h"
#include "strimesh.h"

class STrimeshWidget : public UiTrimeshWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        STrimeshWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~STrimeshWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<STrimesh> mObject;
};

#endif

