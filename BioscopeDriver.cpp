#include "BioscopeDriver.hpp"
#include "Bioscope.hpp"

const int BioscopeDriver::TICK_INTERVAL = 40;

BioscopeDriver::BioscopeDriver(QObject *parent) :
    QObject(parent),
    m_bioscope(0),
    m_timerId(-1),
    m_state(STOPPED)
{
}

BioscopeDriver::~BioscopeDriver()
{
    close();
}

void BioscopeDriver::open(const QString &path)
{
    close();
    m_bioscope = new Bioscope(path, this);
    connect(m_bioscope, SIGNAL(streamEnd()), SLOT(stop()));
}

void BioscopeDriver::close()
{
    stop();
    delete m_bioscope;
    m_bioscope = NULL;
}

void BioscopeDriver::play()
{
    m_timerId = startTimer(TICK_INTERVAL);
    m_state = PLAYING;
}

void BioscopeDriver::stop()
{
    if (m_timerId >= 0) {
        killTimer(m_timerId);
        m_timerId = -1;
        m_state = STOPPED;
    }
}

void BioscopeDriver::timerEvent(QTimerEvent *)
{
    Q_ASSERT(m_bioscope);
    emitCurrentFrame();
}

qint64 BioscopeDriver::time() const
{
    return m_bioscope ? m_bioscope->time() : 0;
}

void BioscopeDriver::seek(qint64 ms)
{
    if (m_bioscope) {
        m_bioscope->seek(ms);
        emitCurrentFrame();
    }
}

qint64 BioscopeDriver::duration() const
{
    return m_bioscope ? m_bioscope->duration() : 0;
}

void BioscopeDriver::emitCurrentFrame()
{
    qint64 time = m_bioscope->time();

    m_bioscope->frame( m_frame );
    emit timedFrame( time , m_frame );
}

int BioscopeDriver::width() const
{
    return m_bioscope ? m_bioscope->width() : 0;
}

int BioscopeDriver::height() const
{
    return m_bioscope ? m_bioscope->height() : 0;
}
