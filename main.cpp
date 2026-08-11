#include "src/mainwindow/mainwindow.h"
#include <QApplication>
#include "src/lib/mercury.hpp"
#include <memory>

int main(int argc, char *argv[])
{
    std::unique_ptr<Mercury::ThreadPool> pool = std::make_unique<Mercury::ThreadPool>(4);
    QApplication app(argc, argv);
    MainWindow window(nullptr, pool.get());
    window.show();
    return app.exec();
}
