#ifndef SRC_OPENGL_OPENGLWIDGET_H_
#define SRC_OPENGL_OPENGLWIDGET_H_

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWheelEvent>

#include "Controller/facade.h"
#include "Settings/settings.h"

class OpenGLWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
  OpenGLWidget() = default;
  explicit OpenGLWidget(QWidget *parent = nullptr);
  ~OpenGLWidget() = default;

  void InitFacade(s21::Facade *facade);
  void InitSettings(s21::Settings *settings);
  void InitModel(std::string path);

  void SetTextureStatus(bool val);
  void SetTexture(QString path);
  void SetMatrixRotation(QQuaternion val);

  void BufferFilling();
  void SetDefaultCoord();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void keyPressEvent(QKeyEvent *e) override;

 private:
  struct VertexData {
    explicit VertexData(QVector3D p, QVector2D t, QVector3D n)
        : position(p), textPosition(t), normals(n) {}
    explicit VertexData(QVector3D p) : position(p) {}
    explicit VertexData(QVector3D p, QVector2D t)
        : position(p), textPosition(t) {}
    explicit VertexData(QVector3D p, QVector3D n) : position(p), normals(n) {}
    QVector3D position;
    QVector2D textPosition;
    QVector3D normals;
  };

  s21::Facade *control_widget_;
  s21::Settings *settings_;
  QOpenGLTexture *texture_;

  QMatrix4x4 projection_matrix_;
  QOpenGLShaderProgram program_;
  QOpenGLBuffer vertex_buffer_;
  QOpenGLBuffer index_buffer_;

  QQuaternion matrix_rotation_;
  QVector2D mouse_position_;
  bool object_move_;
  double scale_rotate_;
  double move_x_;
  double move_y_;
  float light_position_x;
  float light_position_y;
  float light_position_z;
  int buffering_mode_;

  void InitShaders();
  void SetModeStatus(s21::Vertex const &object);
  void DrawObject(s21::StatusStorage setting_preferences);
  void FillingNoNormaliesAndNoTexturies(s21::Vertex const &object,
                                        QVector<VertexData> &vertexes,
                                        QVector<GLuint> &indexes);
  void FillingNoNormaliesWithTexturies(s21::Vertex const &object,
                                       QVector<VertexData> &vertexes,
                                       QVector<GLuint> &indexes);
  void FillingWithNormaliesNoTexturies(s21::Vertex const &object,
                                       QVector<VertexData> &vertexes,
                                       QVector<GLuint> &indexes);
  void FillingWithAll(s21::Vertex const &object, QVector<VertexData> &vertexes,
                      QVector<GLuint> &indexes);
  void FillingPointsAndLines(s21::Vertex const &object,
                             QVector<VertexData> &vertexes,
                             QVector<GLuint> &indexes,
                             int check_texturies_and_normal);
};

#endif  // SRC_OPENGL_OPENGLWIDGET_H_
