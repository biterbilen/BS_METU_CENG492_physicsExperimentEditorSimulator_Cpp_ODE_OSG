#include "slight.h"

#include "seditorcallback.h"

string SLight::mCounter = "1";

SLight::SLight() : SObject(SObject::STATIC)
{
    mObjectType = SObject::LIGHT;    
    mPosition.set(0,0,0);
    mDirection.set(3,0,0);
    mRadius = 0.35;
    mLength = 500;    
    mName = "Light" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);
    mCatBits = 2;    
    mColBits = (unsigned long)pow(2.0,32) - 1 ;                
}

SLight::SLight(osg::Vec3 pos) : SObject(SObject::STATIC)
{
    mObjectType = SObject::LIGHT;
    mPosition = pos;
    mDirection.set(3,0,0);
    mRadius = 0.35;
    mLength = 500;
    mName = "Light" + mCounter;    
    mCounter = incrementString(mCounter, mCounter.length()-1);
    mCatBits = 2;        
    mColBits = (unsigned long)pow(2.0,32) - 1 ;                
}

void SLight::init()
{   
    mDirection = osg::Matrix(mOrientation.inverse()) * osg::Vec3(0,0,1);
}

osg::Vec3 SLight::getDirection()
{
    return mDirection;
}

float SLight::getLength()
{
    return mLength;
}

void SLight::setLength(float len)
{
    mLength = len;
}
    
osg::Node * SLight::createVisualObject()
{
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the source white
    osg::StateSet *state = new osg::StateSet();
    osg::Material *material = new osg::Material();
    osg::Vec4 sourceColor( 1.0f, 1.0f, 0.0f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, sourceColor );
    state->setAttribute( material );
    
    // light source
    osg:: ShapeDrawable *lightDraw;
    lightDraw = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0), mRadius), hints);
    lightDraw->setStateSet( state);
  
    // Create a Geometry Node, and add the drawable to that node
    osg::Geode *geode = new Geode();
    geode->addDrawable(lightDraw);
    
    osg::PositionAttitudeTransform *transNode = new osg::PositionAttitudeTransform();
    transNode->addChild(geode);
   
    transNode->setPosition(mPosition);
    transNode->setAttitude(mOrientation);
    
    return transNode;         
}

void SLight::createEditorObject()
{
    init();
    
     /** Remove previously created child nodes of the transform node */
    while(mPosAttTransNode->getNumChildren() > 0)
    {
        mPosAttTransNode->removeChild(mPosAttTransNode->getChild(0));
    }
    
    osg::TessellationHints * hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5f);

    // Set up a StateSet to make the source white
    osg::StateSet *state = new osg::StateSet();
    osg::Material *material = new osg::Material();
    osg::Vec4 sourceColor( 1.0f, 1.0f, 0.0f, 1.0f );
    material->setDiffuse( osg::Material::FRONT_AND_BACK, sourceColor );
    state->setAttribute( material );
    
    // light source
    osg:: ShapeDrawable *lightDraw;
    lightDraw = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0,0,0), mRadius), hints);
    lightDraw->setStateSet( state);
    
    // Direction Vector    
    osg::ShapeDrawable* drawable;
    osg::Cone * cone = new osg::Cone(mDirection, 0.1, 0.4); 
    
    osg::Quat quat;
    quat.makeRotate(osg::Vec3(0, 0, 1), mDirection);
    cone->setRotation(quat);
    
    drawable = new osg::ShapeDrawable(cone);
    drawable->setStateSet( state );

    // Array of the direction vector
    osg::Geometry *geom=new osg::Geometry();
    osg::Vec3Array *vertices=new osg::Vec3Array();

    vertices->push_back(osg::Vec3(0,0,0));
    vertices->push_back(mDirection);
    geom->setVertexArray(vertices);
        
    osg::Vec4Array *color = new osg::Vec4Array();
    color->push_back(osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ));
    geom->setColorArray(color);
    geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
    
    // Create a Geometry Node, and add the drawable to that node
    SOsgGeode * geode = new SOsgGeode();
    geode->addDrawable(lightDraw);
    geode->addDrawable(geom);
    geode->addDrawable(drawable);    

    geode->setSObject(this);
       
    mPosAttTransNode->addChild(geode);
    
    SEditorCallback * editorCB = new SEditorCallback(this);
    mPosAttTransNode->setUpdateCallback(editorCB);
}
 
void SLight::pushRay(SRay *ray)
{
    int size = mRays.size();
    mRays.resize(size+1);
    mRays[size] = ray;    
}

int SLight::getRayVectorSize()
{
    return mRays.size() ;   
}

// resizes the rays vector and destroys the invalid geoms 
//TODO destroy invalid NODES besides the invalid geoms
void SLight::resizeRayVector() 
{
    unsigned int size = mRays.size();
    int firstInvalidIndex = size;
    for( unsigned int i = 1 ; i < size ; i++ )
    {         
         if (!mRays[i-1]->areStartEndEqual(mRays[i].get()))
         {
             firstInvalidIndex = i;
             for ( ; i < mRays.size() ; i++)
             { 
                 mLightSceneData->removeChild(mRays[i]->getGeode());
                 mRays[i]->geomDestroy();
             }
             break;
         }
    }
    mRays.resize(firstInvalidIndex);
}

void SLight::clearRays()
{
    mRays.clear();
}

void SLight::createNewRays(dWorldID worldID, dSpaceID spaceID)
{ 
    int size = mRays.size();       
    if ( (mRays[size-1]->getEndPoint() == osg::Vec3(-9999,-9999,-9999))  &&  
         ((size == 1) || (size > 1 && isOptical(mRays[size-2]->getCollidedObjectType() ))) )    
    {
        mRays[size-1]->setInfiniteEnd();        
    }
    
    mLightSceneData->addChild(mRays[size-1]->createVisualObject());
//     std::cout << "CREATENEWRAYS:  size before addition    " << size << std::endl;    
    if (isOptical(mRays[size-1]->getCollidedObjectType()))
    {
        
        SRay *ray = new SRay(mRays[size-1]->getEndPoint());
        // TODO ERASE THE following 2 lines if they are unnecessary -they are in the constructeor also
        ray->setCollideBits(mColBits);
        ray->setCategoryBits(mCatBits);     
               
        mRays.push_back(ray);
        
        //ode returns the opposite direction for surface normals
        // HENCE take the minus(-) of the normal
//         mRays[size]->calculateDirection(mRays[size-1]->getDirection(),-(mRays[size-1]->getNormal()));
        dGeomID collGeom = mRays[size-1]->getCollidedObjectGeom();
        SObject * collidedObj = (*mLightObjectMap)[collGeom];        
        if (collidedObj == NULL) // plane 
        {
            mRays[size]->calculateDirection(mRays[size-1]->getDirection(),-(mRays[size-1]->getNormal()));    
        }
        else if (collidedObj->isMirror())
        {
//                  std::cout << " mirror collision" << std::endl;
            mRays[size]->calculateDirection(mRays[size-1].get(),collidedObj);
        }
        else if (collidedObj->isLens())
        {                 
            // the ray may be entering the lens or leaving the lens
            if (size > 2) 
            {
                dGeomID collGeomPrev = mRays[size-2]->getCollidedObjectGeom();
                SObject * collidedObjPrev = (*mLightObjectMap)[collGeomPrev];
                // leaving the lens                
                if (collidedObjPrev  == collidedObj && collidedObjPrev->getObjectType() == SObject::CONVEXLENS ) 
                {                            
                    mRays[size]->calculateDirection(mRays[size-1].get(), ((SConvexLens* )collidedObj)->getRefractionIndex(), 1, collidedObj);  
                }        
                else if (collidedObjPrev  == collidedObj && collidedObjPrev->getObjectType() == SObject::CONCAVELENS ) 
                {      
                }                
                // entering the lens
                else if (collidedObjPrev  != collidedObj && collidedObjPrev->getObjectType() == SObject::CONVEXLENS ) 
                {                              
                    mRays[size]->calculateDirection(mRays[size-1].get(), 1, ((SConvexLens* )collidedObj)->getRefractionIndex(), collidedObj);  
                }    
                else if (collidedObjPrev  != collidedObj && collidedObjPrev->getObjectType() == SObject::CONCAVELENS ) 
                {      
                }                
            }
            // the ray should be exactly entering the lens
            else 
            {                                
                // entering the lens    
                if (collidedObj->getObjectType() == SObject::CONVEXLENS ) 
                {                       
                    mRays[size]->calculateDirection(mRays[size-1].get(), 1, ((SConvexLens* )collidedObj)->getRefractionIndex(), collidedObj);             
                }    
                else if (collidedObj->getObjectType() == SObject::CONCAVELENS ) 
                {     
                    
                }
            }
        }   
        else
        {
            mRays[size]->calculateDirection(mRays[size-1]->getDirection(),-(mRays[size-1]->getNormal()));    
        }
        //TODO set length of the new ray
        mRays[size]->createSimObject(worldID, spaceID); 
        mLightObjectMap->insert(pair<dGeomID, SObject*>(mRays[size]->getGeomID(),ray));       
    }     
//     std::cout << "CREATENEWRAYS:  end     " ;
//     print();
}

void SLight::setObjectMap(map<dGeomID, SObject*> *mObjectMap)
{
    mLightObjectMap = mObjectMap;
}

string SLight::getCounter()
{
    return mCounter;
}

void SLight::setCounter(string counter)
{
    mCounter = counter;
} 

void SLight::print()
{
    int size = mRays.size();
    cout << "mRays elements:" << std::endl;
    for(int i = 0; i < size ; i++) 
    {        
        mRays[i]->print();
    }    
}
