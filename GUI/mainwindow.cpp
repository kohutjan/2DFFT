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
    ui->play2DFiltersCombo->setEnabled(false);
    ui->playKernelSizeSpin->setEnabled(false);
    ui->playSpecFiltersCombo->setEnabled(false);
    ui->playRadiusSpin->setEnabled(false);
    ui->playForward->setEnabled(false);
    // Set borders
    this->playBorderSize = 5;
    QString borderSizeStr = "border: " + QString::number(this->playBorderSize) + "px solid black";
    ui->play2DInput->setStyleSheet(borderSizeStr);
    ui->playSpecInput->setStyleSheet(borderSizeStr);
    ui->play2DFilter->setStyleSheet(borderSizeStr);
    ui->playSpecFilter->setStyleSheet(borderSizeStr);
    ui->play2DOutput->setStyleSheet(borderSizeStr);
    ui->playSpecOutput->setStyleSheet(borderSizeStr);
    // Set Pipes
    QString pathToImgs =  QDir::currentPath() + "/images/";
    QString greenStar = pathToImgs + "green_star.png";
    QString greenEqual = pathToImgs + "green_equal.png";
    QString redDown = pathToImgs + "red_down.png";
    QString redMul = pathToImgs + "red_mul.png";
    QString redEqual = pathToImgs + "red_equal";
    QString redUp = pathToImgs + "red_up.png";
    this->SetPipeToLabel(ui->play2DInputFilterPipe, greenStar);
    this->SetPipeToLabel(ui->play2DFilterOutputPipe, greenEqual);
    this->SetPipeToLabel(ui->playSpecInputFilterPipe, redMul);
    this->SetPipeToLabel(ui->playSpecFilterOutputPipe, redEqual);
    this->SetPipeToLabel(ui->play2DInputSpecInputPipe, redDown);
    this->SetPipeToLabel(ui->play2DFilterSpecFilterPipe, redDown);
    this->SetPipeToLabel(ui->play2DOutputSpecOutputPipe, redUp);
    QObject::connect(ui->playKernelSizeSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::playKernelSizeSelection);
    QObject::connect(ui->playRadiusSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::playRadiusSelection);
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

    axisY->setTitleText("Computation time [ms]");
    axisY->setLabelsBrush(axisBrush);
    axisY->setGridLineVisible(false);
    axisY->setLabelFormat("%0.3e");

    chart->setAxisX(axisX);
    chart->setAxisY(axisY);

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart, ui->analytics_tab);
    chartView->setObjectName("ana_chartview");
    ui->analytics_tab->layout()->replaceWidget(ui->ana_temp_label, chartView);
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
      this->SetPixmapToLabel(ui->play2DInput, inputPixmap);
      // Create spectrum of input image and show it
      this->playInputImg = imread(this->playInputPath, CV_LOAD_IMAGE_GRAYSCALE);
      Mat inputSpectrumImg = this->GetSpectrumImg(this->playInputImg);
      this->SetImgToLabel(ui->playSpecInput, inputSpectrumImg);
      if (ui->playSpecFiltersCombo->currentText().isEmpty())
      {
          ui->play2DFiltersCombo->setEnabled(true);
      }
      if (ui->play2DFiltersCombo->currentText().isEmpty())
      {
          ui->playSpecFiltersCombo->setEnabled(true);
      }
    }
}

void MainWindow::playSetFilter()
{
    String filter2DType = ui->play2DFiltersCombo->currentText().toStdString();
    String filterSpecType = ui->playSpecFiltersCombo->currentText().toStdString();
    if (not filter2DType.empty())
    {
        int kernelSize = ui->playKernelSizeSpin->value();
        this->playFilter = this->filterLoader.Get2DFilter(filter2DType, filter2DType, kernelSize).getValues();
        if (filter2DType == "SobelX" || filter2DType == "SobelY")
        {
            ui->playKernelSizeSpin->setValue(3);
        }
        if (filter2DType == "DoG" && kernelSize == 3)
        {
            ui->playKernelSizeSpin->setValue(5);
        }
    }
    if (not filterSpecType.empty())
    {
        int radius = ui->playRadiusSpin->value();
        Filter filter = this->filterLoader.GetSpecFilter(filterSpecType, filterSpecType, radius, this->playInputImg);
        this->playFilter = filter.getValues();
    }

}

void MainWindow::playShowFilter()
{
    Mat filterSpectrumImg = this->GetSpectrumImg(this->playFilter, this->playInputImg);
    this->SetImgToLabel(ui->playSpecFilter, filterSpectrumImg);
    // Convert filter so it can be shown
    Mat filterGray;
    double min, max;
    minMaxLoc(this->playFilter, &min, &max);
    if (abs(min - max) < 0.000001)
    {
        this->playFilter.convertTo(filterGray, CV_32F, 0, 0.5);
    }
    else
    {
        normalize(this->playFilter, filterGray, 0, 1, CV_MINMAX);
    }
    filterGray.convertTo(filterGray, CV_8U, 255);
    // Set the filter height to the height of it's spectrum
    QImage filterQtImg = this->ConvertOpenCVToQtImg(filterGray);
    QPixmap filterPixmap = QPixmap::fromImage(filterQtImg);
    int scaleFilterW = ui->playSpecFilter->pixmap()->width();
    int scaleFilterH = ui->playSpecFilter->pixmap()->height();
    ui->play2DFilter->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->play2DFilter->setPixmap(filterPixmap.scaled(scaleFilterW, scaleFilterH, Qt::KeepAspectRatio));
}

void MainWindow::play2DFilterParamsChanged()
{
    String filterType = ui->play2DFiltersCombo->currentText().toStdString();
    if (not filterType.empty())
    {
        this->playSetFilter();
        this->playShowFilter();
        ui->playKernelSizeSpin->setEnabled(true);
        ui->playForward->setEnabled(true);
        ui->playSpecFiltersCombo->setEnabled(false);
    }
    else
    {
        ui->playKernelSizeSpin->setEnabled(false);
        ui->playForward->setEnabled(false);
        ui->playSpecFiltersCombo->setEnabled(true);
    }
}

void MainWindow::on_play2DFiltersCombo_currentIndexChanged(const QString &arg1)
{
    this->play2DFilterParamsChanged();
}

void MainWindow::playSpecFilterParamsChanged()
{
    String filterType = ui->playSpecFiltersCombo->currentText().toStdString();
    if (not filterType.empty())
    {
        this->playSetFilter();
        this->playShowFilter();
        ui->playRadiusSpin->setEnabled(true);
        ui->playForward->setEnabled(true);
        ui->play2DFiltersCombo->setEnabled(false);
    }
    else
    {
        ui->playRadiusSpin->setEnabled(false);
        ui->playForward->setEnabled(false);
        ui->play2DFiltersCombo->setEnabled(true);
    }
}

void MainWindow::on_playSpecFiltersCombo_currentIndexChanged(const QString &arg1)
{
    this->playSpecFilterParamsChanged();
}

void MainWindow::playKernelSizeSelection()
{
    int kernelSize = ui->playKernelSizeSpin->value();
    if ((kernelSize % 2) == 0)
    {
        --kernelSize;
        ui->playKernelSizeSpin->setValue(kernelSize);
    }
    this->play2DFilterParamsChanged();
}

void MainWindow::playRadiusSelection()
{
    this->playSpecFilterParamsChanged();
}

void MainWindow::on_playForward_clicked()
{
    string convolutionType = ui->playConvolutionType->currentText().toStdString();
    // Prepare spatial convolution
    Mat srcF;
    Mat dstF;
    Mat src = this->playInputImg.clone();
    Mat dst = this->playInputImg.clone();
    src.convertTo(srcF, CV_32FC1);
    dst.convertTo(dstF, CV_32FC1);
    if (convolutionType == "OpenCV")
    {
        Mat flipedFilter = this->playFilter.clone();
        flip(this->playFilter, flipedFilter, -1);
        spatialConvolution.setData(srcF, dstF, flipedFilter);
        spatialConvolution.OpenCVRegular();
    }
    else
    {
        spatialConvolution.setData(srcF, dstF, this->playFilter);
        spatialConvolution.Regular();
    }
    normalize(dstF, dstF, 0, 1, CV_MINMAX);
    dstF.convertTo(dst, CV_8UC1, 255);
    // Show ouptut
    this->SetImgToLabel(ui->play2DOutput, dst);
    // Create spectrum of output and show it
    Mat outputSpectrumImg = this->GetSpectrumImg(dst);
    this->SetImgToLabel(ui->playSpecOutput, outputSpectrumImg);
    // Measure times
    string filter2DType = ui->play2DFiltersCombo->currentText().toStdString();
    string filterSpecType = ui->playSpecFiltersCombo->currentText().toStdString();
    Filter filter;
    string filterType;
    if (not filter2DType.empty())
    {
        int kernelSize = ui->playKernelSizeSpin->value();
        filter = filterLoader.Get2DFilter(filter2DType, filter2DType, kernelSize);
        filterType = filter2DType;
    }
    if (not filterSpecType.empty())
    {
        int radius = ui->playRadiusSpin->value();
        filter = this->filterLoader.GetSpecFilter(filterSpecType, filterSpecType, radius, this->playInputImg);
        filterType = filterSpecType;
    }
    map<string, Filter> filters;
    filters[filterType] = filter;
    vector<string> filtersInsertOrder(1, filterType);
    Run run;
    if (convolutionType == "OpenCV")
    {
        run.SetConvolutions(false, false, true);
        run.SetOpenCVConvolutions(true, true);
    }
    else
    {
        run.SetConvolutions(true, true, true);
    }
    int numberOfIterations = 5;
    run.SetIterations(numberOfIterations);
    run.AddImagePath(this->playInputPath);
    run.setFilters(filters, filtersInsertOrder);
    run.InitFilterStatistics();
    run.Start();
    FilterStatistic statistic = run.statistics[filterType];
    double spatialDuration;
    if (convolutionType == "OpenCV")
    {
        spatialDuration = statistic.openCVFilter2DDurations[this->playInputPath].count() / numberOfIterations;
    }
    else
    {
        spatialDuration = statistic.spatialDurations[this->playInputPath].count() / numberOfIterations;
    }
    double FFTImgDuration = statistic.FFTImgDurations[this->playInputPath].count() / numberOfIterations;
    double FFTFilterDuration = statistic.FFTFilterDurations[this->playInputPath].count() / numberOfIterations;
    double MULDuration = statistic.MULDurations[this->playInputPath].count() / numberOfIterations;
    double IFFTDuration = statistic.IFFTDurations[this->playInputPath].count() / numberOfIterations;
    double FFTDuration = statistic.frequencyDurations[this->playInputPath].count() / numberOfIterations;
    QString separableDuration;
    if (convolutionType == "OpenCV")
    {
        if (not statistic.openCVSeparableDurations.empty())
        {
            separableDuration = QString::number(statistic.openCVSeparableDurations[this->playInputPath].count() / numberOfIterations) + " ms";
        }
        else
        {
            separableDuration = "inseparable";
        }
    }
    else
    {
        if (not statistic.separableDurations.empty())
        {
            separableDuration = QString::number(statistic.separableDurations[this->playInputPath].count() / numberOfIterations) + " ms";
        }
        else
        {
            separableDuration = "inseparable";
        }
    }
    // Show measured times
    ui->play2DInputFilterTime->setText(QString::number(spatialDuration, 'g', 3));
    ui->play2DInputSpecInputTime->setText(QString::number(FFTImgDuration, 'g', 3));
    ui->play2DFilterSpecFilterTime->setText(QString::number(FFTFilterDuration, 'g', 3));
    ui->playSpecInputFilterTime->setText(QString::number(MULDuration, 'g', 3));
    ui->play2DOutputSpecOutputTime->setText(QString::number(IFFTDuration, 'g', 3));
    ui->play2DTime->setText("Spatial domain: " + QString::number(spatialDuration, 'g', 6) + " ms");
    ui->playSpecTime->setText("Frequential domain: " + QString::number(FFTDuration, 'g', 6) + " ms");
    ui->playSeparableTime->setText("Spatial domain (separable): " + separableDuration);
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
    bool reg_con_cv = false;
    bool sep_con_cv = false;

    if (ui->ana_reg_spatial_con_check->isChecked())
    {
        reg_con = true;
    }

    if (ui->ana_sep_spatial_con_check->isChecked())
    {
        sep_con = true;
    }

    if (ui->ana_reg_spatial_con_check_cv->isChecked())
    {
        reg_con_cv = true;
    }

    if (ui->ana_sep_spatial_con_check_cv->isChecked())
    {
        sep_con_cv = true;
    }

    if (ui->ana_frequential_con_check->isChecked())
    {
        fr_con = true;
    }

    if (!(reg_con || sep_con || fr_con || reg_con_cv || sep_con_cv))
    {
        ui->ana_error_out_label->setStyleSheet("QLabel { color: red }");
        ui->ana_error_out_label->setText("Error: Please choose at least one convolution type.");
        return;
    }
    else
    {
        analytics_run->SetConvolutions(reg_con, sep_con, fr_con);
        analytics_run->SetOpenCVConvolutions(reg_con_cv, sep_con_cv);
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
    QLineSeries *regular_cv = new QLineSeries;
    QLineSeries *separable_cv = new QLineSeries;
    QLineSeries *frequential = new QLineSeries;

    QScatterSeries *regular_scatter = new QScatterSeries;
    QScatterSeries *separable_scatter = new QScatterSeries;
    QScatterSeries *regular_scatter_cv = new QScatterSeries;
    QScatterSeries *separable_scatter_cv = new QScatterSeries;
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

        if (reg_con_cv)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(regular_scatter_cv);
            regular_scatter_cv->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            regular_scatter_cv->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            regular_scatter_cv->setName("Filter2D OpenCV");
        }

        if (sep_con_cv)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(separable_scatter_cv);
            separable_scatter_cv->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            separable_scatter_cv->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            separable_scatter_cv->setName("sepFilter2D OpenCV");
        }

        if (fr_con)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(frequential_scatter);
            frequential_scatter->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            frequential_scatter->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            frequential_scatter->setName("frequency convolution");
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

        if (reg_con_cv)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(regular_cv);
            regular_cv->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            regular_cv->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            regular_cv->setName("Filter2D OpenCV");
        }

        if (sep_con_cv)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(separable_cv);
            separable_cv->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            separable_cv->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            separable_cv->setName("sepFilter2D OpenCV");
        }

        if (fr_con)
        {
            ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->addSeries(frequential);
            frequential->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisX());
            frequential->attachAxis(ui->analytics_tab->findChild<QtCharts::QChartView *>("ana_chartview")->chart()->axisY());
            frequential->setName("frequency convolution");
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
        analytics_run->Start();

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

        if (reg_con_cv)
        {
            double meanDuration = 0.0;
            for (auto& imagePath: imagePaths)
            {
              meanDuration += analytics_run->statistics[filter_name_std].openCVFilter2DDurations[imagePath].count() / iterations;
            }
            meanDuration = meanDuration / imagePaths.size();

            if (max_size == min_size)
            {
                regular_scatter_cv->append(i, meanDuration);
            }
            else
            {
                regular_cv->append(i, meanDuration);
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

        if (sep_con_cv)
        {
            double meanDuration = 0.0;
            for (auto& imagePath: imagePaths)
            {
              meanDuration += analytics_run->statistics[filter_name_std].openCVSeparableDurations[imagePath].count() / iterations;
            }
            meanDuration = meanDuration / imagePaths.size();

            if (max_size == min_size)
            {
                separable_scatter_cv->append(i, meanDuration);
            }
            else
            {
                separable_cv->append(i, meanDuration);
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
                meanDuration += analytics_run->statistics[filter_name_std].frequencyDurations[imagePath].count() / iterations;
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


// --- Helpers ---

void MainWindow::SetPipeToLabel(QLabel *label, QString pathToImg)
{
    QPixmap pixmap(pathToImg);
    int scaleW = label->width();
    int scaleH = label->height();
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setPixmap(pixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}

void MainWindow::SetImgToLabel(QLabel *label, Mat img)
{
    QImage qImg = this->ConvertOpenCVToQtImg(img);
    QPixmap pixmap = QPixmap::fromImage(qImg);
    this->SetPixmapToLabel(label, pixmap);
}

void MainWindow::SetPixmapToLabel(QLabel *label, QPixmap &pixmap)
{
    int scaleW = label->width() - this->playBorderSize * 2;
    int scaleH = label->height() - this->playBorderSize * 2;
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setPixmap(pixmap.scaled(scaleW, scaleH, Qt::KeepAspectRatio));
}

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

Mat MainWindow::GetSpectrumImg(Mat filter, Mat img)
{
    Mat spectrum;
    Size dftSize;
    dftSize.width = getOptimalDFTSize(img.cols);
    dftSize.height = getOptimalDFTSize(img.rows);
    Mat filterPadded;
    filterPadded = Mat::zeros(dftSize, CV_32FC1);
    filter.convertTo(filterPadded(Rect(0, 0, filter.cols, filter.rows)), filterPadded.type());
    dft(filterPadded, spectrum, DFT_COMPLEX_OUTPUT, filter.rows);
    this->RearrangeSpectrum(spectrum);
    Mat spectrumImg;
    this->SpectrumMagnitude(spectrum).convertTo(spectrumImg, CV_8UC1, 255);
    return spectrumImg;
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

