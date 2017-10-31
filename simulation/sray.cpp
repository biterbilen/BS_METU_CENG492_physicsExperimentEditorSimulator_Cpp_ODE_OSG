#include "sray.h"

string SRay::mCounter = "1";

//FIXME aborted
SRay::SRay(osg::Vec3 pos) : SObject(SObject::STATIC)
{
    //name set
    mObjectType = SObject::RAY;
    mName = "Ray" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);

    mPosition = pos;
    mEnd.set(-9999,-9999,-9999);
    mDirection.set(0,0,1);
    mLength = 500;
    mOrientation = osg::Quat(0,osg::Vec3(0,0,1));
    mCatBits = 2;    
    mColBits = (unsigned long)pow(2.0,32) - 1 ; 
    mName = "Ray" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);                   
}

SRay::SRay(osg::Vec3 pos, osg::Vec3 dir) : SObject(SObject::STATIC)
{
    mObjectType = SObject::RAY;
    mPosition = pos;
    mEnd.set(-9999,-9999,-9999);
    mDirection = dir;
    mLength = 500;
    mOrientation = osg::Quat(0,osg::Vec3(0,0,1));
    mCatBits = 2;
    mColBits = (unsigned long)pow(2.0,32) - 1;
    mName = "Ray" + mCounter;
    mCounter = incrementString(mCounter, mCounter.length()-1);                   
}

 SRay::~ SRay()
 { 
//      std::cout << "Ray Destroyed" << std::endl; 
 }

void SRay::setEndPoint(osg::Vec3 end)
{
    mEnd = end;
}
   
osg::Vec3 SRay::getEndPoint()
{
    return mEnd;
}

void SRay::setNormal(osg::Vec3 normal)
{
     mNormal = normal;
}

osg::Vec3 SRay::getNormal()
{
    return mNormal;
}

void SRay::setLength(float len) 
{
    mLength = len;    
}

float SRay::getLength()
{
    return mLength;    
}

void SRay::setDirection(osg::Vec3 dir)
{
    mDirection = dir;
}

osg::Vec3 SRay::getDirection()
{
    return mDirection;
}

string SRay::getCounter()
{
    return mCounter;
}

void SRay::setCounter(string counter)
{
    mCounter = counter;
}
    
    
void SRay::setCollidedObjectType(SObject::ObjTypes type)
{
    mCollidedObject = type;
}

SObject::ObjTypes SRay::getCollidedObjectType()
{
    return mCollidedObject;
}

void SRay::setCollidedObjectGeom(dGeomID geom)
{
    mCollidedObjectGeom = geom;
}

dGeomID SRay::getCollidedObjectGeom()
{
    return mCollidedObjectGeom;
}

bool SRay::areStartEndEqual(SRay *ray)
{
    if (mEnd == ray->getPosition())
        return true;
    else 
        return false;
}

//TODO THESE 2 SHOULD BE WRIITEN AS OPTICAL OBJECT FUNCTIONS
void SRay::calculateDirection(osg::Vec3 sourceDirection, osg::Vec3 surfaceNormal)
{
    mDirection = sourceDirection - (surfaceNormal) * ( (sourceDirection*2) * (surfaceNormal) ); 
}

void SRay::calculateDirection(SRay * ray, SObject *collObj)
{
    //ode returns the opposite direction for surface normals
        // HENCE take the minus(-) of the normal
    
    SObject::ObjTypes ot = collObj->getObjectType();
    osg::Vec3 sourceDirection = ray->getDirection();
    osg::Vec3 surfaceNormal = -ray->getNormal();  
    if ( ot == SObject::FLATMIRROR || 
    ( ot == SObject::CONMIRROR  && ((SConMirror *)collObj)->isValidPoint(ray->getEndPoint() )) || 
    ot ==SObject::CONVEXLENS)
    {
//         std::cout << "flat: dir is different" << std::endl;        
        mDirection = sourceDirection - (surfaceNormal) * ( (sourceDirection*2) * (surfaceNormal) );
    }
    
    else
    {
//         std::cout << " dir is the same as the previous one" << std::endl;     
        mDirection = sourceDirection;        
    }
    
}

void SRay::calculateDirection(SRay * ray, float nu1, float nu2,SObject *collObj)
{
//     std::cout << "calculate ray direction nu1=" << nu1 << " nu2=" << nu2 << std::endl; 
    SObject::ObjTypes ot = collObj->getObjectType();
    osg::Vec3 L = -ray->getDirection();
    osg::Vec3 N = -ray->getNormal();    
    
    float ref = nu1/nu2;
    float onedivref = 1.0 / ref;
        
    if ( ot == SObject::CONVEXLENS  && ((SConvexLens *)collObj)->isValidPoint(ray->getEndPoint() ))
    {   
        if(nu1 < nu2) // the ray gets closer to the normal
        {
            std::cout << "the ray gets closer to the normal" <<std::endl;
            float theta1 = acos((L*N)/(L.length()*N.length())); // the angle between the incoming ray and the lens normal
            float theta2 = asin(ref * sin(theta1));// the angle between the transmission ray and the normal of the lens        
            mDirection = N * ( ref*cos(theta1) - cos(theta2) ) - ( L * ref)  ; 
            
            cout<<"theta1= "<<theta1;                        
            cout<<"    theta2= "<<theta2;
            cout<<"    new mDirection= "<<mDirection<<endl<<endl;                           
        }
           
        else // the ray gets away from the normal
        {
            std::cout << "the ray gets away from the normal" <<std::endl;
            osg::Vec3 Nprime = -ray->getNormal();  
            osg::Vec3 Tprime = -ray->getDirection();
            
            float theta1 = acos ( (N * Tprime) / (N.length()*Tprime.length()) );
            float theta2 = asin ( sin(theta1)*(nu1/nu2) );
            
            cout<< " refraction - reflection test" << ref * sin(theta1)<<endl;
            if(ref * sin(theta1)<=1) // refraction 
            {
                std::cout << "REFRACTION" << std::endl;
                mDirection = ( -Tprime * (nu1/nu2) ) 
                                + ( Nprime * ( (nu2/nu1)*(nu2/nu1) * cos(theta2) - (nu2/nu1) * cos(theta1) ) );
                cout<<"theta1= "<<theta1;                        
                cout<<"    theta2= "<<theta2;
                cout<<"    new mDirection= "<<mDirection<<endl<<endl;                
            }  
            else //reflection
            {
                std::cout << "REFLECTION" << std::endl;
                mDirection = L - (-N)*((L*2)*L);         
            }        
        }        
    }
//     else if ( ot == SObject::CONCAVELENS  && ((SConcaveLens *)collObj)->isValidPoint(ray->getEndPoint() ))
//     {
//     }
    else
    {
        std::cout << " dir is the same as the previous one" << std::endl;     
        mDirection = ray->getDirection();       
    }
    
    
}
void SRay::geomDestroy()
{
    dGeomDestroy(mGeomID);
}

void SRay::setInfiniteEnd()
{
    mEnd = mDirection*mLength + mPosition;
}

void SRay::print()
{    
    cout << "    name:" << mName;  
    cout << "    start:" << mPosition[0] << " "<< mPosition[1] << " " << mPosition[2] ;
    cout << "    end:" << mEnd[0] << " "<< mEnd[1] << " " << mEnd[2] ;
    cout << "    dir:" << mDirection[0] << " " << mDirection[1] << " " << mDirection[2] ;
    cout << "    normal:" << mNormal[0] << " " << mNormal[1] << " " << mNormal[2];
    cout << "    collidedObject:" << mCollidedObject ;
    cout << "    length:" << mLength << std::endl;
}
    
void SRay::createSimObject(dWorldID world, dSpaceID space)
{
    mBodyID = 0;  
    mGeomID = dCreateRay (space, mLength);
    dGeomSetCategoryBits(mGeomID,mCatBits);
    dGeomSetCollideBits(mGeomID,mColBits);
    dGeomRaySet(mGeomID, 
                mPosition.x(), mPosition.y(), mPosition.z(),    
                mDirection[0], mDirection[1], mDirection[2]);
    dGeomSetBody(mGeomID, mBodyID);
}

osg::Node * SRay::createVisualObject()
{
    // ray drawing
    osg::Geometry *geom=new osg::Geometry();
    osg::Vec3Array *vertices=new osg::Vec3Array();

    vertices->push_back(mPosition);
    vertices->push_back(mEnd);
    geom->setVertexArray(vertices);
        
    osg::Vec4Array *color = new osg::Vec4Array();
    color->push_back(osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ));
    geom->setColorArray(color);
    
    geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
    
    //normal drawing
    osg::Geometry *geomNormal=new osg::Geometry();
    osg::Vec3Array *verticesNormal=new osg::Vec3Array();

    verticesNormal->push_back(mEnd-mNormal);
    verticesNormal->push_back(mEnd+mNormal);
    geomNormal->setVertexArray(verticesNormal);
        
    osg::Vec4Array *colorNormal = new osg::Vec4Array();
    colorNormal->push_back(osg::Vec4( 0.0f, 1.0f, 0.0f, 1.0f ));
    geomNormal->setColorArray(colorNormal);
    
    geomNormal->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    geomNormal->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
    
    // Create a Geometry Node, and add the drawable to that node
    mGeode = new osg::Geode();
    
    mGeode->addDrawable(geom);
    mGeode->addDrawable(geomNormal);

    return mGeode.get();
}
