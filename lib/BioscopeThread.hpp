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

    qint64 time() const { return m_time; }
    void quit();
    void seek(qint64 ms); // schedule seek
    void addFrame(QImage * img); // schedule frame reading / decoding

signals:
    void streamEnd();
    void frameRead(QImage * frame, qint64 ms);
public slots:

private slots:
    void run();

private:

    Bioscope * m_bioscope;
    qint64 m_seekReq, m_time;
    QQueue< QImage * > m_imagePool;

    QMutex m_mutex;
    QWaitCondition m_imageInThePool;
    bool m_quit;
};

#endif // BIOSCOPETHREAD_HPP
