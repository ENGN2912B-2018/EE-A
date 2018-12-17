#ifndef EEA_DATA_VIS_H
#define EEA_DATA_VIS_H

#include <QMainWindow>
#include <QApplication>
#include <QtDataVisualization>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QFileInfo>

namespace Ui {
class EEA_Data_Vis;
}

class EEA_Data_Vis : public QMainWindow
{
    Q_OBJECT

public:
    explicit EEA_Data_Vis(QWidget *parent = 0);
    ~EEA_Data_Vis();

private slots:

    void on_pushButton_clicked();   //3d plot

    void on_lineEdit_returnPressed(); //specify csv

    void on_pushButton_2_clicked(); //2d plot

    void on_pushButton_3_clicked(); //clear 2d plot

    void on_comboBox_activated(int index); //selects x/y/z data

    void on_comboBox_2_currentIndexChanged(const QString &arg1); //sampling rate

    void on_pushButton_4_clicked();

private:
    Ui::EEA_Data_Vis *ui;
    QtDataVisualization::Q3DScatter scatter;
    QVector<double> xdata, ydata, zdata, timeData, plotData;
    bool isFileLoaded, isSampleRateSpecified;

};

#endif // EEA_DATA_VIS_H
