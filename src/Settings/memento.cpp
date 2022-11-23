#include "memento.h"

namespace s21 {
Memento::Memento(StatusStorage value) : saved_info_(value) {}

int Memento::GetTypeProection() { return saved_info_.type_proection; }

QColor Memento::GetBackgroundColor() { return saved_info_.background_color; }

int Memento::GetTypeLines() { return saved_info_.type_lines; }

QColor Memento::GetLinesColor() { return saved_info_.lines_color; }

int Memento::GetSizeLines() { return saved_info_.size_lines; }

int Memento::GetTypeVertexes() { return saved_info_.type_vertexes; }

QColor Memento::GetVertexesColor() { return saved_info_.vertexes_color; }

int Memento::GetSizeVertexes() { return saved_info_.size_vertexes; }

int Memento::GetTypeModel() { return saved_info_.type_model; }

QColor Memento::GetModelColor() { return saved_info_.model_color; }

QColor Memento::GetLightColor() { return saved_info_.light_color; }

double Memento::GetLightPower() { return saved_info_.light_power; }

void Memento::WriteInFile() {
  std::string filename = QDir::homePath().toStdString();
  filename += "/Desktop/3DVIEWERv2/settings.conf";
  std::ofstream file(filename);
  file << saved_info_.type_proection << std::endl;
  file << saved_info_.background_color.redF() << std::endl;
  file << saved_info_.background_color.greenF() << std::endl;
  file << saved_info_.background_color.blueF() << std::endl;
  file << saved_info_.type_lines << std::endl;
  file << saved_info_.lines_color.redF() << std::endl;
  file << saved_info_.lines_color.greenF() << std::endl;
  file << saved_info_.lines_color.blueF() << std::endl;
  file << saved_info_.size_lines << std::endl;
  file << saved_info_.type_vertexes << std::endl;
  file << saved_info_.vertexes_color.redF() << std::endl;
  file << saved_info_.vertexes_color.greenF() << std::endl;
  file << saved_info_.vertexes_color.blueF() << std::endl;
  file << saved_info_.size_vertexes << std::endl;
  file << saved_info_.type_model << std::endl;
  file << saved_info_.model_color.redF() << std::endl;
  file << saved_info_.model_color.greenF() << std::endl;
  file << saved_info_.model_color.blueF() << std::endl;
  file << saved_info_.light_color.redF() << std::endl;
  file << saved_info_.light_color.greenF() << std::endl;
  file << saved_info_.light_color.blueF() << std::endl;
  file << saved_info_.light_power << std::endl;
  file.close();
}

void Memento::WriteOutFile() {
  std::string filename = QDir::homePath().toStdString();
  filename += "/Desktop/3DVIEWERv2/settings.conf";
  std::ifstream file(filename);
  double bg_color_red, bg_color_green, bg_color_blue, lines_color_red,
      lines_color_green, lines_color_blue, vertexes_color_red,
      vertexes_color_green, vertexes_color_blue, model_color_red,
      model_color_green, model_color_blue, light_color_red, light_color_green,
      light_color_blue;

  file >> saved_info_.type_proection;
  file >> bg_color_red;
  file >> bg_color_green;
  file >> bg_color_blue;
  file >> saved_info_.type_lines;
  file >> lines_color_red;
  file >> lines_color_green;
  file >> lines_color_blue;
  file >> saved_info_.size_lines;
  file >> saved_info_.type_vertexes;
  file >> vertexes_color_red;
  file >> vertexes_color_green;
  file >> vertexes_color_blue;
  file >> saved_info_.size_vertexes;
  file >> saved_info_.type_model;
  file >> model_color_red;
  file >> model_color_green;
  file >> model_color_blue;
  file >> light_color_red;
  file >> light_color_green;
  file >> light_color_blue;
  file >> saved_info_.light_power;
  file.close();

  saved_info_.background_color.setRedF(bg_color_red);
  saved_info_.background_color.setGreenF(bg_color_green);
  saved_info_.background_color.setBlueF(bg_color_blue);
  saved_info_.lines_color.setRedF(lines_color_red);
  saved_info_.lines_color.setGreenF(lines_color_green);
  saved_info_.lines_color.setBlueF(lines_color_blue);
  saved_info_.vertexes_color.setRedF(vertexes_color_red);
  saved_info_.vertexes_color.setGreenF(vertexes_color_green);
  saved_info_.vertexes_color.setBlueF(vertexes_color_blue);
  saved_info_.model_color.setRedF(model_color_red);
  saved_info_.model_color.setGreenF(model_color_green);
  saved_info_.model_color.setBlueF(model_color_blue);
  saved_info_.light_color.setRedF(light_color_red);
  saved_info_.light_color.setGreenF(light_color_green);
  saved_info_.light_color.setBlueF(light_color_blue);
}

void Memento::Saving(StatusStorage value) { saved_info_ = value; }
}  // namespace s21
