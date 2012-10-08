#include "BioscopeThread.hpp"
#include "Bioscope.hpp"

BioscopeThread::BioscopeThread(const QString& path, QObject *parent) :
    QThread(parent),
    m_bioscope( new Bioscope(path, this) ), m_seekReq(-1), m_time(0), m_quit(false)
{
    connect(m_bioscope, SIGNAL(streamEnd()), SIGNAL(streamEnd()));
}

void BioscopeThread::seek(qint64 ms)
{
    QMutexLocker locker(&m_mutex);
    m_seekReq = ms;
}

void BioscopeThread::addFrame(QImage * img)
{
    QMutexLocker locker(&m_mutex);
    m_imagePool.enqueue(img);
    m_imageInThePool.wakeOne();
}

void BioscopeThread::quit()
{
    QMutexLocker locker(&m_mutex);
    m_quit = true;
    // fool him into waking up, na na na na na
    m_imageInThePool.wakeOne();
}

void BioscopeThread::run()
{
    QMutexLocker locker(&m_mutex);

    forever {
        // here mutex is always locked

        // wait for buffers in the queue
        while(m_imagePool.isEmpty()) {
            m_imageInThePool.wait(&m_mutex, 500);
            if (m_quit) return;
        }

        // seek has precedence
        if (m_seekReq>=0) {
            m_bioscope->seek( m_seekReq );
            m_time = m_seekReq;
            m_seekReq = -1;
        }

        QImage * img = m_imagePool.dequeue();
        locker.unlock();

        qint64 time = m_bioscope->time();
        m_bioscope->frame( img );

        locker.relock();
        if (m_quit) return;

        if (m_seekReq < 0) {
            m_time = m_bioscope->time();
            emit frameRead( img, time );
        } else {
            // return the image to the queue, for simpler loop
            m_imagePool.enqueue(img);
        }
    }
}

