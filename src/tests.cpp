#include <gtest/gtest.h>

#include "Model/parsingmodel.h"

TEST(test_count, test_count_vert) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  ASSERT_EQ(test.GetObject().count_of_vertexes, 8);
}

TEST(test_count, test_count_edge) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  ASSERT_EQ(test.GetObject().count_of_edge, 24);
}

TEST(test_count, test_count_triangles) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  ASSERT_EQ(test.GetObject().count_of_triangles, 0);
}

TEST(test_count, test_count_quads) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  ASSERT_EQ(test.GetObject().count_of_quads, 6);
}

TEST(vertexes_test, vert_position_test) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  double right_numbers[24] = {1,  1, -1, 1,  -1, -1, 1,  1, 1, 1,  -1, 1,
                              -1, 1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1};

  int n = 0;

  for (size_t i = 0; i < test.GetObject().vert.size(); i++) {
    for (size_t j = 0; j < test.GetObject().vert[i].size(); j++) {
      ASSERT_EQ(test.GetObject().vert[i][j], right_numbers[n]);
      n++;
    }
  }
}

TEST(vertexes_test, vert_textcoord_test) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  double right_numbers[28] = {0.625, 0.5,   0.875, 0.5,   0.875, 0.75,  0.625,
                              0.75,  0.375, 0.75,  0.625, 1,     0.375, 1,
                              0.375, 0,     0.625, 0,     0.625, 0.25,  0.375,
                              0.25,  0.125, 0.5,   0.375, 0.5,   0.125, 0.75};

  int n = 0;

  for (size_t i = 0; i < test.GetObject().vert_t.size(); i++) {
    for (size_t j = 0; j < test.GetObject().vert_t[i].size(); j++) {
      ASSERT_EQ(test.GetObject().vert_t[i][j], right_numbers[n]);
      n++;
    }
  }
}

TEST(vertexes_test, vert_normals_test) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  double right_numbers[18] = {0, 1,  0, 0, 0, 1, -1, 0, 0,
                              0, -1, 0, 1, 0, 0, 0,  0, -1};

  int n = 0;

  for (size_t i = 0; i < test.GetObject().vert_n.size(); i++) {
    for (size_t j = 0; j < test.GetObject().vert_n[i].size(); j++) {
      ASSERT_EQ(test.GetObject().vert_n[i][j], right_numbers[n]);
      n++;
    }
  }
}

TEST(test_poly, test_poly_1) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  int right_numbers[72] = {1, 1,  1, 5, 2,  1, 7, 3,  1, 3, 4,  1, 4, 5, 2,
                           3, 4,  2, 7, 6,  2, 8, 7,  2, 8, 8,  3, 7, 9, 3,
                           5, 10, 3, 6, 11, 3, 6, 12, 4, 2, 13, 4, 4, 5, 4,
                           8, 14, 4, 2, 13, 5, 1, 1,  5, 3, 4,  5, 4, 5, 5,
                           6, 11, 6, 5, 10, 6, 1, 1,  6, 2, 13, 6};

  int n = 0;

  for (size_t i = 0; i < test.GetObject().poly.size(); i++) {
    for (size_t j = 0; j < test.GetObject().poly[i].size(); j++) {
      ASSERT_EQ(test.GetObject().poly[i][j], right_numbers[n]);
      n++;
    }
  }
}

TEST(test_scale, test_scale_1) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  test.AthenianScaleModel(2);

  double right_numbers[24] = {2,  2, -2, 2,  -2, -2, 2,  2, 2, 2,  -2, 2,
                              -2, 2, -2, -2, -2, -2, -2, 2, 2, -2, -2, 2};
  int n = 0;

  for (size_t i = 0; i < test.GetObject().vert.size(); i++) {
    for (size_t j = 0; j < test.GetObject().vert[i].size(); j++) {
      ASSERT_EQ(test.GetObject().vert[i][j], right_numbers[n]);
      n++;
    }
  }
}

TEST(test_rotate_x, test_rotate_x_1) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  test.AthenianRotateModel(150, AXISX);

  double right_numbers[24] = {1,  -0.366025, 1.36603,   1,  1.36603,  0.366025,
                              1,  -1.36603,  -0.366025, 1,  0.366025, -1.36603,
                              -1, -0.366025, 1.36603,   -1, 1.36603,  0.366025,
                              -1, -1.36603,  -0.366025, -1, 0.366025, -1.36603};

  int n = 0;

  for (size_t i = 0; i < test.GetObject().vert.size(); i++) {
    for (size_t j = 0; j < test.GetObject().vert[i].size(); j++) {
      ASSERT_NEAR(test.GetObject().vert[i][j], right_numbers[n], 0.00001);
      n++;
    }
  }
}

TEST(test_rotate_y, test_rotate_y_1) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  test.AthenianRotateModel(125, AXISY);

  double right_numbers[24] = {-1.39273,  1, -0.245576, -1.39273,  -1, -0.245576,
                              0.245576,  1, -1.39273,  0.245576,  -1, -1.39273,
                              -0.245576, 1, 1.39273,   -0.245576, -1, 1.39273,
                              1.39273,   1, 0.245576,  1.39273,   -1, 0.245576};

  int n = 0;

  for (size_t i = 0; i < test.GetObject().vert.size(); i++) {
    for (size_t j = 0; j < test.GetObject().vert[i].size(); j++) {
      ASSERT_NEAR(test.GetObject().vert[i][j], right_numbers[n], 0.00001);
      n++;
    }
  }
}

TEST(test_rotate_z, test_rotate_z_1) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  test.AthenianRotateModel(270, AXISZ);

  double right_numbers[24] = {1, -1, -1, -1, -1, -1, 1, -1, 1, -1, -1, 1,
                              1, 1,  -1, -1, 1,  -1, 1, 1,  1, -1, 1,  1};

  int n = 0;

  for (size_t i = 0; i < test.GetObject().vert.size(); i++) {
    for (size_t j = 0; j < test.GetObject().vert[i].size(); j++) {
      ASSERT_NEAR(test.GetObject().vert[i][j], right_numbers[n], 0.001);
      n++;
    }
  }
}

TEST(test_move_x, test_move_x_1) {
  s21::ParsingModel test;
  test.OpenFile("Objects/cube.obj");

  test.AthenianMove(3, AXISX);

  double right_numbers[24] = {4, 1, -1, 4, -1, -1, 4, 1, 1, 4, -1, 1,
                              2, 1, -1, 2, -1, -1, 2, 1, 1, 2, -1, 1};

  int n = 0;

  for (size_t i = 0; i < test.GetObject().vert.size(); i++) {
    for (size_t j = 0; j < test.GetObject().vert[i].size(); j++) {
      ASSERT_NEAR(test.GetObject().vert[i][j], right_numbers[n], 0.001);
      n++;
    }
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
