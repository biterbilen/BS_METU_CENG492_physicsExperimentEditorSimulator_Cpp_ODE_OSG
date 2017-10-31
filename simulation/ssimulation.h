#ifndef _S_SIMULATION_H
#define _S_SIMULATION_H 1

#include <osg/Geode>
#include <osg/Geometry>
#include <osgUtil/SceneView>
#include <ode/ode.h>
#include <senvironment.h>
#include <ssimcallback.h>
#include <sjoint.h>
#include <sbobby.h>
#include <sbobbymath.h>
#include <sray.h>
#include <slight.h>
#include <svelocity.h>
#include <spool.h>

using namespace std;

class SSimulation
{

public:
  SSimulation(SEnvironment* env);
  ~SSimulation();

  float stepToSec() const;
  void setTimeStep(float f);
  
  static void odeCollisionCallbackStaticWrapper(void * data, dGeomID o1, dGeomID o2);
  void odeCollisionCallback(dGeomID o1, dGeomID o2);
  
  osg::Node* createBase(const osg::Vec3& center,float radius);

  void setSceneData(osg::Node * node);
  osg::Group* getSceneData();
  void worldStep();
  
  dWorldID getWorld() const { return mWorldID; }
  dSpaceID getSpace() const { return mSpaceID; }
  dJointGroupID getJointGroup() const {return mJointGroupID; }
  
  void setEnvironment(SEnvironment* env) { mEnvironment = env; }
  SEnvironment* getEnvironment(){return mEnvironment;}

private:
  dPlane * mGround;
  osg::ref_ptr<osg::Group> mSceneData;
  
  map<dGeomID, SObject*> mFluid;
  map<dGeomID, SObject*> mObjectMap;
  
  dWorldID mWorldID;
  dSpaceID mSpaceID;
  dJointGroupID mJointGroupID;
  SEnvironment* mEnvironment;
  float mMu;
  float mStepSize;    
  int mStep;
  float mTimeStep;
  unsigned int mIterations;
};

#endif

