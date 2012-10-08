#ifndef BIOSCOPEDRIVER_HPP
#define BIOSCOPEDRIVER_HPP

#include "stable.h"

class Bioscope;

// encapsulate Bioscope in the separate thread and provide playback API
class BioscopeDriver : public QObject
{
    Q_OBJECT
public:
    enum State {
        STOPPED,
        PLAYING
    };
    BioscopeDriver(QObject *parent = 0);
    ~BioscopeDriver();

    void open(const QString& path);
    void close();
    State state() const { return m_state; }
    qint64 duration() const;
    qint64 time() const;
    int width() const;
    int height() const;

signals:
    void timedFrame(qint64 ms, QImage frame);

public slots:
    void play();
    void stop();
    void seek(qint64 ms);

private:
    void timerEvent(QTimerEvent *);

    void emitCurrentFrame();

    Bioscope * m_bioscope;
    int m_timerId;
    State m_state;
    QImage m_frame;

    static const int TICK_INTERVAL;
};

#endif // BIOSCOPEDRIVER_HPP
