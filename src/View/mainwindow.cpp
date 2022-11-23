#include "mainwindow.h"

#include "ui_mainwindow.h"

namespace s21 {
MainWindow::MainWindow(Settings *configuration, Facade *control,
                       QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      controller_(control),
      configuration_(configuration) {
  ui_->setupUi(this);
  save_config_.WriteOutFile();
  configuration_->Restore(save_config_);

  std::string filename = QDir::homePath().toStdString();
  filename += "/Desktop/3DVIEWERv2/settings.conf";
  std::ifstream file(filename);
  if (file.is_open() == 0) {
    SetDefaultSettings();
  }
  file.close();

  SetConfigurationOnUi();
  ui_->widget->InitFacade(controller_);
  ui_->widget->InitSettings(configuration_);
  ui_->chooseTexture->setDisabled(true);
  ui_->chooseTextureOff->setDisabled(true);
}

MainWindow::~MainWindow() {
  save_config_.Saving(configuration_->Save());
  save_config_.WriteInFile();
  delete ui_;
}

void MainWindow::on_openFile_clicked() {
  QString filepath = QFileDialog::getOpenFileName(0, "Открыть", "", "*.obj");
  if (!filepath.isEmpty()) {
    std::string input_path = filepath.toStdString();
    ui_->widget->InitModel(input_path);
    QFileInfo filename(filepath);
    ui_->labelFilename->setText(filename.fileName());
    ui_->labelEdgesCount->setText(
        QString::number(controller_->GetObject().count_of_edge));
    ui_->labelVertexCount->setText(
        QString::number(controller_->GetObject().count_of_vertexes));
    if (controller_->GetObject().vert_t.size()) {
      ui_->chooseTexture->setDisabled(false);
      ui_->chooseTextureOff->setDisabled(false);
    } else {
      ui_->chooseTexture->setDisabled(true);
      ui_->chooseTextureOff->setDisabled(true);
    }
    ui_->widget->update();
  }
}

void MainWindow::on_pushButtonMovingX_clicked() {
  controller_->AthenianMove(ui_->doubleSpinBoxMoving->value(), AXISX);
  ui_->widget->BufferFilling();
  ui_->widget->update();
}

void MainWindow::on_pushButtonMovingY_clicked() {
  controller_->AthenianMove(ui_->doubleSpinBoxMoving->value(), AXISY);
  ui_->widget->BufferFilling();
  ui_->widget->update();
}

void MainWindow::on_pushButtonMovingZ_clicked() {
  controller_->AthenianMove(ui_->doubleSpinBoxMoving->value(), AXISZ);
  ui_->widget->BufferFilling();
  ui_->widget->update();
}

void MainWindow::on_pushButtonRotateX_clicked() {
  controller_->AthenianRotate(ui_->doubleSpinBoxRotate->value(), AXISX);
  ui_->widget->BufferFilling();
  ui_->widget->update();
}

void MainWindow::on_pushButtonRotateY_clicked() {
  controller_->AthenianRotate(ui_->doubleSpinBoxRotate->value(), AXISY);
  ui_->widget->BufferFilling();
  ui_->widget->update();
}

void MainWindow::on_pushButtonRotateZ_clicked() {
  controller_->AthenianRotate(ui_->doubleSpinBoxRotate->value(), AXISZ);
  ui_->widget->BufferFilling();
  ui_->widget->update();
}

void MainWindow::on_pushButtonPlusScale_clicked() {
  controller_->AthenianScale(1.1);
  ui_->widget->BufferFilling();
  ui_->widget->update();
}

void MainWindow::on_pushButtonMinusScale_clicked() {
  controller_->AthenianScale(0.9);
  ui_->widget->BufferFilling();
  ui_->widget->update();
}

void MainWindow::on_chooseColorBG_clicked() {
  QColor color = ColorChooseMenu(ui_->labelChoosedColorBG);
  if (color.isValid()) {
    configuration_->SetBGColor(color);
    ui_->widget->update();
  }
}

void MainWindow::on_chooseColorLines_clicked() {
  QColor color = ColorChooseMenu(ui_->labelChoosedColorLines);
  if (color.isValid()) {
    configuration_->SetEdgeColor(color);
    ui_->widget->update();
  }
}

void MainWindow::on_chooseColorVertex_clicked() {
  QColor color = ColorChooseMenu(ui_->labelChoosedColorVertex);
  if (color.isValid()) {
    configuration_->SetVertexColor(color);
    ui_->widget->update();
  }
}

void MainWindow::on_doubleSpinBoxSizeLines_valueChanged(double index) {
  configuration_->SetSizeLines(index);
  ui_->widget->update();
}

void MainWindow::on_comboBoxTypeProection_currentIndexChanged(int index) {
  configuration_->SetTypeProection(index);
  ui_->widget->update();
}

void MainWindow::on_comboBoxTypeLines_currentIndexChanged(int index) {
  configuration_->SetTypeLines(index);
  ui_->widget->update();
}

void MainWindow::on_comboBoxTypeVertex_currentIndexChanged(int index) {
  configuration_->SetTypeVertexes(index);
  ui_->widget->update();
}

void MainWindow::on_doubleSpinBoxSizeVertex_valueChanged(double index) {
  configuration_->SetSizeVertexes(index);
  ui_->widget->update();
}

void MainWindow::on_comboBoxTypeModel_currentIndexChanged(int index) {
  configuration_->SetTypeModel(index);
  ui_->widget->update();
}

void MainWindow::on_chooseColorModel_clicked() {
  QColor color = ColorChooseMenu(ui_->labelChoosedColorModel);
  if (color.isValid()) {
    configuration_->SetModelColor(color);
    ui_->widget->update();
  }
}

void MainWindow::on_chooseColorLight_clicked() {
  QColor color = ColorChooseMenu(ui_->labelChoosedColorLight);
  if (color.isValid()) {
    configuration_->SetLightColor(color);
    ui_->widget->update();
  }
}

void MainWindow::on_doubleSpinBoxPowerLight_valueChanged(double index) {
  configuration_->SetLightPower(index);
  ui_->widget->update();
}

void MainWindow::on_resetSettings_clicked() {
  SetDefaultSettings();
  SetConfigurationOnUi();
  ui_->widget->SetDefaultCoord();
  ui_->widget->update();
}

void MainWindow::on_chooseTexture_clicked() {
  QString texture_path =
      QFileDialog::getOpenFileName(0, "Открыть", "", "*.bmp");
  if (!texture_path.isEmpty()) {
    ui_->widget->SetTextureStatus(1);
    ui_->widget->SetTexture(texture_path);
    ui_->widget->update();
  }
}

void MainWindow::on_chooseTextureOff_clicked() {
  ui_->widget->SetTextureStatus(0);
  ui_->widget->update();
}

void MainWindow::on_pushButtonImage_clicked() {
  QImage image = ui_->widget->grabFramebuffer();
  QString FileName = QFileDialog::getSaveFileName(
      this, "Open a file", QDir::homePath(), "BMP (*.bmp);; JPEG (*.jpeg)");
  if (!FileName.isNull() && !FileName.isEmpty()) {
    image.save(FileName);
  }
}

void MainWindow::on_pushButtonGif_clicked() {
  gif_ = new QGifImage(QSize(640, 480));
  gif_->setDefaultDelay(100);
  time_ = new QTimer(this);
  time_->start(100);
  while (1) {
    if (gif_->frameCount() < 50) {
      QImage image;
      ui_->widget->SetMatrixRotation(
          QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 7.2));
      ui_->widget->update();
      image = ui_->widget->grabFramebuffer();
      image = image.scaled(640, 480, Qt::IgnoreAspectRatio,
                           Qt::SmoothTransformation);
      gif_->addFrame(image);
    } else {
      QString FileName = QFileDialog::getSaveFileName(
          this, "Open a file", QDir::homePath(), "GIF (*.gif)");
      gif_->save(FileName);
      time_->stop();
      break;
    }
  }
}

void MainWindow::on_pushButtonScreencast_clicked() {
  gif_ = new QGifImage(QSize(640, 480));
  gif_->setDefaultDelay(100);
  time_ = new QTimer(this);
  time_->start(100);
  connect(time_, SIGNAL(timeout()), this, SLOT(ScreencastSlot()));
}

void MainWindow::ScreencastSlot() {
  if (gif_->frameCount() < 50) {
    QImage image;
    image = ui_->widget->grabFramebuffer();
    image =
        image.scaled(640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    gif_->addFrame(image);
  } else {
    QString FileName = QFileDialog::getSaveFileName(
        this, "Open a file", QDir::homePath(), "GIF (*.gif)");
    gif_->save(FileName);
    time_->stop();
  }
}

void MainWindow::SetConfigurationOnUi() {
  s21::StatusStorage setting_preferences = configuration_->Save();
  ui_->comboBoxTypeProection->setCurrentIndex(
      setting_preferences.type_proection);
  ui_->labelChoosedColorBG->setStyleSheet(
      "border: 1px solid #20232A; border-width: 2px; border-radius: 5px; "
      "background-color: #" +
      QString::number(setting_preferences.background_color.rgb(), 16));
  ui_->comboBoxTypeLines->setCurrentIndex(setting_preferences.type_lines);
  ui_->labelChoosedColorLines->setStyleSheet(
      "border: 1px solid #20232A; border-width: 2px; border-radius: 5px; "
      "background-color: #" +
      QString::number(setting_preferences.lines_color.rgb(), 16));
  ui_->doubleSpinBoxSizeLines->setValue(setting_preferences.size_lines);
  ui_->comboBoxTypeVertex->setCurrentIndex(setting_preferences.type_vertexes);
  ui_->labelChoosedColorVertex->setStyleSheet(
      "border: 1px solid #20232A; border-width: 2px; border-radius: 5px; "
      "background-color: #" +
      QString::number(setting_preferences.vertexes_color.rgb(), 16));
  ui_->doubleSpinBoxSizeVertex->setValue(setting_preferences.size_vertexes);

  ui_->comboBoxTypeModel->setCurrentIndex(setting_preferences.type_model);
  ui_->labelChoosedColorModel->setStyleSheet(
      "border: 1px solid #20232A; border-width: 2px; border-radius: 5px; "
      "background-color: #" +
      QString::number(setting_preferences.model_color.rgb(), 16));
  ui_->labelChoosedColorLight->setStyleSheet(
      "border: 1px solid #20232A; border-width: 2px; border-radius: 5px; "
      "background-color: #" +
      QString::number(setting_preferences.light_color.rgb(), 16));
  ui_->doubleSpinBoxPowerLight->setValue(setting_preferences.light_power);
}

void MainWindow::SetDefaultSettings() {
  configuration_->SetTypeProection(0);
  configuration_->SetBGColor(QColor(128, 128, 128, 0));
  configuration_->SetTypeLines(1);
  configuration_->SetEdgeColor(QColor(0, 0, 0, 0));
  configuration_->SetSizeLines(1);
  configuration_->SetTypeVertexes(0);
  configuration_->SetVertexColor(QColor(0, 0, 0, 0));
  configuration_->SetSizeVertexes(1);
  configuration_->SetTypeModel(0);
  configuration_->SetModelColor(QColor(25, 65, 20, 1));
  configuration_->SetLightColor(QColor(255, 255, 255, 0));
  configuration_->SetLightPower(15);
}

QColor MainWindow::ColorChooseMenu(QLabel *ui) {
  QColor color = QColorDialog::getColor();
  if (color.isValid()) {
    ui->setStyleSheet(
        "border: 1px solid #20232A; border-width: 2px; border-radius: 5px; "
        "background-color: #" +
        QString::number(color.rgb(), 16));
  }
  return color;
}

// Пасхалка

void MainWindow::on_easterEgg_clicked() {
  QDesktopServices::openUrl(QUrl(
      "https://www.youtube.com/watch?v=dQw4w9WgXcQ&ab_channel=RickAstley"));
}
}  // namespace s21
