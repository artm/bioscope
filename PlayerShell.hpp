#ifndef PLAYERSHELL_HPP
#define PLAYERSHELL_HPP

#include "stable.h"

class BioscopeDriver;

class PlayerShell : public QMainWindow
{
    Q_OBJECT

public:
    PlayerShell(QWidget *parent = 0);
    ~PlayerShell();


public slots:
    void open();
    void open(const QString& path);
    void on_playBut_clicked();
    void on_stopBut_clicked();
    void on_slider_valueChanged(int value);

    void on_bioscope_display(QImage frame);

private:
    void resizeEvent(QResizeEvent *);

    void setupDriver();
    void setupUi();
    void setupActions();

    void resizeUI();

    class Detail;
    QScopedPointer<Detail> m_detail;
};

#endif // PLAYERSHELL_HPP
