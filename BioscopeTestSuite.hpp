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
    void testBioscope_seekRead();

private:
    QStringList m_args;
    QString m_goodFilename, m_badFilename;
};

#endif // BIOSCOPETESTSUITE_HPP
