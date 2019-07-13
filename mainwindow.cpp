#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <opencv.hpp>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showImage()
{
    cv::Mat image = labImage.image_show;
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText("Iamge read error!");
        msgBox.exec();
    }
    else
    {
        cv::Mat rgb;
        QImage img;
        if(image.channels() == 3)
        {
            cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB);
            img = QImage(const_cast<unsigned char*>(rgb.data), rgb.cols, rgb.rows, rgb.cols*rgb.channels(), QImage::Format_RGB888);   
        }
        else
        {
            img = QImage(const_cast<unsigned char*>(image.data), image.cols, image.rows, image.cols*image.channels(), QImage::Format_RGB888);
        }
        ui->Image->setPixmap(QPixmap::fromImage(img));
        ui->Image->setAlignment(Qt::AlignCenter);
        ui->Image->setOffset();
    }
}

void MainWindow::updateImage(int x=-1, int y=-1)
{
    if(x < 0 || y < 0)
    {
        showImage();
    }
    else
    {
        int width = ui->Image->pixmap()->rect().width();
        int height = ui->Image->pixmap()->rect().height();
        if(x > width || y > height)
        {
            showImage();
        }
        else
        {
            cv::Mat& img = labImage.image_show;
            cv::circle(img, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), 4);
            if(labImage.needDraw)
            {
                cv::line(img, cv::Point(x, y), labImage.pre_point, cv::Scalar(0, 255, 255), 4);
                labImage.num_semantic_lines++;
                labImage.lines.push_back(mLine(x, y, labImage.pre_point.x, labImage.pre_point.y));
                labImage.needDraw = false;
            }
            else
            {
                labImage.needDraw = true;
                labImage.pre_point = cv::Point(x, y);
            }
            showImage();
        }
    }
}

void MainWindow::on_actionopen_folder_triggered()
{
    root_dir = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("path"), QDir::currentPath()));
    record_filename = QDir::toNativeSeparators(root_dir + '/' + "record.txt").toStdString();
    label_filename = QDir::toNativeSeparators(root_dir + '/' + "label.txt").toStdString();
    nameList = getDirImageNames(root_dir);
    totalNums = nameList.size();
    initRecord(record_filename, record);

    char message[1000];
    sprintf(message, "Label save path: %s               total:%d, %d was done!", label_filename.data(), totalNums, record.size());
    QString msg(message);
    ui->statusBar->showMessage(msg);

    on_Next_clicked();

}

void MainWindow::on_Image_clicked()
{
    QPoint pos = ui->Image->pointPos;
    pos = ui->Image->mapFromGlobal(pos);
    int transX = pos.x() - ui->Image->xoffset;
    int transY = pos.y() - ui->Image->yoffset;
    updateImage(transX, transY);
}

void MainWindow::on_Reset_clicked()
{
    labImage.image_show = cv::imread(labImage.abs_path);
    labImage.num_semantic_lines = 0;
    labImage.needDraw = 0;
    labImage.lines.clear();
    showImage();
}

void MainWindow::on_Label_clicked()
{
    addLabel(label_filename, labImage);
    addRecord(record_filename, record, labImage.name);
    char message[1000];
    sprintf(message, "Label save path: %s               total:%d, %d was done!", label_filename.data(), totalNums, record.size());
    QString msg(message);
    ui->statusBar->showMessage(msg);
    ui->Label->setDisabled(true);
    ui->Reset->setDisabled(true);
}

void MainWindow::on_Next_clicked()
{
    std::string filename = getNextFilename(record, nameList);
    if(filename == "")
    {
        QMessageBox msgBox;
        msgBox.setText("All image labels are completed.");
        msgBox.exec();
    }
    else
    {
        labImage.name = filename;
        labImage.abs_path = QDir::toNativeSeparators(root_dir + '/' + QString::fromStdString(filename)).toStdString();
        labImage.image_show = cv::imread(labImage.abs_path);
        labImage.num_semantic_lines = 0;
        labImage.needDraw = 0;
        labImage.lines.clear();

        showImage();

        ui->Label->setDisabled(false);
        ui->Reset->setDisabled(false);
    }

}
