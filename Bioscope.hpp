#ifndef BIOSCOPE_HPP
#define BIOSCOPE_HPP

#include "stable.h"

/// Access to framebased video files
class Bioscope : public QObject
{
    Q_OBJECT
public:
    struct Error {
        explicit Error(const QString& message) : m_message(message) {}
        QString message() { return m_message; }
    private:
        QString m_message;
    };
    struct IOError : public Error {
        explicit IOError(const QString& path) : Error(QString("IOError(%1)").arg(path)) {}
    };
    struct NoFile : public Error {
        explicit NoFile(const QString& path) : Error(QString("File %1 doesn't exist").arg(path)) {}
    };
    struct UnsupportedFile : public Error {
        explicit UnsupportedFile(const QString& path) : Error(QString("Unsupported file: %1").arg(path)) {}
    };
    struct AVError : public Error {
        explicit AVError(const QString& path, int av_err);
    };

    Bioscope(const QString& path, QObject *parent = 0);
    ~Bioscope();

    qint64 duration() const;
    int width() const;
    int height() const;
    void frame(QImage * frame);
    void seek( qint64 ms );
    qint64 time();

    static bool supportedFile(const QString& path);
signals:

    void streamEnd();

public slots:

private:
    class Detail;
    QScopedPointer<Detail> m_detail;
};

#endif // BIOSCOPE_HPP
