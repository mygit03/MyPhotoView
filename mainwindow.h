#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QToolButton>
#include <QFileInfoList>
#include <QMenu>
#include <QTimer>
#include <QSlider>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *e);

    virtual void mouseDoubleClickEvent(QMouseEvent *e);

    virtual void resizeEvent(QResizeEvent *e);

    virtual void mousePressEvent(QMouseEvent *e);

private slots:
    void slot_large();

    void slot_small();

    void slot_left();

    void on_tBtn_full_clicked();

    void slot_right();

    void on_tBtn_del_clicked();

    void slot_open();

    void slot_toLeft();

    void slot_toRight();

    void slot_action(QAction *act);

    void on_act_saveAs_triggered();

    void on_act_close_triggered();

    void on_act_quit_triggered();

    void on_act_about_triggered();

    void on_act_Qt_triggered();

    void on_act_clear_triggered();

    void slot_display();

    void slot_slider(int value);

    void on_act_large_triggered();

private:
    void getImgInfoList(QFileInfoList &imgInfoList);
    void setRecentList();

private:
    Ui::MainWindow *ui;

    QToolButton *tBtn;
    QPixmap pix;

    bool isOpen;       //是否打开文件
    QFileInfoList imgInfoList;  //图片列表
    int index;                  //当前显示图片位置
    QString path;               //当前图片的路径
    QString m_fileName;         //当前显示的图片
    QMenu *menu;                //最近打开列表
    QList<QAction *> actList;
    bool isExit;                //是否已打开

    QTimer *timer;              //幻灯片放映定时器
    int curIndex;               //幻灯片显示位置
    bool isFull;                //是否全屏

    QSlider *slider;

    QSize m_size;                 //图片初始大小
};

#endif // MAINWINDOW_H
