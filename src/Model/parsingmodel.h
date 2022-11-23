#ifndef SRC_MODEL_PARSINGMODEL_H_
#define SRC_MODEL_PARSINGMODEL_H_

#include <cmath>
#include <fstream>
#include <string>
#include <vector>

constexpr int AXISX = 0;
constexpr int AXISY = 1;
constexpr int AXISZ = 2;

namespace s21 {
class Vertex {
 public:
  Vertex()
      : count_of_triangles(0),
        count_of_quads(0),
        count_of_vertexes(0),
        count_of_edge(0) {}
  int count_of_triangles;
  int count_of_quads;
  int count_of_vertexes;
  int count_of_edge;
  std::vector<std::vector<double>> vert;
  std::vector<std::vector<double>> vert_t;
  std::vector<std::vector<double>> vert_n;
  std::vector<std::vector<int>> poly;
};

class ParsingModel {
 public:
  ParsingModel() = default;
  ~ParsingModel() = default;

  void OpenFile(std::string path);
  void AthenianMove(double moving, int axis);
  void AthenianRotateModel(double angle, int axis);
  void AthenianScaleModel(double scale);

  Vertex GetObject();

 private:
  Vertex object_;

  void CountVertexAndFaces(std::string line);
  void ClearObject();
  void VertFilling(std::string line);
  void VertNFilling(std::string line);
  void VertTFilling(std::string line);
  void PolyFilling(std::string line);
  bool CheckLine(std::string line);
  template <typename T>
  std::vector<T> VectorEntry(std::vector<T> vector, std::string line);
};
}  // namespace s21

#endif  // SRC_MODEL_PARSINGMODEL_H_
