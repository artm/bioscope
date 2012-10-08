#ifndef BIOSCOPEDRIVER_HPP
#define BIOSCOPEDRIVER_HPP

#include "stable.h"

class BioscopeThread;

// encapsulate Bioscope in the separate thread and provide playback API
class BioscopeDriver : public QObject
{
    Q_OBJECT
public:
    enum State {
        STOPPED,
        PLAYING
    };
    // time value at discontinuities
    static const qint64 DISCONTINUITY = -1;

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
    // to be connected to m_bioscopeThread...
    void scheduleSeek(qint64 ms);
    void scheduleFrame(QImage& img);

    // emitted at playback rate
    void display(QImage img);

public slots:
    void play();
    void stop();
    void seek(qint64 ms);

    void enqueueFrame(QImage& img, qint64 ms);

private:
    void timerEvent(QTimerEvent *);

    BioscopeThread * m_bioscopeThread;
    int m_timerId;
    State m_state;
    QVector<QImage> m_frames;
    struct TimedImage {
        qint64 ms;
        QImage * img;
        TimedImage(qint64 _ms, QImage * _img) : ms(_ms), img(_img) {}
    };
    QQueue< TimedImage > m_displayQueue;

    QTime m_referenceTimer;
    qint64 m_referencePlayTime;

    // cached metadata
    qint64 m_duration;
    int m_width, m_height;

    static const int TICK_INTERVAL;
    static const int BUFFER_SIZE;
};

#endif // BIOSCOPEDRIVER_HPP
