#include <toolmakeshape.h>

void ToolMakeShape::mouseMoved(float x, float y, CameraView *)
{
    pos=osg::Vec3(2*(x-0.5),2*(y-0.5),0);
}

void ToolMakeShape::mouseReleased(float x, float y, CameraView * camView)
{
    pos=osg::Vec3(2*(x-0.5),2*(y-0.5),0);
    
    // Put the sphere in the mouse location (more or less, mouse is in 2D ;)
    osg::Matrix matrix = camView->getSceneView()->getViewMatrix();
    osg::Matrix matrix2 = camView->getSceneView()->getProjectionMatrix();
    osg::Matrix transform = osg::Matrix::inverse(matrix2) * osg::Matrix::inverse(matrix);
    float z = -transform(3, 2)/transform(2,2);
  	pos[2] = z;
    
    if (editor->getSceneGraph() != NULL)
    {
        pos = pos*transform;
        //pos[2] = 0.5;

        SObject* obj = makeShape();
        obj->createEditorObject();
        osg::Node * node = obj->getTransformNode();

        osg::ref_ptr<ComAddChild> com = new ComAddChild(node, editor.get());
        com->execute();
    }
}







