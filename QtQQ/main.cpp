#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include "userlogin.h"
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
 /*   CCMainWindow w;
    w.show();*/
    a.setQuitOnLastWindowClosed(false);
   UserLogin* u = new UserLogin;

   u->show();
    

    return a.exec();
 
}
