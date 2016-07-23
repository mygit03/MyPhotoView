#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    setWindowTitle(tr("关于本程序"));
    setWindowIcon(QIcon(":images/full.png"));
}

About::~About()
{
    delete ui;
}
