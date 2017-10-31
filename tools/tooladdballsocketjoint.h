#ifndef _TOOL_ADD_BSOCKET_JOINT_H
#define _TOOL_ADD_BSOCKET_JOINT_H 1

#include "tooladdjoint.h"
#include "sballsocket.h"

class ToolAddBallSocketJoint: public ToolAddJoint {
public:
    ToolAddBallSocketJoint(): ToolAddJoint("add-ball-socket-joint") {}
    virtual SJoint *makeJoint() { 
        SBallSocket * ballSocket = new SBallSocket();
        ballSocket->setAnchor((mObj1->getPosition() + mObj2->getPosition()) /2);        
        return ballSocket;
   }
    virtual Tool* clone() { return new ToolAddBallSocketJoint(); }
};

#endif

