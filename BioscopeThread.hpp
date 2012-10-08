#ifndef BIOSCOPETHREAD_HPP
#define BIOSCOPETHREAD_HPP

#include "stable.h"

class Bioscope;
/*

  We use the main loop / asynchronous signals to implement the queue...

  Someone has to allocate several images, schedule initial queue of
  reads and make sure they are displayed at the right rate. By default
  that's BioscopeDriver's job.

 */
class BioscopeThread : public QThread
{
    Q_OBJECT
public:
    BioscopeThread(const QString& path, QObject *parent = 0);

    // thread safe
    qint64 time() const { return m_time; }
signals:
    void streamEnd();
    void frameRead(QImage frame);
public slots:
    void seek(qint64 ms); // schedule seek
    void frame(QImage& img); // schedule frame reading / decoding
    // use quit() to schedule exit
private:
    Bioscope * m_bioscope;
    qint64 m_seekReq, m_time;
};

#endif // BIOSCOPETHREAD_HPP
