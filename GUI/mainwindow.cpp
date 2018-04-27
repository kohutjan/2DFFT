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
    // --- Playground start ---
    this->playFilters["Mean"] = this->filterLoader.GetMean("Mean", "mean", 5);
    this->playFilters["Gaussian"] = this->filterLoader.GetGauss("Gaussian", "gauss", 5, 1);
    ui->playForward->setEnabled(false);
    // --- Playground end ---
    
    // --- Analytics start ---
    QtCharts::QChart *chart = new QtCharts::QChart();
    QFont font;
    font.setPixelSize(18);
    chart->setTitleFont(font);
    chart->setTitleBrush(QBrush(Qt::black));
    chart->setTitle("Computation time in regards to kernel size chart");

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();

    QFont labelsFont;
    labelsFont.setPixelSize(12);
    axisX->setLabelsFont(labelsFont);
    axisY->setLabelsFont(labelsFont);

    QPen axisPen(Qt::black);
    axisPen.setWidth(2);
    axisX->setLinePen(axisPen);
    axisY->setLinePen(axisPen);

    QBrush axisBrush(Qt::black);
    axisX->setTitleText("Kernel size [-]");
    axisX->setLabelsBrush(axisBrush);
    axisX->setGridLineVisible(false);
    axisX->setLabelFormat("%d");

    axisY->setTitleText("Computation time [s]");
    axisY->setLabelsBrush(axisBrush);
    axisY->setGridLineVisible(false);
    axisY->setLabelFormat("%0.10e");

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart, ui->analytics_tab);
    chartView->setObjectName("ana_chartview");
    ui->analytics_tab->layout()->replaceWidget(ui->ana_temp_label, chartView);

    this->playFilters["Mean"] = this->filterLoader.GetMean("Mean", "mean", 5);
    this->playFilters["Gaussian"] = this->filterLoader.GetGauss("Gaussian", "gauss", 5, 0.1);
    QObject::connect(ui->ana_filter_combo, static_cast<void (QComboBox::*)(const QString &)> (&QComboBox::currentIndexChanged),
                     this, &MainWindow::comboBoxAnaFilterSelection);
    QObject::connect(ui->ana_min_kernel_size_spin, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxAnaFilterParamsSelection);
    QObject::connect(ui->ana_max_kernel_size_spin, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxAnaFilterParamsSelection);
    QObject::connect(ui->ana_step_size_spin, &QAbstractSpinBox::editingFinished, this, &MainWindow::spinBoxAnaFilterParamsSelection);
    analytics_run = new Run();
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

void MainWindow::on_ana_path_button_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(0, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (dir.length() > 0)
    {
        ui->ana_path_label->setText(dir);
    }
}

void MainWindow::on_ana_static_sig_rad_clicked()
{
    ui->ana_sigma_spin->setEnabled(true);
}

void MainWindow::on_ana_dynamic_sig_rad_clicked()
{
    ui->ana_sigma_spin->setEnabled(false);
}

void MainWindow::on_ana_run_button_clicked()
{
    //ui->ana_menu_container->setEnabled(false);
    QDirIterator *imageIterator = new QDirIterator(ui->ana_path_label->text());
    if (!imageIterator->hasNext())
    {
        ui->ana_error_out_label->setStyleSheet("QLabel { color: red }");
        ui->ana_error_out_label->setText("Error: Please choose an existing directory for the set.");
        return;
    }
    else
    {
        ui->ana_error_out_label->setText("");
    }

    bool imageFound = false;
    while (imageIterator->hasNext())
    {
        QString suffix = imageIterator->fileInfo().suffix();
        if (QString::compare(suffix, "png", Qt::CaseInsensitive) == 0 ||
                QString::compare(suffix, "jpg", Qt::CaseInsensitive) == 0 ||
                QString::compare(suffix, "jpeg", Qt::CaseInsensitive) == 0 ||
                QString::compare(suffix, "bmp", Qt::CaseInsensitive) == 0)
        {
            QString q_path = imageIterator->filePath();
            string std_path = q_path.toUtf8().constData();
            analytics_run->AddImagePath(std_path);
            imageFound = true;
        }
        imageIterator->next();
    }

    if (imageFound == false)
    {
        ui->ana_error_out_label->setStyleSheet("QLabel { color: red }");
        ui->ana_error_out_label->setText("Error: Please choose a set directory with at least one image.");
        return;
    }
    else
    {
        ui->ana_error_out_label->setText("");
    }

    bool reg_con = false;
    bool sep_con = false;
    bool fr_con = false;

    if (ui->ana_reg_spatial_con_check->isChecked())
    {
        reg_con = true;
    }

    if (ui->ana_sep_spatial_con_check->isChecked())
    {
        sep_con = true;
    }

    if (ui->ana_frequential_con_check->isChecked())
    {
        fr_con = true;
    }

    if (!(reg_con || sep_con || fr_con))
    {
        ui->ana_error_out_label->setStyleSheet("QLabel { color: red }");
        ui->ana_error_out_label->setText("Error: Please choose at least one convolution type.");
        return;
    }
    else
    {
        analytics_run->SetConvolutions(reg_con, sep_con, fr_con);
        ui->ana_error_out_label->setText("");
    }

    QString filter_name;
    int filter_type;
    int sigma;
    if (ui->ana_filter_combo->currentIndex() == 0)
    {
        ui->ana_error_out_label->setStyleSheet("QLabel { color: red }");
        ui->ana_error_out_label->setText("Error: Please choose a filter.");
        return;
    }
    else
    {
        filter_name = ui->ana_filter_combo->currentText();

        if (QString::compare(filter_name, "Gaussian") == 0)
        {
            filter_type = GAUSS;

            if(ui->ana_static_sig_rad->isChecked())
            {
                sigma = ui->ana_sigma_spin->value();
            }
            else
            {
                sigma = -1;
            }
        }
        else if (QString::compare(filter_name, "Mean") == 0)
        {
            filter_type = MEAN;
        }

        ui->ana_error_out_label->setText("");
    }

    int iterations = ui->ana_iters_p_step_spin->value();
    analytics_run->SetIterations(iterations);

    int min_size = ui->ana_min_kernel_size_spin->value();
    int max_size = ui->ana_max_kernel_size_spin->value();
    int step_size = ui->ana_step_size_spin->value();

    ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->removeAllSeries();

    FilterLoader *filter_loader = new FilterLoader();
    QLineSeries *regular = new QLineSeries;
    QLineSeries *separable = new QLineSeries;
    QLineSeries *frequential = new QLineSeries;

    QScatterSeries *regular_scatter = new QScatterSeries;
    QScatterSeries *separable_scatter = new QScatterSeries;
    QScatterSeries *frequential_scatter = new QScatterSeries;

    if (max_size == min_size)
    {
        if (reg_con)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(regular_scatter);
            regular_scatter->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            regular_scatter->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            regular_scatter->setName("regular spatial convolution");
        }

        if (sep_con)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(separable_scatter);
            separable_scatter->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            separable_scatter->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            separable_scatter->setName("separable spatial convolution");
        }

        if (fr_con)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(frequential_scatter);
            frequential_scatter->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            frequential_scatter->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            frequential_scatter->setName("frequential spatial convolution");
        }

        ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX()->setRange(max_size - 2, max_size + 2);
    }
    else
    {
        if (reg_con)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(regular);
            regular->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            regular->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            regular->setName("regular spatial convolution");
        }

        if (sep_con)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(separable);
            separable->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            separable->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            separable->setName("separable spatial convolution");
        }

        if (fr_con)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(frequential);
            frequential->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            frequential->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            frequential->setName("frequential spatial convolution");
        }

        ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX()->setRange(min_size, max_size);
    }

    if (step_size == 0) // zarazka, pokud je 0, tak min_size == max_size a for by cyklil nekonecne
    {
        step_size = 1;
    }

    ui->ana_error_out_label->setStyleSheet("QLabel { color: green }");
    ui->ana_error_out_label->setText("Info: Run started.");

    string filter_name_std = filter_name.toUtf8().constData();
    vector<string> imagePaths = analytics_run->GetImagePaths();
    double maxDuration = 0.0;
    for (int i = min_size; i <= max_size; i += step_size)
    {
        std::map<std::string, Filter> filters;
        std::vector<std::string> filtersInsertOrder;
        Filter filter;
        if (filter_type == GAUSS)
        {
            filter = filter_loader->GetGauss(filter_name_std, filter_name_std, i, sigma);
        }
        else if (filter_type == MEAN)
        {
            filter = filter_loader->GetMean(filter_name_std, filter_name_std, i);
        }

        filters[filter_name_std] = filter;
        filtersInsertOrder.push_back(filter_name_std);
        analytics_run->setFilters(filters, filtersInsertOrder);
        analytics_run->InitFilterStatistics();
        analytics_run->Start(false); //nastavit na true pro kontrolu vysledku filtrace

        if (reg_con)
        {
            double meanDuration = 0.0;
            for (auto& imagePath: imagePaths)
            {
              meanDuration += analytics_run->statistics[filter_name_std].spatialDurations[imagePath].count() / iterations;
            }
            meanDuration = meanDuration / imagePaths.size();

            if (max_size == min_size)
            {
                regular_scatter->append(i, meanDuration);
            }
            else
            {
                regular->append(i, meanDuration);
            }

            if (maxDuration < meanDuration)
            {
                maxDuration = meanDuration;
            }
        }

        if (sep_con)
        {
            double meanDuration = 0.0;
            for (auto& imagePath: imagePaths)
            {
              meanDuration += analytics_run->statistics[filter_name_std].separableDurations[imagePath].count() / iterations;
            }
            meanDuration = meanDuration / imagePaths.size();

            if (max_size == min_size)
            {
                separable_scatter->append(i, meanDuration);
            }
            else
            {
                separable->append(i, meanDuration);
            }

            if (maxDuration < meanDuration)
            {
                maxDuration = meanDuration;
            }
        }

        if (fr_con)
        {
            double meanDuration = 0.0;
            for (auto& imagePath: imagePaths)
            {
                meanDuration += analytics_run->statistics[filter_name_std].frequentialDurations[imagePath].count() / iterations;
            }
            meanDuration = meanDuration / imagePaths.size();

            if (max_size == min_size)
            {
                frequential_scatter->append(i, meanDuration);
            }
            else
            {
                frequential->append(i, meanDuration);
            }

            if (maxDuration < meanDuration)
            {
                maxDuration = meanDuration;
            }
        }

        ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY()->setRange(0, maxDuration * 1.5);
    }

    ui->ana_error_out_label->setText("Info: Run finished.");
    //ui->ana_menu_container->setEnabled(true);
}

void MainWindow::comboBoxAnaFilterSelection(const QString & selection_text)
{
    if (selection_text == "Gaussian")
    {
        ui->ana_static_sig_rad->setEnabled(true);
        ui->ana_dynamic_sig_rad->setEnabled(true);

        if (ui->ana_static_sig_rad->isChecked())
        {
            ui->ana_sigma_spin->setEnabled(true);
        }
        else
        {
            ui->ana_sigma_spin->setEnabled(false);
        }

    }
    else
    {
        ui->ana_sigma_spin->setEnabled(false);
        ui->ana_static_sig_rad->setEnabled(false);
        ui->ana_dynamic_sig_rad->setEnabled(false);
    }
}



void MainWindow::spinBoxAnaFilterParamsSelection()
{
    int min = ui->ana_min_kernel_size_spin->value();
    int max = ui->ana_max_kernel_size_spin->value();
    int step = ui->ana_step_size_spin->value();

    if ((min%2) == 0)
    {
        min = min - 1;
        ui->ana_min_kernel_size_spin->setValue(min);
    }

    if ((max%2) == 0)
    {
        max = max - 1;
        ui->ana_max_kernel_size_spin->setValue(max);
    }

    if(max < min)
    {
        ui->ana_max_kernel_size_spin->setValue(min);
        max = min;
    }

    if ((step%2) != 0)
    {
        step = step - 1;
        ui->ana_step_size_spin->setValue(step);
    }

    if (((max != min) && (step == 0)))
    {
        ui->ana_step_size_spin->setValue(2);
    }
    else if (step > (max - min))
    {
        ui->ana_step_size_spin->setValue(max - min);
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



