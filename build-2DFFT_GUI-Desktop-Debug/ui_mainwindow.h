/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *run_layout;
    QLabel *run_err_label;
    QPushButton *run_button;
    QGridLayout *result_layout;
    QLabel *freq_time_label;
    QLabel *regular_time_label;
    QLabel *separ_time_label;
    QLabel *regular_time_result_label;
    QLabel *separ_time_result_label;
    QLabel *freq_time_result_label;
    QGraphicsView *picture_gview;
    QFormLayout *menu_layout;
    QLabel *create_filter_label;
    QPushButton *create_filter_button;
    QLabel *select_filter_label;
    QComboBox *select_filter_combobox;
    QLabel *convo_types_label;
    QCheckBox *convo_type_regular_check;
    QCheckBox *convo_type_separ_check;
    QCheckBox *convo_type_freq_check;
    QSpacerItem *verticalSpacer;
    QLabel *change_pic_label;
    QPushButton *change_pic_button;
    QSpacerItem *verticalSpacer_3;
    QLabel *show_orig_label;
    QPushButton *show_orig_button;
    QLabel *show_regular_label;
    QPushButton *show_regular_button;
    QLabel *show_separ_label;
    QPushButton *show_separ_button;
    QLabel *show_freq_label;
    QPushButton *show_freq_button;
    QSpacerItem *verticalSpacer_2;
    QLabel *iters_label;
    QSpinBox *iters_spinbox;
    QDialogButtonBox *select_filter_dialog;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1081, 604);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        run_layout = new QVBoxLayout();
        run_layout->setSpacing(6);
        run_layout->setObjectName(QStringLiteral("run_layout"));
        run_err_label = new QLabel(centralWidget);
        run_err_label->setObjectName(QStringLiteral("run_err_label"));

        run_layout->addWidget(run_err_label);

        run_button = new QPushButton(centralWidget);
        run_button->setObjectName(QStringLiteral("run_button"));

        run_layout->addWidget(run_button);


        gridLayout->addLayout(run_layout, 1, 1, 1, 1);

        result_layout = new QGridLayout();
        result_layout->setSpacing(6);
        result_layout->setObjectName(QStringLiteral("result_layout"));
        freq_time_label = new QLabel(centralWidget);
        freq_time_label->setObjectName(QStringLiteral("freq_time_label"));

        result_layout->addWidget(freq_time_label, 2, 0, 1, 1);

        regular_time_label = new QLabel(centralWidget);
        regular_time_label->setObjectName(QStringLiteral("regular_time_label"));

        result_layout->addWidget(regular_time_label, 0, 0, 1, 1);

        separ_time_label = new QLabel(centralWidget);
        separ_time_label->setObjectName(QStringLiteral("separ_time_label"));

        result_layout->addWidget(separ_time_label, 1, 0, 1, 1);

        regular_time_result_label = new QLabel(centralWidget);
        regular_time_result_label->setObjectName(QStringLiteral("regular_time_result_label"));
        regular_time_result_label->setMinimumSize(QSize(500, 0));

        result_layout->addWidget(regular_time_result_label, 0, 1, 1, 1);

        separ_time_result_label = new QLabel(centralWidget);
        separ_time_result_label->setObjectName(QStringLiteral("separ_time_result_label"));
        separ_time_result_label->setMinimumSize(QSize(500, 0));

        result_layout->addWidget(separ_time_result_label, 1, 1, 1, 1);

        freq_time_result_label = new QLabel(centralWidget);
        freq_time_result_label->setObjectName(QStringLiteral("freq_time_result_label"));
        freq_time_result_label->setMinimumSize(QSize(500, 0));

        result_layout->addWidget(freq_time_result_label, 2, 1, 1, 1);


        gridLayout->addLayout(result_layout, 1, 0, 1, 1);

        picture_gview = new QGraphicsView(centralWidget);
        picture_gview->setObjectName(QStringLiteral("picture_gview"));
        picture_gview->setMinimumSize(QSize(640, 480));

        gridLayout->addWidget(picture_gview, 0, 0, 1, 1);

        menu_layout = new QFormLayout();
        menu_layout->setSpacing(6);
        menu_layout->setObjectName(QStringLiteral("menu_layout"));
        create_filter_label = new QLabel(centralWidget);
        create_filter_label->setObjectName(QStringLiteral("create_filter_label"));

        menu_layout->setWidget(1, QFormLayout::LabelRole, create_filter_label);

        create_filter_button = new QPushButton(centralWidget);
        create_filter_button->setObjectName(QStringLiteral("create_filter_button"));

        menu_layout->setWidget(1, QFormLayout::FieldRole, create_filter_button);

        select_filter_label = new QLabel(centralWidget);
        select_filter_label->setObjectName(QStringLiteral("select_filter_label"));

        menu_layout->setWidget(2, QFormLayout::LabelRole, select_filter_label);

        select_filter_combobox = new QComboBox(centralWidget);
        select_filter_combobox->setObjectName(QStringLiteral("select_filter_combobox"));

        menu_layout->setWidget(2, QFormLayout::FieldRole, select_filter_combobox);

        convo_types_label = new QLabel(centralWidget);
        convo_types_label->setObjectName(QStringLiteral("convo_types_label"));

        menu_layout->setWidget(4, QFormLayout::LabelRole, convo_types_label);

        convo_type_regular_check = new QCheckBox(centralWidget);
        convo_type_regular_check->setObjectName(QStringLiteral("convo_type_regular_check"));

        menu_layout->setWidget(4, QFormLayout::FieldRole, convo_type_regular_check);

        convo_type_separ_check = new QCheckBox(centralWidget);
        convo_type_separ_check->setObjectName(QStringLiteral("convo_type_separ_check"));

        menu_layout->setWidget(5, QFormLayout::FieldRole, convo_type_separ_check);

        convo_type_freq_check = new QCheckBox(centralWidget);
        convo_type_freq_check->setObjectName(QStringLiteral("convo_type_freq_check"));

        menu_layout->setWidget(6, QFormLayout::FieldRole, convo_type_freq_check);

        verticalSpacer = new QSpacerItem(20, 60, QSizePolicy::Minimum, QSizePolicy::Expanding);

        menu_layout->setItem(7, QFormLayout::SpanningRole, verticalSpacer);

        change_pic_label = new QLabel(centralWidget);
        change_pic_label->setObjectName(QStringLiteral("change_pic_label"));

        menu_layout->setWidget(8, QFormLayout::LabelRole, change_pic_label);

        change_pic_button = new QPushButton(centralWidget);
        change_pic_button->setObjectName(QStringLiteral("change_pic_button"));

        menu_layout->setWidget(8, QFormLayout::FieldRole, change_pic_button);

        verticalSpacer_3 = new QSpacerItem(20, 25, QSizePolicy::Minimum, QSizePolicy::Expanding);

        menu_layout->setItem(9, QFormLayout::SpanningRole, verticalSpacer_3);

        show_orig_label = new QLabel(centralWidget);
        show_orig_label->setObjectName(QStringLiteral("show_orig_label"));

        menu_layout->setWidget(10, QFormLayout::LabelRole, show_orig_label);

        show_orig_button = new QPushButton(centralWidget);
        show_orig_button->setObjectName(QStringLiteral("show_orig_button"));

        menu_layout->setWidget(10, QFormLayout::FieldRole, show_orig_button);

        show_regular_label = new QLabel(centralWidget);
        show_regular_label->setObjectName(QStringLiteral("show_regular_label"));

        menu_layout->setWidget(11, QFormLayout::LabelRole, show_regular_label);

        show_regular_button = new QPushButton(centralWidget);
        show_regular_button->setObjectName(QStringLiteral("show_regular_button"));

        menu_layout->setWidget(11, QFormLayout::FieldRole, show_regular_button);

        show_separ_label = new QLabel(centralWidget);
        show_separ_label->setObjectName(QStringLiteral("show_separ_label"));

        menu_layout->setWidget(12, QFormLayout::LabelRole, show_separ_label);

        show_separ_button = new QPushButton(centralWidget);
        show_separ_button->setObjectName(QStringLiteral("show_separ_button"));

        menu_layout->setWidget(12, QFormLayout::FieldRole, show_separ_button);

        show_freq_label = new QLabel(centralWidget);
        show_freq_label->setObjectName(QStringLiteral("show_freq_label"));

        menu_layout->setWidget(13, QFormLayout::LabelRole, show_freq_label);

        show_freq_button = new QPushButton(centralWidget);
        show_freq_button->setObjectName(QStringLiteral("show_freq_button"));

        menu_layout->setWidget(13, QFormLayout::FieldRole, show_freq_button);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        menu_layout->setItem(14, QFormLayout::SpanningRole, verticalSpacer_2);

        iters_label = new QLabel(centralWidget);
        iters_label->setObjectName(QStringLiteral("iters_label"));

        menu_layout->setWidget(15, QFormLayout::LabelRole, iters_label);

        iters_spinbox = new QSpinBox(centralWidget);
        iters_spinbox->setObjectName(QStringLiteral("iters_spinbox"));
        iters_spinbox->setMinimum(1);
        iters_spinbox->setMaximum(99999);

        menu_layout->setWidget(15, QFormLayout::FieldRole, iters_spinbox);

        select_filter_dialog = new QDialogButtonBox(centralWidget);
        select_filter_dialog->setObjectName(QStringLiteral("select_filter_dialog"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(select_filter_dialog->sizePolicy().hasHeightForWidth());
        select_filter_dialog->setSizePolicy(sizePolicy);
        select_filter_dialog->setLayoutDirection(Qt::LeftToRight);
        select_filter_dialog->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        select_filter_dialog->setCenterButtons(false);

        menu_layout->setWidget(3, QFormLayout::FieldRole, select_filter_dialog);


        gridLayout->addLayout(menu_layout, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "2D Filtering tool", 0));
        run_err_label->setText(QString());
        run_button->setText(QApplication::translate("MainWindow", "Run", 0));
        freq_time_label->setText(QApplication::translate("MainWindow", "Frequential time:", 0));
        regular_time_label->setText(QApplication::translate("MainWindow", "Regular spatial time:", 0));
        separ_time_label->setText(QApplication::translate("MainWindow", "Separable spatial time:", 0));
        regular_time_result_label->setText(QString());
        separ_time_result_label->setText(QString());
        freq_time_result_label->setText(QString());
        create_filter_label->setText(QApplication::translate("MainWindow", "Add a custom filter:", 0));
        create_filter_button->setText(QApplication::translate("MainWindow", "Create a filter", 0));
        select_filter_label->setText(QApplication::translate("MainWindow", "Select a filter:", 0));
        convo_types_label->setText(QApplication::translate("MainWindow", "Choose convolution types:", 0));
        convo_type_regular_check->setText(QApplication::translate("MainWindow", "Regular spatial convolution", 0));
        convo_type_separ_check->setText(QApplication::translate("MainWindow", "Separable spatial convolution", 0));
        convo_type_freq_check->setText(QApplication::translate("MainWindow", "Frequential convolution", 0));
        change_pic_label->setText(QApplication::translate("MainWindow", "Change the picture:", 0));
        change_pic_button->setText(QApplication::translate("MainWindow", "Select a file", 0));
        show_orig_label->setText(QApplication::translate("MainWindow", "Original:", 0));
        show_orig_button->setText(QApplication::translate("MainWindow", "Show", 0));
        show_regular_label->setText(QApplication::translate("MainWindow", "Regular spatial:", 0));
        show_regular_button->setText(QApplication::translate("MainWindow", "Show", 0));
        show_separ_label->setText(QApplication::translate("MainWindow", "Separable spatial:", 0));
        show_separ_button->setText(QApplication::translate("MainWindow", "Show", 0));
        show_freq_label->setText(QApplication::translate("MainWindow", "Frequential:", 0));
        show_freq_button->setText(QApplication::translate("MainWindow", "Show", 0));
        iters_label->setText(QApplication::translate("MainWindow", "Iterations:", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
