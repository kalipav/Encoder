///////////////////////////
// �����:  Kalinin Pavel //
// ����:   main.cpp      //
// ������: 04.05.2019    //
///////////////////////////

#include <QtGui/QApplication>
#include "Encoder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Encoder w;
    w.show();
    
    return a.exec();
}
