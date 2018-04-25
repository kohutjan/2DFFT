#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->playFilters["Mean"] = this->filterLoader.GetMean("Mean", "mean", 5);
    this->playFilters["Gaussian"] = this->filterLoader.GetGauss("Gaussian", "gauss", 5, 0.1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playLoadImg_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec())
      fileNames = dialog.selectedFiles();
      QPixmap pixmap(fileNames[0]);
      int scaleW = ui->playInputImg->width();
      int scaleH = ui->playInputImg->height();
      ui->playInputImg->setScaledContents(true);
      ui->playInputImg->setPixmap(pixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatio));
}



void MainWindow::on_playFiltersCombo_currentIndexChanged(const QString &arg1)
{
    Mat filter = this->playFilters[arg1.toStdString()].getValues();
    int scaleW = ui->play2DFilter->width();
    int scaleH = ui->play2DFilter->height();
    int maxScale = max(scaleW, scaleH);
    cv::resize(filter, filter, Size(maxScale, maxScale));

}

