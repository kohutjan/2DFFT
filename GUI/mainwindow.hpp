#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "../include/run.hpp"
#include "../include/filter_loader.hpp"
#include <map>
#include <string>
#include <iostream>

#define GAUSS 0
#define MEAN 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_playLoadImg_clicked();
    void on_playFiltersCombo_currentIndexChanged(const QString &arg1);
    void on_ana_path_button_clicked();
    void on_ana_run_button_clicked();
    void on_ana_static_sig_rad_clicked();
    void on_ana_dynamic_sig_rad_clicked();
    void on_playForward_clicked();

private:
    Ui::MainWindow *ui;
    FilterLoader filterLoader;
    Run *analytics_run;
    void comboBoxAnaFilterSelection(const QString& selection_text);
    void spinBoxAnaFilterParamsSelection();
    SpatialConvolution spatialConvolution;
    FrequencyConvolution frequencyConvolution;
    std::string playInputPath;
    std::map<std::string, Filter> playFilters;
    void on_change_pic_button_clicked();
    void comboBoxFilterSelection(const QString& selection_text);
    void spinBoxFilterParamsSelection();
    QImage ConvertOpenCVToQtImg(cv::Mat input);
    cv::Mat GetSpectrumImg(cv::Mat input);
    void RearrangeSpectrum(cv::Mat magI);
    cv::Mat SpectrumMagnitude(cv::Mat specCplx);
};

#endif // MAINWINDOW_HPP
