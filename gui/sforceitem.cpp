
#include "sforceitem.h"

#include <qstring.h>

SForceItem::SForceItem(SForce * force, QListViewItem* parent)
        : SGuiTreeViewItem(SGuiTreeViewItem::FORCE, parent)
{
    mForce = force;
    this->setText(0, QString(mForce->getName()));
    setRenameEnabled(0, true);
    setDragEnabled(true);
}

void SForceItem::okRename(int col)
{
    if(col ==0)
    {
        QListViewItem::okRename(col);
    
        mForce->setName(this->text(0));
    }
}

