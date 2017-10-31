#ifndef _S_FILE_INPUT_OUTPUT_H
#define _S_FILE_INPUT_OUTPUT_H 1

#include <sstream>
#include <qstring.h>
#include <fstream>
#include <iostream>
#include <vector>

//ENVIRONMENT RELATED
#include "senvironment.h"

//OBJECT RELATED
#include "sobject.h"
#include "sbox.h"
#include "ssphere.h"
#include "scappedcylinder.h"
#include "scylinder.h"
#include "sgear.h"
#include "spool.h"
#include "srope.h"
#include "sray.h"
#include "slight.h"
#include "sflatmirror.h"
#include "sconvexlens.h"
#include "sconmirror.h" 


//FORCE RELATED
#include "sforce.h"
#include "snormalforce.h"
#include "sforceatpos.h"
#include "sforceatrelpos.h"
#include "srelforceatpos.h"
#include "srelforceatrelpos.h"
#include "srelforce.h"
#include "sreltorque.h"
#include "storque.h"
#include "strimesh.h"
#include "sinclinedplane.h"
#include "sflatmirror.h"


//JOINT RELATED
#include "sjoint.h"
#include "shinge.h"
#include "shinge2.h"
#include "sslider.h"
#include "suniversal.h"
#include "sballsocket.h"

using namespace std;

/**@class SFileInputOutput
*@brief This class manipulates all the file input and output functions
*When reading it creates a new environment for the simulation from the file.
*And this file is used to store the new generated environments for decreasing
*workload to make the things again and again for each time.
*/
class SFileInputOutput
{

private:
    float mGravityX;
    float mGravityY;
    float mGravityZ;
    float mERP;
    float mCFM;
    float mMu;
    float mStepSize;
    
    list<SObject*>::iterator objIter;
    SObject *objPtr;

    //for naming counts
    string mBoxCount ;
    string mSphereCount;
    string mCylinderCount ;
    string mCappedCylinderCount ;
    string mGearCount;
    string mPoolCount ;
    string mInclinedPlaneCount ;
    string mRopeCount ;
    string mRayCount ;
    string mLightCount ;
    string mFlatMirrorCount;
    string mConMirrorCount ;
    string mConcaveLensCount ;
    string mConvexLensCount ;
    string mTrimeshCount ;
    
    
    string mBallSocketCount;
    string mHingeCount;
    string mHinge2Count;
    string mSliderCount;
    string mUniversalCount;
     
    string mNormalForceCount;
    string mRelForceCount;
    string mRelForceAtPosCount;
    string mRelForceAtRelPosCount;
    string mForceAtRelPosCount;
    string mForceAtPosCount;
    string mNormalTorque;
    string mRelTorqueCount;
    
       
    fstream mFile;
    ostringstream sFile;

public:
/**@constructor  SFileInputOutput()
*In the constructor the naming variables are set
*/
    SFileInputOutput();

/**@destructor  ~SFileInputOutput()
*The file is closed
*/   
    ~SFileInputOutput();

/**@function bool openFile(QString FileName)
*This function gets the file name for reading or writing
*If the file is present at the location then it can be opened for
* reading and writing. If the file is not present then a false 
*value is returned.
*/

    bool openFile(QString FileName,int type);
    
/**@function Environment * readFile(QString FileName)
*This function reads the file and generates an environment
*class to give it to the simulation. After the environment
*variables are set the object , joint and force variables are                
*taken from the file according to a predecided order
 */
    
    SEnvironment * readFile(QString FileName,SEnvironment*);

/**@function void writeFile(QString FileName,Environment *)
*This function takes the filename and the environment instance that is
*being worked on and generates an environment file to be used later
*/
    bool writeFile(QString FileName, SEnvironment *);
    
/**@function void closeFile(QString FileName,Environment *)
*Closes the file that is opened for reading or writing
*/    
    
    void closeFile();

/**@function  void setForces(SObject * obj,string type,SEnvironment* env)
*While reading from the file this functions takes the variables from the
*file and sets the properties of the objects' forces
*/       
    void setForces(SObject * obj,string type,SEnvironment* env);
/**@function  void setForces(SObject * obj,string type,SEnvironment* env)
*Writes the parameters of the joint variables of the environment
*to the file
*/   
    void writeJointParams( string type,SJoint* j);
};


#endif

