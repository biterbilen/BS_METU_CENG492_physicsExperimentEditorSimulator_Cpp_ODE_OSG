
#include "sosggeode.h"


SOsgGeode::SOsgGeode() { mObj = NULL; }

SOsgGeode::~SOsgGeode() {}

void SOsgGeode::setSObject(SObject * obj) 
{ 
    mObj = obj; 
}

SObject * SOsgGeode::getSObject()
{
    return mObj.get();
}

