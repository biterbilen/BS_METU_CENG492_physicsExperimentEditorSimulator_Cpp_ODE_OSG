#include "smainwindow.h"

#include <qapplication.h>

#include "tool.h"
#include "toolbox.h"
#include "toolmakepool.h"
#include "toolmakebox.h"
#include "toolmakesphere.h"
#include "toolmakeccylinder.h"
#include "toolmakegear.h"
#include "toolmakeinclinedplane.h"

#include "toolmakelightsource.h"
#include "toolmakeflatmirror.h"
#include "toolmakeconmirror.h"
#include "toolmakeconvexlens.h"

#include "toolpick.h"
#include "toolpan.h"
#include "toolorbit.h"
#include "toolrotate.h"
#include "toolmove.h"
#include "toolzoom.h"
#include "toolnew.h"
#include "toolundo.h"
#include "toolredo.h"
#include "tooladdnormaltorque.h"
#include "tooladdnormalforce.h"
#include "tooladdnormalforceatpos.h"
#include "tooladdnormalforceatrelpos.h"
#include "tooladdrelativetorque.h"
#include "tooladdrelativeforce.h"
#include "tooladdrelativeforceatpos.h"
#include "tooladdrelativeforceatrelpos.h"
#include "tooladdballsocketjoint.h"
#include "tooladdsliderjoint.h"
#include "tooladdhingejoint.h"
#include "tooladdhinge2joint.h"
#include "tooladduniversaljoint.h"

#include "toolviewfactory.h"
#include "drawingareaview.h"

static void registerTools()
{
    osg::ref_ptr<Tool> toolmakelightsource=new ToolMakeLightSource();
    osg::ref_ptr<Tool> toolmakeflatmirror=new ToolMakeFlatMirror();
    osg::ref_ptr<Tool> toolmakeconmirror=new ToolMakeConMirror();
    osg::ref_ptr<Tool> toolmakeconvexlens=new ToolMakeConvexLens();
    osg::ref_ptr<Tool> toolmakeinclinedplane=new ToolMakeInclinedPlane();
    osg::ref_ptr<Tool> toolmakegear=new ToolMakeGear();
    osg::ref_ptr<Tool> toolmakepool=new ToolMakePool();
    osg::ref_ptr<Tool> toolmakebox=new ToolMakeBox();
    osg::ref_ptr<Tool> toolmakesphere=new ToolMakeSphere();
    osg::ref_ptr<Tool> toolmakeccylinder=new ToolMakeCCylinder();
    osg::ref_ptr<Tool> toolrotate=new ToolRotate();
    osg::ref_ptr<Tool> toolmove=new ToolMove();
    osg::ref_ptr<Tool> toolzoom=new ToolZoom();
    osg::ref_ptr<Tool> toolpan=new ToolPan();
    osg::ref_ptr<Tool> toolpick=new ToolPick();
    osg::ref_ptr<Tool> toolorbit=new ToolOrbit();
    osg::ref_ptr<Tool> toolundo=new ToolUndo();
    osg::ref_ptr<Tool> toolredo=new ToolRedo();
    osg::ref_ptr<Tool> tooladdnormaltoque =new ToolAddNormalTorque();
    osg::ref_ptr<Tool> tooladdnormalforce =new ToolAddNormalForce();
    osg::ref_ptr<Tool> tooladdnormalforceatpos =new ToolAddNormalForceAtPos();
    osg::ref_ptr<Tool> tooladdnormalforceatrelpos =new ToolAddNormalForceAtRelPos();
    osg::ref_ptr<Tool> tooladdrelativetorque =new ToolAddRelativeTorque();
    osg::ref_ptr<Tool> tooladdrelativeforce =new ToolAddRelativeForce();
    osg::ref_ptr<Tool> tooladdrelativeforceatpos =new ToolAddRelativeForceAtPos();
    osg::ref_ptr<Tool> tooladdrelativeforceatrelpos =new ToolAddRelativeForceAtRelPos();
    osg::ref_ptr<Tool> tooladdballsocketjoint =new ToolAddBallSocketJoint();
    osg::ref_ptr<Tool> tooladdsliderjoint =new ToolAddSliderJoint();
    osg::ref_ptr<Tool> tooladdhingejoint =new ToolAddHingeJoint();
    osg::ref_ptr<Tool> tooladdhinge2joint =new ToolAddHinge2Joint();
    osg::ref_ptr<Tool> tooladduniversaljoint =new ToolAddUniversalJoint();
    
    ToolBox::instance()->registerTool(toolmakeinclinedplane.get());
    ToolBox::instance()->registerTool(toolmakegear.get());
    ToolBox::instance()->registerTool(toolmakepool.get());
    ToolBox::instance()->registerTool(toolmakebox.get());
    ToolBox::instance()->registerTool(toolmakesphere.get());
    ToolBox::instance()->registerTool(toolmakeccylinder.get());
    ToolBox::instance()->registerTool(toolmakelightsource.get());
    ToolBox::instance()->registerTool(toolmakeflatmirror.get());
    ToolBox::instance()->registerTool(toolmakeconmirror.get());
    ToolBox::instance()->registerTool(toolmakeconvexlens.get());
    ToolBox::instance()->registerTool(toolrotate.get());
    ToolBox::instance()->registerTool(toolmove.get());
    ToolBox::instance()->registerTool(toolzoom.get());
    ToolBox::instance()->registerTool(toolpan.get());
    ToolBox::instance()->registerTool(toolpick.get());
    ToolBox::instance()->registerTool(toolorbit.get());
    ToolBox::instance()->registerTool(toolundo.get());
    ToolBox::instance()->registerTool(toolredo.get());
    ToolBox::instance()->registerTool(tooladdnormaltoque.get());
    ToolBox::instance()->registerTool(tooladdnormalforce.get());
    ToolBox::instance()->registerTool(tooladdnormalforceatpos.get());
    ToolBox::instance()->registerTool(tooladdnormalforceatrelpos.get());
    ToolBox::instance()->registerTool(tooladdrelativetorque.get());
    ToolBox::instance()->registerTool(tooladdrelativeforce.get());
    ToolBox::instance()->registerTool(tooladdrelativeforceatpos.get());
    ToolBox::instance()->registerTool(tooladdrelativeforceatrelpos.get());
    ToolBox::instance()->registerTool(tooladdballsocketjoint.get());
    ToolBox::instance()->registerTool(tooladdsliderjoint.get());
    ToolBox::instance()->registerTool(tooladdhingejoint.get());
    ToolBox::instance()->registerTool(tooladdhinge2joint.get());
    ToolBox::instance()->registerTool(tooladduniversaljoint.get());
    ToolBox::instance()->registerTool(new ToolNew());
    
    osg::ref_ptr<ToolView> drawingarea=new DrawingAreaView();
    ToolViewFactory::instance()->registerView(toolmakeflatmirror.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakelightsource.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakeconmirror.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakeconvexlens.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakeinclinedplane.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakegear.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakepool.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakebox.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakesphere.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmakeccylinder.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolrotate.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolmove.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolzoom.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolpan.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolpick.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(toolorbit.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdnormaltoque.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdnormalforce.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdnormalforceatpos.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdnormalforceatrelpos.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdrelativetorque.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdrelativeforce.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdrelativeforceatpos.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdrelativeforceatrelpos.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdballsocketjoint.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdsliderjoint.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdhingejoint.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladdhinge2joint.get(),drawingarea.get());
    ToolViewFactory::instance()->registerView(tooladduniversaljoint.get(),drawingarea.get());
}

int main( int argc, char **argv )
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication a( argc, argv );

    if ( !QGLFormat::hasOpenGL() )
    {
        qWarning( "This system has no OpenGL support. Exiting." );
        return -1;
    }

    registerTools();
    osg::ref_ptr<SMainWindow> mainWidget = new SMainWindow();
    mainWidget->resize(800, 600);
    a.setMainWidget( mainWidget.get() );
    mainWidget->show();

    return a.exec();
}



