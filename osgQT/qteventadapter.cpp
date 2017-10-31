#include <iostream>
#include "qteventadapter.h"

// This should be easier than this:
osg::Timer QtEventAdapter::timer_;
const osg::Timer_t QtEventAdapter::initialTick_=QtEventAdapter::timer_.tick();

QtEventAdapter::QtEventAdapter()
{
    // Initialise all values to 'empty'
    eventType_  = NONE; // adapter does not encapsulate any events.
    key_        = -1;   // set to 'invalid' key value.
    button_     = -1;   // set to 'invalid' button value.
    mX_         = -1;   // set to 'invalid' position value.
    mY_         = -1;   // set to 'invalid' position value.
    buttonMask_ = 0;    // default to no mouse buttons being pressed.
	modKeyMask_ = 0;    // default to no mod keys being pressed
    timestamp_  = timer_.delta_s(initialTick_,timer_.tick());
}

void QtEventAdapter::adaptMousePressEvent(const QMouseEvent *e)
{
    eventType_ = PUSH;
    setMouseEventDetails(e);
}

void QtEventAdapter::adaptMouseReleaseEvent(const QMouseEvent *e)
{
    eventType_ = RELEASE;
    setMouseEventDetails(e);
}

void QtEventAdapter::adaptMouseMoveEvent(const QMouseEvent *e)
{
    if(e->state()&Qt::NoButton){
        eventType_=MOVE;
    }else{
        eventType_ = DRAG;      // NB, in Qt, a drag event is drag n drop
    }

    setMouseEventDetails(e);
}

void QtEventAdapter::adaptKeyPressEvent(const QKeyEvent *e)
{
    eventType_ = KEYDOWN;
    key_ = e->ascii();
	if(e->state() & Qt::ControlButton) modKeyMask_ &= GUIEventAdapter::MODKEY_CTRL;
	if(e->state() & Qt::ShiftButton) modKeyMask_ &= GUIEventAdapter::MODKEY_SHIFT;
	if(e->state() & Qt::MetaButton) modKeyMask_ &= GUIEventAdapter::MODKEY_META;
	if(e->state() & Qt::AltButton) modKeyMask_ &= GUIEventAdapter::MODKEY_ALT;
}

void QtEventAdapter::adaptKeyReleaseEvent(const QKeyEvent *e)
{
    eventType_ = KEYUP;
    key_ = e->ascii();
	if(e->state() & Qt::ControlButton) modKeyMask_ &= GUIEventAdapter::MODKEY_CTRL;
	if(e->state() & Qt::ShiftButton) modKeyMask_ &= GUIEventAdapter::MODKEY_SHIFT;
	if(e->state() & Qt::MetaButton) modKeyMask_ &= GUIEventAdapter::MODKEY_META;
	if(e->state() & Qt::AltButton) modKeyMask_ &= GUIEventAdapter::MODKEY_ALT;
}

void QtEventAdapter::adaptResize(int xMin, int yMin, int xMax, int yMax)
{
    eventType_ = RESIZE;
    xMin_ = xMin;
    yMin_ = yMin;
    xMax_ = xMax;
    yMax_ = yMax;
}

void QtEventAdapter::adaptFrame(/*int mouseX, int mouseY*/)
{
    eventType_ = FRAME;
    
    /*
    mX_ = mouseX;
    mY_ = mouseY;
    */
}

void QtEventAdapter::setMouseEventDetails(const QMouseEvent* e)
{
    button_ = e->button();
    if(e->stateAfter() & QEvent::LeftButton) buttonMask_ |= LEFT_MOUSE_BUTTON;
    if(e->stateAfter() & QEvent::MidButton) buttonMask_  |= MIDDLE_MOUSE_BUTTON;
    if(e->stateAfter() & QEvent::RightButton)buttonMask_ |= RIGHT_MOUSE_BUTTON;

    mX_ = e->x();
    mY_ = e->y();

	if(e->state() & Qt::ControlButton) modKeyMask_ &= GUIEventAdapter::MODKEY_CTRL;
	if(e->state() & Qt::ShiftButton) modKeyMask_ &= GUIEventAdapter::MODKEY_SHIFT;
	if(e->state() & Qt::MetaButton) modKeyMask_ &= GUIEventAdapter::MODKEY_META;
	if(e->state() & Qt::AltButton) modKeyMask_ &= GUIEventAdapter::MODKEY_ALT;
}
