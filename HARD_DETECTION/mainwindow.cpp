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

            }
            else if(i == 1)
            {
                ui->btn_disk_2->setText(ent->mnt_dir);
                second_disk.str_dev = ent->mnt_fsname;
                second_disk.str_filesys = ent->mnt_type;
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
    ui->lbl_device->setText(first_disk.str_dev);
    ui->lbl_filesys->setText(first_disk.str_filesys);
    which_btn = SELECT_FIRST;
}

void MainWindow::on_btn_disk_2_clicked()
{
    ui->lbl_device->setText(second_disk.str_dev);
    ui->lbl_filesys->setText(second_disk.str_filesys);
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
    QString dev;
    if(which_btn == SELECT_FIRST)
    {
        dev = first_disk.str_dev;
    }
    else if(which_btn == SELECT_SECOND)
    {
        dev = second_disk.str_dev;
    }
    if(which_btn == SELECT_FIRST || which_btn == SELECT_SECOND)
    {
        //QString data;
        //data.toStdString().c_str();
        QString cmd = QString("umount %1").arg(dev);
        system(cmd.toStdString().c_str());
    }
}
