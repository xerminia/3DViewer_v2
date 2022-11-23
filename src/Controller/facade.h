#ifndef SRC_CONTROLLER_FACADE_H_
#define SRC_CONTROLLER_FACADE_H_

#include "Model/parsingmodel.h"

namespace s21 {
class Facade {
 public:
  Facade() = default;
  explicit Facade(s21::ParsingModel *model) : model_(model) {}
  ~Facade() = default;

  void InitObject(std::string path);
  void AthenianMove(double moving, int axis);
  void AthenianRotate(double angle, int axis);
  void AthenianScale(double scale);

  Vertex GetObject();

 private:
  s21::ParsingModel *model_;
};
}  // namespace s21

#endif  // SRC_CONTROLLER_FACADE_H_
