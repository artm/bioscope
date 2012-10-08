#include "BioscopeDriver.hpp"
#include "BioscopeThread.hpp"
#include "Bioscope.hpp"

const int BioscopeDriver::TICK_INTERVAL = 40;
const int BioscopeDriver::BUFFER_SIZE = 2;

BioscopeDriver::BioscopeDriver(QObject *parent) :
    QObject(parent),
    m_bioscopeThread(0),
    m_timerId(-1),
    m_state(STOPPED),
    m_frames(BUFFER_SIZE),
    m_displayTime(0),
    m_referencePlayTime(0),
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
    connect(m_bioscopeThread, SIGNAL(frameRead(QImage&, qint64)), SLOT(enqueueFrame(QImage&, qint64)));
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
    m_displayTime = 0;
    m_duration = 0;
    m_width = m_height = 0;
}

void BioscopeDriver::play()
{
    m_displayQueue.clear();
    m_timerId = startTimer(TICK_INTERVAL);
    m_state = PLAYING;

    m_referencePlayTime = m_displayTime = m_bioscopeThread->time();
    m_referenceTimer.start();

    // schedule the frame reads
    for(int i=0; i<m_frames.count(); ++i)
        emit scheduleFrame( m_frames[i] );
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
    m_referencePlayTime += m_referenceTimer.restart();
    // drop late frames
    while( ! m_displayQueue.isEmpty() && m_displayQueue.head().ms < m_referencePlayTime+20 ) {
        emit scheduleFrame( *(m_displayQueue.dequeue().img) );
    }

    if ( ! m_displayQueue.isEmpty() ) {
        // display oldest available image and schedule read
        TimedImage ti = m_displayQueue.dequeue();
        emit display( *(ti.img) );
        // after having displayed m_frame is available for reading
        emit scheduleFrame( *(ti.img) );
    }
}

qint64 BioscopeDriver::time() const
{
    return m_displayTime;
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
    emit scheduleSeek(m_referencePlayTime = ms);
    m_referenceTimer.start();
}

void BioscopeDriver::enqueueFrame(QImage &img, qint64 ms)
{
    m_displayQueue.enqueue(TimedImage(ms, &img));
    m_displayTime = ms;
}
