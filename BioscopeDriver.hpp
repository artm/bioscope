#ifndef BIOSCOPEDRIVER_HPP
#define BIOSCOPEDRIVER_HPP

class Bioscope;

// encapsulate Bioscope in the separate thread and provide playback API
class BioscopeDriver : public QObject
{
    Q_OBJECT
public:
    BioscopeDriver(QObject *parent = 0);
    ~BioscopeDriver();

    Bioscope * bioscope() { return m_bioscope; }
    void open(const QString& path);
    void close();
signals:

public slots:
    void play();
    void stop();

private:
    Bioscope * m_bioscope;
};

#endif // BIOSCOPEDRIVER_HPP
