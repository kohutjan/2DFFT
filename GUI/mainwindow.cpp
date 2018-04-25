#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include "../include/run.hpp"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->filter_cbox, static_cast<void (QComboBox::*)(const QString &)> (&QComboBox::currentIndexChanged),
                     this, &MainWindow::comboBoxSelection);
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

void MainWindow::comboBoxSelection(const QString & selection_text)
{
    if (selection_text == "Gaussian")
    {
        ui->sigma_sbox->setEnabled(true);
    }
    else
    {
        ui->sigma_sbox->setEnabled(false);
    }

}

