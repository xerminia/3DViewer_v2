#ifndef SRC_SETTINGS_MEMENTO_H_
#define SRC_SETTINGS_MEMENTO_H_

#include <QColor>
#include <QDir>
#include <fstream>

namespace s21 {
struct StatusStorage {
  int type_proection;
  QColor background_color;
  int type_lines;
  QColor lines_color;
  double size_lines;
  int type_vertexes;
  QColor vertexes_color;
  double size_vertexes;
  int type_model;
  QColor model_color;
  QColor light_color;
  double light_power;
};

class Memento {
 public:
  Memento() = default;
  explicit Memento(StatusStorage value);
  ~Memento() = default;

  int GetTypeProection();
  QColor GetBackgroundColor();
  int GetTypeLines();
  QColor GetLinesColor();
  int GetSizeLines();
  int GetTypeVertexes();
  QColor GetVertexesColor();
  int GetSizeVertexes();
  int GetTypeModel();
  QColor GetModelColor();
  QColor GetLightColor();
  double GetLightPower();

  void WriteInFile();
  void WriteOutFile();

  void Saving(StatusStorage value);

 private:
  StatusStorage saved_info_;
};
}  // namespace s21

#endif  // SRC_SETTINGS_MEMENTO_H_
