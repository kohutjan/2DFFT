#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QtCharts>

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QtCharts::QChart *chart = new QtCharts::QChart();
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart, ui->analytics_tab);
    ui->analytics_tab->layout()->replaceWidget(ui->ana_temp_label, chartView);

    this->playFilters["Mean"] = this->filterLoader.GetMean("Mean", "mean", 5);
    this->playFilters["Gaussian"] = this->filterLoader.GetGauss("Gaussian", "gauss", 5, 0.1);
    QObject::connect(ui->ana_filter_cbox, static_cast<void (QComboBox::*)(const QString &)> (&QComboBox::currentIndexChanged),
                     this, &MainWindow::comboBoxAnaFilterSelection);
    QObject::connect(ui->ana_min_kernel_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxAnaFilterParamsSelection);
    QObject::connect(ui->ana_max_kernel_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxAnaFilterParamsSelection);
    QObject::connect(ui->ana_step_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxAnaFilterParamsSelection);

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

void MainWindow::on_ana_path_button_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (dir.length() > 0)
    {
        ui->ana_path_label->setText(dir);
        // set Run::dir_selected = true;
    }
}

void MainWindow::on_ana_static_sig_rad_clicked()
{
    ui->ana_sigma_sbox->setEnabled(true);
}

void MainWindow::on_ana_dynamic_sig_rad_clicked()
{
    ui->ana_sigma_sbox->setEnabled(false);
}

void MainWindow::on_ana_run_button_clicked()
{
    // zkontrolovat, jestli byla zvolena cesta
    // zkontrolovat, jestli byl zvoleny filtr
    // zkontrolovat, jestli byl zvoleny aspon jeden druh konvoluce

    // QDirIteratorem vytahnout obrazky z cesty
    // zkontrolovat, jestli tam jsou obrazky
    // ulozit cesty k obrazkum do this->imagePaths

    // nastavit this->iterations, spatial, separable, frequency, min_size, max_size, step_size
    // nacist filtry jako v FilterLoader::LoadFromStream
    // Run::InitFilterStatistics()
    // Run::Start(false)
}

void MainWindow::comboBoxAnaFilterSelection(const QString & selection_text)
{
    if (selection_text == "Gaussian")
    {
        ui->ana_static_sig_rad->setEnabled(true);
        ui->ana_dynamic_sig_rad->setEnabled(true);

        if (ui->ana_static_sig_rad->isChecked())
        {
            ui->ana_sigma_sbox->setEnabled(true);
        }
        else
        {
            ui->ana_sigma_sbox->setEnabled(false);
        }

    }
    else
    {
        ui->ana_sigma_sbox->setEnabled(false);
        ui->ana_static_sig_rad->setEnabled(false);
        ui->ana_dynamic_sig_rad->setEnabled(false);
    }
}

void MainWindow::on_playFiltersCombo_currentIndexChanged(const QString &arg1)
{
    Mat filter = this->playFilters[arg1.toStdString()].getValues();
    int scaleW = ui->play2DFilter->width();
    int scaleH = ui->play2DFilter->height();
    int maxScale = max(scaleW, scaleH);
    cv::resize(filter, filter, Size(maxScale, maxScale));
}

void MainWindow::spinBoxAnaFilterParamsSelection()
{
    int min = ui->ana_min_kernel_size_sbox->value();
    int max = ui->ana_max_kernel_size_sbox->value();
    int step = ui->ana_step_size_sbox->value();

    if(max < min)
    {
        ui->ana_max_kernel_size_sbox->setValue(min);
        max = min;
    }

    if ((min%2) == 0)
    {
        ui->ana_min_kernel_size_sbox->setValue(min - 1);
    }

    if ((max%2) == 0)
    {
        ui->ana_max_kernel_size_sbox->setValue(max - 1);
    }

    if ((step%2) != 0)
    {
        ui->ana_step_size_sbox->setValue(step - 1);
    }

    if (((max != min) && (step == 0)))
    {
        ui->ana_step_size_sbox->setValue(1);
    }
    else if (step > (max - min))
    {
        ui->ana_step_size_sbox->setValue(max - min);
    }
}
