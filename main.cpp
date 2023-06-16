#include "protractor.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    protractor c(nullptr);
    c.show();
    return QApplication::exec();
}
