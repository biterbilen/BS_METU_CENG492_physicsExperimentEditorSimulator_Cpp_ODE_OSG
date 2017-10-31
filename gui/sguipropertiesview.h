#ifndef _S_GUI_PROPERTIES_VIEW_H
#define _S_GUI_PROPERTIES_VIEW_H 1

#include "uiPropertiesViewWidget.h"
#include "sguiproperties.h"
#include "sguitreeviewitem.h"

/**@class SGuiPropertiesView
 *@brief This class is the view for the poperties of objects
 */
class SGuiPropertiesView : public UiPropertiesViewWidget
{
    Q_OBJECT

public:
    SGuiPropertiesView(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~SGuiPropertiesView();

    void updateProperties();

    SGuiProperties * getProperties() const { return mProperties; }

protected slots:
    void treeViewSelectionChanged(QListViewItem *);
    void applyButtonClicked();
    void cancelButtonClicked();

signals:
    void applyClicked();
    void cancelClicked();

private:
    void propertiesShow(SObject * obj);
    void propertiesShow(SJoint * joint);
    void propertiesShow(SForce * force);
    void propertiesShow(SEnvironment * env);

    SGuiProperties * mProperties;
    SGuiTreeViewItem * mTreeViewSelectedItem;
};

#endif
