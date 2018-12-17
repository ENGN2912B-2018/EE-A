#include "eea_data_vis.h"
#include "ui_eea_data_vis.h"
#include <QtDataVisualization>
#include <QFileDialog>


using namespace QtDataVisualization;

EEA_Data_Vis::EEA_Data_Vis(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EEA_Data_Vis)
{
    ui->setupUi(this);
    //initialize booleans to false upon startup
    isFileLoaded = false;
    isSampleRateSpecified = false;

}

EEA_Data_Vis::~EEA_Data_Vis()
{
    delete ui;
}

// create 3D plot
void EEA_Data_Vis::on_pushButton_clicked()
{
    if (isFileLoaded){
        // declare variables
        scatter.setFlags(scatter.flags() ^ Qt::FramelessWindowHint);
        QScatter3DSeries *series = new QScatter3DSeries;
        QScatterDataArray data;

        // read in data from x, y, and z vectors
        for (int i=0; i < 100; ++i){    //only plotting 100 data points for rendering purposes
            data << QVector3D(xdata[i], ydata[i], zdata[i]);
        }
        Q3DTheme *theme = new Q3DTheme(Q3DTheme::ThemeArmyBlue);
        // add data, specify size, set title, etc
        scatter.setActiveTheme(theme);
        scatter.axisX()->setLabelAutoRotation(30);
        scatter.axisY()->setLabelAutoRotation(90);
        scatter.axisZ()->setLabelAutoRotation(30);
        scatter.axisX()->setTitle(QStringLiteral("X (m)"));
        scatter.axisY()->setTitle(QStringLiteral("Y (m)"));
        scatter.axisZ()->setTitle(QStringLiteral("Z (m)"));
        scatter.setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
        series->dataProxy()->addItems(data);
        scatter.addSeries(series);
        QString title = "Embedded Position Tracking: 3D Data Visualization";
        scatter.setTitle(title);
        scatter.setMinimumWidth(500);
        scatter.setMinimumHeight(500);
        // show plot
        scatter.show();
    }
    else {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Cannot plot without data.");
        messageBox.setFixedSize(500,200);
    }

}


//read data from csv and populate x, y, and z data vectors
void EEA_Data_Vis::on_lineEdit_returnPressed()
{
    QString filename = ui->lineEdit->text();
    QFile file(filename);

    // error check on user input data
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","File not found. Enter a different file name.");
        messageBox.setFixedSize(500,200);
     }
    else {
        isFileLoaded = true;
        QMessageBox msgBox;
        msgBox.setText("Success. File was properly loaded.");
        msgBox.setFixedSize(500, 200);
        msgBox.exec();
    }

    QByteArray line = file.readLine(); //read the first line before we populate data vectors to ignore "x, y, z"
    int count = 0;
    while (!file.atEnd()) {
        count++;
        line = file.readLine();
        QList<QByteArray> xyzList = line.split(',');
        xdata.append(xyzList[0].toFloat());    //grab string and convert to double
        ydata.append(xyzList[1].toFloat());
        zdata.append(xyzList[2].split('\n')[0].toFloat());
    }

    plotData = xdata; //default case
}


//2D plot
void EEA_Data_Vis::on_pushButton_2_clicked()
{
    if (isFileLoaded){  //make sure csv is loaded

        if (isSampleRateSpecified){ //make sure sample rate is specifed
            // create graph and assign data to it:
            ui->customPlot->addGraph();
            ui->customPlot->graph(0)->setData(timeData, plotData);

            // give the axes some labels:
            ui->customPlot->xAxis->setLabel("Time (s)");

            //set y axis title based on user selection
            if (plotData == xdata){
                ui->customPlot->yAxis->setLabel("X (m)");
            }
            else if (plotData == ydata){
                ui->customPlot->yAxis->setLabel("Y (m)");
            }
            else {
                ui->customPlot->yAxis->setLabel("Z (m)");
            }
            // set colors - code taken from : https://www.qcustomplot.com/index.php/demos/styleddemo
            //ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 4));
            ui->customPlot->graph(0)->setPen(QPen(Qt::green, 1));
            ui->customPlot->xAxis->setLabelColor(Qt::white);
            ui->customPlot->yAxis->setLabelColor(Qt::white);
            ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
            ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
            ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
            ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
            ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
            ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
            ui->customPlot->xAxis->setTickLabelColor(Qt::white);
            ui->customPlot->yAxis->setTickLabelColor(Qt::white);
            ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
            ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
            ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
            ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
            ui->customPlot->xAxis->grid()->setSubGridVisible(true);
            ui->customPlot->yAxis->grid()->setSubGridVisible(true);
            ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
            ui->customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
            ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
            ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
            QLinearGradient plotGradient;
            plotGradient.setStart(0, 0);
            plotGradient.setFinalStop(0, 350);
            plotGradient.setColorAt(0, QColor(80, 80, 80));
            plotGradient.setColorAt(1, QColor(50, 50, 50));
            ui->customPlot->setBackground(plotGradient);
            QLinearGradient axisRectGradient;
            axisRectGradient.setStart(0, 0);
            axisRectGradient.setFinalStop(0, 350);
            axisRectGradient.setColorAt(0, QColor(80, 80, 80));
            axisRectGradient.setColorAt(1, QColor(30, 30, 30));
            ui->customPlot->axisRect()->setBackground(axisRectGradient);

            // autoscale axis ranges, so we see all data
            ui->customPlot->rescaleAxes(true);

            ui->customPlot->replot();

        }
        else {
            QMessageBox messageBox;
            messageBox.critical(0,"Specify sampling rate","Cannot plot without sample rate.");
            messageBox.setFixedSize(500,200);
        }

    }
    else {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Cannot plot without data.");
        messageBox.setFixedSize(500,200);
    }


}
// clear plot
void EEA_Data_Vis::on_pushButton_3_clicked()
{
    // clear data
    ui->customPlot->graph(0)->data()->clear();
    // reset axes to some default size
    ui->customPlot->xAxis->setRange(0, 5);
    ui->customPlot->yAxis->setRange(0, 5);
    // replot
    ui->customPlot->replot();

}

// specify x/y/x vs time
void EEA_Data_Vis::on_comboBox_activated(int index)
{
    if (isFileLoaded){
        if (index == 0){
            plotData = xdata;
        }
        else if (index == 1){
            plotData = ydata;
        }
        else {
            plotData = zdata;
        }

    }
    else {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Load file before attempting to plot.");
        messageBox.setFixedSize(500,200);
    }

}

// specify sampling rate
void EEA_Data_Vis::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    // clear time vector before changing it based on sampling rate
    timeData.clear();
    double sampleRate = arg1.toDouble();
    //qDebug() << sampleRate;
    // use specified sample rate to create time vector that will be used for plotting
    for (int i=0; i<xdata.size(); ++i)
    {
      timeData.append(i/sampleRate);
    }
    isSampleRateSpecified = true;

}

// let user know that they must press return to input csv file name
void EEA_Data_Vis::on_pushButton_4_clicked()
{
    QMessageBox::information(this, "Information", "Press enter/return to execute load.");
}
