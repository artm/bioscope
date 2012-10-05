#include "Bioscope.hpp"

struct Bioscope::Detail {
    static bool avInitialized;

    Detail() {
        if (!avInitialized) {
            av_register_all();
            avInitialized = true;
        }
    }
};

bool Bioscope::Detail::avInitialized = false;

Bioscope::Bioscope(const QString &, QObject *parent) :
    QObject(parent),
    m_detail( new Detail )
{
    throw UnsupportedFile();
}

Bioscope::~Bioscope()
{

}


bool Bioscope::supportedFile(const QString & path)
{
    try {
        Bioscope tmp(path);
        return true;
    } catch (IOError&) {
        return false;
    } catch (UnsupportedFile&) {
        return false;
    }
}
