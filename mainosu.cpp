#include "mainosu.h"
#include <QDir>

#include <QDirModel>

Mainosu::Mainosu(QWidget *parent) :
    QWidget(parent)
{

   //创建窗口
    createWidgits();



}

Mainosu::~Mainosu()
{
    delete ui;
}

void Mainosu::createWidgits()
{
    //play
    playBtn = new QToolButton(this);
    playBtn->setEnabled(false);
    QIcon qi;
    playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playBtn->setToolTip(QString("play"));
    connect(playBtn, &QAbstractButton::clicked, this, &Mainosu::startPlay);

    //file
    QAbstractButton *openBtn = new QToolButton(this);
    openBtn->setText(QString("..."));
    openBtn->setToolTip(QString("Open a file"));
    openBtn->setFixedSize(playBtn->sizeHint());
    connect(openBtn, &QAbstractButton::clicked, this, &Mainosu::openfile);

    //vol
    volumeBtn = new VolumeButton(this);

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


     model = new QStandardItemModel;
     model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("Name"));
    //关联
    treeinit();


    QBoxLayout *treeLayout = new QHBoxLayout;
    treeLayout->setMargin(0);
    treeLayout->addWidget(treedir);


    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(openBtn);
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


}

void Mainosu::openfile()
{
    //打开osu对应的音乐列表
    QFileDialog fileDialog(this);
    //fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    //fileDialog.setWindowTitle(QString("Open OSU file dir"));
   // fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));

    fileDialog.setFileMode(QFileDialog::DirectoryOnly);

    if (fileDialog.exec() == QDialog::Accepted)
    {
       //playUrl(fileDialog.selectedUrls().constFirst());

        S_filepath =  fileDialog.selectedFiles().constFirst();
        qDebug() << S_filepath;

        QDir dir(fileDialog.selectedFiles().constFirst());
        dir.setFilter(QDir::Dirs);
        QFileInfoList list = dir.entryInfoList();
        int file_count = list.count();

        qDebug()<< file_count;
        for(int i = 2; i < file_count; ++i){
            qDebug()<< S_filepath +"/"+list[i].fileName();
            QDir d(S_filepath +"/"+list[i].fileName());
            if(d.exists()){
                d.setFilter(QDir::Files);
                QFileInfoList filelist     = d.entryInfoList();
                int count = filelist.count();
                qDebug()<<"dir->son:\n";
                qDebug()<< count;
            }
        }
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

void Mainosu::setPositionSlider()
{

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


void Mainosu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
        QPixmap pix;
        pix.load("C:/Users/Circle/Desktop/123.png");
        painter.drawPixmap(0, 0, 650, 600, pix);

}
