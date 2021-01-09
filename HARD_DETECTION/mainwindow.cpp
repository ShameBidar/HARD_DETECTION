#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDebug>
#include <QStringListModel>
#include <mntent.h>
#include <QProcess>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    struct mntent *ent;
    FILE *aFile;

    aFile = setmntent("/proc/mounts", "r");
    if (aFile == NULL) {
      perror("setmntent");
      exit(1);
    }
    int i = 0;
    while (NULL != (ent = getmntent(aFile)) && i<2) {

        if(strstr(ent->mnt_fsname,"/sd") && strcmp(ent->mnt_dir,"/") != 0)
        {
            //blisting << QString::fromLocal8Bit(arr);
            if(i == 0)
            {
                ui->btn_disk_1->setText(ent->mnt_dir);
                first_disk.str_dev = ent->mnt_fsname;
                first_disk.str_filesys = ent->mnt_type;
                first_disk.str_mp = ent->mnt_dir;
            }
            else if(i == 1)
            {
                ui->btn_disk_2->setText(ent->mnt_dir);
                second_disk.str_dev = ent->mnt_fsname;
                second_disk.str_filesys = ent->mnt_type;
                first_disk.str_mp = ent->mnt_dir;
            }
            i++;

        }
    }
    endmntent(aFile);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_disk_1_clicked()
{
    QString cmd = QString("df -k %1").arg(first_disk.str_dev);
    QProcess qproc;
    qproc.start(cmd);
    qproc.waitForFinished();
    QString stdout = qproc.readAllStandardOutput();
    qDebug() << stdout;

    //qDebug() << system(cmd.toStdString().c_str());
    ui->lbl_device->setText(first_disk.str_dev);
    ui->lbl_filesys->setText(first_disk.str_filesys);
    ui->btn_disk_1->setStyleSheet("background-color: rgb(44, 44, 44); color: rgb(63, 169, 245);border: 0px;");
    ui->btn_disk_2->setStyleSheet("background-color: rgb(44, 44, 44); color: rgb(206,206,206);border: 0px;");
    which_btn = SELECT_FIRST;
}

void MainWindow::on_btn_disk_2_clicked()
{
    QString cmd = QString("df -k %1").arg(second_disk.str_dev);
    qDebug() << cmd;
    qDebug() << system(cmd.toStdString().c_str());
    ui->lbl_device->setText(second_disk.str_dev);
    ui->lbl_filesys->setText(second_disk.str_filesys);
    ui->btn_disk_2->setStyleSheet("background-color: rgb(44, 44, 44); color: rgb(63, 169, 245);border: 0px;");
    ui->btn_disk_1->setStyleSheet("background-color: rgb(44, 44, 44); color: rgb(206,206,206);border: 0px;");

    which_btn = SELECT_SECOND;
}

/***********************
 * dev='/dev/sdb'
sudo umount "$dev"
printf "o\nn\np\n1\n\n\nw\n" | sudo fdisk "$dev"
sudo mkfs.ext4 "${dev}1"
 * *********************/
void MainWindow::on_btn_format_clicked()
{
    QString dev = "";
    QString mp = "";
    if(which_btn == SELECT_FIRST)
    {
        dev = first_disk.str_dev;
        mp = first_disk.str_mp;
    }
    else if(which_btn == SELECT_SECOND)
    {
        dev = second_disk.str_dev;
        mp = second_disk.str_mp;
    }
    if(which_btn == SELECT_FIRST || which_btn == SELECT_SECOND)
    {
        //QString data;
        //data.toStdString().c_str();
        //QString cmd = QString("mkfs.ext4 %1").arg(dev);
        QString cmd = QString("umount %1").arg(mp);
        qDebug() << cmd;
        int st = system(cmd.toStdString().c_str());
        qDebug() << "exec status: " << st;
        if(st == 0)
        {
            cmd = QString("sudo mkfs.ext4 %1").arg(dev);
            qDebug() << cmd;
            int st = system(cmd.toStdString().c_str());
            qDebug() << "exec status: " << st;
        }
    }
}
