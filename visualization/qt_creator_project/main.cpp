#include "eea_data_vis.h"


using namespace QtDataVisualization;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EEA_Data_Vis w;
    w.show();
    return a.exec();
}
