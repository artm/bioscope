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
    m_bioscope->frame();
}

qint64 BioscopeDriver::time() const
{
    if (m_bioscope)
        return m_bioscope->time();
    else
        return 0;
}

void BioscopeDriver::seek(qint64 ms)
{
    if (m_bioscope)
        m_bioscope->seek(ms);
}

qint64 BioscopeDriver::duration() const
{
    if (m_bioscope)
        return m_bioscope->duration();
    else
        return 0;
int BioscopeDriver::width() const
{
    return m_bioscope ? m_bioscope->width() : 0;
}

int BioscopeDriver::height() const
{
    return m_bioscope ? m_bioscope->height() : 0;
}
