#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDebug>
#include <QStringListModel>
#include <mntent.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList listing;
    QStringListModel *model = new QStringListModel(/*some QObject * as a parent, e.g. ui->ListPic */ ui->listView);

    struct mntent *ent;
    FILE *aFile;

    aFile = setmntent("/proc/mounts", "r");
    if (aFile == NULL) {
      perror("setmntent");
      exit(1);
    }
    while (NULL != (ent = getmntent(aFile))) {
        char arr[400];
        sprintf(arr, "%s    %s    %s", ent->mnt_fsname, ent->mnt_type ,ent->mnt_dir);
        if(strstr(ent->mnt_fsname,"/sd"))
        {
            listing << QString::fromLocal8Bit(arr);
        }
    }
    model->setStringList(listing);
    ui->listView->setModel(model);
    endmntent(aFile);


}

MainWindow::~MainWindow()
{
    delete ui;
}

