#include "BioscopeDriver.hpp"
#include "Bioscope.hpp"

BioscopeDriver::BioscopeDriver(QObject *parent) :
    QObject(parent),
    m_bioscope(0)
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
}

void BioscopeDriver::close()
{
    delete m_bioscope;
    m_bioscope = NULL;
}

void BioscopeDriver::play()
{
}

void BioscopeDriver::stop()
{
}
