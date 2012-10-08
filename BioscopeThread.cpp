#include "BioscopeThread.hpp"
#include "Bioscope.hpp"

BioscopeThread::BioscopeThread(const QString& path, QObject *parent) :
    QThread(parent),
    m_bioscope( new Bioscope(path, this) ), m_seekReq(-1), m_time(0)
{
    connect(m_bioscope, SIGNAL(streamEnd()), SIGNAL(streamEnd()));
}

void BioscopeThread::seek(qint64 ms)
{
    // this way multiple seek requests between reads will get squashed into one
    m_seekReq = ms;
}

void BioscopeThread::frame(QImage &img)
{
    if (m_seekReq>=0) {
        m_bioscope->seek( m_seekReq );
        m_time = m_seekReq;
        m_seekReq = -1;
    }
    m_bioscope->frame( img );
    m_time = m_bioscope->time();
    emit frameRead( img );
}
