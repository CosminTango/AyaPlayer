#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDir>
#include <QList>
#include <QMediaContent>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //=================================Initializations===========================//
    // Initialize the player itself
    ayaPlayer = new QMediaPlayer(this);
    // Initialize the playlist
    ayaPlaylist = new QMediaPlaylist(this);
    // Set the playlist to the player
    ayaPlayer->setPlaylist(ayaPlaylist);
    //===========================================================================//

    connect(ayaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionChanged);
    connect(ayaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationChanged);
    connect(ayaPlayer, &QMediaPlayer::stateChanged, this, &MainWindow::on_stateChanged);
    connect(ayaPlaylist, &QMediaPlaylist::currentMediaChanged, this, &MainWindow::on_currentMediaChanged);
    qDebug() << ayaPlayer->errorString();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sliderSeek_sliderMoved(int position)
{
    ayaPlayer->setPosition(position);
}

void MainWindow::on_sliderVolume_sliderMoved(int position)
{
    ayaPlayer->setVolume(position);
}

void MainWindow::on_startButton_clicked()
{
    ayaPlaylist->setCurrentIndex(0);
    ayaPlayer->play();
}

void MainWindow::on_stopButton_clicked()
{
    stopButtonPressed = true;
    ayaPlayer->stop();
}

void MainWindow::on_positionChanged(qint64 position)
{
    ui->sliderSeek->setValue(position);
}

void MainWindow::on_durationChanged(qint64 position)
{
    ui->sliderSeek->setMaximum(position);    
}

void MainWindow::on_stateChanged()
{
    if(ayaPlayer->state() == QMediaPlayer::PlayingState)
    {
        stopButtonPressed = false;
    }
    // If the user wants to repeat
    if(ui->radioRepeat->isChecked())
    {
        // Has the media reached the end?
        if(ayaPlayer->state() == QMediaPlayer::StoppedState)
        {
            if(stopButtonPressed==false)
            {
                // Yes, so repeat the file
                ayaPlayer->setPosition(0);
                ayaPlayer->play();
                qDebug() << stopButtonPressed;
                stopButtonPressed = false;
            }
        }
    }
}

void MainWindow::on_currentMediaChanged()
{
    ui->listSongs->setCurrentRow(ayaPlaylist->currentIndex());
}

void MainWindow::on_mediaButton_clicked()
{
    QList<QMediaContent> content;
    QString directory = QFileDialog::getExistingDirectory(this,tr("Select folder to import MP3s from"));
    if(directory.isEmpty())
        return;
    QDir dir(directory);
    QStringList files = dir.entryList(QStringList() << "*.mp3",QDir::Files);

    for(const QString& f:files)
    {
        content.push_back(QUrl::fromLocalFile(dir.path()+"/" + f));
        QFileInfo fi(f);
        ui->listSongs->addItem(fi.fileName());
    }
    ayaPlaylist->addMedia(content);
    ui->listSongs->setCurrentRow(ayaPlaylist->currentIndex() != -1? ayaPlaylist->currentIndex():0);
}

void MainWindow::on_listSongs_doubleClicked(const QModelIndex &index)
{
    ayaPlayer->stop();
    ayaPlaylist->setCurrentIndex(ui->listSongs->currentRow());
    qDebug() << ui->listSongs->currentRow() << " cuurrent row";
    qDebug() << ayaPlaylist->currentIndex();
    ayaPlayer->play();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        qDebug() << ui->listSongs->currentItem();
        delete ui->listSongs->currentItem();
    }
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        ayaPlayer->stop();
        ayaPlaylist->setCurrentIndex(ui->listSongs->currentRow());
        ayaPlayer->play();
    }
    if(event->key() == Qt::UpArrow)
    {
        ui->listSongs->setCurrentRow(ayaPlaylist->currentIndex() >-1? ayaPlaylist->currentIndex()-1:0);
    }
    if(event->key() == Qt::DownArrow)
    {
        ui->listSongs->setCurrentRow(ayaPlaylist->currentIndex() < ayaPlaylist->mediaCount()? ayaPlaylist->currentIndex()+1:ayaPlaylist->mediaCount());
    }
}
