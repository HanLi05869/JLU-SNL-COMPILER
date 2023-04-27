#include "mainwindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}




//#include "mainwindow.h"

//#include <QApplication>
//#include <iostream>
//using namespace std;

//int main(int argc, char *argv[])
//{
//    cout <<1<< endl;
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}
