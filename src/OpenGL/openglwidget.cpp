#define GL_SILENCE_DEPRECATION

#include "openglwidget.h"

#include <cmath>

constexpr int MODE_NO_NORMALIES_NO_TEXTURIES = 1;
constexpr int MODE_NO_NORMALIES_WITH_TEXTURIES = 2;
constexpr int MODE_WITH_NORMALIES_NO_TEXTURIES = 3;
constexpr int MODE_WITH_ALL = 4;

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      control_widget_(nullptr),
      settings_(nullptr),
      texture_(0),
      index_buffer_(QOpenGLBuffer::IndexBuffer),
      scale_rotate_(-10),
      move_x_(0),
      move_y_(0),
      light_position_x(0),
      light_position_y(0),
      light_position_z(0),
      buffering_mode_(-1) {
  QSurfaceFormat format;
  format.setVersion(4, 0);
  setFormat(format);
  setFocusPolicy(Qt::ClickFocus);
}

void OpenGLWidget::InitFacade(s21::Facade *facade) { control_widget_ = facade; }

void OpenGLWidget::InitSettings(s21::Settings *settings) {
  settings_ = settings;
}

void OpenGLWidget::InitModel(std::string path) {
  // setFormat(QSurfaceFormat::OpenGL);
  control_widget_->InitObject(path);
  SetModeStatus(control_widget_->GetObject());
  BufferFilling();
  SetDefaultCoord();
}

void OpenGLWidget::SetTextureStatus(bool val) {
  settings_->SetTextureStatus(val);
}

void OpenGLWidget::SetTexture(QString path) {
  texture_ = new QOpenGLTexture(QImage(path).mirrored());  // задаем текстуру
  texture_->setMinificationFilter(
      QOpenGLTexture::Nearest);  // какие то фильтры на текстуру
  texture_->setMagnificationFilter(QOpenGLTexture::Linear);
  texture_->setWrapMode(QOpenGLTexture::Repeat);  // в отдельный метод
}

void OpenGLWidget::SetMatrixRotation(QQuaternion val) {
  matrix_rotation_ = val * matrix_rotation_;
}

void OpenGLWidget::BufferFilling() {
  QVector<VertexData> vertexes;
  QVector<GLuint> indexes;

  s21::Vertex object = control_widget_->GetObject();
  size_t check_texturies_and_normal = 1;  // для отрисовки линий

  if (buffering_mode_ == MODE_NO_NORMALIES_NO_TEXTURIES) {
    FillingNoNormaliesAndNoTexturies(object, vertexes, indexes);
  } else if (buffering_mode_ == MODE_NO_NORMALIES_WITH_TEXTURIES) {
    FillingNoNormaliesWithTexturies(object, vertexes, indexes);
    check_texturies_and_normal = 2;
  } else if (buffering_mode_ == MODE_WITH_NORMALIES_NO_TEXTURIES) {
    FillingWithNormaliesNoTexturies(object, vertexes, indexes);
    check_texturies_and_normal = 2;
  } else if (buffering_mode_ == MODE_WITH_ALL) {
    FillingWithAll(object, vertexes, indexes);
    check_texturies_and_normal = 3;
  }

  FillingPointsAndLines(object, vertexes, indexes, check_texturies_and_normal);

  vertex_buffer_.destroy();
  vertex_buffer_.create();
  vertex_buffer_.bind();
  vertex_buffer_.allocate(vertexes.constData(),
                          vertexes.size() * sizeof(VertexData));
  vertex_buffer_.release();

  index_buffer_.destroy();
  index_buffer_.create();
  index_buffer_.bind();
  index_buffer_.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
  index_buffer_.release();
}

void OpenGLWidget::SetDefaultCoord() {
  settings_->SetTextureStatus(0);
  texture_ = 0;
  move_x_ = 0;
  move_y_ = 0;
  light_position_x = 0;
  light_position_y = 0;
  light_position_z = 0;
  scale_rotate_ = -10;
  matrix_rotation_.setX(0);
  matrix_rotation_.setY(0);
  matrix_rotation_.setZ(0);
  matrix_rotation_.setScalar(1);
}

void OpenGLWidget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  InitShaders();
}

void OpenGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  float aspect = w / (float)h;
  projection_matrix_.setToIdentity();
  projection_matrix_.perspective(45, aspect, 0.01, 1000.0);
}

void OpenGLWidget::paintGL() {
  //     glShadeModel(GL_FLAT);
  //    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
  s21::StatusStorage setting_preferences =
      settings_->Save();  // берем настройки

  glClearColor(setting_preferences.background_color.redF(),
               setting_preferences.background_color.greenF(),
               setting_preferences.background_color.blueF(),
               setting_preferences.background_color.alphaF());  // цвет фона

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // чистим буфер фона
  QMatrix4x4 model_matrix;  //  модельная матрица
  QMatrix4x4 view_matrix;  // видовая матрица камеры (view_matrix)
  model_matrix.setToIdentity();  // делаем ее единичной
  view_matrix.setToIdentity();   // делаем матрицу ед
  view_matrix.translate(move_x_, move_y_,
                        scale_rotate_);  // перемещение и отдаление матрицы
  // задаем тип проекции
  if (setting_preferences.type_proection == 0) {
    view_matrix.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 100.0f);
  }

  if (settings_->GetTextureStatus()) texture_->bind(0);  // биндим текстуру

  view_matrix.rotate(matrix_rotation_);  // поворот матрицы

  // цвет света
  QVector4D light_color = {setting_preferences.light_color.redF(),
                           setting_preferences.light_color.greenF(),
                           setting_preferences.light_color.blueF(),
                           setting_preferences.light_color.alphaF()};

  program_.bind();
  program_.setUniformValue("flag_texture", settings_->GetTextureStatus());
  program_.setUniformValue("flag_type", setting_preferences.type_model);
  program_.setUniformValue("u_projection_matrix",
                           projection_matrix_);  // задаем матрицу проекции
  program_.setUniformValue("u_view_matrix",
                           view_matrix);  // задаем видовую матрицу
  program_.setUniformValue("u_model_matrix",
                           model_matrix);  // задаем модельную матрицу
  program_.setUniformValue(
      "u_texture", 0);  // говорим чтобы накладывалась текстура под номером 0
  program_.setUniformValue(
      "u_light_position",
      QVector4D(light_position_x, light_position_y, light_position_z,
                1.0));  // позиция источника света
  program_.setUniformValue(
      "u_light_power", (float)setting_preferences.light_power);  // сила света
  program_.setUniformValue("u_light_color", light_color);  // цвет света

  vertex_buffer_.bind();  // биндим буффер для вершин

  int offset = 0;
  int vertLoc = program_.attributeLocation("a_position");
  program_.enableAttributeArray(vertLoc);
  program_.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));
  offset += sizeof(QVector3D);
  int texLoc = program_.attributeLocation("a_texcoord");
  program_.enableAttributeArray(texLoc);
  program_.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));
  offset += sizeof(QVector2D);
  int normLoc = program_.attributeLocation("a_normal");
  program_.enableAttributeArray(normLoc);
  program_.setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

  index_buffer_.bind();  // биндим буффер с индексами

  DrawObject(setting_preferences);
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
  if ((event->angleDelta().y()) > 0)
    scale_rotate_ += 0.5;
  else if ((event->angleDelta().y()) < 0)
    scale_rotate_ -= 0.5;

  update();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    object_move_ = 1;
  } else if (event->button() == Qt::RightButton) {
    object_move_ = 0;
  }
  mouse_position_ = QVector2D(event->position());
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (object_move_) {
    QVector2D diff = QVector2D(event->position()) - mouse_position_;
    float angle = diff.length() / 2.0;
    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);
    matrix_rotation_ =
        QQuaternion::fromAxisAndAngle(axis, angle) * matrix_rotation_;

  } else {
    move_x_ += 7 * (event->position().x() - mouse_position_.x()) / width();
    move_y_ +=
        -1 * (7 * (event->position().y() - mouse_position_.y()) / height());
  }
  mouse_position_ = QVector2D(event->position());
  update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
    case Qt::Key_W:
      light_position_y++;
      break;
    case Qt::Key_S:
      light_position_y--;
      break;
    case Qt::Key_D:
      light_position_x++;
      break;
    case Qt::Key_A:
      light_position_x--;
      break;
    case Qt::Key_Space:
      light_position_z++;
      break;
    case Qt::Key_Shift:
      light_position_z--;
      break;
  }
  update();
}

void OpenGLWidget::InitShaders() {
  if (!program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
    close();
  if (!program_.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                        ":/fshader.fsh"))
    close();
  if (!program_.link()) close();
}

void OpenGLWidget::SetModeStatus(s21::Vertex const &object) {
  if (!object.vert_n.size() && !object.vert_t.size()) {
    buffering_mode_ = MODE_NO_NORMALIES_NO_TEXTURIES;
  } else if (!object.vert_n.size() && object.vert_t.size()) {
    buffering_mode_ = MODE_NO_NORMALIES_WITH_TEXTURIES;
  } else if (object.vert_n.size() && !object.vert_t.size()) {
    buffering_mode_ = MODE_WITH_NORMALIES_NO_TEXTURIES;
  } else if (object.vert_n.size() && object.vert_t.size()) {
    buffering_mode_ = MODE_WITH_ALL;
  }
}

void OpenGLWidget::DrawObject(s21::StatusStorage setting_preferences) {
  // glShadeModel(GL_FLAT);
  if (setting_preferences.type_model) {
    // цвет модели
    QVector4D base_color = {setting_preferences.model_color.redF(),
                            setting_preferences.model_color.greenF(),
                            setting_preferences.model_color.blueF(),
                            setting_preferences.model_color.alphaF()};
    program_.setUniformValue("u_base_color", base_color);

    // отрисовка 4 точками (квадратов)
    glDrawElements(GL_QUADS, control_widget_->GetObject().count_of_quads * 4,
                   GL_UNSIGNED_INT, 0);

    // отрисовка 3 точками (треугольники)
    glDrawElements(
        GL_TRIANGLES, control_widget_->GetObject().count_of_triangles * 3,
        GL_UNSIGNED_INT,
        (GLvoid *)(sizeof(GLubyte) *
                   (control_widget_->GetObject().count_of_quads * 16)));
  }

  // отрисовывать вершины или нет
  if (setting_preferences.type_vertexes != 0) {
    // размер вершин
    glPointSize(setting_preferences.size_vertexes);

    // тип вершин
    if (setting_preferences.type_vertexes == 1) {
      glDisable(GL_TRIANGLE_STRIP);
      glEnable(GL_POINT_SMOOTH);
    } else if (setting_preferences.type_vertexes == 2) {
      glDisable(GL_POINT_SMOOTH);
      glEnable(GL_TRIANGLE_STRIP);
    }

    // цвет вершин
    QVector4D vertexColor = {setting_preferences.vertexes_color.redF(),
                             setting_preferences.vertexes_color.greenF(),
                             setting_preferences.vertexes_color.blueF(),
                             setting_preferences.vertexes_color.alphaF()};
    program_.setUniformValue("u_base_color", vertexColor);

    // отрисовка вершин
    glDrawElements(
        GL_POINTS, control_widget_->GetObject().count_of_vertexes,
        GL_UNSIGNED_INT,
        (GLvoid *)(sizeof(GLubyte) *
                   ((control_widget_->GetObject().count_of_quads * 16) +
                    (control_widget_->GetObject().count_of_triangles * 12))));
  }

  // тип отображения модели
  if (setting_preferences.type_lines) {
    // задаем ширину линий
    glLineWidth(setting_preferences.size_lines);

    // тип линий
    if (setting_preferences.type_lines == 2) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00F0);
    } else if (setting_preferences.type_lines == 1) {
      glDisable(GL_LINE_STIPPLE);
    }

    // цвет линий
    QVector4D edges_color = {setting_preferences.lines_color.redF(),
                             setting_preferences.lines_color.greenF(),
                             setting_preferences.lines_color.blueF(),
                             setting_preferences.lines_color.alphaF()};
    program_.setUniformValue("u_base_color", edges_color);

    // отрисовка линий
    glDrawElements(
        GL_LINES, control_widget_->GetObject().count_of_edge * 2,
        GL_UNSIGNED_INT,
        (GLvoid *)(sizeof(GLubyte) *
                   ((control_widget_->GetObject().count_of_vertexes * 4) +
                    (control_widget_->GetObject().count_of_quads * 16) +
                    (control_widget_->GetObject().count_of_triangles * 12))));
  }
}

void OpenGLWidget::FillingNoNormaliesAndNoTexturies(
    s21::Vertex const &object, QVector<VertexData> &vertexes,
    QVector<GLuint> &indexes) {
  // quads
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size() && object.poly[i].size() == 4;
         j++) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2])));
      indexes.append(indexes.size());
    }
  }
  // triangles
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size() && object.poly[i].size() == 3;
         j++) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2])));
      indexes.append(indexes.size());
    }
  }
}

void OpenGLWidget::FillingNoNormaliesWithTexturies(
    s21::Vertex const &object, QVector<VertexData> &vertexes,
    QVector<GLuint> &indexes) {
  // quads
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size() && object.poly[i].size() == 8;
         j += 2) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2]),
                     QVector2D(object.vert_t[object.poly[i][j + 1] - 1][0],
                               object.vert_t[object.poly[i][j + 1] - 1][1])));
      indexes.append(indexes.size());
    }
  }
  // triangles
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size() && object.poly[i].size() == 6;
         j += 2) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2]),
                     QVector2D(object.vert_t[object.poly[i][j + 1] - 1][0],
                               object.vert_t[object.poly[i][j + 1] - 1][1])));
      indexes.append(indexes.size());
    }
  }
}

void OpenGLWidget::FillingWithNormaliesNoTexturies(
    s21::Vertex const &object, QVector<VertexData> &vertexes,
    QVector<GLuint> &indexes) {
  // quads
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size() && object.poly[i].size() == 8;
         j += 2) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2]),
                     QVector3D(object.vert_n[object.poly[i][j + 1] - 1][0],
                               object.vert_n[object.poly[i][j + 1] - 1][1],
                               object.vert_n[object.poly[i][j + 1] - 1][2])));
      indexes.append(indexes.size());
    }
  }
  // triangles
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size() && object.poly[i].size() == 6;
         j += 2) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2]),
                     QVector3D(object.vert_n[object.poly[i][j + 1] - 1][0],
                               object.vert_n[object.poly[i][j + 1] - 1][1],
                               object.vert_n[object.poly[i][j + 1] - 1][2])));
      indexes.append(indexes.size());
    }
  }
}

void OpenGLWidget::FillingWithAll(s21::Vertex const &object,
                                  QVector<VertexData> &vertexes,
                                  QVector<GLuint> &indexes) {
  // quads
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size() && object.poly[i].size() == 12;
         j += 3) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2]),
                     QVector2D(object.vert_t[object.poly[i][j + 1] - 1][0],
                               object.vert_t[object.poly[i][j + 1] - 1][1]),
                     QVector3D(object.vert_n[object.poly[i][j + 2] - 1][0],
                               object.vert_n[object.poly[i][j + 2] - 1][1],
                               object.vert_n[object.poly[i][j + 2] - 1][2])));
      indexes.append(indexes.size());
    }
  }
  // triangles
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size() && object.poly[i].size() == 9;
         j += 3) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2]),
                     QVector2D(object.vert_t[object.poly[i][j + 1] - 1][0],
                               object.vert_t[object.poly[i][j + 1] - 1][1]),
                     QVector3D(object.vert_n[object.poly[i][j + 2] - 1][0],
                               object.vert_n[object.poly[i][j + 2] - 1][1],
                               object.vert_n[object.poly[i][j + 2] - 1][2])));
      indexes.append(indexes.size());
    }
  }
}

void OpenGLWidget::FillingPointsAndLines(const s21::Vertex &object,
                                         QVector<VertexData> &vertexes,
                                         QVector<GLuint> &indexes,
                                         int check_texturies_and_normal) {
  // points
  for (size_t i = 0; i < object.vert.size(); i++) {
    vertexes.append(VertexData(
        QVector3D(object.vert[i][0], object.vert[i][1], object.vert[i][2])));
    indexes.append(indexes.size());
  }
  // lines
  for (size_t i = 0; i < object.poly.size(); i++) {
    for (size_t j = 0; j < object.poly[i].size();
         j += check_texturies_and_normal) {
      vertexes.append(
          VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                               object.vert[object.poly[i][j] - 1][1],
                               object.vert[object.poly[i][j] - 1][2])));
      indexes.append(indexes.size());
      if (j) {
        vertexes.append(
            VertexData(QVector3D(object.vert[object.poly[i][j] - 1][0],
                                 object.vert[object.poly[i][j] - 1][1],
                                 object.vert[object.poly[i][j] - 1][2])));
        indexes.append(indexes.size());
      }
    }
    vertexes.append(
        VertexData(QVector3D(object.vert[object.poly[i][0] - 1][0],
                             object.vert[object.poly[i][0] - 1][1],
                             object.vert[object.poly[i][0] - 1][2])));
    indexes.append(indexes.size());
  }
}
