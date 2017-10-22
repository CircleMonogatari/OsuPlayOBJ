#include "mainosu.h"
#include <QDir>
#include <QMessageBox>
#include <QDirModel>
#include <QTime>
#include <QDesktopServices>

Mainosu::Mainosu(QWidget *parent) :
    QWidget(parent)
{
   //创建窗口
    createWidgits();
   //关联medilplay
    connect(&mediaPlayer, &QMediaPlayer::positionChanged, this, &Mainosu::updatePosition);
    connect(&mediaPlayer, &QMediaPlayer::durationChanged, this, &Mainosu::updateDuration);
    //connect(&mediaPlayer, &QMediaObject::metaDataAvailableChanged, this, &Mainosu::updateInfo);
   getfiletree();
}


static QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void Mainosu::updatePosition(qint64 position)
{
    positionSlidet->setValue(position);
    positionLabel->setText(formatTime(position));

    if((duration_end == position) &&(position > 0)){
        qDebug()<<"play end";
        item_temp->setBackground(QBrush(QColor("white")));
        mark_poi+=1;
        if (mark_poi == mark_max){
            mark_poi = 0;
        }

        QTime t;
        t.start();
        while(t.elapsed()<2000);
        model->setSortRole(mark_poi);

        item_temp = model->item(mark_poi);
        item_temp->setBackground(QBrush(QColor("red")));

        QModelIndex i = model->index(mark_poi, 0);
        qDebug()<<model->data(i, Qt::DisplayRole).toString();
        QString s = model->data(i, Qt::DisplayRole).toString();

        QDir q(S_filepath+"/"+s);
            q.setFilter(QDir::Files);
            QFileInfoList list = q.entryInfoList();
            for(int i = 0; i < list.count(); ++i){
               qDebug()<< list[i].fileName();
               qDebug()<< list[i].filePath();
               QString pat(list[i].fileName());
               if (pat.contains("mp3", Qt::CaseSensitive)){
                   playUrl(list[i].filePath());
                   item_temp->setBackground(QBrush(QColor("green")));
                   playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
                   break;
                 }
            }
    }
}

void Mainosu::updateDuration(qint64 duration)
{
    positionSlidet->setRange(0, duration);
    positionSlidet->setEnabled(duration > 0);
    positionSlidet->setPageStep(duration / 10);
    duration_end = duration;
    //updateInfo();
    qDebug()<<"update";
}




Mainosu::~Mainosu()
{
    delete ui;
}

void Mainosu::createWidgits()
{
    H_tree_max = 0;
    H_tree_num = 0;
    //play
    playBtn = new QToolButton(this);
    playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playBtn->setToolTip(QString("play"));
    connect(playBtn, &QAbstractButton::clicked, this, &Mainosu::startPlay);

    //setPlaybackRate(qreal rate)
    QAbstractButton *backRate_btn = new QToolButton(this);
    backRate_btn->setText(QString("1.5"));
    backRate_btn->setToolTip(QString("PlaybackRate"));
    connect(backRate_btn, &QAbstractButton::clicked, this, &Mainosu::startPlay);


    //file
    QAbstractButton *openBtn = new QToolButton(this);
    openBtn->setText(QString("..."));
    openBtn->setToolTip(QString("Open a file"));
    openBtn->setFixedSize(playBtn->sizeHint());

    connect(openBtn, &QAbstractButton::clicked, this, &Mainosu::openfile);

    //vol
    volumeBtn = new VolumeButton(this);
    volumeBtn->setToolTip(tr("Adjust volume"));
    volumeBtn->setVolume(mediaPlayer.volume());
    connect(volumeBtn, &VolumeButton::volumeChanged, &mediaPlayer, &QMediaPlayer::setVolume);

    //Slider
    positionSlidet = new QSlider(Qt::Horizontal, this);
    positionSlidet->setEnabled(false);
    positionSlidet->setToolTip("Seek");
    connect(positionSlidet, &QSlider::valueChanged, this , &Mainosu::setPositionSlider);

    infoLabel = new QLabel(this);
    positionLabel = new QLabel(QString("00:00"), this);
    positionLabel->setMinimumWidth(positionLabel->sizeHint().width());

    //treedir
     treedir = new QTreeView(this);
     treedir->setMinimumSize(300,400);
     treedir->setEditTriggers(QAbstractItemView::NoEditTriggers);
     model = new QStandardItemModel;

     model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("filename"));


    //关联
    //treeinit();
    // connect(treedir, &QTreeView::doubleClicked(QModelIndex), this, &Mainosu::treedoubleback(const QModelIndex));

    // connect(treedir,SIGNAL(doubleClicked ( const QModelIndex)),this,SLOT(treedoubleback(const QModelIndex)));
     connect(treedir, &QTreeView::doubleClicked, this, &Mainosu::treedoubleback);


    QBoxLayout *treeLayout = new QHBoxLayout;
    treeLayout->setMargin(0);
    treeLayout->addWidget(treedir);


    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(openBtn);
    controlLayout->addWidget(backRate_btn);
    controlLayout->addWidget(playBtn);
    controlLayout->addWidget(positionSlidet);
    controlLayout->addWidget(positionLabel);
    controlLayout->addWidget(volumeBtn);

    QBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(infoLabel);
    mainLayout->addLayout(treeLayout);
    mainLayout->addLayout(controlLayout);

    QBoxLayout *DownFile = new QHBoxLayout(this);
     DownFile->addWidget(infoLabel);
     DownFile->addLayout(mainLayout);

}

void Mainosu::createShortcuts()
{

}

void Mainosu::createTaskbar()
{
   // taskbarBtn = new

}

void Mainosu::startPlay()
{
    //开始你的表演
    if (mediaPlayer.mediaStatus() == QMediaPlayer::NoMedia)
        //openFile();
        qDebug()<<"没有歌曲";
    else if (mediaPlayer.state() == QMediaPlayer::PlayingState)
      {
        mediaPlayer.pause();
        qDebug()<<"pause";
        playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
     }
    else{
        mediaPlayer.play();
        playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        }
}

void Mainosu::openfile()
{
    //打开osu对应的音乐列表
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    if (fileDialog.exec() == QDialog::Accepted)
    {

        S_filepath =  fileDialog.selectedFiles().constFirst();
        qDebug() << S_filepath;

            //out
        QFile file("./config");
        if(!file.open(QFile::WriteOnly | QFile::Text))
            {
               qDebug () << "getfiletree ERROR";
               return;
            }
            QTextStream out(&file);
            //输出内容到缓冲区
            out << S_filepath;
            file.flush();//将缓冲区的内容输出的文本
            file.close();//关闭文件



        QDir dir(fileDialog.selectedFiles().constFirst());
        dir.setFilter(QDir::Dirs);
        QFileInfoList list = dir.entryInfoList();
        file_count = list.count();

        mark_max = file_count;
        qDebug()<< file_count;
        H_tree_max = file_count;
        for(int i = 2; i < file_count; ++i){

            QStandardItem *item = new QStandardItem(list[i].fileName());
            model->setItem(i-2,0, item);

            }

        this->treedir->setModel(model);
    }

    //拿到所有文件列表
    //addtreeviewer
    //clear tree
}

void Mainosu::playUrl(const QUrl &url)
{
    qDebug() << "qwe";
    qDebug() << url.toLocalFile();
    playBtn->setEnabled(true);

    mediaPlayer.setMedia(url);
    mediaPlayer.play();
}

void Mainosu::setPositionSlider(qint64 position)
{
    if (qAbs(mediaPlayer.position() - position) > 99)
        mediaPlayer.setPosition(position);
}

void Mainosu::treeinit()
{

    QStandardItem *item1 = new QStandardItem("d1");
    QStandardItem *item2 = new QStandardItem("d2");
    QStandardItem *item3 = new QStandardItem("d3");
    QStandardItem *item4 = new QStandardItem("d4");

   model->setItem(0,0, item1);
   model->setItem(1,0, item2);
   model->setItem(2,0, item3);
   model->setItem(3,0, item4);

   for(int i = 0; i< 30; ++i){
       QStandardItem *num = new QStandardItem(i);
       model->setItem(i+4,0, num);
   }

   QStandardItem *item5 = new QStandardItem("fifth");
   item4->appendRow(item5);

    //treedir->setRootIndex(model->index("H:\\01c\\OSU\\Songs"));
    this->treedir->setModel(model);

}

void Mainosu::treedoubleback(const QModelIndex index)
{
    if(item_temp != NULL){
         item_temp->setBackground(QBrush(QColor("white")));
    }

    qDebug()<<"treedoubleback singa";
    mark_poi = index.row();
    qDebug()<< index.row();
    item_temp = model->item(mark_poi);
    item_temp->setBackground(QBrush(QColor("red")));


    QAbstractItemModel* m=(QAbstractItemModel*)index.model();
    for(int columnIndex = 0; columnIndex < m->columnCount(); columnIndex++)
    {
        QModelIndex x=m->index(index.row(),columnIndex);

        QString s= x.data().toString();
        QDir q(S_filepath+"/"+s);
        q.setFilter(QDir::Files);
        QFileInfoList list = q.entryInfoList();
        for(int i = 0; i < list.count(); ++i){
           qDebug()<< list[i].fileName();
           qDebug()<< list[i].filePath();
           QString pat(list[i].fileName());
           if (pat.contains("mp3", Qt::CaseSensitive)){
               playUrl(list[i].filePath());
               item_temp->setBackground(QBrush(QColor("green")));
               playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
               break;
           }
        }
    }
}

void Mainosu::keyReleaseEvent(QKeyEvent *e)
{


}

void Mainosu::PlaybackRate()
{
    //qreal rate;

    qreal rate = 2;
    mediaPlayer.setPlaybackRate(rate);
}

void Mainosu::updateInfo()
{
    QStringList info;
    if (!fileName.isEmpty())
        info.append(fileName);
    if (mediaPlayer.isMetaDataAvailable()) {
        QString author = mediaPlayer.metaData(QStringLiteral("Author")).toString();
        if (!author.isEmpty())
            info.append(author);
        QString title = mediaPlayer.metaData(QStringLiteral("Title")).toString();
        if (!title.isEmpty())
            info.append(title);
    }
    info.append(formatTime(mediaPlayer.duration()));
    infoLabel->setText(info.join(tr(" - ")));

}

void Mainosu::getfiletree()
{

   //in
       QFile filew("./config");
          //以只读和文本的方式打开该文件
          if(!filew.open(QFile::ReadOnly | QFile::Text))
          {
             qDebug () << "getfiletree ERROR";
             return;
          }
          QTextStream in(&filew);
          //输出内容到缓冲区

            QString sbuf;
            sbuf = in.readAll();
            qDebug()<<sbuf;


            S_filepath = sbuf;
            QDir dir(sbuf);
            dir.setFilter(QDir::Dirs);
            QFileInfoList list = dir.entryInfoList();
            file_count = list.count();
            mark_max = file_count;
            qDebug()<< file_count;
            H_tree_max = file_count;
            for(int i = 2; i < file_count; ++i){

                QStandardItem *item = new QStandardItem(list[i].fileName());
                model->setItem(i-2,0, item);

                }
            this->treedir->setModel(model);
            this->treedir->setColumnWidth(0,30);
          filew.flush();//将缓冲区的内容输出的文本
          filew.close();//关闭文件
}





void Mainosu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
        QPixmap pix;
        pix.load("./bj.png");
        painter.drawPixmap(0, 0, 650, 600, pix);

}
