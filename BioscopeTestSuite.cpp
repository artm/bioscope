#include "BioscopeTestSuite.hpp"

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
    QTest::qExec( this );

    return 1;
}

void BioscopeTestSuite::initTestCase()
{
    QStringList files;
    m_args.pop_front(); // don't care about program name
    foreach(QString arg, m_args) {
        if (!arg.startsWith("-"))
            files << arg;
    }
    QCOMPARE(files.length(), 2);
    m_goodFilename = files[0];
    m_badFilename = files[1];
}

void BioscopeTestSuite::cleanupTestCase()
{
}

void BioscopeTestSuite::init()
{
}

void BioscopeTestSuite::cleanup()
{
}

# include "Bioscope.hpp"
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

void BioscopeTestSuite::testBioscope_seekRead()
{
    Bioscope bios(m_goodFilename);
    QString framesDir = m_goodFilename.replace(QRegExp("[^\\.]+$"), "frames");
    QImage refFrame, frame;

    // first test frame loading without seeking
    QVERIFY( refFrame.load( framesDir + "/00000001.png" ) );
    frame = bios.frame();
    refFrame = refFrame.convertToFormat( frame.format() );
    QCOMPARE(frame, refFrame);

    // load dumped frames one by one and compare to corresponding frames decoded by bios
    QRegExp frameNumRe(".*(\\d+)\\.png$");
    qint64 msPerFrame = 1000 / 25;
    QDir dir(framesDir);
    foreach(QString refFile, dir.entryList(QStringList() << "*.png", QDir::NoFilter, QDir::Name)) {
        QVERIFY( frameNumRe.exactMatch( refFile ) );
        QVERIFY( refFrame.load(dir.filePath( refFile) ));
        int frameNum = frameNumRe.cap(1).toInt() - 1;

        bios.seek( frameNum * msPerFrame);
        frame = bios.frame();
        refFrame = refFrame.convertToFormat( frame.format() );

        QCOMPARE(frame, refFrame);
    }
}
