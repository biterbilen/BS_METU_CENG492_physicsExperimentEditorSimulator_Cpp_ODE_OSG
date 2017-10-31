#include "senvironment.h"

#include "slight.h"

SEnvironment::SEnvironment()
{
    mGravity = osg::Vec3(0, 0, -9.8);
    mStepSize = 0.01;
    mIterations = 20;
    mSolverType = SEnvironment::FASTSTEP;
    mERP = -1;
    mCFM = -1;
    mMu = 0.3;
}

SEnvironment::~SEnvironment()
{}

void SEnvironment::setGravity(osg::Vec3 gra)
{
    mGravity = gra;    
}

void SEnvironment::setMu(float m)
{
    mMu = m;
}

void SEnvironment::setSolverType(SolverType st)
{
    mSolverType = st;    
}

void SEnvironment::setStepSize(float ss)
{
    mStepSize = ss;    
}

void SEnvironment::setIterations(unsigned int iter)
{
    mIterations = iter;    
}

void SEnvironment::addObject(SObject* obj)
{
    mObjects.push_back(obj);    
}

bool SEnvironment::removeObject(SObject* obj)
{
    if(obj->zeroJoints())
    {
        for(list<SForce*>::iterator i = obj->getForceBegin() ; i != obj->getForceEnd() ; i++)
            mForces.remove(*i);
        mObjects.remove(obj);
        return true;
    }
    else
        return false;
}

void SEnvironment::removeObjectJoint(SObject* obj)
{
    obj->removeConnections();
    for(list<SJoint*>::iterator i = obj->getJointBegin() ; i != obj->getJointEnd() ; i++)
        mJoints.remove(*i);
    for(list<SForce*>::iterator j = obj->getForceBegin() ; j != obj->getForceEnd() ; j++)
        mForces.remove(*j);
    mObjects.remove(obj);    
}

void SEnvironment::addJoint(SJoint* joint)
{
    mJoints.push_back(joint);
    joint->getBody1()->addJoint(joint);
    joint->getBody2()->addJoint(joint);    
}

void SEnvironment::removeJoint(SJoint* joint)
{
    joint->getBody1()->removeConnection(joint);
    joint->getBody2()->removeConnection(joint);
    mJoints.remove(joint);    
}

void SEnvironment::addForce(SForce* force)
{
    mForces.push_back(force);
    force->getObject()->addForce(force);
}

void SEnvironment::removeForce(SForce* force)
{
    force->getObject()->removeForce(force);
    mForces.remove(force);    
}

void SEnvironment::setERP(float e)
{
    mERP = e;    
}

void SEnvironment::setCFM(float c)
{
    mCFM = c;    
}

void SEnvironment::resetIndices()
{
    for(list<SJoint*>::iterator i = mJoints.begin() ; i != mJoints.end() ; i++)
        (*i)->resetJoint();    
}

void SEnvironment::setIndices()
{
    int i = 0;
    for(list<SObject*>::iterator o = mObjects.begin() ; o != mObjects.end() ; o++, i++)
        (*o)->writeIndices(i);    
}

list<SObject*>::iterator SEnvironment::getObjectBegin()
{
    return mObjects.begin();
}

list<SObject*>::iterator SEnvironment::getObjectEnd()
{
    return mObjects.end();
}

list<SJoint*>::iterator SEnvironment::getJointBegin()
{
    return mJoints.begin();
}

list<SJoint*>::iterator SEnvironment::getJointEnd()
{
    return mJoints.end();
}

list<SForce*>::iterator SEnvironment::getForceBegin()
{
    return mForces.begin();
}

list<SForce*>::iterator SEnvironment::getForceEnd()
{
    return mForces.end();
}

bool SEnvironment::nameExists(string name)
{
    for(list<SObject*>::iterator i = mObjects.begin() ; i != mObjects.end() ; i++)
        if ((*i)->getName() == name)
            return true;
    return false;
}

//Chart Related
list<SChart*>::iterator SEnvironment::getChartBegin(){
    return mCharts.begin();
}

list<SChart*>::iterator SEnvironment::getChartEnd(){
    return mCharts.end();
}

void SEnvironment::addChart(SChart * chart){
    mCharts.push_back(chart);
}

void SEnvironment::removeChart(SChart * chart){
    mCharts.remove(chart);
}

void SEnvironment::reset()
{
    for(list<SObject*>::iterator i = mObjects.begin() ; i != mObjects.end() ; i++)
    {
         std::cout << (*i)->getName() << std::endl;
         (*i)->clearBobbies();
         
        if((*i)->getObjectType() == SObject::LIGHT)
        {
            ((SLight*)(*i))->clearRays();
        }
    }
    
    for(list<SChart*>::iterator i = mCharts.begin() ; i != mCharts.end() ; i++)
    {
        (*i)->resetChart();
    }
}

