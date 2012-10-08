#include "BioscopeDriver.hpp"
#include "BioscopeThread.hpp"
#include "Bioscope.hpp"

const int BioscopeDriver::TICK_INTERVAL = 40;

BioscopeDriver::BioscopeDriver(QObject *parent) :
    QObject(parent),
    m_bioscopeThread(0),
    m_timerId(-1),
    m_state(STOPPED),
    m_duration(0), m_width(0), m_height(0)
{
}

BioscopeDriver::~BioscopeDriver()
{
    close();
}

void BioscopeDriver::open(const QString &path)
{
    close();
    m_bioscopeThread = new BioscopeThread(path, this);
    connect(this, SIGNAL(scheduleSeek(qint64)), m_bioscopeThread, SLOT(seek(qint64)));
    connect(this, SIGNAL(scheduleFrame(QImage&)), m_bioscopeThread, SLOT(frame(QImage&)));
    connect(m_bioscopeThread, SIGNAL(streamEnd()), SLOT(stop()));

    Bioscope* bios = m_bioscopeThread->findChild<Bioscope*>();
    m_duration = bios->duration();
    m_width = bios->width();
    m_height = bios->height();

    m_bioscopeThread->start();
}

void BioscopeDriver::close()
{
    stop();
    if (m_bioscopeThread) {
        m_bioscopeThread->quit();
        m_bioscopeThread->wait(1000);
        delete m_bioscopeThread;
        m_bioscopeThread = NULL;
    }
    m_state = STOPPED;
    m_duration = 0;
    m_width = m_height = 0;
}

void BioscopeDriver::play()
{
    m_timerId = startTimer(TICK_INTERVAL);
    m_state = PLAYING;
    // schedule the first frame
    emit scheduleFrame(m_frame);
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
    // display oldest available image and schedule read
    emit display(m_frame);
    // after having displayed m_frame is available for reading
    emit scheduleFrame(m_frame);
}

qint64 BioscopeDriver::time() const
{
    return m_bioscopeThread ? m_bioscopeThread->time() : 0;
}

qint64 BioscopeDriver::duration() const
{
    return m_duration;
}

int BioscopeDriver::width() const
{
    return m_width;
}

int BioscopeDriver::height() const
{
    return m_height;
}

void BioscopeDriver::seek(qint64 ms)
{
    emit scheduleSeek(ms);
}
