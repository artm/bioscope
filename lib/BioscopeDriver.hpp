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
    State state() const;
    qint64 duration() const;
    qint64 time() const;
    int width() const;
    int height() const;

signals:
    // emitted at playback rate
    void display(QImage img);

public slots:
    void play();
    void stop();
    void seek(qint64 ms);

    void enqueueFrame(QImage * img, qint64 ms);

private:
    void timerEvent(QTimerEvent *);

    void displayOldest();

    class Detail;
    QScopedPointer<Detail> m_detail;
};

#endif // BIOSCOPEDRIVER_HPP
