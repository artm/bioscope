#ifndef BIOSCOPEDRIVER_HPP
#define BIOSCOPEDRIVER_HPP

#include "stable.h"

class Bioscope;

// encapsulate Bioscope in the separate thread and provide playback API
class BioscopeDriver : public QObject
{
    Q_OBJECT
public:
    BioscopeDriver(QObject *parent = 0);
    ~BioscopeDriver();

    Bioscope * bioscope() { return m_bioscope; }
    void open(const QString& path);
    void close();
signals:

public slots:
    void play();
    void stop();

private:
    void timerEvent(QTimerEvent *);

    Bioscope * m_bioscope;
    int m_timerId;

    static const int TICK_INTERVAL;
};

#endif // BIOSCOPEDRIVER_HPP
