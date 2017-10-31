#include "sobject.h"

#include <math.h>

SObject::SObject(Types t)
{
    mType = t;
    mPosition.set(0,0,0);
    mOrientation = osg::Quat(0, osg::Vec3(0, 0, 1));
    mBobby = false;
    mLinVelocity = new SLinVelocity(osg::Vec3(0,0,0));
    mAngVelocity = new SAngVelocity(osg::Vec3(0,0,0));
    mCatBits = 4;
    mColBits = (unsigned long)pow(2.0,32) - 2;
    mPosAttTransNode = new osg::PositionAttitudeTransform();
}

void SObject::setPosition(osg::Vec3 pos)
{
    mPosition = pos;
}

void SObject::setPosition(float x, float y, float z)
{
    mPosition.set(x,y,z);
}

bool SObject::hasBobbies()
{
    return mBobby;
}

void SObject::clearBobbies()
{
    if(mBobby)
    {
//         list<osg::ref_ptr<SBobby> >::iterator i;
//         for(i = mBobbies.begin() ; i != mBobbies.end() ; i++)
//             mBobbies.remove((*i));
        
        mBobbies.clear();
    }
}

bool SObject::isOptical(ObjTypes t)
{
    if(t==CONCAVELENS || 
        t==CONVEXLENS ||         
        t==CONMIRROR ||                                     
        t==FLATMIRROR )
        return true;
    else
        return false;
}

bool SObject::isMirror()
{
    if( mObjectType==CONMIRROR || mObjectType==FLATMIRROR )
        return true;
    else
        return false;
}
bool SObject::isLens()
{
    if( mObjectType==CONCAVELENS || mObjectType==CONVEXLENS )
        return true;
    else
        return false;
}
    

bool SObject::canHaveChart()
{
    if(mObjectType==BOX || 
        mObjectType==SPHERE || 
        mObjectType==CAPPEDCYLINDER || 
        mObjectType==CYLINDER ||
        mObjectType==TRIMESH)
        return true;
    else
        return false;
}
void SObject::setBobby(bool b)
{
    mBobby = b;
}

osg::Vec3 SObject::getPosition() const
{
    return mPosition;
}

void SObject::setName(string s)
{
    mName = s;
}

string SObject::getName() const
{
    return mName;
}

SObject::Types SObject::getType() const
{
    return mType;
}

void SObject::setType(SObject::Types t)
{
    mType = t;
}

SObject::ObjTypes SObject::getObjectType() const
{
    return mObjectType;
}

void SObject::setObjectType(SObject::ObjTypes obj)
{
    mObjectType = obj;
}

float SObject::getMass() const
{
    return mMass.mass;
}

void SObject::setOrientation(osg::Quat ori)
{
    mOrientation = ori;
}

osg::Quat SObject::getOrientation() const
{
    return mOrientation;
}

osg::Vec3 SObject::getEulerAngles()
{
    osg::Vec3 euler;

    double sqw = mOrientation.w() * mOrientation.w();
    double sqx = mOrientation.x() * mOrientation.x();
    double sqy = mOrientation.y() * mOrientation.y();
    double sqz = mOrientation.z() * mOrientation.z();

    euler[0] = atan2(2.0 * (mOrientation.x() * mOrientation.y() + mOrientation.z() * mOrientation.w()),(sqx - sqy - sqz + sqw));
    euler[1] = atan2(2.0 * (mOrientation.y() * mOrientation.z() + mOrientation.x() * mOrientation.w()),(-sqx - sqy + sqz + sqw));
    euler[2] = asin(-2.0 * (mOrientation.x() * mOrientation.z() - mOrientation.y() * mOrientation.w())/sqx + sqy + sqz + sqw);

    return euler;
}

void SObject::setBodyID(dBodyID id)
{
    mBodyID = id;
}

dBodyID SObject::getBodyID() const
{
    return mBodyID;
}

dGeomID SObject::getGeomID() const
{
    return mGeomID;
}

bool SObject::isStatic() const
{
    if(mType == STATIC)
        return true;
    else
        return false;
}

bool SObject::isDynamic() const
{
    if(mType == DYNAMIC)
        return true;
    else
        return false;
}

bool SObject::canAddForce()
{
    if(mObjectType == SObject::BOX || mObjectType == SObject::SPHERE ||
       mObjectType == SObject::CAPPEDCYLINDER || mObjectType == SObject::CYLINDER || 
       mObjectType == SObject::TRIMESH )
        return true;
    else return false;
}

void SObject::addForce(SForce* f)
{
    mForces.push_back(f);
}

void SObject::removeForce(SForce* f)
{
    mForces.remove(f);
}

void SObject::setAngVelocity(osg::Vec3 v)
{
    mAngVelocity->setVelocity(v);
}

void SObject::setLinVelocity(osg::Vec3 v)
{
    mLinVelocity->setVelocity(v);
}

bool SObject::canAddJoint()
{
     if(mObjectType == SObject::BOX || mObjectType == SObject::SPHERE ||
       mObjectType == SObject::CAPPEDCYLINDER || mObjectType == SObject::CYLINDER || 
       mObjectType == SObject::TRIMESH )
        return true;
    else return false;
}

void SObject::addJoint(SJoint* j)
{
    mJoints.push_back(j);
}

void SObject::removeJoint(SJoint* j)
{
    mJoints.remove(j);
}

int SObject::getForceCount() const
{
    return mForces.size();
}

void SObject::writeIndices(int index)
{
    for(list<SJoint*>::iterator i = mJoints.begin() ; i != mJoints.end() ; i++)
        (*i)->setBodyIndex(index);
}

void SObject::removeConnections()
{
    for(list<SJoint*>::iterator i = mJoints.begin() ; i != mJoints.end() ; i++)
    {
        if((*i)->getBody1() == this)
            (*i)->getBody2()->removeConnection(*i);
        else
            (*i)->getBody1()->removeConnection(*i);
    }
}

void SObject::removeConnection(SJoint* joint)
{
    mJoints.remove(joint);
}

bool SObject::zeroJoints()
{
    return (mJoints.begin() == mJoints.end());
}

list<SJoint*>::iterator SObject::getJointBegin()
{
    return mJoints.begin();
}

list<SJoint*>::iterator SObject::getJointEnd()
{
    return mJoints.end();
}

list<SForce*>::iterator SObject::getForceBegin()
{
    return mForces.begin();
}

list<SForce*>::iterator SObject::getForceEnd()
{
    return mForces.end();
}

list<osg::ref_ptr<SBobby> >::iterator SObject::getBobbyBegin()
{
    return mBobbies.begin();
}

list<osg::ref_ptr<SBobby> >::iterator SObject::getBobbyEnd()
{
    return mBobbies.end();
}

void SObject::setCollideBits(unsigned long col)
{
    mColBits = col;
}

void SObject::setCategoryBits(unsigned long cat)
{
    mCatBits = cat;
}

void SObject::setOdeCollideBits()
{
    dGeomSetCategoryBits (mGeomID, mColBits);   
}

void SObject::setOdeCategoryBits()
{
    dGeomSetCollideBits (mGeomID, mCatBits);   
}    

unsigned long SObject::getCollideBits()
{
    return mColBits;    
}

unsigned long SObject::getCategoryBits()
{
    return mCatBits;    
}

float SObject::getSubmergedAmount()
{
    float totVolume = 0;
    float totSubmergedVolume = 0;

    for(list<osg::ref_ptr<SBobby> >::iterator i = mBobbies.begin() ; i != mBobbies.end() ; i++)
    {
        totVolume = totVolume + (*i)->getTotalVolume();
        totSubmergedVolume = totSubmergedVolume + (*i)->getSubmergedVolume();
    }

    if (totVolume == 0)
        return 0;
    else
        return totSubmergedVolume / totVolume;
}

void SObject::updateBobbies(float waterLevel, float liquidDensity)
{
    for(list<osg::ref_ptr<SBobby> >::iterator i = mBobbies.begin() ; i != mBobbies.end() ; i++)
        (*i)->update(waterLevel, liquidDensity);
}

float SObject::getObjectLinearVel()
{
   
    float totLinVel = 0;
    const dReal * linvel = dBodyGetLinearVel(mBodyID);
    cout << "lin vel: " << linvel[0] << " " << linvel[1] << " " << linvel[2] << endl;
    osg::Vec3* vec = new osg::Vec3(linvel[0],linvel[1],linvel[2]);
    totLinVel = vec->length();
    return totLinVel;
}

float SObject::getObjectAngularVel()
{
    
    float totAngVel = 0;
    const dReal * angvel = dBodyGetAngularVel(mBodyID);
    osg::Vec3 *vec = new osg::Vec3(angvel[0],angvel[1],angvel[2]);
    totAngVel = vec->length();
    return totAngVel;
}

float SObject::getObjectPositionX()
{
    return (dBodyGetPosition(mBodyID))[0];
}

float SObject::getObjectPositionY()
{
    return (dBodyGetPosition(mBodyID))[1];
}

float SObject::getObjectPositionZ()
{
    return (dBodyGetPosition(mBodyID))[2];
}

float SObject::getObjectForce()
{
    float totForce = 0;
    const dReal *force = (dBodyGetForce(mBodyID));
    osg::Vec3 *vec = new osg::Vec3(force[0],force[1],force[2]);
    cout << "force 1:" << force[0] << "force 2:" << force[1] << "force 3:" << force[2] << endl;
    totForce = vec->length();       
    return totForce;
}

float SObject::getObjectTorque()
{

    float totTorque = 0;
    const dReal *torque = (dBodyGetTorque(mBodyID));
    osg::Vec3 *vec = new osg::Vec3(torque[0],torque[1],torque[2]);
    totTorque = vec->length(); 
    return totTorque;
}

float SObject::getObjectAcc()
{
    float totForce = 0;
    float acc = 0;
    const dReal * force = (dBodyGetForce(mBodyID));
    osg::Vec3 *vec = new osg::Vec3(force[0],force[1],force[2]);
    totForce = vec->length();
    return acc = totForce / getMass();
}

string SObject::incrementString(string s, int n)
{
    if(s[n] == '9')
    {
        s[n] = '0';
        if(n == 0)
            s = "1" + s;
        else
            s = incrementString(s, n-1);    
    }
    else
    {
        switch(s[n])
            {
                case '0' : s[n] = '1'; break;
                case '1' : s[n] = '2'; break;
                case '2' : s[n] = '3'; break;
                case '3' : s[n] = '4'; break;
                case '4' : s[n] = '5'; break;
                case '5' : s[n] = '6'; break;
                case '6' : s[n] = '7'; break;
                case '7' : s[n] = '8'; break;
                default  : s[n] = '9'; break;            
            }    
    }    
    return s;
}
