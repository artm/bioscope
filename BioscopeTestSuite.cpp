#include "BioscopeTestSuite.hpp"
#include "Bioscope.hpp"
#include "BioscopeDriver.hpp"
#include "PlayerShell.hpp"

const int BioscopeTestSuite::MS_PER_FRAME = 1000/25;
const QRegExp BioscopeTestSuite::FRAME_NUM_RE(".*(\\d+)\\.png$");

BioscopeTestSuite::BioscopeTestSuite(const QStringList& args) :
    QObject(0),
    m_args(args)
{
}

BioscopeTestSuite::~BioscopeTestSuite()
{

}

int BioscopeTestSuite::runTests()
{
    try {
        QStringList args = m_args;
        args.removeAll("--test");
        return QTest::qExec( this, args );
    } catch (Bioscope::Error& e) {
        qCritical() << qPrintable(e.message());
        return 1;
    } catch (...) {
        return 1;
    }
}

void BioscopeTestSuite::initTestCase()
{
}

void BioscopeTestSuite::cleanupTestCase()
{
}

void BioscopeTestSuite::init()
{
    m_goodFilename = "data/edje.mov";
    m_badFilename = "data/bad-movie.mov";
}

void BioscopeTestSuite::cleanup()
{
}

void BioscopeTestSuite::testBioscope_supportedFile()
{
    QVERIFY( !Bioscope::supportedFile("--this-file-should-never-exist--") );
    QVERIFY( !Bioscope::supportedFile(m_badFilename) );
    QVERIFY( Bioscope::supportedFile(m_goodFilename) );
}

void BioscopeTestSuite::testBioscope_metadata()
{
    Bioscope bios(m_goodFilename);
    QCOMPARE(bios.duration(), 196760LL);
    QCOMPARE(bios.width(), 320);
    QCOMPARE(bios.height(), 180);
}

void BioscopeTestSuite::testBioscope_rollRead()
{
    // reading without seeking
    Bioscope bios(m_goodFilename);
    QDir dir(QString(m_goodFilename).replace(QRegExp("[^\\.]+$"), "frames"));

    QImage frame;
    int lastFrame = -1; // this way we skip seek the first time
    foreach(QString refFile, dir.entryList(QStringList() << "*.png", QDir::NoFilter, QDir::Name)) {
        QImage refFrame;
        QVERIFY( FRAME_NUM_RE.exactMatch( refFile ) );
        int frameNum = FRAME_NUM_RE.cap(1).toInt() - 1;
        QVERIFY( refFrame.load(dir.filePath( refFile) ));

        if ( frameNum != lastFrame+1)
            break;
        lastFrame = frameNum;

        bios.frame( &frame );
        refFrame = refFrame.convertToFormat( frame.format() );

        QCOMPARE(frame, refFrame);
        QCOMPARE( bios.time(), (long long)((frameNum + 1) * MS_PER_FRAME) );
    }
}

void BioscopeTestSuite::testBioscope_seekRead()
{
    // reading with seeking (to each frame in turn)
    Bioscope bios(m_goodFilename);
    QDir dir(QString(m_goodFilename).replace(QRegExp("[^\\.]+$"), "frames"));

    QImage frame;
    // load dumped frames one by one and compare to corresponding frames decoded by bios
    foreach(QString refFile, dir.entryList(QStringList() << "*.png", QDir::NoFilter, QDir::Name)) {
        QImage refFrame;
        QVERIFY( FRAME_NUM_RE.exactMatch( refFile ) );
        int frameNum = FRAME_NUM_RE.cap(1).toInt() - 1;
        QVERIFY( refFrame.load(dir.filePath( refFile) ));

        bios.seek( frameNum * MS_PER_FRAME);
        QCOMPARE( bios.time(), (long long)(frameNum * MS_PER_FRAME) );
        bios.frame( &frame );
        refFrame = refFrame.convertToFormat( frame.format() );

        QCOMPARE(frame, refFrame);
        QCOMPARE( bios.time(), (long long)((frameNum + 1) * MS_PER_FRAME) );
    }
}

void BioscopeTestSuite::testBioscopeDriver_metadata()
{
    BioscopeDriver driver;
    driver.open(m_goodFilename);
    QCOMPARE(driver.duration(), 196760LL);
    QCOMPARE(driver.width(), 320);
    QCOMPARE(driver.height(), 180);
}

template <typename T>
void TestWithin( T actual, T expected, T margin,
                 const char * actualStr,
                 const char * expectedStr,
                 const char * file, int line
                 )
{
    T delta = abs(actual-expected);
    if ( delta > margin )
        QTest::qFail(qPrintable(
                  QString( "\n   The difference between [%1 = %2] and [%3 = %4] above margin [%5 > %6]")
                  .arg(actualStr).arg(actual).arg(expectedStr).arg(expected).arg(delta).arg(margin)), file, line );

}

#define TEST_IS_WITHIN(actual, expected, margin) TestWithin((actual), (expected), (margin), #actual, #expected, __FILE__, __LINE__)

void BioscopeTestSuite::testBioscopeDriver_play()
{
    BioscopeDriver driver;
    driver.open(m_goodFilename);

    QCOMPARE( driver.state(), BioscopeDriver::STOPPED );
    driver.play();
    m_stopwatch.start();
    QCOMPARE( driver.state(), BioscopeDriver::PLAYING );
    QTest::qWait(1000);
    driver.stop();
    int playTime = m_stopwatch.elapsed();
    QCOMPARE( driver.state(), BioscopeDriver::STOPPED );
    TEST_IS_WITHIN( (int)driver.time(), playTime, 2 * MS_PER_FRAME );
}

void BioscopeTestSuite::testBioscopeDriver_autoStop()
{
    BioscopeDriver driver;
    driver.open(m_goodFilename);

    // jump at half second before end
    driver.seek( driver.duration() - 500 );

    QCOMPARE( driver.state(), BioscopeDriver::STOPPED );
    driver.play();
    QCOMPARE( driver.state(), BioscopeDriver::PLAYING );

    QTest::qWait(600);
    QCOMPARE( driver.state(), BioscopeDriver::STOPPED );
}

void BioscopeTestSuite::testBioscopeDriver_seekWhileRolling()
{
    BioscopeDriver driver;
    driver.open(m_goodFilename);
    driver.play();

    int waitTime = 200, seekTime = 5000;

    QTest::qWait(waitTime);
    driver.seek(seekTime);
    QTest::qWait(waitTime);
    driver.stop();
    TEST_IS_WITHIN( (int)driver.time(), seekTime + waitTime, 2 * MS_PER_FRAME );

    // seek backwards
    seekTime = driver.time() / 2;
    driver.play();

    QTest::qWait(waitTime);
    driver.seek(seekTime);
    QTest::qWait(waitTime);
    driver.stop();
    TEST_IS_WITHIN( (int)driver.time(), seekTime + waitTime, 2 * MS_PER_FRAME );

}

void BioscopeTestSuite::testBioscopeDriver_seekWhileNotRolling()
{
    BioscopeDriver driver;
    QSignalSpy spy(&driver, SIGNAL(display(QImage)));

    driver.open(m_goodFilename);

    QCOMPARE( spy.count(), 0);

    driver.seek(0);
    QTest::qWait(50);
    QCOMPARE( spy.count(), 1);

    driver.seek(5000);
    QTest::qWait(50);
    QCOMPARE( spy.count(), 2);

    driver.seek(2500);
    QTest::qWait(50);
    QCOMPARE( spy.count(), 3);
}

void BioscopeTestSuite::testBioscopeGUI_timing()
{
    PlayerShell w;
    w.open(m_goodFilename);
    w.showMaximized();

    BioscopeDriver * driver = w.findChild<BioscopeDriver*>("bioscope");

    QWidget * play = w.findChild<QWidget*>("playBut");
    QWidget * stop = w.findChild<QWidget*>("stopBut");

    QTest::mouseClick(play, Qt::LeftButton);
    m_stopwatch.start();
    QTest::qWait(5000);
    QTest::mouseClick(stop, Qt::LeftButton);

    int playTime = m_stopwatch.elapsed();
    QCOMPARE( driver->state(), BioscopeDriver::STOPPED );
    TEST_IS_WITHIN( (int)driver->time(), playTime, 2 * MS_PER_FRAME );
}

