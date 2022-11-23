#include "settings.h"

namespace s21 {
Settings::Settings() : texture_status_(0) {}

Settings::Settings(StatusStorage value) : info_(value), texture_status_(0) {}

StatusStorage Settings::Save() { return info_; }

void Settings::Restore(Memento mem) {
  info_.type_proection = mem.GetTypeProection();
  info_.background_color = mem.GetBackgroundColor();
  info_.type_lines = mem.GetTypeLines();
  info_.lines_color = mem.GetLinesColor();
  info_.size_lines = mem.GetSizeLines();
  info_.type_vertexes = mem.GetTypeVertexes();
  info_.vertexes_color = mem.GetVertexesColor();
  info_.size_vertexes = mem.GetSizeVertexes();
  info_.type_model = mem.GetTypeModel();
  info_.model_color = mem.GetModelColor();
  info_.light_color = mem.GetLightColor();
  info_.light_power = mem.GetLightPower();
}

void Settings::SetTypeProection(int value) { info_.type_proection = value; }

void Settings::SetBGColor(QColor color) { info_.background_color = color; }

void Settings::SetTypeLines(int value) { info_.type_lines = value; }

void Settings::SetEdgeColor(QColor color) { info_.lines_color = color; }

void Settings::SetSizeLines(double value) { info_.size_lines = value; }

void Settings::SetTypeVertexes(int value) { info_.type_vertexes = value; }

void Settings::SetVertexColor(QColor color) { info_.vertexes_color = color; }

void Settings::SetSizeVertexes(double value) { info_.size_vertexes = value; }

void Settings::SetTypeModel(int value) { info_.type_model = value; }

void Settings::SetModelColor(QColor color) { info_.model_color = color; }

void Settings::SetLightColor(QColor color) { info_.light_color = color; }

void Settings::SetLightPower(double value) { info_.light_power = value; }

void Settings::SetTextureStatus(bool val) { texture_status_ = val; }

bool Settings::GetTextureStatus() { return texture_status_; }
}  // namespace s21
