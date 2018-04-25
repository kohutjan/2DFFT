#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "../include/run.hpp"
#include "../include/filter_loader.hpp"
#include <map>
#include <string>
#include <iostream>

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

private:
    Ui::MainWindow *ui;
    FilterLoader filterLoader;
    std::map<std::string, Filter> playFilters;
    void comboBoxAnaFilterSelection(const QString& selection_text);
    void spinBoxAnaFilterParamsSelection();
};

#endif // MAINWINDOW_HPP
