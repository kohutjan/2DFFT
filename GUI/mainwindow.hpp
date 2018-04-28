#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLabel>
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
    // --- Playground start ---
    void playSetFilter();
    void playShowFilter();
    void playFilterParamsChanged();
    void playKernelSizeSelection();
    std::string playInputPath;
    cv::Mat playInputImg;
    cv::Mat playFilter;
    int playBorderSize;
    // --- Playground end ---
    // --- Analytics start ---
    Run *analytics_run;
    void comboBoxAnaFilterSelection(const QString& selection_text);
    void spinBoxAnaFilterParamsSelection();
    void on_change_pic_button_clicked();
    void comboBoxFilterSelection(const QString& selection_text);
    void spinBoxFilterParamsSelection();
    // --- Analytics end ---
    // --- Helpers ---
    FilterLoader filterLoader;
    SpatialConvolution spatialConvolution;
    FrequencyConvolution frequencyConvolution;
    QImage ConvertOpenCVToQtImg(cv::Mat input);
    cv::Mat GetSpectrumImg(cv::Mat input);
    cv::Mat GetSpectrumImg(cv::Mat filter, cv::Mat img);
    void RearrangeSpectrum(cv::Mat magI);
    cv::Mat SpectrumMagnitude(cv::Mat specCplx);
    void SetPipeToLabel(QLabel *label, QString pathToImg);
    void SetImgToLabel(QLabel *label, cv::Mat img);
    void SetPixmapToLabel(QLabel *label, QPixmap &pixmap);

};

#endif // MAINWINDOW_HPP
