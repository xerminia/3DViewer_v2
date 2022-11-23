#ifndef SRC_VIEW_MAINWINDOW_H_
#define SRC_VIEW_MAINWINDOW_H_

#include <QColorDialog>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMainWindow>
#include <QTimer>

#include "Controller/facade.h"
#include "OpenGL/openglwidget.h"
#include "QtGifImage/src/gifimage/qgifimage.h"
#include "QtWidgets"
#include "Settings/settings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow() = default;
  MainWindow(Settings *configuration, Facade *control,
             QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_openFile_clicked();
  void on_pushButtonMovingX_clicked();
  void on_pushButtonMovingY_clicked();
  void on_pushButtonMovingZ_clicked();
  void on_pushButtonRotateX_clicked();
  void on_pushButtonRotateY_clicked();
  void on_pushButtonRotateZ_clicked();
  void on_pushButtonPlusScale_clicked();
  void on_pushButtonMinusScale_clicked();

  void on_chooseColorBG_clicked();
  void on_chooseColorLines_clicked();
  void on_chooseColorVertex_clicked();
  void on_doubleSpinBoxSizeLines_valueChanged(double index);
  void on_comboBoxTypeProection_currentIndexChanged(int index);
  void on_comboBoxTypeLines_currentIndexChanged(int index);
  void on_comboBoxTypeVertex_currentIndexChanged(int index);
  void on_doubleSpinBoxSizeVertex_valueChanged(double index);
  void on_comboBoxTypeModel_currentIndexChanged(int index);
  void on_chooseColorModel_clicked();
  void on_chooseColorLight_clicked();
  void on_doubleSpinBoxPowerLight_valueChanged(double index);
  void on_resetSettings_clicked();

  void on_chooseTexture_clicked();
  void on_chooseTextureOff_clicked();

  void on_pushButtonImage_clicked();
  void on_pushButtonGif_clicked();
  void on_pushButtonScreencast_clicked();
  void ScreencastSlot();

  void on_easterEgg_clicked();

 private:
  Ui::MainWindow *ui_;
  Facade *controller_;
  Settings *configuration_;
  Memento save_config_;

  QGifImage *gif_;
  QTimer *time_;

  void SetConfigurationOnUi();
  void SetDefaultSettings();
  QColor ColorChooseMenu(QLabel *ui);
};
}  // namespace s21

#endif  // SRC_VIEW_MAINWINDOW_H_
