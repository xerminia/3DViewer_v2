#include "parsingmodel.h"

namespace s21 {
void ParsingModel::OpenFile(std::string path) {
  ClearObject();
  std::ifstream file(path);
  std::string line;
  while (std::getline(file, line)) {
    CountVertexAndFaces(line);
  }
  if (object_.vert_n.size() && object_.vert_t.size())
    object_.count_of_edge /= 3;
  else if (object_.vert_n.size() || object_.vert_t.size())
    object_.count_of_edge /= 2;
  file.close();
}

void ParsingModel::AthenianMove(double moving, int axis) {
  for (size_t i = 0; i < object_.vert.size(); i++) {
    object_.vert[i][axis] += moving;
  }
}

void ParsingModel::AthenianRotateModel(double angle, int axis) {
  angle = (angle * M_PI) / 180;
  for (size_t i = 0; i < object_.vert.size(); i++) {
    if (axis == AXISX) {
      double temp_y = object_.vert[i][AXISY];
      double temp_z = object_.vert[i][AXISZ];
      object_.vert[i][AXISY] = cos(angle) * temp_y - sin(angle) * temp_z;
      object_.vert[i][AXISZ] = sin(angle) * temp_y + cos(angle) * temp_z;
    } else if (axis == AXISY) {
      double temp_x = object_.vert[i][AXISX];
      double temp_z = object_.vert[i][AXISZ];
      object_.vert[i][AXISX] = cos(angle) * temp_x + sin(angle) * temp_z;
      object_.vert[i][AXISZ] = -sin(angle) * temp_x + cos(angle) * temp_z;
    } else if (axis == AXISZ) {
      double temp_x = object_.vert[i][AXISX];
      double temp_y = object_.vert[i][AXISY];
      object_.vert[i][AXISX] = cos(angle) * temp_x - sin(angle) * temp_y;
      object_.vert[i][AXISY] = sin(angle) * temp_x + cos(angle) * temp_y;
    }
  }
}

void ParsingModel::AthenianScaleModel(double scale) {
  for (size_t i = 0; i < object_.vert.size(); i++) {
    object_.vert[i][AXISX] *= scale;
    object_.vert[i][AXISY] *= scale;
    object_.vert[i][AXISZ] *= scale;
  }
}

Vertex ParsingModel::GetObject() { return object_; }

void ParsingModel::CountVertexAndFaces(std::string line) {
  if (*line.begin() == 'v' && *(line.begin() + 1) == ' ') {
    VertFilling(line);
  } else if (*line.begin() == 'v' && *(line.begin() + 1) == 't') {
    VertTFilling(line);
  } else if (*line.begin() == 'v' && *(line.begin() + 1) == 'n') {
    VertNFilling(line);
  } else if (*line.begin() == 'f') {
    PolyFilling(line);
  }
}

void ParsingModel::ClearObject() {
  object_.count_of_triangles = 0;
  object_.count_of_quads = 0;
  object_.count_of_edge = 0;
  object_.count_of_vertexes = 0;
  object_.vert.clear();
  object_.poly.clear();
  object_.vert_n.clear();
  object_.vert_t.clear();
}

void ParsingModel::VertFilling(std::string line) {
  std::vector<double> tmp_vector;
  tmp_vector = VectorEntry(tmp_vector, line);
  object_.vert.push_back(tmp_vector);
  object_.count_of_vertexes++;
}

void ParsingModel::VertNFilling(std::string line) {
  std::vector<double> tmp_vector;
  tmp_vector = VectorEntry(tmp_vector, line);
  object_.vert_n.push_back(tmp_vector);
}

void ParsingModel::VertTFilling(std::string line) {
  std::vector<double> tmp_vector;
  tmp_vector = VectorEntry(tmp_vector, line);
  object_.vert_t.push_back(tmp_vector);
}

void ParsingModel::PolyFilling(std::string line) {
  std::vector<int> tmp_vector;
  tmp_vector = VectorEntry(tmp_vector, line);
  if (tmp_vector.size() == 4 || tmp_vector.size() == 8 ||
      tmp_vector.size() == 12)
    object_.count_of_quads++;
  else
    object_.count_of_triangles++;
  object_.count_of_edge += tmp_vector.size();
  object_.poly.push_back(tmp_vector);
}

bool ParsingModel::CheckLine(std::string line) {
  return (*line.begin() >= 0 && *line.begin() <= '9' && *line.begin() != ' ' &&
          *line.begin() != '\r') ||
         *line.begin() == '-';
}

template <typename T>
std::vector<T> ParsingModel::VectorEntry(std::vector<T> vector,
                                         std::string line) {
  size_t count = 0;
  while (line.begin() != line.end()) {
    if (CheckLine(line)) {
      vector.push_back(std::stod(line, &count));
      line.erase(0, count);
      while (line.begin() != line.end() && *line.begin() != ' ') {
        if (*line.begin() == '/') {
          line.erase(0, 1);
          break;
        }
        line.erase(0, 1);
      }
    } else {
      line.erase(0, 1);
    }
  }
  return vector;
}
}  // namespace s21
