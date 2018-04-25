#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

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
    void on_change_pic_button_clicked();

private:
    Ui::MainWindow *ui;
    void comboBoxFilterSelection(const QString& selection_text);
    void spinBoxFilterParamsSelection();
};

#endif // MAINWINDOW_HPP
