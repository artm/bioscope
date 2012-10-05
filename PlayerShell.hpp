#ifndef PLAYERSHELL_HPP
#define PLAYERSHELL_HPP

#include <QtGui/QMainWindow>

class PlayerShell : public QMainWindow
{
    Q_OBJECT

public:
    PlayerShell(QWidget *parent = 0);
    ~PlayerShell();

public slots:
    void open();

private:
    void setupUi();
    void setupActions();
};

#endif // PLAYERSHELL_HPP
