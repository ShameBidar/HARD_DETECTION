#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DevModel
{
public:
    QString str_filesys;
    QString str_dev;
    QString str_mp;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum WHICH_BTN
    {
        SELECT_FIRST,
        SELECT_SECOND,
    };
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btn_disk_1_clicked();

    void on_btn_disk_2_clicked();

    void on_btn_format_clicked();

private:
    Ui::MainWindow *ui;
    DevModel first_disk;
    DevModel second_disk;
    WHICH_BTN which_btn;
};
#endif // MAINWINDOW_H
