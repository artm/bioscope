#ifndef BIOSCOPETESTSUITE_HPP
#define BIOSCOPETESTSUITE_HPP

#include "stable.h"

class BioscopeTestSuite : public QObject
{
    Q_OBJECT
public:
    BioscopeTestSuite(const QStringList& args);
    ~BioscopeTestSuite();
    int runTests();

private slots:
    void initTestCase(); // will be called before the first testfunction is executed.
    void cleanupTestCase(); // will be called after the last testfunction was executed.
    void init(); // will be called before each testfunction is executed.
    void cleanup(); // will be called after every testfunction.

    void testBioscope_supportedFile();
    void testBioscope_metadata();
    void testBioscope_rollRead();
    void testBioscope_seekRead();

    void testBioscopeDriver_metadata();
    void testBioscopeDriver_play();
    void testBioscopeDriver_autoStop();
    void testBioscopeDriver_seekWhileRolling();
    void testBioscopeDriver_seekWhileNotRolling();

    void testBioscopeGUI_timing();

private:
    QStringList m_args;
    QString m_goodFilename, m_badFilename;

    struct MetaData {
        qint64 duration;
        int width, height;
    };
    MetaData m_goodMetaData;

    QTime m_stopwatch;

    static const QRegExp FRAME_NUM_RE;
    static const int MS_PER_FRAME;
};

#endif // BIOSCOPETESTSUITE_HPP
