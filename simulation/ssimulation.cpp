#include "ssimulation.h"

#include <osgUtil/Optimizer>

float SSimulation::stepToSec() const
{
    return ((float)mStep)/(1000.0/mTimeStep);
}

void SSimulation::setTimeStep(float f)
{
    mTimeStep = f;
}

SSimulation::SSimulation(SEnvironment* env)
{
    mStep = 0;
    mEnvironment = env;
    mMu = mEnvironment->getMu();
    mStepSize = mEnvironment->getStepSize();
    mIterations = mEnvironment->getIterations();

    mWorldID = dWorldCreate();
    mSpaceID = dHashSpaceCreate(0);
    mJointGroupID = dJointGroupCreate(1000000);

    dWorldSetGravity (mWorldID, mEnvironment->getGravity().x(),
                      mEnvironment->getGravity().y(),
                      mEnvironment->getGravity().z());
    if(mEnvironment->getCFM() != -1)
        dWorldSetCFM(mWorldID, mEnvironment->getCFM());
    if(mEnvironment->getERP() != -1)
        dWorldSetERP(mWorldID, mEnvironment->getERP());

    // OSG sceneData initialization
    mSceneData = new osg::Group();
    mSceneData->addChild(createBase(osg::Vec3(0, 0, 0), 50));

    SObject* current;
    for(list<SObject*>::iterator i = mEnvironment->getObjectBegin() ; i != mEnvironment->getObjectEnd() ; i++)
    {
        current = *i;
        
        current->init();       
        current->createSimObject(mWorldID, mSpaceID);  
        
        if(current->hasBobbies())            
            for(list<osg::ref_ptr<SBobby> >::iterator b = current->getBobbyBegin() ; b != current->getBobbyEnd() ; b++)
            {
                (*b)->setTime(mStepSize);
                (*b)->setGravity(mEnvironment->getGravity());                
            }                    
        
        //LIGHT does not have any geom       
        if (current->getObjectType() != SObject::LIGHT)
        { 
            current->setOdeCollideBits();
            current->setOdeCategoryBits();
        }
        // In the simulation constructor the ray objects are newly created ode objects they should be displayed graphically after the collision detection process 
        //FIXME if you want them to be present as osg objects when they are ready as ode objects
        if (current->getObjectType() != SObject::RAY) 
        {
            mSceneData->addChild(current->createVisualObject());
        }

        if(current->getAngVelocity() != osg::Vec3(0, 0, 0))
        {
            SAngVelocity * angVel = new SAngVelocity(current->getAngVelocity());
            angVel->createSimulationVelocity(current->getBodyID());
        }

        if(current->getLinVelocity() != osg::Vec3(0, 0, 0))
        {
            SLinVelocity * linVel = new SLinVelocity(current->getLinVelocity());
            linVel->createSimulationVelocity(current->getBodyID());
        }

        if((current->isDynamic() && current->hasBobbies()) || current->getObjectType() == SObject::POOL)
        {
//             SObject *mapper = mFluid[current->getGeomID()];
            mFluid[current->getGeomID()] = current;
        }

        if((current->getObjectType() == SObject::LIGHT)) //create a ray object in the environment
        {
            osg::Vec3 dir =  ((SLight*)current)->getDirection();
            SRay *ray = new SRay(current->getPosition(), dir);
            ray->setLength(((SLight*)current)->getLength());
            ((SLight *)current)->setSceneNode(mSceneData.get());
            ((SLight *)current)->setObjectMap(&mObjectMap);
            ((SLight *)current)->pushRay(ray);

             ray->init();
             ray->createSimObject(mWorldID, mSpaceID); 
             ray->setOdeCollideBits();
             ray->setOdeCategoryBits();
             
             mObjectMap.insert(pair<dGeomID, SObject*>(ray->getGeomID(), ray));
        }
        else
        {
            if(current->getObjectType() == SObject::CONVEXLENS)            
                mObjectMap.insert(pair<dGeomID, SObject*>(((SConvexLens *)current)->getGeomID2(),current));                
//             else if(current->getObjectType() == SObject::CONCAVELENS)
//                 mObjectMap.insert(pair<dGeomID, SObject*>((SConcaveLens *)current->getGeomID2(),current));                      
            
            // for light rays those intersect with the object, we should know the collided object type
//             SObject *mapper = mObjectMap[current->getGeomID()];
//             mObjectMap[current->getGeomID()] = current;
            mObjectMap.insert(pair<dGeomID, SObject*>(current->getGeomID(),current));

//             std::cout << (mObjectMap[current->getGeomID()]->getObjectType()==SObject::LIGHT? "-LIGHT-" :  "") ;
//             std::cout << (mObjectMap[current->getGeomID()]->getObjectType()==SObject::RAY? "-RAY-" :  ""); 
//             std::cout << (mObjectMap[current->getGeomID()]->getObjectType()==SObject::CAPPEDCYLINDER? "-CAPPED-" :  ""); 
//             std::cout << current <<" is ADDED "<<std::endl;
        }
    }

//     std::cout << "object map size: " << mObjectMap.size() << std::endl;
    for(list<SJoint*>::iterator i = mEnvironment->getJointBegin() ; i != mEnvironment->getJointEnd() ; i++)
    {
        (*i)->createSimulationJoint(mWorldID, mJointGroupID);
        mSceneData->addChild((*i)->createVisualJoint());
    }
    
    osgUtil::Optimizer optimizer;
    optimizer.optimize(mSceneData.get());
}

SSimulation::~SSimulation()
{
    dJointGroupDestroy(mJointGroupID);
    dSpaceDestroy(mSpaceID);
    dWorldDestroy(mWorldID);
}

void SSimulation::odeCollisionCallbackStaticWrapper(void * data, dGeomID o1, dGeomID o2)
{
    // ((cat1 & col2) || (cat2 & col1))
    if ((dGeomGetCategoryBits(o1) & dGeomGetCollideBits(o2)) || 
        (dGeomGetCategoryBits(o2) & dGeomGetCollideBits(o1))) 
    {
        // call the callback with o1 and o2    
        SSimulation* me = (SSimulation*) data;    
        me->odeCollisionCallback(o1,o2);    
    }
}

void SSimulation::odeCollisionCallback(dGeomID o1, dGeomID o2)
{    
//     if (dGeomGetClass(o2) == dPlaneClass)
//         std::cout << "    odeCollisionCallback: " << mObjectMap[o1]->getName() << " and plane" ;    
//     else if (dGeomGetClass(o1) == dPlaneClass)
//         std::cout << "    odeCollisionCallback: plane and " << mObjectMap[o2]->getName() ;         
//     else
//         std::cout << "    odeCollisionCallback: " << mObjectMap[o1]->getName() << " and " << mObjectMap[o2]->getName() ;
//      
//     if (dGeomGetClass(o1) == dRayClass)
//         ((SRay *)mObjectMap[o1])->print();
//     
//     if (dGeomGetClass(o2) == dRayClass)
//             ((SRay *)mObjectMap[o2])->print();

    int i;
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);

    if(mFluid[o1] != 0 && mFluid[o2] != 0)
    {
//         std::cout << "fluid collision part" << std::endl;
        if(mFluid[o1]->getObjectType() == SObject::POOL && mFluid[o2]->hasBobbies())
        {
            SPool * p = (SPool *)mFluid[o1];
            mFluid[o2]->updateBobbies(p->getWaterLevel(), p->getLiquidDensity());
            return;
        }
        else if(mFluid[o2]->getObjectType() == SObject::POOL && mFluid[o1]->hasBobbies())
        {
            SPool * p = (SPool *)mFluid[o2];
            mFluid[o1]->updateBobbies(p->getWaterLevel(), p->getLiquidDensity());
            return;
        }
    }
    
    // Don't do anything if the two objects are connected with a joint
    if (b1 && b2 && dAreConnectedExcluding (b1,b2, dJointTypeContact))
    {
//         std::cout << "bodies are connected" << std::endl;
        return;
    }

    if (dGeomGetClass(o1) == dRayClass && dGeomGetClass(o2) == dRayClass) 
    {
        return;
    }

    // one of them is a ray              
    else if ((dGeomGetClass(o1) == dRayClass) 
        || (dGeomGetClass(o2) == dRayClass)) 
    {        
        dContact contactRay; // 1 point for ray collision
//         contactRay.surface.mode = dContactSoftCFM | dContactApprox1;
//         contactRay.surface.mu = mMu;
//         contactRay.surface.mu2 = 0;
//         contactRay.surface.soft_cfm = 0.001;
        osg::Vec3 end,normal;
        end.set(0,0,0);
        SObject::ObjTypes type;
        dGeomID collGeom;
        SRay *ray = 0;
        
        if (dCollide (o1,o2,1,&contactRay.geom,sizeof(dContact)))
        {
            // PLANE is NOT IN THE MOBJECTMAP structure
            if (dGeomGetClass(o1) == dRayClass && dGeomGetClass(o2) == dPlaneClass)
            {
                ray = (SRay *) mObjectMap[o1];         
                collGeom = o2;                                        
                //pretend it a nonoptical object i.e. BOX
                type = SObject::BOX;
            }
            else if (dGeomGetClass(o2) == dRayClass && dGeomGetClass(o1) == dPlaneClass)
            {
                ray = (SRay *) mObjectMap[o2];         
                collGeom = o1;                        
                //pretend it a nonoptical object i.e. BOX
                type = SObject::BOX;
            }
            else if (mObjectMap[o1]->getObjectType()==SObject::RAY)
            {
                ray = (SRay *) mObjectMap[o1]; 
                collGeom = o2;                        
                type = mObjectMap[o2]->getObjectType();
            }
            else if (mObjectMap[o2]->getObjectType()==SObject::RAY)
            {
                ray = (SRay *) mObjectMap[o2]; 
                collGeom = o1;                        
                type = mObjectMap[o1]->getObjectType();                                           
            }                        
            end.set(contactRay.geom.pos[0],contactRay.geom.pos[1],contactRay.geom.pos[2]);
            normal.set(contactRay.geom.normal[0],contactRay.geom.normal[1],contactRay.geom.normal[2]); 
            // DO NOTHING... THE START POINT COLLIDES WITH THE GEOM IN THE NEXT STEP AGAIN. PREVENT THIS        
            osg::Vec3 diff = ray->getPosition() - end; 
            if (diff.length() > 0.001)            
            {
//                 std::cout << "    Intersection!!    point: "<< end[0] << " " << end[1] << " " << end[2];     
//                 std::cout << "    normal: "<< normal[0] << " " << normal[1] << " " << normal[2] ;
//                 std::cout << "    diff.length()=" << diff.length() << std::endl;
                
                ray->setEndPoint(end);                
                ray->setNormal(normal);                
                ray->setCollidedObjectGeom(collGeom);
                ray->setCollidedObjectType(type);                 
                ray->setLength((ray->getEndPoint()-ray->getPosition()).length());
                dGeomRaySetLength(ray->getGeomID(),ray->getLength());                   
            }        
        } 
        
        return;
        //TODO THINK ABOUT THIS
//         dJointID c = dJointCreateContact (mWorldID, mJointGroupID, &contactRay);
//         dJointAttach (c,b1,b2);
                  
    } // else if (mObjectMap[o1] && mObjectMap[o2]) // PLANE is NOT IN THE MOBJECTMAP struture */
    
    
    int maxContacts;
    if( dGeomGetClass(o1) == dTriMeshClass || dGeomGetClass(o2) == dTriMeshClass)
        maxContacts = 50;
    else maxContacts = 5;
    
    dContact contact[maxContacts];
    for (i=0; i<maxContacts; i++)
    {
        contact[i].surface.mode = dContactSoftCFM | dContactApprox1;
        contact[i].surface.mu = this->mMu;
        contact[i].surface.mu2 = 0;
        contact[i].surface.soft_cfm = 0.001;
    }
    if (int numc = dCollide (o1,o2, maxContacts, &contact[0].geom,sizeof(dContact)))
    {
        for (i=0; i<numc; i++)
        {
            dJointID c = dJointCreateContact (mWorldID, mJointGroupID, contact+i);
            dJointAttach (c,b1,b2);
        }
    }        
}

void SSimulation::setSceneData(osg::Node * node)
{
    mSceneData->addChild(node);
}

osg::Group* SSimulation::getSceneData()
{
    return mSceneData.get();
}

void SSimulation::worldStep()
{
//     std::cout << "WORLD STEP: "  << mStep << std::endl;
    dSpaceCollide(mSpaceID, this, &SSimulation::odeCollisionCallbackStaticWrapper);    
    
    switch(mEnvironment->getSolverType())
    {
        case SEnvironment::NORMAL:
        dWorldStep(mWorldID, mStepSize);
        break;
        case SEnvironment::QUICKSTEP:
        dWorldSetQuickStepNumIterations (mWorldID, mIterations);
        dWorldQuickStep(mWorldID, mStepSize);
        break;
        case SEnvironment::FASTSTEP:
        dWorldStepFast1(mWorldID, mStepSize, mIterations);
        break;
    }

    // ADJUST MRAYS vector in LIGHT OBJECTS
    for(list<SObject*>::iterator i = mEnvironment->getObjectBegin() ; i != mEnvironment->getObjectEnd() ; i++)
    {
        //SKIP RAY type OBJECTS -they will e handled in the LIGHT type objects
        if((*i)->getObjectType() == SObject::RAY)
        {
            continue;
        }
            
        if((*i)->getObjectType() == SObject::LIGHT)
        {            
            ((SLight *)(*i))-> resizeRayVector();
            ((SLight *)(*i))-> createNewRays(mWorldID,mSpaceID); 
        }
    }
        
    float currentTime = stepToSec();

    for(list<SForce*>::iterator i = mEnvironment->getForceBegin() ; i != mEnvironment->getForceEnd() ; i++)
    {
        if((*i)->getStart() < currentTime
                &&
                ((((*i)->getStart() + (*i)->getDuration()) > currentTime)
                 ||
                 (*i)->getDuration() == -1
                 ||
                 ((*i)->getDuration() == 0 && (*i)->getStart() + mTimeStep / 1000.0 + 0.0001 >= currentTime)))
        {
            (*i)->createSimulationForce();
        }
    }

    dJointGroupEmpty(mJointGroupID);
        
    list<SChart*>::iterator chartIter; 
    for(chartIter = mEnvironment->getChartBegin(); chartIter != mEnvironment->getChartEnd();chartIter++)
    {
        int j = currentTime / ((*chartIter)->getSeconds());
    
        if(currentTime - ((float)j)*((*chartIter)->getSeconds()) == 0)
        {
            std::cout<< currentTime << endl;
             (*chartIter)->addData();
        }
    }
            
    mStep++;
}

osg::Node* SSimulation::createBase(const osg::Vec3& center,float radius)
{
    osg::Group * gridNode = new osg::Group();
    
    int numTilesX = 5;
    int numTilesY = 5;

    float width = 2*radius;
    float height = 2*radius;

    osg::Vec3 v000(center - osg::Vec3(width*0.5f,height*0.5f,0.0f));
    osg::Vec3 dx(osg::Vec3(width/((float)numTilesX),0.0,0.0f));
    osg::Vec3 dy(osg::Vec3(0.0f,height/((float)numTilesY),0.0f));

    // fill in vertices for grid, note numTilesX+1 * numTilesY+1...
    osg::Vec3Array* coords = new osg::Vec3Array;
    int iy;
    for(iy=0;iy<=numTilesY;++iy)
    {
        for(int ix=0;ix<=numTilesX;++ix)
        {
            coords->push_back(v000+dx*(float)ix+dy*(float)iy);
        }
    }

    //Just two colours - black and white.
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f,1.0f)); // white
    colors->push_back(osg::Vec4(0.0f,0.0f,0.0f,1.0f)); // black
    int numColors=colors->size();

    int numIndicesPerRow=numTilesX+1;
    osg::UByteArray* coordIndices = new osg::UByteArray; // assumes we are using less than 256 points...
    osg::UByteArray* colorIndices = new osg::UByteArray;
    for(iy=0;iy<numTilesY;++iy)
    {
        for(int ix=0;ix<numTilesX;++ix)
        {
            // four vertices per quad.
            coordIndices->push_back(ix +(iy+1)*numIndicesPerRow);
            coordIndices->push_back(ix +iy*numIndicesPerRow);
            coordIndices->push_back((ix+1)+iy*numIndicesPerRow);
            coordIndices->push_back((ix+1)+(iy+1)*numIndicesPerRow);

            // one color per quad
            colorIndices->push_back((ix+iy)%numColors);
        }
    }

    // set up a single normal
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));

    osg::Geometry* geom = new osg::Geometry;
    geom->setVertexArray(coords);
    geom->setVertexIndices(coordIndices);

    geom->setColorArray(colors);
    geom->setColorIndices(colorIndices);
    geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,coordIndices->size()));
   
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(geom);

    gridNode->addChild(geode);

    // ground plane
    mGround = new dPlane(mSpaceID, 0, 0, 1, 0);
    
    return gridNode;
}
