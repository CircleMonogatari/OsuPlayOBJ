#ifndef MAINOSU_H
#define MAINOSU_H

#include <QApplication>
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
#include "config_num.h"


#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<__LINE__

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
    void setPositionSlider(qint64 position);
    void treeinit();
    void treedoubleback(const QModelIndex Qmode);
    void keyReleaseEvent(QKeyEvent *event);

    void updateInfo();
    void getfiletree();


private:
    Ui::Mainosu *ui;


public:
    QWinTaskbarButton *taskbarBtn = NULL;

    QTreeView * treedir = NULL;
    QStandardItemModel  *model = NULL;
    QMediaPlayer mediaPlayer;

    QAbstractButton *playBtn = NULL;
    VolumeButton * volumeBtn = NULL;
    QSlider *positionSlidet = NULL;
    QLabel *infoLabel = NULL;
    QLabel *positionLabel = NULL;
    qint64 duration_end;

    QPoint offset;
    QString fileName;
    int file_count;
    int mark_poi;
    int mark_max;



public slots:
    void updateDuration(qint64 duration);
    void updatePosition(qint64 position);
protected:
void paintEvent(QPaintEvent *);
QString S_filepath;
int H_tree_num;
int H_tree_max;
};

#endif // MAINOSU_H
