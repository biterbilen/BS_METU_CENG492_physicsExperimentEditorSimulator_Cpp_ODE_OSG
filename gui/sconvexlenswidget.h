#ifndef _S_CONVEX_LENS_WIDGET_H
#define _S_CONVEX_LENS_WIDGET_H 1

#include "sguiproperties.h"
#include "uiConvexLensWidget.h"
#include "sconvexlens.h"

class SConvexLensWidget : public UiConvexLensWidget, public SGuiProperties 
{
    //Q_OBJECT
    public:
        SConvexLensWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
        ~SConvexLensWidget() {}
    
    void setObject(SObject * obj);
    
    void save();
    void reset();

    signals:
        void nameChanged();
        
    private:
        osg::ref_ptr<SConvexLens> mObject;
};

#endif

