#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QMessageBox>

#include "about.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isOpen = false;

    tBtn = new QToolButton(this);
    tBtn->resize(100, 100);
    tBtn->setIcon(QIcon(":/images/open.png"));
    tBtn->setIconSize(tBtn->size());
    tBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tBtn->setAutoRaise(true);
    tBtn->move(this->width() / 2 - 50, this->height() / 2 - 60);
    connect(tBtn, SIGNAL(pressed()), this, SLOT(slot_open()));

    connect(ui->act_open, SIGNAL(triggered()), this, SLOT(slot_open()));

    connect(ui->tBtn_large, SIGNAL(pressed()), this, SLOT(slot_large()));
    connect(ui->tBtn_left, SIGNAL(pressed()), this, SLOT(slot_left()));
    connect(ui->tBtn_right, SIGNAL(pressed()), this, SLOT(slot_right()));
    connect(ui->tBtn_toLeft, SIGNAL(pressed()), this, SLOT(slot_toLeft()));
    connect(ui->tBtn_toRight, SIGNAL(pressed()), this, SLOT(slot_toRight()));
//    connect(ui->act_large, SIGNAL(triggered()), this, SLOT(slot_large()));
    connect(ui->act_small, SIGNAL(triggered()), this, SLOT(slot_small()));
    connect(ui->act_toLeft, SIGNAL(triggered()), this, SLOT(slot_toLeft()));
    connect(ui->act_toRight, SIGNAL(triggered()), this, SLOT(slot_toRight()));

    ui->label->setAlignment(Qt::AlignCenter);       //居中显示

    ui->tBtn_left->setEnabled(false);
    ui->tBtn_right->setEnabled(false);
    ui->tBtn_full->setEnabled(false);
    ui->tBtn_large->setEnabled(false);
    ui->tBtn_toLeft->setEnabled(false);
    ui->tBtn_toRight->setEnabled(false);
    ui->tBtn_del->setEnabled(false);

    ui->act_saveAs->setEnabled(false);
    ui->act_close->setEnabled(false);
    ui->act_large->setEnabled(false);
    ui->act_small->setEnabled(false);
    ui->act_toLeft->setEnabled(false);
    ui->act_toRight->setEnabled(false);

    ui->act_clear->setEnabled(false);

    menu = new QMenu(tr("最近打开列表"), this);
    menu->addAction(tr("空"));
    ui->menu_0->insertMenu(ui->act_clear, menu);
    connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(slot_action(QAction*)));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_display()));

    isFull = false;

    slider = new QSlider(this);
    slider->setRange(0, 100);
    slider->hide();
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(slot_slider(int)));

    this->setMinimumSize(950, 655);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape){
        qDebug() << "esc";
        if (this->isFullScreen()){
            timer->stop();
            this->showNormal();
            isFull = false;

            ui->menuBar->show();
            ui->toolBar->show();

            ui->tBtn_del->show();
            ui->tBtn_full->show();
            ui->tBtn_large->show();
            ui->tBtn_left->show();
            ui->tBtn_right->show();
            ui->tBtn_toLeft->show();
            ui->tBtn_toRight->show();
            ui->line->show();
        }
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(isFull){
        return;
    }
    if(isOpen){
        if(isMaximized()){
            this->showNormal();
        }else{
            this->showMaximized();
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    tBtn->move(this->width() / 2 - 50, this->height() / 2 - 60);

    QPoint point = ui->tBtn_large->pos();
    point.setX(point.x() + 5);
    point.setY(point.y() - 15);
    slider->move(point);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    slider->hide();
}

void MainWindow::slot_large()
{
    QPoint point = ui->tBtn_large->pos();
    point.setX(point.x() + 5);
    point.setY(point.y() - 15);
    slider->move(point);
    slider->resize(20, 100);
    slider->setSingleStep(1);
    slider->show();
}

void MainWindow::slot_small()
{
    QPixmap pixSmall;
    int width = pix.width();
    int heigh = pix.height();
    int pixWidth = m_size.width();
    int pixHeigh = m_size.height();
    if(width > pixWidth && heigh > pixHeigh){
        width = pix.width() / 2;
        heigh = pix.height() / 2;
        pixSmall = pix.scaled(width, heigh, Qt::KeepAspectRatio);
        ui->label->setPixmap(pixSmall);

        pix = pixSmall;
        if(width <= pixWidth && heigh <= pixHeigh){
            ui->act_small->setEnabled(false);
        }
    }
}

void MainWindow::slot_left()
{
    getImgInfoList(imgInfoList);

    --index;
    int count = imgInfoList.count();        //当前目录图片个数
    qDebug() << "index:" << index << "left count:" << count;
    if(index < 0){
        index += count;
    }

    m_fileName.clear();
    m_fileName.append(path);
    m_fileName += imgInfoList.at(index).fileName();

    pix.load(m_fileName);
    ui->label->setPixmap(pix);
    qDebug() << "path:" << path << m_fileName;

    m_size = pix.size();

    QString fileName = QFileInfo(m_fileName).fileName();
    setWindowTitle(fileName + tr("- 照片查看器"));

    ui->act_small->setEnabled(false);

    setRecentList();
}

void MainWindow::on_tBtn_full_clicked()
{
    this->showFullScreen();
    isFull = true;

    timer->start(1500);
    curIndex = 0;

    ui->menuBar->hide();
    ui->toolBar->hide();

    ui->tBtn_del->hide();
    ui->tBtn_full->hide();
    ui->tBtn_large->hide();
    ui->tBtn_left->hide();
    ui->tBtn_right->hide();
    ui->tBtn_toLeft->hide();
    ui->tBtn_toRight->hide();
    ui->line->hide();
}

void MainWindow::slot_right()
{
    getImgInfoList(imgInfoList);

    ++index;
    int count = imgInfoList.count();        //当前目录图片个
    qDebug() << "index:" << index << "right count:" << count;
    index %= count;

    m_fileName.clear();
    m_fileName.append(path);
    m_fileName += imgInfoList.at(index).fileName();

    pix.load(m_fileName);
    ui->label->setPixmap(pix);
    qDebug() << "path:" << path << m_fileName;

    m_size = pix.size();

    QString fileName = QFileInfo(m_fileName).fileName();
    setWindowTitle(fileName + tr("- 照片查看器"));

    ui->act_small->setEnabled(false);

    setRecentList();
}

void MainWindow::on_tBtn_del_clicked()
{
    int ret = QMessageBox::warning(this, tr("警告"),
                                   tr("是否删除%1？").arg(QFileInfo(m_fileName).fileName()),
                                   tr("确  定"), tr("取  消"), 0, 1);

    qDebug() << "count:" << imgInfoList.count();
    QString fileName;
    switch (ret) {
    case 0:
        QFile::remove(m_fileName);
        imgInfoList.removeAt(index);

        m_fileName.clear();
        m_fileName.append(path);

        qDebug() << "del index:" << index;
        if(index >= imgInfoList.count()){
            index = imgInfoList.count() - 1;
            qDebug() << "del index::" << index;
            if(index < 0){
                qDebug() << "del index:::" << index;
                ui->label->clear();
                tBtn->show();

                ui->tBtn_full->setEnabled(false);
                ui->tBtn_large->setEnabled(false);
                ui->tBtn_toLeft->setEnabled(false);
                ui->tBtn_toRight->setEnabled(false);
                ui->tBtn_del->setEnabled(false);

                ui->act_saveAs->setEnabled(false);
                ui->act_close->setEnabled(false);
                ui->act_large->setEnabled(false);
                ui->act_small->setEnabled(false);
                ui->act_toLeft->setEnabled(false);
                ui->act_toRight->setEnabled(false);

                setWindowTitle(tr("照片查看器"));
                return;
            }
        }
        m_fileName += imgInfoList.at(index).fileName();

        pix.load(m_fileName);
        ui->label->setPixmap(pix);
        qDebug() << "del count:" << imgInfoList.count() << m_fileName << "index:" << index;

        fileName = QFileInfo(fileName).fileName();
        setWindowTitle(fileName + tr("- 照片查看器"));
        break;
    case 1:
    default:
        break;
    }
    int count = imgInfoList.count();
    if(count <= 1){
        ui->tBtn_left->setEnabled(false);
        ui->tBtn_right->setEnabled(false);
    }else{
        ui->tBtn_left->setEnabled(true);
        ui->tBtn_right->setEnabled(true);
    }
    qDebug() << "del count:" << imgInfoList.count() << count;
}

void MainWindow::slot_open()
{
    path = QDir::homePath();
    m_fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), path,
                                                    tr("image(*.png *.jpg);;AllFile(*)"));

    getImgInfoList(imgInfoList);

    if(!m_fileName.isEmpty()){
        tBtn->hide();
        isOpen = true;

        pix.load(m_fileName);
        ui->label->setPixmap(pix);

        m_size = pix.size();

        QString fileName = QFileInfo(m_fileName).fileName();
        setWindowTitle(fileName + tr("- 照片查看器"));

        ui->tBtn_full->setEnabled(true);
        ui->tBtn_large->setEnabled(true);
        ui->tBtn_toLeft->setEnabled(true);
        ui->tBtn_toRight->setEnabled(true);
        ui->tBtn_del->setEnabled(true);

        ui->act_saveAs->setEnabled(true);
        ui->act_close->setEnabled(true);
        ui->act_large->setEnabled(true);
//        ui->act_small->setEnabled(true);
        ui->act_toLeft->setEnabled(true);
        ui->act_toRight->setEnabled(true);

        this->setMinimumSize(0, 0);

        setRecentList();

        path = QFileInfo(m_fileName).absolutePath();
    }
}

void MainWindow::slot_toLeft()
{
    QMatrix matrix;
    matrix.rotate(-90);

    QPixmap pixScaled;
    pixScaled = pix.transformed(matrix, Qt::SmoothTransformation);
    ui->label->setPixmap(pixScaled);

    pix = pixScaled;
}

void MainWindow::slot_toRight()
{
    QMatrix matrix;
    matrix.rotate(90);

    QPixmap pixScaled;
    pixScaled = pix.transformed(matrix, Qt::SmoothTransformation);
    ui->label->setPixmap(pixScaled);

    pix = pixScaled;
}

void MainWindow::slot_action(QAction *act)
{
    qDebug() << "text:" << act->text();
    m_fileName = act->text();
    if(m_fileName != tr("空")){

        getImgInfoList(imgInfoList);
        QString tmpName;
        bool isDel = true;
        foreach (QFileInfo info, imgInfoList) {
            tmpName = info.absoluteFilePath();
            if(tmpName.compare(m_fileName) == 0){
                isDel = false;
                qDebug() << "name:" << tmpName << m_fileName;
            }
        }
        if(isDel){
            QMessageBox::warning(this, tr("警告"), tr("该图片不存在！"));
            return;
        }
        pix.load(m_fileName);
        ui->label->setPixmap(pix);

        m_fileName = QFileInfo(m_fileName).fileName();
        setWindowTitle(m_fileName + tr(" - 照片查看器"));
    }
    if(!tBtn->isHidden()){
        tBtn->hide();
    }

    ui->tBtn_left->setEnabled(true);
    ui->tBtn_right->setEnabled(true);
    ui->tBtn_full->setEnabled(true);
    ui->tBtn_large->setEnabled(true);
    ui->tBtn_toLeft->setEnabled(true);
    ui->tBtn_toRight->setEnabled(true);
    ui->tBtn_del->setEnabled(true);

    ui->act_saveAs->setEnabled(true);
    ui->act_close->setEnabled(true);
    ui->act_large->setEnabled(true);
    ui->act_small->setEnabled(true);
    ui->act_toLeft->setEnabled(true);
    ui->act_toRight->setEnabled(true);
}

void MainWindow::getImgInfoList(QFileInfoList &imgInfoList)
{
    imgInfoList.clear();

    QDir dir = QFileInfo(m_fileName).absolutePath();
    QFileInfoList infoList = dir.entryInfoList(QDir::Files);
    qDebug() << "GET:" << infoList.count() << dir;

    QFileInfo info;
    for(int i = 0; i < infoList.count(); i++){
        info = infoList.at(i);
        qDebug() << i << info.absoluteFilePath();
        QString suffix = info.suffix();

        //获取图片并加入图片列表
        if(suffix == "png" ||
                suffix == "jpg" ||
                suffix == "bmp" ||
                suffix == "gif")
        {
            imgInfoList.append(info);
            qDebug() << "getImgInfoList:" << i << info.absoluteFilePath() << info.suffix();
        }
    }

    //遍历获取当前显示图片的位置
    for(int j = 0; j < imgInfoList.count(); j++){
        info = imgInfoList.at(j);
//        qDebug() << j << info.absoluteFilePath() << info.fileName()
//                 << QFileInfo(m_fileName).fileName();
        QFileInfo curImgInfo = QFileInfo(m_fileName);
        if(info.fileName() == curImgInfo.fileName())
        {
            index = j;
            qDebug() << j << info.absoluteFilePath() << info.suffix() << "index" << index;
        }
    }

    int count = imgInfoList.count();
    if(count == 1){
        ui->tBtn_left->setEnabled(false);
        ui->tBtn_right->setEnabled(false);
    }else{
        ui->tBtn_left->setEnabled(true);
        ui->tBtn_right->setEnabled(true);
    }
}

void MainWindow::setRecentList()
{
    actList = menu->actions();
    isExit = false;
    if(actList.at(0)->text() == tr("空")){
        menu->clear();
    }else{
        foreach (QAction *act, actList) {
            if(act->text() == m_fileName){
                isExit = true;
            }
        }
    }
    if(!isExit){
        menu->addAction(m_fileName);
    }
    int count = actList.count();
    if(count){
        ui->act_clear->setEnabled(true);
    }
}

void MainWindow::on_act_saveAs_triggered()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    path.append(tr("/未命名"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), path,
                                 tr("PNG(*.png);;BMP(*.bmp);;JPEG(*.jpg *.jpeg)"));
    qDebug() << "path:" << path << "fileName:" << fileName;

    if(!fileName.isEmpty()){
        pix.save(fileName);
    }
}

void MainWindow::on_act_close_triggered()
{
    ui->label->clear();

    ui->tBtn_left->setEnabled(false);
    ui->tBtn_right->setEnabled(false);
    ui->tBtn_full->setEnabled(false);
    ui->tBtn_large->setEnabled(false);
    ui->tBtn_toLeft->setEnabled(false);
    ui->tBtn_toRight->setEnabled(false);
    ui->tBtn_del->setEnabled(false);

    ui->act_saveAs->setEnabled(false);
    ui->act_close->setEnabled(false);
    ui->act_large->setEnabled(false);
    ui->act_small->setEnabled(false);
    ui->act_toLeft->setEnabled(false);
    ui->act_toRight->setEnabled(false);

    tBtn->show();

    this->resize(950, 655);
}

void MainWindow::on_act_quit_triggered()
{
    this->close();
}

void MainWindow::on_act_about_triggered()
{
    About dialog;
    dialog.exec();
}

void MainWindow::on_act_Qt_triggered()
{
    qApp->aboutQt();
}

void MainWindow::on_act_clear_triggered()
{
    menu->clear();
    menu->addAction(tr("空"));

    ui->act_clear->setEnabled(false);
}

void MainWindow::slot_display()
{
//    getImgInfoList(imgInfoList);
    QString tmpName;
    int count = imgInfoList.count();
    curIndex %= count;
    tmpName = imgInfoList.at(curIndex++).absoluteFilePath();
    pix.load(tmpName);
    ui->label->setPixmap(pix);
}

void MainWindow::slot_slider(int value)
{
    qDebug() << "value:" << value;

    QPixmap pixmap;
    int width = pix.width() * (1 + value / 50);
    int heigh = pix.height() * (1 + value / 50);
    pixmap = pix.scaled(width, heigh, Qt::KeepAspectRatio);
    ui->label->setPixmap(pixmap);
}

void MainWindow::on_act_large_triggered()
{
    QPixmap pixlarge;
    int width = pix.width();
    int heigh = pix.height();
    pixlarge = pix.scaled(width * 2, heigh * 2, Qt::KeepAspectRatio);
    ui->label->setPixmap(pixlarge);

    pix = pixlarge;

    ui->act_small->setEnabled(true);
}
