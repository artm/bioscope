#include "PlayerShell.hpp"
#include "Bioscope.hpp"
#include "BioscopeDriver.hpp"

struct PlayerShell::Detail {
    BioscopeDriver * driver;
    QGraphicsView * canvas;
    QGraphicsPixmapItem * pixItem;
    QAbstractSlider * slider;
};

PlayerShell::PlayerShell(QWidget *parent)
    : QMainWindow(parent),
      m_detail(new Detail)
{
    setupDriver();
    setupUi();
    setupActions();

    QMetaObject::connectSlotsByName(this);
}

PlayerShell::~PlayerShell()
{

}

void PlayerShell::setupDriver()
{
    m_detail->driver = new BioscopeDriver(this);
    m_detail->driver->setObjectName("bioscope");
}

void PlayerShell::setupUi()
{
    QUiLoader loader;
    QFile file(":/forms/player.ui");
    file.open(QFile::ReadOnly);
    setCentralWidget( loader.load(&file, this) );
    file.close();

    m_detail->canvas = findChild<QGraphicsView*>("canvas");
    m_detail->canvas->setScene( new QGraphicsScene( this ) );
    m_detail->pixItem = new QGraphicsPixmapItem(0, m_detail->canvas->scene());

    m_detail->slider = findChild<QAbstractSlider *>("slider");

}

void PlayerShell::setupActions()
{
    QMenuBar * menu = new QMenuBar(0);
    QMenu * submenu = menu->addMenu("&File");

    submenu->addAction("Open", this, SLOT(open()), QKeySequence::Open);
}

void PlayerShell::open()
{
    open( QFileDialog::getOpenFileName(this, "Open video file",
                                       QDesktopServices::storageLocation(QDesktopServices::MoviesLocation)) );

}

void PlayerShell::open(const QString& path)
{
    if (Bioscope::supportedFile(path)) {
        m_detail->driver->open(path);
        m_detail->driver->seek(0);
        m_detail->canvas->setMinimumSize( m_detail->driver->width(), m_detail->driver->height() );
    }
}

void PlayerShell::on_playBut_clicked()
{
    m_detail->driver->play();
}

void PlayerShell::on_stopBut_clicked()
{
    m_detail->driver->stop();
}

void PlayerShell::on_slider_valueChanged(int value)
{
    qDebug() << "sliding";
    qint64 pos =  m_detail->driver->duration() * value / m_detail->slider->maximum();
    m_detail->driver->seek( pos );
}

void PlayerShell::on_bioscope_timedFrame(qint64 ms, QImage frame)
{
    m_detail->slider->blockSignals(true);
    m_detail->slider->setValue( m_detail->slider->maximum() * ms / m_detail->driver->duration() );
    m_detail->slider->blockSignals(false);

    m_detail->pixItem->setPixmap( QPixmap::fromImage(frame) );
    m_detail->canvas->fitInView( m_detail->pixItem, Qt::KeepAspectRatio );
}

void PlayerShell::resizeEvent(QResizeEvent *)
{
    resizeUI();
}

void PlayerShell::resizeUI()
{
    m_detail->canvas->fitInView( m_detail->pixItem, Qt::KeepAspectRatio );
    /*
    m_detail->slider->setMaximum( m_detail->slider->width() );
    if (m_detail->driver->duration())
        m_detail->slider->setValue( m_detail->slider->maximum() * m_detail->driver->time() / m_detail->driver->duration() );
    */
}

