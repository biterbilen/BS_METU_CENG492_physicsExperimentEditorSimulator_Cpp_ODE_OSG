#include "sjointitem.h"

#include <qstring.h>

SJointItem::SJointItem(SJoint * joint, QListViewItem* parent)
        : SGuiTreeViewItem(SGuiTreeViewItem::JOINT, parent)
{
    mJoint = joint;
    this->setText(0, QString(mJoint->getName()));
    setRenameEnabled(0, true);
    setDragEnabled(true);
}

void SJointItem::okRename(int col)
{
    if(col ==0)
    {
        QListViewItem::okRename(col);
    
        mJoint->setName(this->text(0));
    }
}
