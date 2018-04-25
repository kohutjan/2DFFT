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
                     this, &MainWindow::comboBoxFilterSelection);
    QObject::connect(ui->min_kernel_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxFilterParamsSelection);
    QObject::connect(ui->max_kernel_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxFilterParamsSelection);
    QObject::connect(ui->step_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxFilterParamsSelection);
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

void MainWindow::on_change_pic_button_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (dir.length() > 0)
        ui->change_pic_label->setText(dir);
}

void MainWindow::comboBoxFilterSelection(const QString & selection_text)
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

void MainWindow::spinBoxFilterParamsSelection()
{
    int min = ui->min_kernel_size_sbox->value();
    int max = ui->max_kernel_size_sbox->value();
    int step = ui->step_size_sbox->value();

    if(max < min)
    {
        ui->max_kernel_size_sbox->setValue(min);
        max = min;
    }

    if ((min%2) == 0)
    {
        ui->min_kernel_size_sbox->setValue(min - 1);
    }

    if ((max%2) == 0)
    {
        ui->max_kernel_size_sbox->setValue(max - 1);
    }

    if ((step%2) != 0)
    {
        ui->step_size_sbox->setValue(step - 1);
    }

    if (((max != min) && (step == 0)))
    {
        ui->step_size_sbox->setValue(1);
    }
    else if (step > (max - min))
    {
        ui->step_size_sbox->setValue(max - min);
    }
}
