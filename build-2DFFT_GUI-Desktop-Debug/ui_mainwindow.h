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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QSpinBox *spinBox;
    QLabel *label_7;
    QPushButton *pushButton_3;
    QFormLayout *formLayout_2;
    QLabel *label_10;
    QLabel *label_8;
    QLabel *label_11;
    QGraphicsView *graphicsView;
    QFormLayout *formLayout;
    QLabel *label_4;
    QComboBox *comboBox;
    QLabel *label_5;
    QPushButton *pushButton_2;
    QLabel *label_9;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox;
    QLabel *label_2;
    QLabel *label_6;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_12;
    QPushButton *pushButton_4;
    QLabel *label_13;
    QPushButton *pushButton_6;
    QLabel *label_14;
    QPushButton *pushButton_7;
    QLabel *label_15;
    QPushButton *pushButton_5;
    QLabel *label_19;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1081, 627);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setMaximum(99999);

        gridLayout_2->addWidget(spinBox, 0, 1, 1, 1);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout_2->addWidget(pushButton_3, 1, 1, 1, 1);


        gridLayout->addLayout(gridLayout_2, 1, 1, 1, 1);

        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, label_10);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, label_8);

        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, label_11);


        gridLayout->addLayout(formLayout_2, 1, 0, 1, 1);

        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setMinimumSize(QSize(640, 480));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_4);

        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, comboBox);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_5);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        formLayout->setWidget(2, QFormLayout::FieldRole, pushButton_2);

        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_9);

        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        formLayout->setWidget(3, QFormLayout::FieldRole, checkBox_2);

        checkBox_3 = new QCheckBox(centralWidget);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        formLayout->setWidget(4, QFormLayout::FieldRole, checkBox_3);

        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        formLayout->setWidget(5, QFormLayout::FieldRole, checkBox);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(6, QFormLayout::FieldRole, label_2);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout->setWidget(7, QFormLayout::FieldRole, label_6);

        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));

        formLayout->setWidget(8, QFormLayout::FieldRole, label_16);

        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QStringLiteral("label_17"));

        formLayout->setWidget(9, QFormLayout::FieldRole, label_17);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(10, QFormLayout::LabelRole, label);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        formLayout->setWidget(10, QFormLayout::FieldRole, pushButton);

        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));

        formLayout->setWidget(12, QFormLayout::LabelRole, label_12);

        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        formLayout->setWidget(12, QFormLayout::FieldRole, pushButton_4);

        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));

        formLayout->setWidget(13, QFormLayout::LabelRole, label_13);

        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        formLayout->setWidget(13, QFormLayout::FieldRole, pushButton_6);

        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));

        formLayout->setWidget(14, QFormLayout::LabelRole, label_14);

        pushButton_7 = new QPushButton(centralWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        formLayout->setWidget(14, QFormLayout::FieldRole, pushButton_7);

        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));

        formLayout->setWidget(15, QFormLayout::LabelRole, label_15);

        pushButton_5 = new QPushButton(centralWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        formLayout->setWidget(15, QFormLayout::FieldRole, pushButton_5);

        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QStringLiteral("label_19"));

        formLayout->setWidget(11, QFormLayout::FieldRole, label_19);


        gridLayout->addLayout(formLayout, 0, 1, 1, 1);

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
        label_7->setText(QApplication::translate("MainWindow", "Iterations:", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "Run", 0));
        label_10->setText(QApplication::translate("MainWindow", "Regular spatial time:", 0));
        label_8->setText(QApplication::translate("MainWindow", "Separable spatial time:", 0));
        label_11->setText(QApplication::translate("MainWindow", "Frequential time:", 0));
        label_4->setText(QApplication::translate("MainWindow", "Choose a filter:", 0));
        label_5->setText(QApplication::translate("MainWindow", "Add a custom filter:", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "Create a filter", 0));
        label_9->setText(QApplication::translate("MainWindow", "Choose convolution types:", 0));
        checkBox_2->setText(QApplication::translate("MainWindow", "Regular spatial convolution", 0));
        checkBox_3->setText(QApplication::translate("MainWindow", "Separable spatial convolution", 0));
        checkBox->setText(QApplication::translate("MainWindow", "Frequential convolution", 0));
        label_2->setText(QString());
        label_6->setText(QString());
        label_16->setText(QString());
        label_17->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Change the picture:", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Select a file", 0));
        label_12->setText(QApplication::translate("MainWindow", "Original:", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Show", 0));
        label_13->setText(QApplication::translate("MainWindow", "Regular spatial:", 0));
        pushButton_6->setText(QApplication::translate("MainWindow", "Show", 0));
        label_14->setText(QApplication::translate("MainWindow", "Separable spatial:", 0));
        pushButton_7->setText(QApplication::translate("MainWindow", "Show", 0));
        label_15->setText(QApplication::translate("MainWindow", "Frequential:", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "Show", 0));
        label_19->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
