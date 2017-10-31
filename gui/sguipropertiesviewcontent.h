#ifndef _S_GUI_PROPERTIES_VIEW_CONTENT_H
#define _S_GUI_PROPERTIES_VIEW_CONTENT_H 1

#include <qwidget.h>
#include <qscrollview.h>

class SGuiPropertiesViewContent : public QScrollView
{
    Q_OBJECT
public:
    SGuiPropertiesViewContent( QWidget* parent, const char* name = 0 );
    void setContent( QWidget * wid );
    void calculateSize( );
protected:
    void resizeEvent( QResizeEvent* /* ev */ );
private:
    QWidget * mContent;
};

#endif


