#include <QApplication>

#include "View/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::Settings settings;
  s21::ParsingModel model;
  s21::Facade control(&model);
  s21::MainWindow view(&settings, &control);
  view.setWindowTitle("3DVIEWERv2.1");
  view.show();
  return a.exec();
}
