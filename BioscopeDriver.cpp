#include "BioscopeDriver.hpp"
#include "BioscopeThread.hpp"
#include "Bioscope.hpp"

struct BioscopeDriver::Detail {
    BioscopeThread * bioscopeThread;
    int timerId;
    State state;
    QVector<QImage> frames;
    struct TimedImage {
        qint64 ms;
        QImage * img;
        TimedImage(qint64 _ms, QImage * _img) : ms(_ms), img(_img) {}
    };
    QQueue< TimedImage > displayQueue;

    QTime referenceTimer;
    qint64 referencePlayTime;

    // cached metadata
    qint64 duration;
    int width, height;

    static const int TICK_INTERVAL = 1000/25;
    static const int BUFFER_SIZE = 2;

    Detail() :
        bioscopeThread(0),
        timerId(-1),
        state(BioscopeDriver::STOPPED),
        frames(BUFFER_SIZE),
        referencePlayTime( BioscopeDriver::DISCONTINUITY ),
        duration(0), width(0), height(0)
    {

    }
};

BioscopeDriver::BioscopeDriver(QObject *parent) :
    QObject(parent),
    m_detail( new Detail )
{
}

BioscopeDriver::~BioscopeDriver()
{
    close();
}

void BioscopeDriver::open(const QString &path)
{
    close();
    m_detail->bioscopeThread = new BioscopeThread(path, this);
    connect(m_detail->bioscopeThread, SIGNAL(frameRead(QImage*, qint64)), SLOT(enqueueFrame(QImage*, qint64)));
    connect(m_detail->bioscopeThread, SIGNAL(streamEnd()), SLOT(stop()));

    Bioscope* bios = m_detail->bioscopeThread->findChild<Bioscope*>();
    m_detail->duration = bios->duration();
    m_detail->width = bios->width();
    m_detail->height = bios->height();

    m_detail->bioscopeThread->start();
}

void BioscopeDriver::close()
{
    stop();
    if (m_detail->bioscopeThread) {
        m_detail->bioscopeThread->quit();
        m_detail->bioscopeThread->wait(1000);
        delete m_detail->bioscopeThread;
        m_detail->bioscopeThread = NULL;
    }
    m_detail->state = STOPPED;
    m_detail->duration = 0;
    m_detail->width = m_detail->height = 0;
}

void BioscopeDriver::play()
{
    m_detail->displayQueue.clear();
    m_detail->timerId = startTimer(Detail::TICK_INTERVAL);
    m_detail->state = PLAYING;

    m_detail->referencePlayTime = DISCONTINUITY;
    m_detail->referenceTimer.start();

    // schedule the frame reads
    for(int i=0; i<m_detail->frames.count(); ++i)
        m_detail->bioscopeThread->addFrame( & m_detail->frames[i] );
}

void BioscopeDriver::stop()
{
    if (m_detail->timerId >= 0) {
        killTimer(m_detail->timerId);
        m_detail->timerId = -1;
        m_detail->state = STOPPED;
    }
}

void BioscopeDriver::timerEvent(QTimerEvent *)
{
    if (m_detail->referencePlayTime != DISCONTINUITY) {
        m_detail->referencePlayTime += m_detail->referenceTimer.restart();
        // drop late frames
        while( ! m_detail->displayQueue.isEmpty()
               && m_detail->displayQueue.head().ms < m_detail->referencePlayTime - Detail::TICK_INTERVAL/2 ) {
            m_detail->bioscopeThread->addFrame( m_detail->displayQueue.dequeue().img );
        }
    }
    displayOldest();
}

void BioscopeDriver::displayOldest()
{
    if ( ! m_detail->displayQueue.isEmpty() ) {
        // display oldest available image and schedule read
        Detail::TimedImage ti = m_detail->displayQueue.dequeue();
        if (m_detail->referencePlayTime == DISCONTINUITY)
            m_detail->referencePlayTime = ti.ms;
        emit display( *(ti.img) );

        // after having displayed m_detail->frame is available for reading
        if (m_detail->state == PLAYING)
            m_detail->bioscopeThread->addFrame( ti.img );
        else
            m_detail->displayQueue.clear();
    }
}

qint64 BioscopeDriver::time() const
{
    return m_detail->referencePlayTime;
}

qint64 BioscopeDriver::duration() const
{
    return m_detail->duration;
}

int BioscopeDriver::width() const
{
    return m_detail->width;
}

int BioscopeDriver::height() const
{
    return m_detail->height;
}

void BioscopeDriver::seek(qint64 ms)
{
    m_detail->referencePlayTime = DISCONTINUITY;

    while( ! m_detail->displayQueue.isEmpty() ) {
        m_detail->bioscopeThread->addFrame( m_detail->displayQueue.dequeue().img );
    }
    m_detail->bioscopeThread->seek( ms );
    if (m_detail->state == STOPPED) {
        // give it just one frame
        m_detail->bioscopeThread->addFrame( & m_detail->frames[0] );
    }
}

void BioscopeDriver::enqueueFrame(QImage * img, qint64 ms)
{
    m_detail->displayQueue.enqueue( Detail::TimedImage(ms, img) );
    if (m_detail->state == STOPPED)
        displayOldest();
}

BioscopeDriver::State BioscopeDriver::state() const
{
    return m_detail->state;
}

