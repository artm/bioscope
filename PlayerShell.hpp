#ifndef PLAYERSHELL_HPP
#define PLAYERSHELL_HPP

#include <QtGui/QMainWindow>

class PlayerShell : public QMainWindow
{
    Q_OBJECT

public:
    PlayerShell(QWidget *parent = 0);
    ~PlayerShell();
};

#endif // PLAYERSHELL_HPP
