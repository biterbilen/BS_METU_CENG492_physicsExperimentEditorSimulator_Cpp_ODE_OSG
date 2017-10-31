#ifndef __TOOL_H__
#define __TOOL_H__

#include <osg/Node>
#include <osg/Referenced>
#include <editor.h>
#include <string>

/** @class Tool
 * @brief The generic tool
 *
 * This interface encapsulates the API of a tool. These tools can
 * only receive two events. One when the user selects them ( init() event ),
 * and another when a node is selected ( selected(...) event ).
 *
 * If your tool needs mouse info, consider using DrawingAreaTool. The reason
 * why this info isn't here is that most tools don't need this kind of info
 * and it's a lot of information wasted (mouse sends a lot of information per
 * second that should be ignored and is a nonsense).
 */
class Tool: public osg::Referenced {
    std::string name;
protected:
    osg::ref_ptr<Editor> editor;
public:
    /**
     * Constructor. Note that you will need to provide a name in your
     * subclasses. And take care of the name that you give, if there is
     * another tool with the same name, the editor will call one of them,
     * and wich one is undefined.
     */
    Tool(std::string name) { this->name=name; }
    virtual ~Tool() {}

    /**
     * This method gives the name of the tool.
     */
    std::string getName() { return name; }

    /**
     * Selected event. Called when a node is selected. If a node was selected
     * before the tool was activated, the Editor will issue this event just
     * after the init() event.
     */
    virtual void selected(osg::Node *) {}

    /**
     * The init() event. This one is called when the user selects the tool.
     * A lot of tools only implement this event because they don't modify
     * nodes, like for example the 'undo' and 'redo' tools.
     *
     * When this method is called, the editor attribute already has a valid
     * reference.
     */
    virtual void init() {} // Sets the tool to its initial state

    /**
     * Released event. Called when the tool is released, that is, when the
     * user selects another tool. NOTE: If the tool fails to initialize, then
     * the system won't call done(), keep that in mind when designing tools.
     */
    virtual void done() {}

    /**
     * Called by the Editor to inform about which is the editor. You probabily
     * don't need to catch this, as you will have protected access to the
     * reference in this superclass.
     */
    virtual void setEditor(Editor *e) { editor=e; }

    /**
     * Prototype pattern. This method allow to register an instance of the
     * tool as an 'example' in the Editor as a generic tool, and when the
     * user selects it, copy the concrete tool without the Editor knowing
     * the concrete class of the tool. So the editor works with a different
     * instance of the tool each time the user selects it, but the editor
     * doesn't know with class to instance... The advantage is simple, you
     * can add more and more tools without modifying the editor. And the
     * different GUI's (if some day are more than the GNOME one) can include
     * different tool sets and reduce the code size without modifying the
     * editor.
     */
    virtual Tool* clone()=0;
};

/* Proxy to allow tool plugins to register themselves */
template<class T>
class RegisterToolProxy {
public:
    RegisterToolProxy() {
	Editor::instance()->registerTool(new T);
    }
};


#endif

