#include <check.h>
#include <stdio.h>

#include "3d_process.h"

START_TEST(scan_obj_test) {
  data_t data;
  char buf0[16] = "", buf1[16] = "", buf2[16] = "";

  scan_obj_size(&data, "obj/1.obj");
  struct_malloc(&data);
  scan_obj(&data, "obj/1.obj");

  ck_assert_int_eq(data.n_vertexes, 8);
  ck_assert_int_eq(data.n_facets, 6);
  ck_assert_int_eq(data.vert_in_facet, 4);

  snprintf(buf0, sizeof(buf0), "%lf", data.vertexes[4][0]);
  snprintf(buf1, sizeof(buf1), "%lf", data.vertexes[4][1]);
  snprintf(buf2, sizeof(buf2), "%lf", data.vertexes[4][2]);

  ck_assert_str_eq(buf0, "1.000000");
  ck_assert_str_eq(buf1, "1.000000");
  ck_assert_str_eq(buf2, "0.000000");

  struct_free(&data);
}
END_TEST

START_TEST(move_scale_obj_test) {
  data_t data;
  char buf0[16] = "", buf1[16] = "", buf2[16] = "";

  scan_obj_size(&data, "obj/1.obj");
  struct_malloc(&data);
  scan_obj(&data, "obj/1.obj");

  ck_assert_int_eq(data.n_vertexes, 8);
  ck_assert_int_eq(data.n_facets, 6);
  ck_assert_int_eq(data.vert_in_facet, 4);

  s21_move(&data, 1, 1, 1);
  scale(&data, 2, 2, 2);

  snprintf(buf0, sizeof(buf0), "%lf", data.vertexes[4][0]);
  snprintf(buf1, sizeof(buf1), "%lf", data.vertexes[4][1]);
  snprintf(buf2, sizeof(buf2), "%lf", data.vertexes[4][2]);

  ck_assert_str_eq(buf0, "4.000000");
  ck_assert_str_eq(buf1, "4.000000");
  ck_assert_str_eq(buf2, "2.000000");

  struct_free(&data);
}
END_TEST

START_TEST(turn_obj_test) {
  data_t data;
  char buf0[16] = "", buf1[16] = "", buf2[16] = "";

  scan_obj_size(&data, "obj/1.obj");
  struct_malloc(&data);
  scan_obj(&data, "obj/1.obj");

  ck_assert_int_eq(data.n_vertexes, 8);
  ck_assert_int_eq(data.n_facets, 6);
  ck_assert_int_eq(data.vert_in_facet, 4);

  turn_x(&data, 6.28);
  turn_y(&data, 6.28);
  turn_z(&data, 6.28);

  snprintf(buf0, sizeof(buf0), "%lf", data.vertexes[4][0]);
  snprintf(buf1, sizeof(buf1), "%lf", data.vertexes[4][1]);
  snprintf(buf2, sizeof(buf2), "%lf", data.vertexes[4][2]);

  ck_assert_str_eq(buf0, "1.003185");
  ck_assert_str_eq(buf1, "0.996805");
  ck_assert_str_eq(buf2, "0.000000");

  struct_free(&data);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);

  tcase_add_test(tc1_1, scan_obj_test);
  tcase_add_test(tc1_1, move_scale_obj_test);
  tcase_add_test(tc1_1, turn_obj_test);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
