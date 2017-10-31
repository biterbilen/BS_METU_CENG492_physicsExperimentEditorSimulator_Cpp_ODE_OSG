#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "history.h"
#include <osg/Referenced>

/** @class Command
 * @brief Represents a *simple* and *atomic* action over the scenegraph.
 *
 * All the actions performed over the OSG scenegraph (the one loaded by the
 * user, not the one handled by osgedit with grid, selection, etc) must be
 * encapsulated in a Command, and executed as a Command. There are two
 * reasons:
 *
 * - The first and more important is that Commands, can be undo() and redo()
 *   as they will be contained in a history. So if you perform any operation
 *   directly over the scenegraph, user won't be able to undo it if he/she
 *   made a mistake.
 *
 * - The second is that commands allow (although it's unimplemented yet)
 *   to create macros or even scripts for batch processing. So if users need
 *   to perform the same complex operation over 50 files or over 50 nodes
 *   in a single file, it's more comfortable to do it once, save the history
 *   in some way, and perform this bunch of commands over the rest of items
 *   with much less clicks.
 *
 * So, please, use commands! users will thank you ;-)
 */
class Command: public osg::Referenced {
protected:
    /**
     * This variable tells if this command requires an update on openGL views
     */
    bool mNeedsRedraw;
    
    /**
     * Called by some class when the command must be executed.
     * Well, in fact they will call execute, that adds this to the history
     * and then executes...
     */
    virtual void realExecute()=0;
public:
    Command() { mNeedsRedraw = false; }
    virtual ~Command() {}

    bool needsRedraw() { return mNeedsRedraw; }
    /**
     * Wrapper around the real execute method, that adds this command to the
     * history. This way, any command that is executed in the editor will
     * go transparently to the history.
     */
    void execute(bool addToHistory=true) {
	realExecute();
	if (addToHistory) History::addCommand(this);
    }

    /**
     * Called by the history when some class wants to un-execute the last
     * command. It's important to implement this method. Keep information
     * needed to return to the previous state.
     */
    virtual void undo()=0;
};

#endif
