#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QDebug>
#include <QFileDialog>
#include <QList>
#include <QMediaContent>
#include <QKeyEvent>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sliderSeek_sliderMoved(int position);

    void on_sliderVolume_sliderMoved(int position);

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

    void on_stateChanged();
    void on_mediaButton_clicked();

    void on_currentMediaChanged();

    void on_listSongs_doubleClicked(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
    QMediaPlayer *ayaPlayer;
    QMediaPlaylist *ayaPlaylist;
    bool stopButtonPressed = false;
};

#endif // MAINWINDOW_H
