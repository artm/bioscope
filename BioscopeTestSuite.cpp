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
void BioscopeTestSuite::testBioscope()
{
    QVERIFY( !Bioscope::supportedFile("--this-file-should-never-exist--") );
    QVERIFY( !Bioscope::supportedFile(m_badFilename) );
    QVERIFY( Bioscope::supportedFile(m_goodFilename) );
}
