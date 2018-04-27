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
    // --- Playground start ---
    this->playFilters["Mean"] = this->filterLoader.GetMean("Mean", "mean", 155);
    this->playFilters["Gaussian"] = this->filterLoader.GetGauss("Gaussian", "gauss", 155, 1);
    ui->playForward->setEnabled(false);
    // --- Playground end ---
    // --- Analytics start ---
    QObject::connect(ui->filter_cbox, static_cast<void (QComboBox::*)(const QString &)> (&QComboBox::currentIndexChanged),
                     this, &MainWindow::comboBoxFilterSelection);
    QObject::connect(ui->min_kernel_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxFilterParamsSelection);
    QObject::connect(ui->max_kernel_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxFilterParamsSelection);
    QObject::connect(ui->step_size_sbox, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxFilterParamsSelection);
    // --- Analytics end ---
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- Playground start ---

void MainWindow::on_playLoadImg_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec())
    {
      // Show input image
      fileNames = dialog.selectedFiles();
      this->playInputPath = fileNames[0].toStdString();
      QPixmap inputPixmap(fileNames[0]);
      int scaleW = ui->play2DInput->width();
      int scaleH = ui->play2DInput->height();
      ui->play2DInput->setPixmap(inputPixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatio));
      // Create spectrum of input image and show it
      Mat inputImg = imread(this->playInputPath, CV_LOAD_IMAGE_GRAYSCALE);
      Mat inputSpectrumImg = this->GetSpectrumImg(inputImg);
      QImage inputSpectrumQtImg = this->ConvertOpenCVToQtImg(inputSpectrumImg);
      QPixmap inputSpectrumPixmap = QPixmap::fromImage(inputSpectrumQtImg);
      ui->playSpecInput->setPixmap(inputSpectrumPixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatio));
      if (not ui->playFiltersCombo->currentText().toStdString().empty())
      {
          ui->playForward->setEnabled(true);
      }
    }
}

void MainWindow::on_playFiltersCombo_currentIndexChanged(const QString &arg1)
{
    if (not arg1.toStdString().empty())
    {
        // Show filter
        Mat filter = this->playFilters[arg1.toStdString()].getValues();
        int scaleW = ui->play2DFilter->width();
        int scaleH = ui->play2DFilter->height();
        Mat filterGray;
        normalize(filter, filterGray, 0, 1, CV_MINMAX);
        filterGray.convertTo(filterGray, CV_8U, 255);
        QImage img = this->ConvertOpenCVToQtImg(filterGray);
        QPixmap pixmap = QPixmap::fromImage(img);
        ui->play2DFilter->setPixmap(pixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatio));
        // Create spectrum of filter and show it
        Mat filterSpectrumImg = this->GetSpectrumImg(filter);
        QImage filterSpectrumQtImg = this->ConvertOpenCVToQtImg(filterSpectrumImg);
        QPixmap filterSpectrumPixmap = QPixmap::fromImage(filterSpectrumQtImg);
        ui->playSpecFilter->setPixmap(filterSpectrumPixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatio));
        if (not this->playInputPath.empty())
        {
          ui->playForward->setEnabled(true);
        }
    }
    else
    {
        ui->playForward->setEnabled(false);
    }
}

void MainWindow::on_playForward_clicked()
{
    string filterName = ui->playFiltersCombo->currentText().toStdString();
    if (not (filterName.empty() || this->playInputPath.empty()))
    {
        Filter filter = this->playFilters[filterName];
        //Prepare spatial convolution
        Mat flipedFilter = filter.getValues();
        flip(filter.getValues(), flipedFilter, -1);
        Mat src = imread(this->playInputPath, CV_LOAD_IMAGE_GRAYSCALE);
        Mat dst = src.clone();
        Mat srcF;
        Mat dstF;
        src.convertTo(srcF, CV_32FC1);
        dst.convertTo(dstF, CV_32FC1);
        spatialConvolution.setData(srcF, dstF, flipedFilter);
        spatialConvolution.OpenCVRegular();
        dstF.convertTo(dst, CV_8UC1);
        int scaleW = ui->play2DOutput->width();
        int scaleH = ui->play2DOutput->height();
        // Show ouptut
        QImage outputQtImg = this->ConvertOpenCVToQtImg(dst);
        QPixmap outputPixmap = QPixmap::fromImage(outputQtImg);
        ui->play2DOutput->setPixmap(outputPixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatio));
        // Create spectrum of output and show it
        Mat outputSpectrumImg = this->GetSpectrumImg(dst);
        QImage outputSpectrumQtImg = this->ConvertOpenCVToQtImg(outputSpectrumImg);
        QPixmap outputSpectrumPixmap = QPixmap::fromImage(outputSpectrumQtImg);
        ui->playSpecOutput->setPixmap(outputSpectrumPixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatio));
    }
}

// --- Playground end ---

// --- Analytics start ---

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


// --- Analytics end ---


// --- Helper methods ---


QImage MainWindow::ConvertOpenCVToQtImg(cv::Mat input)
{
    QVector<QRgb> colorTable;
    for (int i=0; i<256; i++)
        colorTable.push_back(qRgb(i,i,i));
    const uchar *qImageBuffer = (const uchar*)input.data;
    QImage img(qImageBuffer, input.cols, input.rows, input.step, QImage::Format_Indexed8);
    img.setColorTable(colorTable);
    return img;
}

Mat MainWindow::GetSpectrumImg(Mat input)
{
    Mat spectrum;
    Size dftSize;
    dftSize.width = getOptimalDFTSize(input.cols);
    dftSize.height = getOptimalDFTSize(input.rows);
    Mat inputPadded;
    inputPadded = Mat::zeros(dftSize, CV_32FC1);
    input.convertTo(inputPadded(Rect(0, 0, input.cols, input.rows)), inputPadded.type());
    dft(inputPadded, spectrum, DFT_COMPLEX_OUTPUT, input.rows);
    this->RearrangeSpectrum(spectrum);
    Mat spectrumImg;
    this->SpectrumMagnitude(spectrum).convertTo(spectrumImg, CV_8UC1, 255);
    return spectrumImg;
}

void MainWindow::RearrangeSpectrum(Mat magI)
{
    int cx = magI.cols/2;
    int cy = magI.rows/2;
    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

//ZPO cv3
Mat MainWindow::SpectrumMagnitude(Mat specCplx)
{
    Mat specMag, planes[2];
    split(specCplx, planes);
    magnitude(planes[0], planes[1], planes[0]);
    log( (planes[0] + cv::Scalar::all(1)), specMag );
    normalize( specMag, specMag, 0, 1, CV_MINMAX );
    return specMag;
}



