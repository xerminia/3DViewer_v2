#include "facade.h"

namespace s21 {
void Facade::InitObject(std::string path) { model_->OpenFile(path); }

void Facade::AthenianMove(double moving, int axis) {
  model_->AthenianMove(moving, axis);
}

void Facade::AthenianRotate(double angle, int axis) {
  model_->AthenianRotateModel(angle, axis);
}

void Facade::AthenianScale(double scale) { model_->AthenianScaleModel(scale); }

Vertex Facade::GetObject() { return model_->GetObject(); }
}  // namespace s21
