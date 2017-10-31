#include "sfileinputoutput.h"
#include <list>
#include <iterator>

using namespace std;

SFileInputOutput::SFileInputOutput(){

    mBoxCount                      = "1";
    mSphereCount                 = "1";
    mCylinderCount               = "1";
    mCappedCylinderCount   = "1";
    mGearCount                    = "1";
    mPoolCount                     = "1";
    mInclinedPlaneCount       = "1";
    mRopeCount                   = "1";
    mRayCount                     = "1";
    mLightCount                    = "1";
    mConMirrorCount            = "1";
    mConcaveLensCount      = "1";
    mConvexLensCount        = "1";
    mTrimeshCount               = "1";
    mFlatMirrorCount             = "1";
    
    
    mBallSocketCount              = "1";
    mHingeCount                     = "1";
    mHinge2Count                   = "1";
    mSliderCount                     = "1";
    mUniversalCount               = "1";
    
    mNormalForceCount =         "1";
    mRelForceCount =               "1";
    mRelForceAtPosCount =      "1";
    mRelForceAtRelPosCount = "1";
    mForceAtRelPosCount =      "1";
    mForceAtPosCount =           "1";
    mNormalTorque =                "1";
    mRelTorqueCount =             "1";

}

SFileInputOutput::~SFileInputOutput()
{
    closeFile();
}

bool SFileInputOutput::openFile(QString FileName,int type)
{
    if(type == 1)
    {
        mFile.open(FileName,ios::in);


        if(!mFile.is_open())
        {   
            return false;
        }
        else
        {   
            return true;
        }
    }
    else
    {
        mFile.open(FileName,ios::out);

        if(!mFile.is_open())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

void SFileInputOutput::closeFile()
{
    if(mFile.is_open())
        mFile.close();
}

SEnvironment * SFileInputOutput::readFile(QString FileName,SEnvironment* env)
{

    if(openFile(FileName,1))
    {
        /**
         *The Order is
         *1 - gravity x gravity y gravity z
         *2 - Mu
         *3 - stepsize
         *4 - CFM
         *5 - ERP
         */
        
        string dummySTR, solType;
        int iters;                                        //for unuseful data
        float dim1 , dim2 , dim3 , dim4;    //for float values
            
        //COUNTS
        
        //OBJECT COUNTS
        
        mFile >> dummySTR;
        
        mFile >> mBoxCount;
        mFile >> mSphereCount;
        mFile >> mCylinderCount;
        mFile >> mCappedCylinderCount;
        mFile >> mGearCount;
        mFile >> mPoolCount;
        mFile >> mInclinedPlaneCount;
        mFile >> mRopeCount;
        mFile >> mLightCount;
        mFile >> mFlatMirrorCount;
        mFile >> mConMirrorCount;
        mFile >> mConcaveLensCount;
        mFile >> mConvexLensCount;
        mFile >> mTrimeshCount;
        
        //JOINT COUNTS
        mFile >> dummySTR;
        
        mFile >> mBallSocketCount;
        mFile >> mHingeCount;
        mFile >> mHinge2Count;
        mFile >> mUniversalCount;
        mFile >> mSliderCount;
        
        //FORCE COUNTS
        mFile >> dummySTR;
        
        mFile >> mNormalForceCount;
        mFile >> mRelForceCount;
        mFile >> mRelForceAtPosCount;
        mFile >> mRelForceAtRelPosCount;
        mFile >> mForceAtRelPosCount;
        mFile >> mForceAtPosCount;
        mFile >> mNormalTorque;
        mFile >> mRelTorqueCount;
        
        mFile >> mGravityX >> mGravityY >> mGravityZ ;
        mFile >> mMu >> mStepSize >> mCFM >> mERP  >> solType >> iters ;

        env->setCFM(mCFM);
        env->setERP(mERP);
        env->setGravity(osg::Vec3(mGravityX,mGravityY,mGravityZ));
        env->setMu(mMu);
        env->setStepSize(mStepSize);
        
        if(strcmp(solType.c_str(),"NORMAL") == 0)
            env->setSolverType(SEnvironment::NORMAL);
        else if(strcmp(solType.c_str(),"FAST") == 0)
            env->setSolverType(SEnvironment::FASTSTEP);
        else if(strcmp(solType.c_str(),"QUICK") == 0)
            env->setSolverType(SEnvironment::QUICKSTEP);

        env->setIterations(iters);
        
        mFile >> dummySTR;                    //The object type string will be read and controlled

        while(!mFile.eof())
        {
                       
            /*The box object and the forces related to it*/
            if(strcmp(dummySTR.c_str(),"BOX") == 0)
            {
                
                SBox * box;
                /**
                 *BOX                            
                 *1 - TYPE - STATIC,DYNAMIC    
                 *2 - ORIENTATION - X Y Z    
                 *3 - DIMENSIONS - X Y Z    
                 *4 - POSITION - X Y Z             
                 *5 - MASS                    
                 *6 - NAME                    
                */

                mFile >> dummySTR;
                //TYPE
                if(strcmp(dummySTR.c_str(),"STATIC") == 0)
                {
                    box = new SBox(SObject::STATIC);
                }
                else
                    box = new SBox(SObject::DYNAMIC);

                //Object Type
                box->setObjectType(SObject::BOX);
                //ORIENTATION
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                box-> setOrientation(osg::Quat(dim1, dim2, dim3, dim4));
                //LENGTHS
                mFile >> dim1 >> dim2 >> dim3;
                box ->setDimensions(osg::Vec3(dim1,dim2,dim3));
                //POSITION
                mFile >> dim1 >> dim2 >> dim3;
                box->setPosition(dim1,dim2,dim3);
                //MASS
                mFile >> dim1;
                box->setMass(dim1);
                //NAME
                mFile >> dummySTR;
                box ->setName(dummySTR);
                //With Bobby or NOT
                mFile >> dummySTR;
                if(strcmp(dummySTR.c_str(),"TRUE") == 0)
                    box->setBobby(true);
                else
                    box->setBobby(false);
                               
                //Linear Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;
                box->setLinVelocity(osg::Vec3(dim1, dim2, dim3));
                
                //Angular Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;                                          
                box->setAngVelocity(osg::Vec3(dim1, dim2, dim3));
                
               

                //FORCE
                mFile >> dummySTR;              //FORCE word is read here
                                                                //Until the other force word is read 
                                                                //the forces are taken according to thier types    
                
                mFile >> dummySTR;
                
                while(strcmp(dummySTR.c_str(),"/FORCE") != 0)
                {
                    setForces(box,dummySTR,env);
      
                    mFile >> dummySTR;
                }
                
                
                box->setCounter(mBoxCount);                                
                env->addObject(box);
                            
            }
             /*The box object and the forces related to it*/
            else if(strcmp(dummySTR.c_str(),"SPHERE") == 0)
            {
            
                SSphere * sphere;

                mFile >> dummySTR;
                //TYPE
                if(strcmp(dummySTR.c_str(),"STATIC") == 0)
                {
                    sphere = new SSphere(SObject::STATIC);
                }
                else
                    sphere = new SSphere(SObject::DYNAMIC);

                //Object Type
                sphere->setObjectType(SObject::SPHERE);
               //ORIENTATION
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                sphere->setOrientation(osg::Quat(dim1, dim2, dim3, dim4));
               
                //RADIUS
                mFile >> dim1;
                
                
                sphere->setRadius(dim1);
                //POSITION
                mFile >> dim1 >> dim2 >> dim3;
                sphere->setPosition(dim1,dim2,dim3);
                //MASS
                mFile >> dim1;
                sphere->setMass(dim1);
                //NAME
                mFile >> dummySTR;
                sphere ->setName(dummySTR);
                
                //With Bobby or NOT
                mFile >> dummySTR;
                if(strcmp(dummySTR.c_str(),"TRUE") == 0)
                    sphere->setBobby(true);
                else
                    sphere->setBobby(false);
                               
                //Linear Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;
                sphere->setLinVelocity(osg::Vec3(dim1, dim2, dim3));
                
                //Angular Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;                                          
                sphere->setAngVelocity(osg::Vec3(dim1, dim2, dim3));

                //FORCE
                mFile >> dummySTR;                //FORCE word is read here
                                                                  //Until the other force word is read 
                                                                  //the forces are taken according to thier types    
                
                mFile >> dummySTR;
                
                while(strcmp(dummySTR.c_str(),"/FORCE") != 0)
                { 
                    setForces(sphere,dummySTR,env);

                    mFile >> dummySTR;
                }
                
                sphere->setCounter(mSphereCount);
                env->addObject(sphere);                
                
            
            }
            
            else if(strcmp(dummySTR.c_str(),"CYLINDER") == 0)
            {
                SCylinder * cylinder;
            
                mFile >> dummySTR;
                //TYPE
                if(strcmp(dummySTR.c_str(),"STATIC") == 0)
                {
                    cylinder = new SCylinder(SObject::STATIC);
                }
                else
                    cylinder = new SCylinder(SObject::DYNAMIC);

                //Object Type
                cylinder->setObjectType(SObject::CYLINDER);
                //ORIENTATION
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                cylinder->setOrientation(osg::Quat(dim1, dim2, dim3, dim4));
                //RADIUS
                mFile >> dim1;
                cylinder->setRadius(dim1);
                //HEIGTH
                mFile >> dim1;
                cylinder->setHeight(dim1);
                //POSITION
                mFile >> dim1 >> dim2 >> dim3;
                cylinder->setPosition(dim1,dim2,dim3);
                //MASS
                mFile >> dim1;
                cylinder->setMass(dim1);
                //NAME
                mFile >> dummySTR;
                cylinder->setName(dummySTR);

                 //With Bobby or NOT
                mFile >> dummySTR;
                if(strcmp(dummySTR.c_str(),"TRUE") == 0)
                    cylinder->setBobby(true);
                else
                    cylinder->setBobby(false);
                               
                //Linear Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;
                cylinder->setLinVelocity(osg::Vec3(dim1, dim2, dim3));
                
                //Angular Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;                                          
                cylinder->setAngVelocity(osg::Vec3(dim1, dim2, dim3));
                
                            
                //FORCE
                mFile >> dummySTR;                //FORCE word is read here
                                                                  //Until the other force word is read 
                                                                 //the forces are taken according to thier types    
                
                mFile >> dummySTR;
                
                while(strcmp(dummySTR.c_str(),"/FORCE") != 0)
                {

                    setForces(cylinder,dummySTR,env);
                   
                    mFile >> dummySTR;
                }
                
                cylinder->setCounter(mCylinderCount);
                env->addObject(cylinder);                
                
            
            }
            //CAPPED
            else if(strcmp(dummySTR.c_str(),"CAPPEDCYLINDER") == 0)
            {
                SCappedCylinder * ccylinder;
            
                //sphere den farki sadece heigh t eklenecek olmasi 
                mFile >> dummySTR;
                //TYPE
                if(strcmp(dummySTR.c_str(),"STATIC") == 0)
                {
                    ccylinder = new SCappedCylinder(SObject::STATIC);
                }
                else
                    ccylinder = new SCappedCylinder(SObject::DYNAMIC);

                //Object Type
                ccylinder->setObjectType(SObject::CAPPEDCYLINDER);
                //ORIENTATION
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                ccylinder->setOrientation(osg::Quat(dim1, dim2, dim3, dim4));
                //RADIUS
                mFile >> dim1;
                ccylinder->setRadius(dim1);
                //HEIGTH
                mFile >> dim1;
                ccylinder->setHeight(dim1);
                //POSITION
                mFile >> dim1 >> dim2 >> dim3;
                ccylinder->setPosition(dim1,dim2,dim3);
                //MASS
                mFile >> dim1;
                ccylinder->setMass(dim1);
                //NAME
                mFile >> dummySTR;
                ccylinder->setName(dummySTR);
                
                //With Bobby or NOT
                mFile >> dummySTR;
                if(strcmp(dummySTR.c_str(),"TRUE") == 0)
                    ccylinder->setBobby(true);
                else
                    ccylinder->setBobby(false);
                               
                //Linear Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;
                ccylinder->setLinVelocity(osg::Vec3(dim1, dim2, dim3));
                
                //Angular Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;                                          
                ccylinder->setAngVelocity(osg::Vec3(dim1, dim2, dim3));           

                //FORCE
                mFile >> dummySTR;                //FORCE word is read here
                                                                 //Until the other force word is read 
                                                                 //the forces are taken according to thier types    
                
                mFile >> dummySTR;
                
                while(strcmp(dummySTR.c_str(),"/FORCE") != 0)
                {

                    setForces(ccylinder,dummySTR,env);
 
                    mFile >> dummySTR;
                }
                
                ccylinder->setCounter(mCappedCylinderCount);
                env->addObject(ccylinder);                
                        
            }
            
            else if(strcmp(dummySTR.c_str(),"GEAR") == 0)
            {
                /*
                 * GearType
                 * Position
                 * Orientation   
                 * Radius
                 * ToothNumber 
                 * Name 
                 * Force 
                */          
                      
                SGear *gear;              
                
                //Always DYNAMIC
                //GEAR TYPE
                mFile >> dummySTR;
                //TYPE
                if(strcmp(dummySTR.c_str(),"BOXGEAR") == 0)
                {
                    gear= new SGear(SGear::BOXGEAR);
                }
                else {
                    gear= new SGear(SGear::PRISMGEAR);
                }
                                
                //POSITION
                mFile >> dim1 >> dim2 >> dim3;
                gear->setPosition(dim1,dim2,dim3);
                
                //ORIENTATION
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                // TODO sil
                
                gear->setOrientation(osg::Quat(dim1, dim2, dim3, dim4));
                
                //RADIUS
                mFile >> dim1;
                gear->setRadius(dim1);
                
                //TOOTH NUMBER
                mFile >> dim1;
                gear->setToothNumber((int)dim1);
                
                //MASS
                // FIXME ODE explores when no mass is set. change here when other create simobject function is regulated.
                gear->setMass(1);
                
                //NAME
                mFile >> dummySTR;
                gear->setName(dummySTR);

                
                //With Bobby or NOT
                mFile >> dummySTR;
                if(strcmp(dummySTR.c_str(),"TRUE") == 0)
                    gear->setBobby(true);
                else
                    gear->setBobby(false);
                               
                //Linear Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;
                gear->setLinVelocity(osg::Vec3(dim1, dim2, dim3));
                
                //Angular Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;                                          
                gear->setAngVelocity(osg::Vec3(dim1, dim2, dim3));
                
                //FORCE
                mFile >> dummySTR;                 //FORCE word is read here
                                                                   //Until the other force word is read 
                                                                  //the forces are taken according to thier types    
                
                mFile >> dummySTR;
                
                while(strcmp(dummySTR.c_str(),"/FORCE") != 0)
                {

                     setForces(gear,dummySTR,env);
 
                    mFile >> dummySTR;                                              
                }
                
                gear->setCounter(mGearCount);
                env->addObject(gear);
            }
            //POOL
            else if(strcmp(dummySTR.c_str(),"POOL") == 0)
            {
                SPool* pool = new SPool();
                                               
                mFile >> dim1 >> dim2 >> dim3;
                pool->setPosition(osg::Vec3(dim1,dim2,dim3));
                
                mFile >> dim1 >> dim2 >> dim3;
                pool->setDimensions(osg::Vec3(dim1,dim2,dim3));
                
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                pool->setOrientation(osg::Quat(dim1,dim2,dim3,dim4));
                
                //name
                mFile >> dummySTR;
                pool->setName(dummySTR);
                mFile >> dim1;
                pool->setDepth(dim1);
                mFile >> dim1;
                pool->setLiquidDensity(dim1);
                mFile >> dummySTR;
                mFile >> dummySTR;
                
                pool->setCounter(mPoolCount);
                env->addObject(pool);
            }
            else if(strcmp(dummySTR.c_str(),"TRIMESH") == 0){
                  STrimesh * tri;
                 mFile >> dummySTR;
                 if(strcmp(dummySTR.c_str(),"STATIC")==0)
                   tri = new STrimesh(SObject::STATIC);
                 else
                   tri = new STrimesh(SObject::DYNAMIC);
                 //Position
                 mFile >> dim1 >> dim2 >> dim3;
                 tri->setPosition(osg::Vec3(dim1,dim2,dim3));
                 
                 //Orientation
                 mFile >> dim1 >> dim2 >> dim3 >> dim4;   
                tri->setOrientation(osg::Quat(dim1,dim2,dim3,dim4));
                
                //Name
                mFile >> dummySTR;
                tri->setName(dummySTR);
                
                //Mass
                mFile >> dim1;
                tri->setMass(dim1);
                
                //VERTICES
                mFile >> dummySTR;
                int verCount = 0;
                mFile >> verCount;
                
                dVector3 * v;
                v = new dVector3[verCount];

                for(int i = 0; i < verCount ; i++)
                {
                    mFile >> v[i][0] >> v[i][1] >> v[i][2];
                }
                
                tri->setVertices(v, verCount);
                                
                //INDECES
                mFile >> dummySTR;
                int InCount = 0;
                mFile >> InCount;
                
                int * in;
                in = new int[InCount];
                
                for(int i = 0; i < InCount ;i++)
                {
                    mFile >> in[i];
                }
                
                tri->setIndices(in, InCount);

                //Linear Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;
                tri->setLinVelocity(osg::Vec3(dim1, dim2, dim3));
                
                //Angular Velocity
                mFile >> dummySTR;
                mFile >> dim1 >> dim2 >> dim3;                                          
                tri->setAngVelocity(osg::Vec3(dim1, dim2, dim3));
                  
                
                 //FORCE
                mFile >> dummySTR;                 //FORCE word is read here
                                                                   //Until the other force word is read 
                                                                  //the forces are taken according to thier types    
                
                mFile >> dummySTR;
                
                while(strcmp(dummySTR.c_str(),"/FORCE") != 0)
                {

                     setForces(tri,dummySTR,env);
 
                    mFile >> dummySTR;                                              
                }             
                 tri->setCounter(mTrimeshCount);    
                 env->addObject(tri);
                    
            }
            
            else if(strcmp(dummySTR.c_str(),"INCLINEDPLANE") == 0){
            
                SInclinedPlane *inc = new SInclinedPlane();
                mFile >> dim1 >> dim2 >> dim3;
                
                inc->setPosition(osg::Vec3(dim1,dim2,dim3));
                
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                inc->setOrientation(osg::Quat(dim1,dim2,dim3,dim4));
                
                mFile >> dim1 >> dim2 >> dim3;
                inc->setHeight(dim1);
                inc->setAngle(dim2);
                inc->setWidth(dim3);
                
                mFile >> dummySTR;
                inc->setName(dummySTR);
                
                
                mFile >> dummySTR;
                mFile >> dummySTR;                
                
                inc->setCounter(mInclinedPlaneCount);
                env->addObject(inc);
                    
            }
            
            else if(strcmp(dummySTR.c_str(),"ROPE") == 0){
                 SRope *rope = new SRope();
                    
                //position start
                 mFile >> dim1 >> dim2 >> dim3;
                rope->setStart(osg::Vec3(dim1,dim2,dim3));
                
                //positin end
                mFile >> dim1 >> dim2 >> dim3;
                rope ->setEnd(osg::Vec3(dim1,dim2,dim3));
                
                //radius
                mFile >> dim1;
                rope->setRadius(dim1);
                
                //mass
                mFile >> dim1;
                rope->setMass(dim1);
                
                //name
                mFile >> dummySTR;
                rope->setName(dummySTR);
                    

                                                 
                //FORCE
                mFile >> dummySTR;                 //FORCE word is read here
                                                                   //Until the other force word is read 
                                                                  //the forces are taken according to thier types    
                
                mFile >> dummySTR;
                
                                
                rope->setCounter(mRopeCount);
                env->addObject(rope);
            }
            
          
            else if(strcmp(dummySTR.c_str(),"LIGHT") == 0){
                
                SLight * light = new SLight();
                
                mFile >> dim1 >> dim2 >> dim3;
                light->setPosition(dim1,dim2,dim3);
                
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                light->setOrientation(osg::Quat(dim1,dim2,dim3,dim4));
                
                mFile >> dummySTR;
                light->setName(dummySTR);
                                
                light->setCounter(mLightCount);
                env->addObject(light);
            }
            
            else if(strcmp(dummySTR.c_str(),"FLATMIRROR") == 0){
            
                SFlatMirror * flatmirror = new SFlatMirror();
                
                mFile >> dim1 >> dim2 >> dim3;
                flatmirror->setPosition(dim1,dim2,dim3);
                
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                flatmirror->setOrientation(osg::Quat(dim1,dim2,dim3,dim4));
                
                mFile >> dim1>>dim2;
                flatmirror->setWidth(dim1);
                flatmirror->setLength(dim2);
                
                mFile>> dummySTR;
                flatmirror->setName(dummySTR);
                
                mFile>> dummySTR >> dummySTR;
                
                flatmirror->setCounter(mFlatMirrorCount);
                env->addObject(flatmirror);
            }
            
            else if(strcmp(dummySTR.c_str(),"CONMIRROR") == 0){
                 SConMirror * conmirror = new SConMirror();
                
                mFile >> dim1 >> dim2 >> dim3;
                conmirror->setPosition(dim1,dim2,dim3);
                
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                conmirror->setOrientation(osg::Quat(dim1,dim2,dim3,dim4));
                
                mFile >> dim1>>dim2 >> dim3;
                conmirror->setFocalLength(dim1);
                conmirror->setAzRange(dim2);
                conmirror->setElevRange(dim3);
                
                mFile >> dummySTR;
                conmirror->setName(dummySTR);
                
                //forces
                mFile>> dummySTR >> dummySTR;
                
                
                conmirror->setCounter(mConMirrorCount);
                env->addObject(conmirror);
                
            }
            
            else if(strcmp(dummySTR.c_str(),"CONCAVELENS") == 0){
                
                //concavelens->setCounter(mConcaveLensCount);
            }
            
            else if(strcmp(dummySTR.c_str(),"CONVEXLENS") == 0){
                
                SConvexLens * convexlens = new SConvexLens();
                
                mFile >> dim1 >> dim2 >> dim3;
                convexlens->setPosition(dim1,dim2,dim3);
                
                mFile >> dim1 >> dim2 >> dim3 >> dim4;
                convexlens->setOrientation(osg::Quat(dim1,dim2,dim3,dim4));
                
                //focal len, refrection index , thickness
                mFile >> dim1 >> dim2 >> dim3;
                 convexlens->setFocalLength(dim1);
                convexlens->setRefractionIndex(dim2);
                convexlens->setThickness(dim3);
                
                //name
                mFile >> dummySTR;
                convexlens->setName(dummySTR);
                //force read
                mFile>> dummySTR >> dummySTR;
                
              
                convexlens->setCounter(mConvexLensCount);
                env->addObject(convexlens);
            }
            
                                                                                   
            else if(strcmp(dummySTR.c_str(),"JOINT") == 0)
            {
                mFile >> dummySTR;
                
                while(strcmp(dummySTR.c_str(),"/JOINT") != 0)
                {
                    if(strcmp(dummySTR.c_str(),"BALLSOCKET") == 0){
                        
                        SBallSocket *ballsocket = new SBallSocket();
                       
                        //Name
                        mFile >> dummySTR;
                        ballsocket->setName(dummySTR);
                        //The body index numbers
                        mFile >> dim1 >> dim2;
                        ballsocket->setBodyIndex((int)dim1);
                        ballsocket->setBodyIndex((int)dim2);                        
                        
                        //Burada objelere link yapilacak
                        int i = 0;
                        for(objIter =env->getObjectBegin();i< ballsocket->getBody1Index();objIter++,i++);//void
                        ballsocket->setBody1(*objIter);
                        
                        i = 0;
                        for(objIter = env->getObjectBegin();i < ballsocket->getBody2Index();objIter++,i++)
                            ;//void                        
                        ballsocket->setBody2(*objIter);
                       
                        //Anchor            
                        mFile >> dim1 >> dim2 >> dim3;
                        ballsocket->setAnchor(osg::Vec3(dim1,dim2,dim3));
                        
                        ballsocket->setCounter(mBallSocketCount);
                       
                        //PARAMS
                        mFile >> dummySTR;
                        
                        mFile >> dummySTR;
                        
                        while(strcmp(dummySTR.c_str(),"/PARAMS") != 0)
                        {
                            if(strcmp(dummySTR.c_str(),"LOSTOP") == 0){
                                mFile >> dim1;
                                ballsocket->setLoStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"HISTOP") == 0){
                                mFile >> dim1;
                                ballsocket->setHiStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"VEL") == 0){
                                 mFile >> dim1;
                                ballsocket->setVel(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"FMAX") == 0){
                                mFile >> dim1;
                                ballsocket->setFMax(dim1);
                            }  
                            
                            mFile >> dummySTR;                                                     
                        }
                        
                        ballsocket->setCounter(mBallSocketCount);
                        env->addJoint(ballsocket);
                                
                    }
                    
                    if(strcmp(dummySTR.c_str(),"HINGE") == 0){
                        
                        SHinge *hinge = new SHinge();
                      
                        //Name
                        mFile >> dummySTR;
                        hinge->setName(dummySTR);
                        //The body index numbers
                        mFile >> dim1 >> dim2;
                        hinge->setBodyIndex((int)dim1);
                        hinge->setBodyIndex((int)dim2);                        
                        
                        //Burada objelere link yapilacak
                        int i = 0;
                        for(objIter = env->getObjectBegin();i < hinge->getBody1Index();objIter++,i++){}
                        hinge->setBody1(*objIter);
                        
                        i = 0;
                        for(objIter = env->getObjectBegin();i < hinge->getBody2Index();objIter++,i++){}
                        hinge->setBody2(*objIter);
                        
                        //Anchor            
                        mFile >> dim1 >> dim2 >> dim3;
                        hinge->setAnchor(osg::Vec3(dim1,dim2,dim3));
                        
                        //Axis
                        mFile >> dim1 >> dim2 >> dim3;
                        hinge->setAxis(osg::Vec3(dim1,dim2,dim3));                        
                        
                        //PARAMS                       
                        mFile >> dummySTR;
                        
                        mFile >> dummySTR;
                        
                        while(strcmp(dummySTR.c_str(),"/PARAMS") != 0)
                        {
                            if(strcmp(dummySTR.c_str(),"LOSTOP") == 0){
                                mFile >> dim1;
                                hinge->setLoStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"HISTOP") == 0){
                                mFile >> dim1;
                                hinge->setHiStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"VEL") == 0){
                                 mFile >> dim1;
                                hinge->setVel(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"FMAX") == 0){
                                mFile >> dim1;
                                hinge->setFMax(dim1);
                            } 
                            
                            mFile >> dummySTR;                           
                        }
                        
                        hinge->setCounter(mHingeCount);  
                        env->addJoint(hinge);   
                    }
                    
                    if(strcmp(dummySTR.c_str(),"HINGE2") == 0){
                        
                        SHinge2 *hinge2 = new SHinge2();                        
                        //Name
                        mFile >> dummySTR;
                        hinge2->setName(dummySTR);
                        //The body index numbers
                        mFile >> dim1 >> dim2;
                        hinge2->setBodyIndex((int)dim1);
                        hinge2->setBodyIndex((int)dim2);                        
                        
                        //Burada objelere link yapilacak
                        int i = 0;
                        for(objIter = env->getObjectBegin();i < hinge2->getBody1Index();objIter++,i++){}
                        hinge2->setBody1(*objIter);
                        
                        i = 0;
                        for(objIter = env->getObjectBegin();i < hinge2->getBody2Index();objIter++,i++){}
                        hinge2->setBody2(*objIter);
                        
                        //Anchor            
                        mFile >> dim1 >> dim2 >> dim3;
                        hinge2->setAnchor(osg::Vec3(dim1,dim2,dim3));
                        
                        //Axis1
                        mFile >> dim1 >> dim2 >> dim3;
                        hinge2->setAxis1(osg::Vec3(dim1,dim2,dim3));    
                        //Axis2
                        mFile >> dim1 >> dim2 >> dim3;
                        hinge2->setAxis2(osg::Vec3(dim1,dim2,dim3));                    
                        
                        //PARAMS
                        mFile >> dummySTR;
                        
                        //First parameter is get
                        mFile >> dummySTR;
                        
                        while(strcmp(dummySTR.c_str(),"/PARAMS") != 0)
                        {
                            if(strcmp(dummySTR.c_str(),"LOSTOP") == 0){
                                mFile >> dim1;
                                hinge2->setLoStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"HISTOP") == 0){
                                mFile >> dim1;
                                hinge2->setHiStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"VEL") == 0){
                                 mFile >> dim1;
                                hinge2->setVel(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"FMAX") == 0){
                                mFile >> dim1;
                                hinge2->setFMax(dim1);
                            }
                            else if(strcmp(dummySTR.c_str(),"FMAXAXIS2") == 0){
                                mFile >> dim1;
                                hinge2->setFMaxAxis2(dim1);
                            }
                            else if(strcmp(dummySTR.c_str(),"LOSTOPAXIS2") == 0){
                                mFile >> dim1;
                                hinge2->setLoStopAxis2(dim1);
                            }
                            else if(strcmp(dummySTR.c_str(),"HISTOPAXIS2") == 0){
                                mFile >> dim1;
                                hinge2->setHiStopAxis2(dim1);
                           }
                           else if(strcmp(dummySTR.c_str(),"VELAXIS2") == 0){
                               mFile >> dim1;
                               hinge2->setVelAxis2(dim1);
                           }             
                           
                           mFile >> dummySTR;               
                        }
                         hinge2->setCounter(mHinge2Count);
                          env->addJoint(hinge2);
                                
                    }
                    
                    if(strcmp(dummySTR.c_str(),"UNIVERSAL") == 0){
                        
                        SUniversal *universal = new SUniversal();
                        
                        //Name
                        mFile >> dummySTR;
                        universal->setName(dummySTR);
                        //The body index numbers
                        mFile >> dim1 >> dim2;
                        universal->setBodyIndex((int)dim1);
                        universal->setBodyIndex((int)dim2);                        
                        
                        //Burada objelere link yapilacak
                        int i = 0;
                        for(objIter = env->getObjectBegin();i < universal->getBody1Index();objIter++,i++){}
                        universal->setBody1(*objIter);
                        
                        i = 0;
                        for(objIter = env->getObjectBegin();i < universal->getBody2Index();objIter++,i++){}
                        universal->setBody2(*objIter);
                        
                        //Anchor            
                        mFile >> dim1 >> dim2 >> dim3;
                        universal->setAnchor(osg::Vec3(dim1,dim2,dim3));
                        
                        //Axis1
                        mFile >> dim1 >> dim2 >> dim3;
                        universal->setAxis1(osg::Vec3(dim1,dim2,dim3));    
                        //Axis2
                        mFile >> dim1 >> dim2 >> dim3;
                        universal->setAxis2(osg::Vec3(dim1,dim2,dim3));                    
                        
                        //PARAMS                       
                        mFile >> dummySTR;
                        
                        mFile >> dummySTR;
                        
                        while(strcmp(dummySTR.c_str(),"/PARAMS") != 0)
                        {
                            if(strcmp(dummySTR.c_str(),"LOSTOP") == 0){
                                mFile >> dim1;
                                universal->setLoStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"HISTOP") == 0){
                                mFile >> dim1;
                                universal->setHiStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"VEL") == 0){
                                 mFile >> dim1;
                                universal->setVel(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"FMAX") == 0){
                                mFile >> dim1;
                                universal->setFMax(dim1);
                            }
                            else if(strcmp(dummySTR.c_str(),"FMAXAXIS2") == 0){
                                mFile >> dim1;
                                universal->setFMaxAxis2(dim1);
                            }
                            else if(strcmp(dummySTR.c_str(),"LOSTOPAXIS2") == 0){
                                mFile >> dim1;
                                universal->setLoStopAxis2(dim1);
                            }
                            else if(strcmp(dummySTR.c_str(),"HISTOPAXIS2") == 0){
                                mFile >> dim1;
                                universal->setHiStopAxis2(dim1);
                           }
                           else if(strcmp(dummySTR.c_str(),"VELAXIS2") == 0){
                               mFile >> dim1;
                               universal->setVelAxis2(dim1);
                           }     
                           
                           
                           mFile >> dummySTR;                           
                        }   
                        universal->setCounter(mUniversalCount);
                       env->addJoint(universal);        
                    }
                    
                    if(strcmp(dummySTR.c_str(),"SLIDER") == 0){
                        
                        SSlider *slider = new SSlider();
                        
                        //Name
                        mFile >> dummySTR;
                        slider->setName(dummySTR);
                        //The body index numbers
                        mFile >> dim1 >> dim2;
                        slider->setBodyIndex((int)dim1);
                        slider->setBodyIndex((int)dim2);                        
                        
                        //Burada objelere link yapilacak
                        int i = 0;
                        for(objIter = env->getObjectBegin();i < slider->getBody1Index();objIter++,i++){}
                        slider->setBody1(*objIter);

                        i = 0;
                        for(objIter = env->getObjectBegin();i < slider->getBody2Index();objIter++,i++){}
                        slider->setBody2(*objIter);
                        
                        //Axis            
                        mFile >> dim1 >> dim2 >> dim3;
                        slider->setAxis(osg::Vec3(dim1,dim2,dim3));

                        //PARAMS
                        mFile >> dummySTR;
                        
                        mFile >> dummySTR;
                        
                        while(strcmp(dummySTR.c_str(),"/PARAMS") != 0)
                        {
                            if(strcmp(dummySTR.c_str(),"LOSTOP") == 0){
                                mFile >> dim1;
                                slider->setLoStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"HISTOP") == 0){
                                mFile >> dim1;
                                slider->setHiStop(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"VEL") == 0){
                                 mFile >> dim1;
                                slider->setVel(dim1);
                            }
                            else  if(strcmp(dummySTR.c_str(),"FMAX") == 0){
                                mFile >> dim1;
                                slider->setFMax(dim1);
                            }                            
                            
                            mFile >> dummySTR;
                        }
                         slider->setCounter(mSliderCount);
                         env->addJoint(slider);
                         
                                
                    }
                    
                    mFile >> dummySTR;//JOINTLER ICIN
                }
            }
            
            //for the top most while check
            mFile >> dummySTR;
        }
        closeFile();
        return env;
    }
        
    else
    {
        //return env; NULL
        return env;
    }
}

bool SFileInputOutput::writeFile(QString FileName,SEnvironment *env)
{
    osg::Vec3 vec3;
    osg::Quat quat;
    list<SObject*>::iterator iterObj;
    list<SForce*>::iterator iterForce;
    
    if(openFile(FileName,2))
    {
         
         //Write the gravity info
         vec3 = env->getGravity();
         sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
         
         //Mu, SS , CFM, ERP
         sFile << env->getMu() << " " << env ->getStepSize() << " " << env->getCFM() << " " << env->getERP() << endl;
         
         //STEP and number of iterations
        if(env->getSolverType() == SEnvironment::NORMAL)
            sFile << "NORMAL" << endl;
        if(env->getSolverType() == SEnvironment::FASTSTEP)
            sFile << "FAST" << endl;
        if(env->getSolverType() == SEnvironment::QUICKSTEP)
            sFile << "QUICK" << endl;

        sFile << env->getIterations() << endl;
        
         //Objects
        
         for(iterObj = env->getObjectBegin();iterObj != env->getObjectEnd() ; iterObj++)
         {
             //all the objects will be written to the file and their forced
            //according to theri kind they will be written
            
            //BOX
            if((*iterObj)->getObjectType() == SObject::BOX)
            {
                sFile << "BOX" << endl;
                
                if((*iterObj)->getType() == SObject::DYNAMIC)
                {
                    sFile << "DYNAMIC" << endl;
                }
                else
                    sFile << "STATIC" << endl;
                
                //orientation
                quat = (*iterObj)->getOrientation();
                sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3] << endl;
                //dimensions
                vec3 = ((SBox*)*iterObj)->getDimensions();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl; 
                //position
                vec3 = (*iterObj)->getPosition();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //mass
                sFile << (*iterObj)->getMass() << endl;
                //Name
                sFile << (*iterObj)->getName() << endl;
                //Bobby
                if((*iterObj)->hasBobbies())
                    sFile << "TRUE" << endl;
                else
                    sFile << "FALSE" << endl;
                
                //LinVel
                  sFile << "LINVEL" << endl;
                  osg::Vec3 vec = (*iterObj)->getLinVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;                   

                //AngVel
                  sFile << "ANGVEL" << endl;
                  vec = (*iterObj)->getAngVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;
                  
                  mBoxCount = ((SBox *)*iterObj)->getCounter();
                                     
            }
            else if((*iterObj)->getObjectType() == SObject::SPHERE)
            {
                sFile << "SPHERE" << endl;
                
                if((*iterObj)->getType() == SObject::DYNAMIC)
                {
                    sFile << "DYNAMIC" << endl;
                }
                else
                    sFile << "STATIC" << endl;        
                
                //Orientation
                quat = (*iterObj)->getOrientation();
                sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3] << endl;     
                //RADIUS
                sFile << ((SSphere*)(*iterObj))->getRadius() << endl;
                //Position
                vec3 = (*iterObj)->getPosition();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //mass
                sFile << (*iterObj)->getMass() << endl;
                //Name
                sFile << (*iterObj)->getName() << endl;
                //Bobby
                if((*iterObj)->hasBobbies())
                    sFile << "TRUE" << endl;
                else
                    sFile << "FALSE" << endl;
                
                //LinVel
                  sFile << "LINVEL" << endl;
                  osg::Vec3 vec = (*iterObj)->getLinVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;                   

                //AngVel
                  sFile << "ANGVEL" << endl;
                  vec = (*iterObj)->getAngVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;
                
                 mSphereCount = ((SSphere *)*iterObj)->getCounter(); 
                  
            }
            else if((*iterObj)->getObjectType() == SObject::CYLINDER)
            {
                sFile << "CYLINDER" << endl;
                
                if((*iterObj)->getType() == SObject::DYNAMIC)
                {
                    sFile << "DYNAMIC" << endl;
                }
                else
                    sFile << "STATIC" << endl;        
                
                //orientation
                quat = (*iterObj)->getOrientation();
                sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3] << endl;      
                //RADIUS
                sFile << ((SCylinder*)(*iterObj))->getRadius() << endl;
                //Heigth
                sFile << ((SCylinder*)(*iterObj))->getHeight() << endl;
                //Position
                vec3 = (*iterObj)->getPosition();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //mass
                sFile << (*iterObj)->getMass() << endl;
                //Name
                sFile << (*iterObj)->getName() << endl;
                //Bobby
                if((*iterObj)->hasBobbies())
                    sFile << "TRUE" << endl;
                else
                    sFile << "FALSE" << endl;
                
                //LinVel
                  sFile << "LINVEL" << endl;
                  osg::Vec3 vec = (*iterObj)->getLinVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;                   

                //AngVel
                  sFile << "ANGVEL" << endl;
                  vec = (*iterObj)->getAngVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;
                  
                  
                  mCylinderCount = ((SCylinder *)*iterObj)->getCounter();
            }
            else if((*iterObj)->getObjectType() == SObject::CAPPEDCYLINDER)
            {
                sFile << "CAPPEDCYLINDER" << endl;
                
                if((*iterObj)->getType() == SObject::DYNAMIC)
                {
                    sFile << "DYNAMIC" << endl;
                }
                else
                    sFile << "STATIC" << endl;        
                
                //orientation
                quat = (*iterObj)->getOrientation();
                sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3] << endl;     
                //RADIUS
                sFile << ((SCappedCylinder*)(*iterObj))->getRadius() << endl;
                //Heigth
                sFile << ((SCappedCylinder*)(*iterObj))->getHeight() << endl;
                //Position
                vec3 = (*iterObj)->getPosition();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //mass
                sFile << (*iterObj)->getMass() << endl;
                //Name
                sFile << (*iterObj)->getName() << endl;
                //Bobby
                if((*iterObj)->hasBobbies())
                    sFile << "TRUE" << endl;
                else
                    sFile << "FALSE" << endl;
                
                //LinVel
                  sFile << "LINVEL" << endl;
                  osg::Vec3 vec = (*iterObj)->getLinVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;                   

                //AngVel
                  sFile << "ANGVEL" << endl;
                  vec = (*iterObj)->getAngVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;
                  
                  mCappedCylinderCount = ((SCappedCylinder *)*iterObj)->getCounter();
            }
            else if((*iterObj)->getObjectType() == SObject::GEAR)
            {
                /*
                 * GearType
                 * Position
                 * Orientation   
                 * Radius
                 * ToothNumber 
                 * Name
                 * Forces 
                */       
                
                sFile << "GEAR" << endl;
                
                if( ((SGear*)(*iterObj))->getGearType() == SGear::BOXGEAR )
                {
                    sFile << "BOXGEAR" << endl;
                }
                else
                    sFile << "PRISMGEAR" << endl;        
                
                //Position
                vec3 = (*iterObj)->getPosition();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;                                
                 
                 //Orientation
                quat = (*iterObj)->getOrientation();
                sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3] << endl;                     
                
                //RADIUS
                sFile << ((SGear*)(*iterObj))->getRadius() << endl;
                //TOOTH NUMBER
                sFile << ((SGear*)(*iterObj))->getToothNumber() << endl;
                //mass FIXME gear mass has some problems
                //sFile << (*iterObj)->getMass() << endl;
                //Name
                sFile << (*iterObj)->getName() << endl;
                 //Bobby
                if((*iterObj)->hasBobbies())
                    sFile << "TRUE" << endl;
                else
                    sFile << "FALSE" << endl;
                
                //LinVel
                  sFile << "LINVEL" << endl;
                  osg::Vec3 vec = (*iterObj)->getLinVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;                   

                //AngVel
                  sFile << "ANGVEL" << endl;
                  vec = (*iterObj)->getAngVelocity();
                  sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl; 
                  
                  mGearCount = ((SGear *)*iterObj)->getCounter();   
            }
            
            else if((*iterObj)->getObjectType() == SObject::POOL)
            {
               sFile << "POOL" << endl;        
               //Position
                vec3 = (*iterObj)->getPosition();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;                            
                 
                 //Dimensions
                vec3 = ((SPool*)(*iterObj))->getDimensions();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
                 //Orientation
                quat = (*iterObj)->getOrientation();
                sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3] << endl; 
                
                
                //Name
                sFile << (*iterObj)->getName() << endl;
                
                //liquid density depth
                sFile << ((SPool*)(*iterObj))->getDepth() << " " << ((SPool*)(*iterObj))->getLiquidDensity()<< endl;
                
                mPoolCount = ((SPool *)*iterObj)->getCounter();   
            }
            else if((*iterObj)->getObjectType() == SObject::TRIMESH)
            {
                 sFile << "TRIMESH" << endl;
                 
                if((*iterObj)->getType() == SObject::DYNAMIC)
                {
                    sFile << "DYNAMIC" << endl;
                }
                else
                    sFile << "STATIC" << endl;    
                 
                 //Position
                 vec3 = (*iterObj)->getPosition();
                 sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                 //Orientation
                quat = (*iterObj)->getOrientation();
                sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3] << endl; 
                 
                 //name
                 sFile <<  (*iterObj)->getName() << endl;
                     
                //mass
                sFile <<   (*iterObj)->getMass() << endl;
                 
                //vertices
                dVector3 * Vertices;
                int * Indices;
                int verticeCount = ((STrimesh*)(*iterObj))->getVertexCount();
                int indiceCount = ((STrimesh*)(*iterObj))->getIndexCount();
                
                Vertices = ((STrimesh*)(*iterObj))->getVertices();
                Indices  =  ((STrimesh*)(*iterObj))->getIndices();
                
                sFile << "VERTICES" << endl;
                sFile << verticeCount << endl;
                for(int i = 0;  i < verticeCount ; i++)
                {
                   sFile << Vertices[i][0] << " " << Vertices[i][1]  << " " << Vertices[i][2] << endl;
                }
                
                sFile << "INDECES" << endl;
                sFile << indiceCount << endl;
                
                for(int i = 0; i < indiceCount ;)
                {
                     sFile << Indices[i++] << " ";
                    if( i == indiceCount)
                        break;
                    sFile << Indices[i++] << " ";
                    if( i == indiceCount)
                       break;
                    sFile << Indices[i++] << endl;
                                      
                }
                
                 //LinVel
                sFile << "LINVEL" << endl;
                osg::Vec3 vec = (*iterObj)->getLinVelocity();
                sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;                   

                //AngVel
                sFile << "ANGVEL" << endl;
                vec = (*iterObj)->getAngVelocity();
                sFile << vec[0] <<" "<< vec[1] <<" "<<vec[2] << endl;
                                    
                 mTrimeshCount = ((STrimesh *)*iterObj)->getCounter();
        
            }
            else if((*iterObj)->getObjectType() == SObject::INCLINEDPLANE)
            {
                sFile << "INCLINEDPLANE" << endl;
                //Position
                vec3 = (*iterObj)->getPosition();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //Orientation
                
                quat = (*iterObj)->getOrientation();
                sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3]<< endl;
                //Height, Angle,Width
                sFile << ((SInclinedPlane *)(*iterObj))->getHeight() << " " 
                        << ((SInclinedPlane *)(*iterObj))->getAngle() << " "
                        <<  ((SInclinedPlane *)(*iterObj))->getWidth()<< endl;
                
                //Name
                sFile << (*iterObj)->getName() << endl;
                
                mInclinedPlaneCount = ((SInclinedPlane *)*iterObj)->getCounter();
            }
            
            else if((*iterObj)->getObjectType() == SObject::ROPE){
                sFile << "ROPE" << endl;
                
                //Position
                vec3 = ((SRope*)(*iterObj))->getStart();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
               //end
                 vec3 = ((SRope*)(*iterObj))->getEnd();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl; 
                                     
                 //Radius
                sFile << ((SRope*)*iterObj)->getRadius() << endl;
                    
                //mass
                sFile << (*iterObj)->getMass() << endl;
                //Name
                sFile << (*iterObj)->getName() << endl;
                
                
                mRopeCount = ((SRope *)*iterObj)->getCounter();                    
            }
            else if((*iterObj)->getObjectType() == SObject::LIGHT){
            
                    sFile << "LIGHT" << endl;
                    
                    //Position
                    vec3 = (*iterObj)->getPosition();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl; 
            
                    //Orientation
                    quat = (*iterObj)->getOrientation();
                    sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3]<< endl;
                    
                  //Name
                  sFile << (*iterObj)->getName() << endl;
                    
                    mLightCount = ((SLight *)*iterObj)->getCounter();
             }
            else if((*iterObj)->getObjectType() == SObject::FLATMIRROR){
            
                    sFile << "FLATMIRROR" << endl;
                    //Position
                    vec3 = (*iterObj)->getPosition();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
                    //Orientation
                    quat = (*iterObj)->getOrientation();
                    sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3]<< endl;
                    
                    //Widht,Lenght
                    sFile << ((SFlatMirror*)(*iterObj))->getWidth() << " " << ((SFlatMirror*)(*iterObj))->getLength() << endl;
                    
                  //Name
                  sFile << (*iterObj)->getName() << endl;
                                     
                  mFlatMirrorCount = ((SFlatMirror *)*iterObj)->getCounter();
            
            }
            else if((*iterObj)->getObjectType() == SObject::CONMIRROR){
                     
                     sFile << "CONMIRROR" << endl;
                     //Position
                    vec3 = (*iterObj)->getPosition();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
                    //Orientation
                    quat = (*iterObj)->getOrientation();
                    sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3]<< endl;
                    
                    //focal len,azrange,elevrange
                    sFile << ((SConMirror*)(*iterObj))->getFocalLength() 
                            << " "
                            << ((SConMirror*)(*iterObj))->getAzRange() 
                            << " "
                            << ((SConMirror*)(*iterObj))->getElevRange() << endl;
                    
                    //name
                     sFile << (*iterObj)->getName() << endl;
                    
                    
                    mConMirrorCount = ((SConMirror *)*iterObj)->getCounter();
            }
            else if((*iterObj)->getObjectType() == SObject::CONCAVELENS){
            
                 //   mConcaveLensCount = ((SConcaveLens *)*iterObj)->getCounter();
            }
            else if((*iterObj)->getObjectType() == SObject::CONVEXLENS){
                    
                    sFile << "CONVEXLENS" << endl;
                     //Position
                    vec3 = (*iterObj)->getPosition();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
                    //Orientation
                    quat = (*iterObj)->getOrientation();
                    sFile << quat[0] << " " << quat[1] << " " << quat[2] << " " << quat[3]<< endl;
                     //radius,,thickness
                    sFile << ((SConvexLens*)(*iterObj))->getFocalLength() << " " 
                           << ((SConvexLens*)(*iterObj))->getRefractionIndex() << " "
                           << ((SConvexLens*)(*iterObj))->getThickness() << endl;
                                    
                     //name
                     sFile << (*iterObj)->getName() << endl;
                                        
                    mConvexLensCount  = ((SConvexLens *)*iterObj)->getCounter();
            }
                                    
            sFile << "FORCE" << endl;
            
            //FORCES
            for(iterForce = (*iterObj)->getForceBegin(); iterForce != (*iterObj)->getForceEnd();iterForce++)
            {
               if((*iterForce)->getType() == SForce::NORMALFORCE)
               {
                    sFile << "NORMALFORCE" << endl;
                    //name
                    sFile << (*iterForce)->getName() << endl;
                    //forcevector
                    vec3 = (*iterForce)->getForce();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;

                    //Start
                    sFile << (*iterForce)->getStart() << endl;
                    //duration
                    sFile << (*iterForce)->getDuration() << endl;
                    
                    mNormalForceCount = ((SNormalForce*)(*iterForce))->getCounter();                   
                  
                }
                else if((*iterForce)->getType() == SForce::RELFORCE)
                {
                    sFile << "RELFORCE" << endl;
                    //name
                    sFile << (*iterForce)->getName() << endl;
                    //forcevector
                    vec3 = (*iterForce)->getForce();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                    //bodyindex
                    //?
                    //Start
                    sFile << (*iterForce)->getStart() << endl;
                    //duration
                    sFile << (*iterForce)->getDuration() << endl;
                    
                    mRelForceCount = ((SRelForce* )(*iterForce))->getCounter();
                }
                else if((*iterForce)->getType() == SForce::FORCEATPOS)
                {
                    sFile << "FORCEATPOS" << endl;
                    //name
                    sFile << (*iterForce)->getName() << endl;
                    //forcevector
                    vec3 = (*iterForce)->getForce();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                    //posvector
                    vec3 = ((SForceAtPos*)(*iterForce))->getPosition();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;

                    //Start
                    sFile << (*iterForce)->getStart() << endl;
                    //duration
                    sFile << (*iterForce)->getDuration() << endl;
                    
                    mForceAtPosCount = ((SForceAtPos*)(*iterForce))->getCounter();
                }
                else if((*iterForce)->getType() == SForce::RELFORCEATPOS)
                {
                    sFile << "RELFORCEATPOS" << endl;
                    //name
                    sFile << (*iterForce)->getName() << endl;
                    //forcevector
                    vec3 = (*iterForce)->getForce();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                    //posvector
                    vec3 = ((SRelForceAtPos*)(*iterForce))->getPosition();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;

                    //Start
                    sFile << (*iterForce)->getStart() << endl;
                    //duration
                    sFile << (*iterForce)->getDuration() << endl;
                    
                    mRelForceAtPosCount = ((SRelForceAtPos*)(*iterForce))->getCounter();
                }
                else if((*iterForce)->getType() == SForce::FORCEATRELPOS)
                {
                    sFile << "FORCEATRELPOS" << endl;
                    //name
                    sFile << (*iterForce)->getName() << endl;
                    //forcevector
                    vec3 = (*iterForce)->getForce();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                    //posvector
                    vec3 = ((SForceAtRelPos*)(*iterForce))->getPosition();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;

                    //Start
                    sFile << (*iterForce)->getStart() << endl;
                    //duration
                    sFile << (*iterForce)->getDuration() << endl;
                    
                    mForceAtRelPosCount = ((SForceAtRelPos*)(*iterForce))->getCounter();
                }
                else if((*iterForce)->getType() == SForce::RELFORCEATRELPOS)
                {
                    sFile << "RELFORCEATRELPOS" << endl;
                    //name
                    sFile << (*iterForce)->getName() << endl;
                    //forcevector
                    vec3 = (*iterForce)->getForce();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                    //posvector
                    vec3 = ((SRelForceAtRelPos*)(*iterForce))->getPosition();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;

                    //Start
                    sFile << (*iterForce)->getStart() << endl;
                    //duration
                    sFile << (*iterForce)->getDuration() << endl;
                    
                    mRelForceAtRelPosCount = ((SRelForceAtRelPos*)(*iterForce))->getCounter();
                }
                else if((*iterForce)->getType() == SForce::TORQUE)
                {
                    sFile << "TORQUE" << endl;
                    //name
                    sFile << (*iterForce)->getName() << endl;
                    //forcevector
                    vec3 = (*iterForce)->getForce();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;

                    //Start
                    sFile << (*iterForce)->getStart() << endl;
                    //duration
                    sFile << (*iterForce)->getDuration() << endl;
                    
                    mNormalTorque = ((STorque*)(*iterForce))->getCounter();
                }
                else
                {
                    sFile << "RELTORQUE" << endl;
                    //name
                    sFile << (*iterForce)->getName() << endl;
                    //forcevector
                    vec3 = (*iterForce)->getForce();
                    sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;

                    //Start
                    sFile << (*iterForce)->getStart() << endl;
                    //duration
                    sFile << (*iterForce)->getDuration() << endl;
                    
                    mRelTorqueCount = ((SRelTorque*)(*iterForce))->getCounter();
                }
                             
            }
            sFile << "/FORCE" << endl;
         
         }
         
         //JOINTS
         list<SJoint*>::iterator iterJoint;
         
         sFile << "JOINT" << endl;
         env->setIndices();
         
         for(iterJoint = env->getJointBegin(); iterJoint != env->getJointEnd();iterJoint++){
             
             if((*iterJoint)->getJointType() == SJoint::BALLSOCKET)
            {
                sFile << "BALLSOCKET" << endl;
                //name
                sFile << (*iterJoint)->getName() << endl;
                //body 1&2 Index
                sFile << (*iterJoint)->getBody1Index() << " " << (*iterJoint)->getBody2Index() << endl;
                 
                vec3  = ((SBallSocket*)(*iterJoint))->getAnchor();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
                mBallSocketCount = ((SBallSocket*)(*iterJoint)) ->getCounter();
                writeJointParams("BALLSOCKET",(*iterJoint));                 
            }
            else if((*iterJoint)->getJointType() == SJoint::HINGE)
            {
                sFile << "HINGE" << endl;
                //name
                sFile << (*iterJoint)->getName() << endl;
                //body 1&2 Index
                sFile << (*iterJoint)->getBody1Index() << " " << (*iterJoint)->getBody2Index() << endl;
                 
                vec3  = ((SHinge*)(*iterJoint))->getAnchor();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //Axis
                vec3  = ((SHinge*)(*iterJoint))->getAxis();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
                mHingeCount = ((SHinge*)(*iterJoint))->getCounter();
                
                writeJointParams("HINGE",(*iterJoint));
                
            }
            else if((*iterJoint)->getJointType() == SJoint::HINGE2)
            {
                sFile << "HINGE2" << endl;
                //name
                sFile << (*iterJoint)->getName() << endl;
                //body 1&2 Index
                sFile << (*iterJoint)->getBody1Index() << " " << (*iterJoint)->getBody2Index() << endl;
                //anchor
                vec3  = ((SHinge2*)(*iterJoint))->getAnchor();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //Axis1
                vec3  = ((SHinge2*)(*iterJoint))->getAxis1();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //Axis2
                vec3  = ((SHinge2*)(*iterJoint))->getAxis2();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
               mHinge2Count = ((SHinge2*)(*iterJoint))->getCounter();
                writeJointParams("HINGE2",(*iterJoint));
                
            }            
            else if((*iterJoint)->getJointType() == SJoint::UNIVERSAL)
            {
                sFile << "UNIVERSAL" << endl;
                //name
                sFile << (*iterJoint)->getName() << endl;
                //body 1&2 Index
                sFile << (*iterJoint)->getBody1Index() << " " << (*iterJoint)->getBody2Index() << endl;
                //anchor
                vec3  = ((SUniversal*)(*iterJoint))->getAnchor();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //Axis1
                vec3  = ((SUniversal*)(*iterJoint))->getAxis1();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                //Axis2
                vec3  = ((SUniversal*)(*iterJoint))->getAxis2();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
                mUniversalCount = ((SUniversal*)(*iterJoint))->getCounter();
                
                writeJointParams("UNIVERSAL",(*iterJoint));
                
            }
             else if((*iterJoint)->getJointType() == SJoint::SLIDER)
            {
                sFile << "SLIDER" << endl;
                //name
                sFile << (*iterJoint)->getName() << endl;
                //body 1&2 Index
                sFile << (*iterJoint)->getBody1Index() << " " << (*iterJoint)->getBody2Index() << endl;
                 
                vec3  = ((SSlider*)(*iterJoint))->getAxis();
                sFile << vec3[0] << " " << vec3[1] << " " << vec3[2] << endl;
                
                mSliderCount = ((SSlider*)(*iterJoint))->getCounter();
                
                writeJointParams("SLIDER",(*iterJoint));
                
            }
                 
         }
         
         env->resetIndices();
         sFile << "/JOINT" << endl;
         
        mFile << "OBJECT_COUNTS" << endl;
        mFile << mBoxCount << endl;
        mFile << mSphereCount << endl;           
        mFile  << mCylinderCount << endl;
        mFile  <<  mCappedCylinderCount << endl;
        mFile  << mGearCount << endl;
        mFile  << mPoolCount << endl ;
        mFile  << mInclinedPlaneCount << endl ;
        mFile  << mRopeCount  << endl;
        mFile  << mLightCount  << endl;
        mFile  << mFlatMirrorCount << endl;
        mFile  << mConMirrorCount  << endl;
        mFile  << mConcaveLensCount  << endl;
        mFile  << mConvexLensCount  << endl;
        mFile  << mTrimeshCount << endl;   
        
         mFile << "JOINT_COUNTS" << endl;
         
         mFile << mBallSocketCount << endl;
         mFile << mHingeCount        << endl;
         mFile << mHinge2Count      << endl;
         mFile << mUniversalCount  << endl;
         mFile << mSliderCount        << endl;
         
         mFile << "FORCE_COUNTS" << endl;
         
         mFile <<  mNormalForceCount << endl;
         mFile <<  mRelForceCount       << endl;
         mFile  <<  mRelForceAtPosCount << endl;
         mFile  <<  mRelForceAtRelPosCount << endl;
         mFile  <<  mForceAtRelPosCount  << endl;
         mFile  <<  mForceAtPosCount  << endl;
         mFile  <<  mNormalTorque << endl;
         mFile  <<  mRelTorqueCount << endl;
               
        mFile << sFile.str(); 
        
        closeFile();
        return true;
    }
    else
    {     
        return false;    
    }
}


void SFileInputOutput::setForces(SObject * obj,string dummySTR,SEnvironment* env){
         //NormalForce
         float dim1 , dim2 , dim3 ;
        
        if(strcmp(dummySTR.c_str(),"NORMALFORCE") == 0)
        {
            //create a normal force
            SNormalForce *nForce  = new SNormalForce();
            //set object
            nForce->setObject(obj);
            //name
            mFile >> dummySTR;
            nForce->setName(dummySTR);
            //force vector.(Point)
            mFile >> dim1 >> dim2 >> dim3;
            nForce->setForce(osg::Vec3(dim1,dim2,dim3));
           //Start
            mFile >> dim1;
            nForce-> setStart(dim1);
            //Duration
            mFile >> dim1;
            nForce->setDuration(dim1);
            
            //add the force to environment
            nForce->setCounter(mNormalForceCount);
            env->addForce(nForce);
            
        }
        //RelativeForce
        else if(strcmp(dummySTR.c_str(),"RELFORCE") == 0)
        {
        
            SRelForce *rForce  = new SRelForce();

            //set object
            rForce->setObject(obj);
            //name
            mFile >> dummySTR;
            rForce->setName(dummySTR);
            //force vector.(Point)
            mFile >> dim1 >> dim2 >> dim3;
            rForce->setForce(osg::Vec3(dim1,dim2,dim3));

            //Start
            mFile >> dim1;
            rForce-> setStart(dim1);
            //Duration
            mFile >> dim1;
            rForce->setDuration(dim1);
            
            //add the force to environment
            rForce->setCounter(mRelForceCount);
            env->addForce(rForce);                        
        }
        //NormalForceAtPos
        else if(strcmp(dummySTR.c_str(),"FORCEATPOS") == 0)
        {
        
            SForceAtPos *forceAtPos  = new SForceAtPos();

            //set object
            forceAtPos->setObject(obj);
            //name
            mFile >> dummySTR;
            forceAtPos->setName(dummySTR);
            //force vector.(Point)
            mFile >> dim1 >> dim2 >> dim3;
            forceAtPos->setForce(osg::Vec3(dim1,dim2,dim3));
            //pos vector
            mFile >> dim1 >> dim2 >> dim3;
            forceAtPos->setPosition(osg::Vec3(dim1,dim2,dim3));

            //Start
            mFile >> dim1;
            forceAtPos-> setStart(dim1);
            //Duration
            mFile >> dim1;
            forceAtPos->setDuration(dim1);
            
            //add the force to environment
            forceAtPos->setCounter(mForceAtPosCount);
            env->addForce(forceAtPos);                        
        }
        //NormalForceAtRelativePos
        else if(strcmp(dummySTR.c_str(),"NORMALFORCEATRELPOS") == 0)
        {
    
            SForceAtRelPos *forceAtRelPos  = new SForceAtRelPos();

            //set object
            forceAtRelPos->setObject(obj);
            //name
            mFile >> dummySTR;
            forceAtRelPos->setName(dummySTR);
            //force vector.(Point)
            mFile >> dim1 >> dim2 >> dim3;
            forceAtRelPos->setForce(osg::Vec3(dim1,dim2,dim3));
            //pos vector
            mFile >> dim1 >> dim2 >> dim3;
            forceAtRelPos->setPosition(osg::Vec3(dim1,dim2,dim3));

            //Start
            mFile >> dim1;
            forceAtRelPos-> setStart(dim1);
            //Duration
            mFile >> dim1;
            forceAtRelPos->setDuration(dim1);
            
            //add the force to environment
            forceAtRelPos->setCounter(mForceAtRelPosCount);
            env->addForce(forceAtRelPos);                        
        }
        //RelativeForceAtPos
        else if(strcmp(dummySTR.c_str(),"RELFORCEATPOS") == 0)
        {

            SRelForceAtPos *relForceAtPos  = new SRelForceAtPos();
            //set force type
            //relForceAtPos->setForceType(SForce::RELFORCEATPOS);
            //set object
            relForceAtPos->setObject(obj);
            //name
            mFile >> dummySTR;
            relForceAtPos->setName(dummySTR);
            //force vector.(Point)
            mFile >> dim1 >> dim2 >> dim3;
            relForceAtPos->setForce(osg::Vec3(dim1,dim2,dim3));
            //pos vector
            mFile >> dim1 >> dim2 >> dim3;
            relForceAtPos->setPosition(osg::Vec3(dim1,dim2,dim3));
            //BODY INDEX GEREKIRMI ?
            //mFile >> dim1;
            
            //Start
            mFile >> dim1;
            relForceAtPos-> setStart(dim1);
            //Duration
            mFile >> dim1;
            relForceAtPos->setDuration(dim1);
            
            //add the force to environment
            relForceAtPos->setCounter(mRelForceAtPosCount);
            env->addForce(relForceAtPos);                            
        }
        //RelForAtRelPos
        else if(strcmp(dummySTR.c_str(),"RELFORATRELPOS") == 0)
        {
            SRelForceAtRelPos *relForceAtRelPos  = new SRelForceAtRelPos();
            //set force type
            //relForceAtRelPos->setForceType(SForce::RELFORCEATRELPOS);
            //set object
            relForceAtRelPos->setObject(obj);
            //name
            mFile >> dummySTR;
            relForceAtRelPos->setName(dummySTR);
            //force vector.(Point)
            mFile >> dim1 >> dim2 >> dim3;
            relForceAtRelPos->setForce(osg::Vec3(dim1,dim2,dim3));
            //pos vector
            mFile >> dim1 >> dim2 >> dim3;
            relForceAtRelPos->setPosition(osg::Vec3(dim1,dim2,dim3));
            //BODY INDEX GEREKIRMI ?
            //mFile >> dim1;
            
            //Start
            mFile >> dim1;
            relForceAtRelPos-> setStart(dim1);
            //Duration
            mFile >> dim1;
            relForceAtRelPos->setDuration(dim1);
            
            //add the force to environment
            relForceAtRelPos->setCounter(mRelForceAtRelPosCount);
            env->addForce(relForceAtRelPos);                        
        }
        else if(strcmp(dummySTR.c_str(),"TORQUE") == 0)
        {
            STorque *torque  = new STorque();
            //set force type
            //relForceAtRelPos->setForceType(SForce::RELFORCEATRELPOS);
            //set object
            torque->setObject(obj);
            //name
            mFile >> dummySTR;
            
            torque->setName(dummySTR);
            //force vector.(Point)
            mFile >> dim1 >> dim2 >> dim3;
            cout << dim1 << " " << dim2 <<" "<< dim3 << endl;
            torque->setForce(osg::Vec3(dim1,dim2,dim3));
            
            //BODY INDEX GEREKIRMI ?
            //mFile >> dim1;
            
            //Start
            mFile >> dim1;
            torque-> setStart(dim1);
            //Duration
            mFile >> dim1;
            torque->setDuration(dim1);
            
            //add the force to environment
            torque->setCounter(mNormalTorque);
            env->addForce(torque);                        
        }
        else if(strcmp(dummySTR.c_str(),"RELTORQUE") == 0)
        {
            SRelTorque *relTorque  = new SRelTorque();
            //set force type
            //relForceAtRelPos->setForceType(SForce::RELFORCEATRELPOS);
            //set object
            relTorque->setObject(obj);
            //name
            mFile >> dummySTR;
            
            relTorque->setName(dummySTR);
            //force vector.(Point)
            mFile >> dim1 >> dim2 >> dim3;
            relTorque->setForce(osg::Vec3(dim1,dim2,dim3));
            
            //BODY INDEX GEREKIRMI ?
            //mFile >> dim1;
            
            //Start
            mFile >> dim1;
            relTorque-> setStart(dim1);
            //Duration
            mFile >> dim1;
            relTorque->setDuration(dim1);
            
            //add the force to environment
            relTorque->setCounter(mRelTorqueCount);
            env->addForce(relTorque);                         
        }
}

void SFileInputOutput::writeJointParams(string type, SJoint *joint){
                      
             
             sFile  << "PARAMS" << endl;
                     
            if(joint->isLoStopSet())  
            { 
               sFile << "LOSTOP"<<endl;
               sFile<<joint->getLoStop() << endl;
             }
             if(joint->isHiStopSet())
             {
                 sFile << "HISTOP"<<endl;
                 sFile << joint -> getHiStop() << endl;
             }
             if(joint->isVelSet())
             {
                 sFile << "VEL" << endl;
                 sFile << joint->getVel() << endl;
             }
             if(joint->isFMaxSet())
             {
                 sFile << "FMAX" << endl;
                 sFile << joint -> getFMax();
             }
             
             if(strcmp(type.c_str(),"HINGE2") == 0 || strcmp(type.c_str(),"UNIVERSAL") == 0)
             {
                 if(strcmp(type.c_str(),"HINGE2") == 0)
                 {
                     if(((SHinge2 *)joint)->isLoStopSetAxis2())
                     {
                         sFile << "LOSTOPAXIS2"<<endl;
                         sFile << ((SHinge2 *)joint)->getLoStopAxis2() <<  endl;
                     }
                     if(((SHinge2 *)joint)->isHiStopSetAxis2())
                     {
                         sFile << "HISTOPAXIS2"<<endl;
                         sFile << ((SHinge2 *)joint)->getHiStopAxis2() <<  endl;
                     }
                     if(((SHinge2 *)joint)->isVelSetAxis2())
                     {
                         sFile << "VELAXIS2"<<endl;
                         sFile << ((SHinge2 *)joint)->getVelAxis2() <<  endl;
                     }
                      if(((SHinge2 *)joint)->isFMaxSetAxis2())
                     {
                         sFile << "FMAXAXIS2"<<endl;
                         sFile << ((SHinge2 *)joint)->getLoStopAxis2() <<  endl;
                     }
                     
                 }
                 else
                 {
                      if(((SUniversal *)joint)->isLoStopSetAxis2())
                     {
                         sFile << "LOSTOPAXIS2"<<endl;
                         sFile << ((SUniversal *)joint)->getLoStopAxis2() <<  endl;
                     }
                     if(((SUniversal *)joint)->isHiStopSetAxis2())
                     {
                         sFile << "HISTOPAXIS2"<<endl;
                         sFile << ((SUniversal *)joint)->getHiStopAxis2() <<  endl;
                     }
                     if(((SUniversal *)joint)->isVelSetAxis2())
                     {
                         sFile << "VELAXIS2"<<endl;
                         sFile << ((SUniversal *)joint)->getVelAxis2() <<  endl;
                     }
                      if(((SUniversal *)joint)->isFMaxSetAxis2())
                     {
                         sFile << "FMAXAXIS2"<<endl;
                         sFile << ((SUniversal *)joint)->getLoStopAxis2() <<  endl;
                     }
                 
                 }
             
             }
                   
             sFile  << "/PARAMS" << endl;
}


