#include "Bioscope.hpp"

struct Bioscope::Detail {
    static bool avInitialized;
    AVFormatContext * formatContext;

    Detail() {
        if (!avInitialized) {
            av_register_all();
            avInitialized = true;
        }
    }
};

class LibAvLogger {
public:
    class Error {
    public:
        Error(const QString& msg) : m_message(msg) {}
        QString message() { return m_message; }
    private:
        QString m_message;
    };

    void operator<<(int av_err) {
        if (av_err) {
            char buff[1024];
            QString msg;

            if (av_strerror(av_err, buff, sizeof(buff)) == 0)
                qDebug() << (msg = buff);
            else
                qDebug() << (msg = "Unknown problem with ffmpeg libraries");

            throw Error(msg);
        }
    }
};

bool Bioscope::Detail::avInitialized = false;

Bioscope::Bioscope(const QString & _path, QObject *parent) :
    QObject(parent),
    m_detail( new Detail )
{
    QString path = QFileInfo(_path).canonicalFilePath();

    try {
        LibAvLogger() << av_open_input_file( &m_detail->formatContext,
                                             qPrintable(path), NULL, 0, NULL);
        LibAvLogger() << av_find_stream_info( m_detail->formatContext );
    } catch (LibAvLogger::Error) {
        throw IOError();
    }

}

Bioscope::~Bioscope()
{

}


bool Bioscope::supportedFile(const QString & path)
{
    try {
        Bioscope tmp(path);
        return true;
    } catch (...) {
        return false;
    }
}
