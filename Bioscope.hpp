#ifndef BIOSCOPE_HPP
#define BIOSCOPE_HPP

/// Access to framebased video files
class Bioscope : public QObject
{
    Q_OBJECT
public:
    class Exception {};
    class IOError : public Exception {};
    class UnsupportedFile : public Exception {};

    Bioscope(const QString& path, QObject *parent = 0);
    ~Bioscope();

    static bool supportedFile(const QString& path);
signals:

public slots:

private:
    class Detail;
    QScopedPointer<Detail> m_detail;
};

#endif // BIOSCOPE_HPP
