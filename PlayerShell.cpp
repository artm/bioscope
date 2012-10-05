#include "stable.h"
#include "PlayerShell.hpp"

PlayerShell::PlayerShell(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    setupActions();

    QMetaObject::connectSlotsByName(this);
}

PlayerShell::~PlayerShell()
{

}

void PlayerShell::setupActions()
{
    QMenuBar * menu = new QMenuBar(0);
    QMenu * submenu = menu->addMenu("&File");

    submenu->addAction("Open", this, SLOT(open()), QKeySequence::Open);
}

void PlayerShell::setupUi()
{
    QUiLoader loader;
    QFile file(":/forms/player.ui");
    file.open(QFile::ReadOnly);
    setCentralWidget( loader.load(&file, this) );
    file.close();
}

void PlayerShell::open()
{
    QString path = QFileDialog::getOpenFileName(this, "Open video file",
                                                QDesktopServices::storageLocation(QDesktopServices::MoviesLocation));


}
