#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include <QDir>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QString>
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

void MainWindow::showStatus()
{
    std::string msg;
    char iterMsg[200];
    char pointMsg[200];
    sprintf(iterMsg, "%s\n%d / %d\n", labImage.name.data(), iterpos - labeledNums + 1, totalNums);
    msg += iterMsg;
    for (int i = 0; i < labImage.lines.size(); ++i) {
        sprintf(pointMsg, "%d %d %d %d\n", labImage.lines[i].x1, labImage.lines[i].y1, labImage.lines[i].x2, labImage.lines[i].y2);
        msg += std::string(pointMsg);
    }
    ui->TextLabel->setText(QString::fromStdString(msg));
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
//    label_filename = QDir::toNativeSeparators(root_dir + '/' + "label.txt").toStdString();
    nameList = getDirImageNames(root_dir);
    iterpos = 0;
    labeledNums = 0;
    totalNums = 0;
    for(int i = 0; i < nameList.size(); ++i)
    {
        if(nameList[i].indexOf("label.jpg") == -1)
            totalNums++;
    }
    initRecord(record_filename, record);

    std::string filename = getFirstFilename(nameList);
    if(filename == "")
    {
        QMessageBox msgBox;
        msgBox.setText("No images.");
        msgBox.exec();
    }
    else
    {
        labImage.name = filename;
        labImage.abs_path = QDir::toNativeSeparators(root_dir + '/' + QString::fromStdString(filename)).toStdString();
        labImage.needDraw = 0;
        labImage.lines.clear();
        if (record.find(filename) != record.end())
        {
            labImage.image_show = cv::imread(labImage.abs_path + "label.jpg");
            loadImageLabel(labImage);
        }
        else
        {
            labImage.image_show = cv::imread(labImage.abs_path);
            labImage.num_semantic_lines = 0;
        }


        showImage();
        showStatus();
    }
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
    addLabel(labImage);
    cv::imwrite(labImage.abs_path + "label.jpg", labImage.image_show);
    eraseRecord(record_filename, labImage.name);
    showImage();
    showStatus();
}

void MainWindow::on_Label_clicked()
{
    addLabel(labImage);
    addRecord(record_filename, record, labImage.name);
    cv::imwrite(labImage.abs_path + "label.jpg", labImage.image_show);
    char message[1000];
    sprintf(message, "Label save path: %s               total:%d, %d was done!", labImage.abs_path.replace(labImage.abs_path.end()-4, labImage.abs_path.end(), ".txt").data(), totalNums, record.size());
    QString msg(message);
    ui->statusBar->showMessage(msg);
    showStatus();
}

void MainWindow::on_Next_clicked()
{   
    std::string filename = getNextFilename(nameList, iterpos, labeledNums);

    if (filename != "")
    {
        labImage.name = filename;
        labImage.abs_path = QDir::toNativeSeparators(root_dir + '/' + QString::fromStdString(filename)).toStdString();
        labImage.needDraw = 0;
        labImage.lines.clear();
        if (record.find(filename) != record.end())
        {
            labImage.image_show = cv::imread(labImage.abs_path + "label.jpg");
            loadImageLabel(labImage);
        }
        else
        {
            labImage.image_show = cv::imread(labImage.abs_path);
            labImage.num_semantic_lines = 0;
        }
        showImage();
        showStatus();
    }
}


void MainWindow::on_Prev_clicked()
{
    std::string filename = getPrevFilename(nameList, iterpos, labeledNums);
    if (filename != "")
    {
        labImage.name = filename;
        labImage.abs_path = QDir::toNativeSeparators(root_dir + '/' + QString::fromStdString(filename)).toStdString();
        labImage.needDraw = 0;
        labImage.lines.clear();
        if (record.find(filename) != record.end())
        {
            labImage.image_show = cv::imread(labImage.abs_path + "label.jpg");
            loadImageLabel(labImage);
        }
        else
        {
            labImage.image_show = cv::imread(labImage.abs_path);
            labImage.num_semantic_lines = 0;
        }
        showImage();
        showStatus();
    }

}

void MainWindow::on_Todo_clicked()
{
    std::string filename = getCurrFilename(nameList, record, iterpos, labeledNums);
    if (filename != "")
    {
        labImage.name = filename;
        labImage.abs_path = QDir::toNativeSeparators(root_dir + '/' + QString::fromStdString(filename)).toStdString();
        labImage.needDraw = 0;
        labImage.lines.clear();
        if (record.find(filename) != record.end())
        {
            labImage.image_show = cv::imread(labImage.abs_path + "label.jpg");
            loadImageLabel(labImage);
        }
        else
        {
            labImage.image_show = cv::imread(labImage.abs_path);
            labImage.num_semantic_lines = 0;
        }
        showImage();
        showStatus();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("All iamge labeled done.");
        msgBox.exec();
    }
}
