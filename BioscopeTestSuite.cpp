#include "BioscopeTestSuite.hpp"
#include "Bioscope.hpp"
#include "BioscopeDriver.hpp"

const int BioscopeTestSuite::MS_PER_FRAME = 1000/25;
const QRegExp BioscopeTestSuite::FRAME_NUM_RE(".*(\\d+)\\.png$");

BioscopeTestSuite::BioscopeTestSuite(const QStringList& args) :
    QObject(0),
    m_args(args)
{
    m_args.pop_front(); // don't care about program name
}

BioscopeTestSuite::~BioscopeTestSuite()
{

}

int BioscopeTestSuite::runTests()
{
    try {
        return QTest::qExec( this );
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
    QStringList files;
    foreach(QString arg, m_args) {
        if (!arg.startsWith("-"))
            files << arg;
    }
    QCOMPARE(files.length(), 2);
    m_goodFilename = files[0];
    m_badFilename = files[1];
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

    int lastFrame = -1; // this way we skip seek the first time
    foreach(QString refFile, dir.entryList(QStringList() << "*.png", QDir::NoFilter, QDir::Name)) {
        QImage refFrame, frame;
        QVERIFY( FRAME_NUM_RE.exactMatch( refFile ) );
        int frameNum = FRAME_NUM_RE.cap(1).toInt() - 1;
        QVERIFY( refFrame.load(dir.filePath( refFile) ));

        if ( frameNum != lastFrame+1)
            break;
        lastFrame = frameNum;

        frame = bios.frame();
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

    // load dumped frames one by one and compare to corresponding frames decoded by bios
    foreach(QString refFile, dir.entryList(QStringList() << "*.png", QDir::NoFilter, QDir::Name)) {
        QImage refFrame, frame;
        QVERIFY( FRAME_NUM_RE.exactMatch( refFile ) );
        int frameNum = FRAME_NUM_RE.cap(1).toInt() - 1;
        QVERIFY( refFrame.load(dir.filePath( refFile) ));

        bios.seek( frameNum * MS_PER_FRAME);
        QCOMPARE( bios.time(), (long long)(frameNum * MS_PER_FRAME) );
        frame = bios.frame();
        refFrame = refFrame.convertToFormat( frame.format() );

        QCOMPARE(frame, refFrame);
        QCOMPARE( bios.time(), (long long)((frameNum + 1) * MS_PER_FRAME) );
    }
}

void BioscopeTestSuite::testBioscopeDriver_openClose()
{
    BioscopeDriver driver;
    QVERIFY(driver.bioscope() == NULL);
    driver.open(m_goodFilename);
    QVERIFY(driver.bioscope() != NULL);
    driver.close();
    QVERIFY(driver.bioscope() == NULL);
}

void BioscopeTestSuite::testBioscopeDriver_play()
{
    BioscopeDriver driver;
    driver.open(m_goodFilename);

    driver.play();
    QTime stopwatch;
    stopwatch.start();

    QTest::qWait(1000);

    driver.stop();

    qint64 delta = abs( driver.bioscope()->time() - stopwatch.elapsed() );
    qint64 margin = 2 * MS_PER_FRAME;
    QVERIFY2( delta < margin,
              qPrintable( QString("Time difference (%1ms) above margin (%2ms)").arg(delta).arg(margin) ) );
}

