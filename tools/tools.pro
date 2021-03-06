# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./tools
# Target is a library:  

TEMPLATE = lib
CONFIG += release \
warn_on \
staticlib
QMAKE_CXXFLAGS_RELEASE += -O3
OBJECTS_DIR = ../objects
UI_DIR = ../ui
MOC_DIR = ../moc
INCLUDEPATH = ../editor \
../simulation \
../tools \
../command \
../gui \
../osgQT
LIBS += ../command/libcommand.a
TARGETDEPS += ../command/libcommand.a
SOURCES += drawingareaview.cpp \
           toolbox.cpp \
           toolmakeshape.cpp \
           toolorbit.cpp \
           toolpan.cpp \
           toolpick.cpp \
           toolrotate.cpp \
           toolviewfactory.cpp \
           toolmove.cpp \
           toolzoom.cpp \
           tooladdforce.cpp \
           tooladdjoint.cpp 
HEADERS += drawingareatool.h \
           drawingareaview.h \
           toolbox.h \
           tool.h \
           toolmakebox.h \
           toolmakeshape.h \
           toolmakesphere.h \
           toolmove.h \
           toolnew.h \
           toolorbit.h \
           toolpan.h \
           toolpick.h \
           toolrotate.h \
           toolviewfactory.h \
           toolview.h \
           toolzoom.h \
           toolmakeccylinder.h \
           toolredo.h \
           toolundo.h \
           tooladdnormalforce.h \
           tooladdforce.h \
           tooladdnormalforceatpos.h \
           tooladdnormalforceatrelpos.h \
           tooladdrelativeforce.h \
           tooladdrelativeforceatpos.h \
           tooladdrelativeforceatrelpos.h \
           toolmakepool.h \
           toolmakegear.h \
           toolmakeinclinedplane.h \
           tooladdnormaltorque.h \
           tooladdrelativetorque.h \
           tooladdballsocketjoint.h \
           tooladdsliderjoint.h \
           tooladdhingejoint.h \
           tooladdhinge2joint.h \
           tooladduniversaljoint.h \
           toolmakelightsource.h \
           toolmakeflatmirror.h \
           toolmakeconmirror.h \
           toolmakeconvexlens.h \
           tooladdjoint.h 
