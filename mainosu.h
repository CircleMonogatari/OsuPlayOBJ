#ifndef MAINOSU_H
#define MAINOSU_H

#include <QWidget>
#include <QTreeView>
#include <QLabel>
#include <QDebug>
#include <QAbstractButton>
#include <QToolButton>
#include <QBoxLayout>
#include <QPainter>
#include <QApplication>
#include <QStandardItemModel>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDesktopWidget>
#include <QMediaPlayer>
#include <QStandardPaths>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QSettings>
#include <QIcon>
#include <QDir>
#include <QDirModel>
#include <QFileDialog>
#include <QUrl>
#include <QWinTaskbarButton>
#include "volumebutton.h"



namespace Ui {
class Mainosu;
}

class Mainosu : public QWidget
{
    Q_OBJECT

public:
    explicit Mainosu(QWidget *parent = 0);
    ~Mainosu();

    void createWidgits();
    void createShortcuts();
    void createTaskbar();

    void startPlay();
    void openfile();
    void playUrl(const QUrl &url);
    void setPositionSlider();
    void treeinit();

private:
    Ui::Mainosu *ui;


public:
    QWinTaskbarButton *taskbarBtn = NULL;

    QTreeView * treedir = NULL;
    QStandardItemModel  *model = NULL;
    QMediaPlayer mediaPlayer;

    QAbstractButton *playBtn = NULL;
    QAbstractButton * volumeBtn = NULL;
    QSlider *positionSlidet = NULL;
    QLabel *infoLabel = NULL;
    QLabel *positionLabel = NULL;
    QPoint offset;
    QString fileName;


protected:
void paintEvent(QPaintEvent *);
QString S_filepath;
};

#endif // MAINOSU_H
