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

    QProcess pr_rddisk;
    pr_rddisk.start("lsblk");
    pr_rddisk.waitForStarted();
    pr_rddisk.waitForFinished();
    QStringList device;
    while (!pr_rddisk.atEnd()) {
        QString str = pr_rddisk.readLine();
        str = str.trimmed();
        if(str.startsWith("sd"))
        {
            device.append(str);
            qDebug() << device;
            qDebug() << "*********************************";
        }
    }
    int i=0;
    foreach (const QString &str, device) {
        if(i<2)
        {
            QStringList sdx = str.split(" ");
            sdx.removeAll("");
            if(i == 0)
            {
                //findDiskInfo("/dev/"+sdx.at(0),first_disk);
                first_disk.str_dev = "/dev/" + sdx.at(0);
                first_disk.str_size = sdx.at(3);
                ui->btn_disk_1->setText(first_disk.str_dev);
            }
            if(i == 1)
            {
                second_disk.str_dev = "/dev/" + sdx.at(0);
                second_disk.str_size = sdx.at(3);
                //findDiskInfo("/dev/"+sdx.at(0),second_disk);
                ui->btn_disk_2->setText(second_disk.str_dev);
            }
        }
        i++;
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_disk_1_clicked()
{


    //qDebug() << system(cmd.toStdString().c_str());
    ui->lbl_device->setText(first_disk.str_dev);
    ui->lbl_capacity->setText(first_disk.str_size);
    ui->lbl_used->setText(first_disk.str_used);
    ui->lbl_free->setText(first_disk.str_free);

    ui->btn_disk_1->setStyleSheet("background-color: rgb(44, 44, 44); color: rgb(63, 169, 245);border: 0px;");
    ui->btn_disk_2->setStyleSheet("background-color: rgb(44, 44, 44); color: rgb(206,206,206);border: 0px;");

    which_btn = SELECT_FIRST;
}

void MainWindow::on_btn_disk_2_clicked()
{

    ui->lbl_device->setText(second_disk.str_dev);
    ui->lbl_capacity->setText(second_disk.str_size);
    ui->lbl_used->setText(second_disk.str_used);
    ui->lbl_free->setText(second_disk.str_free);

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

    if(which_btn == SELECT_FIRST)
    {

    }
    else if(which_btn == SELECT_SECOND)
    {

    }

}
