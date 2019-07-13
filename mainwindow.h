#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <set>
#include <opencv.hpp>

#include "labimage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionopen_folder_triggered();

    void on_Image_clicked();

    void on_Reset_clicked();

    void on_Label_clicked();

    void on_Next_clicked();

private:
    Ui::MainWindow *ui;

    std::string record_filename;
    std::string label_filename;
    std::set<std::string> record;
    QString root_dir;
    QStringList nameList;
    int totalNums;

private:
    LabImage labImage;
    void showImage();
    void updateImage(int x, int y);
};

#endif // MAINWINDOW_H