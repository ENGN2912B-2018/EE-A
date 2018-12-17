/********************************************************************************
** Form generated from reading UI file 'eea_data_vis.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EEA_DATA_VIS_H
#define UI_EEA_DATA_VIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_EEA_Data_Vis
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QSplitter *splitter_2;
    QFrame *frame_5;
    QGridLayout *gridLayout_4;
    QLabel *label_3;
    QSplitter *splitter;
    QFrame *frame_4;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboBox_2;
    QComboBox *comboBox;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QCustomPlot *customPlot;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EEA_Data_Vis)
    {
        if (EEA_Data_Vis->objectName().isEmpty())
            EEA_Data_Vis->setObjectName(QStringLiteral("EEA_Data_Vis"));
        EEA_Data_Vis->resize(984, 477);
        centralWidget = new QWidget(EEA_Data_Vis);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        splitter_2 = new QSplitter(frame);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        frame_5 = new QFrame(splitter_2);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame_5);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_3 = new QLabel(frame_5);
        label_3->setObjectName(QStringLiteral("label_3"));
        QFont font;
        font.setFamily(QStringLiteral("DejaVu Sans"));
        font.setPointSize(16);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        label_3->setFont(font);

        gridLayout_4->addWidget(label_3, 0, 0, 1, 1);

        splitter_2->addWidget(frame_5);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        frame_4 = new QFrame(splitter);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_4);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label = new QLabel(frame_4);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_5->addWidget(label);

        lineEdit = new QLineEdit(frame_4);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_5->addWidget(lineEdit);

        pushButton_4 = new QPushButton(frame_4);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_5->addWidget(pushButton_4);


        gridLayout_2->addLayout(horizontalLayout_5, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(frame_4);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboBox_2 = new QComboBox(frame_4);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));

        horizontalLayout_2->addWidget(comboBox_2);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        comboBox = new QComboBox(frame_4);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout_2->addWidget(comboBox, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(frame_4);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_2->addWidget(pushButton_2, 3, 0, 1, 1);

        pushButton_3 = new QPushButton(frame_4);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout_2->addWidget(pushButton_3, 4, 0, 1, 1);

        pushButton = new QPushButton(frame_4);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_2->addWidget(pushButton, 5, 0, 1, 1);

        splitter->addWidget(frame_4);
        frame_2 = new QFrame(splitter);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        customPlot = new QCustomPlot(frame_2);
        customPlot->setObjectName(QStringLiteral("customPlot"));

        horizontalLayout->addWidget(customPlot);

        splitter->addWidget(frame_2);
        splitter_2->addWidget(splitter);

        gridLayout_3->addWidget(splitter_2, 0, 0, 1, 1);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        EEA_Data_Vis->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(EEA_Data_Vis);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 984, 20));
        EEA_Data_Vis->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EEA_Data_Vis);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        EEA_Data_Vis->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(EEA_Data_Vis);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        EEA_Data_Vis->setStatusBar(statusBar);

        retranslateUi(EEA_Data_Vis);

        QMetaObject::connectSlotsByName(EEA_Data_Vis);
    } // setupUi

    void retranslateUi(QMainWindow *EEA_Data_Vis)
    {
        EEA_Data_Vis->setWindowTitle(QApplication::translate("EEA_Data_Vis", "EEA_Data_Vis", nullptr));
        label_3->setText(QApplication::translate("EEA_Data_Vis", "Embedded Spatial Tracking with Kalman Filtering: Data Visualization", nullptr));
        label->setText(QApplication::translate("EEA_Data_Vis", "Load data:", nullptr));
        lineEdit->setPlaceholderText(QApplication::translate("EEA_Data_Vis", "filename.csv", nullptr));
        pushButton_4->setText(QApplication::translate("EEA_Data_Vis", "About", nullptr));
        label_2->setText(QApplication::translate("EEA_Data_Vis", "Sampling rate:", nullptr));
        comboBox_2->setItemText(0, QApplication::translate("EEA_Data_Vis", "100", nullptr));
        comboBox_2->setItemText(1, QApplication::translate("EEA_Data_Vis", "200", nullptr));
        comboBox_2->setItemText(2, QApplication::translate("EEA_Data_Vis", "400", nullptr));
        comboBox_2->setItemText(3, QApplication::translate("EEA_Data_Vis", "800", nullptr));
        comboBox_2->setItemText(4, QApplication::translate("EEA_Data_Vis", "1600", nullptr));
        comboBox_2->setItemText(5, QApplication::translate("EEA_Data_Vis", "3200", nullptr));

        comboBox->setItemText(0, QApplication::translate("EEA_Data_Vis", "x vs. t", nullptr));
        comboBox->setItemText(1, QApplication::translate("EEA_Data_Vis", "y vs. t", nullptr));
        comboBox->setItemText(2, QApplication::translate("EEA_Data_Vis", "z vs. t", nullptr));

        pushButton_2->setText(QApplication::translate("EEA_Data_Vis", "Plot 2D Position Data", nullptr));
        pushButton_3->setText(QApplication::translate("EEA_Data_Vis", "Clear", nullptr));
        pushButton->setText(QApplication::translate("EEA_Data_Vis", "Plot 3D Position Data", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EEA_Data_Vis: public Ui_EEA_Data_Vis {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EEA_DATA_VIS_H
