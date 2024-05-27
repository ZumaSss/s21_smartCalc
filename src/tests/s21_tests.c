#include "s21_tests.h"

START_TEST(s21_test_0) {
  double orig = 10;
  char res_str[256];
  char str[255] = "5 + 5";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(s21_test_1) {
  double orig = 162;
  char res_str[256];
  char str[255] = "   4 * 5 * 2 * 2 * 2 + 2";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(s21_test_2) {
  double orig = 20;
  char res_str[256];
  char str[255] = "  150 / 10 + 5 * 2 / 2";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(s21_test_3) {
  double orig = 9;
  char res_str[256];
  char str[255] = "  5 + 5 - 2 * 3 / 6";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(calc_test) {
  {
    double orig = -6.31492;
    char res_str[256];
    char str[255] = "4^acos(x/4)/tan(2*x)";
    ck_assert_int_eq(driver(str, "1.2", res_str), 0);
    ck_assert_msg(fabs(atof(res_str)) - fabs(orig) <= 0.00001, "test-1 failed");
  }
  {
    double orig = 4;
    char res_str[256];
    char str[255] = "x+x";
    ck_assert_int_eq(driver(str, "2", res_str), 0);
    ck_assert_msg(fabs(atof(res_str)) - fabs(orig) <= 0.00001, "test-2 failed");
  }
  {
    double orig = 123.456;
    char res_str[256];
    char str[255] = "123+0.456";
    ck_assert_int_eq(driver(str, "0", res_str), 0);
    ck_assert_msg(fabs(atof(res_str)) - fabs(orig) <= 0.00001, "test-3 failed");
  }
  {
    double orig = NAN;
    char res_str[256];
    char str[255] = "log(x)";
    ck_assert_int_eq(driver(str, "-2", res_str), 0);
    ck_assert_msg(atof(res_str) != atof(res_str) && orig != orig,
                  "test-4 failed");
  }
  {
    double orig = 1;
    char res_str[256];
    char str[255] = "-(-1)";
    ck_assert_int_eq(driver(str, "0", res_str), 0);
    ck_assert_msg(atof(res_str) == orig, "test-5 failed");
  }
  {
    double orig = 0.362358;
    char res_str[256];
    char str[255] = "cos(10mod2.2)";
    ck_assert_int_eq(driver(str, "0", res_str), 0);
    ck_assert_msg(fabs(atof(res_str)) - fabs(orig) <= 0.00001, "test-6 failed");
  }
  {
    double orig = 1.517427;
    char res_str[256];
    char str[255] = "sqrt(ln(10))";
    ck_assert_int_eq(driver(str, "0", res_str), 0);
    ck_assert_msg(fabs(atof(res_str)) - fabs(orig) <= 0.00001, "test-7 failed");
  }
  {
    double orig = 1.644775;
    char res_str[256];
    char str[255] = "atan(10)+sin(10)";
    ck_assert_int_eq(driver(str, "0", res_str), 0);
    ck_assert_msg(fabs(atof(res_str)) - fabs(orig) <= 0.00001, "test-8 failed");
  }
  {
    double orig = 1.570796;
    char res_str[256];
    char str[255] = "asin(1)";
    ck_assert_int_eq(driver(str, "0", res_str), 0);
    ck_assert_msg(fabs(atof(res_str)) - fabs(orig) <= 0.00001, "test-9 failed");
  }
  {
    double orig = -210;
    char res_str[256];
    char str[255] = "10-20*(-10)";
    ck_assert_int_eq(driver(str, "0", res_str), 0);
    ck_assert_msg(fabs(atof(res_str)) - fabs(orig) <= 0.00001,
                  "test-10 failed");
  }
  {
    char src[100] = "-(o(i(a(10.01)*n(2))/10m2))^q(5)";
    char res_str[256];
    int result = driver(src, NULL, res_str);
    ck_assert_msg(result, "test-11 failed");
  }
  {
    char src[100] = "-(o(i(a(10.01)*n(2))/10m(2))^q(5)";
    char res_str[256];
    int result = driver(src, NULL, res_str);
    ck_assert_msg(result, "test-12 failed");
  }
  {
    char src[100] = "mod+9(())";
    char res_str[256];
    int result = driver(src, NULL, res_str);
    ck_assert_msg(result, "test-13 failed");
  }
  {
    char src[100] = "5mod5+9(())";
    char res_str[256];
    int result = driver(src, NULL, res_str);
    ck_assert_msg(!result, "test-13 failed");
  }
  {
    char src[100] = "md(+9(()))";
    char res_str[256];
    int result = driver(src, NULL, res_str);
    ck_assert_msg(result, "test-14 failed");
  }
  {
    char src[100] = "-(o(i(a(10.01)*n(2))/10md+(2))^q(5)";
    char res_str[256];
    int result = driver(src, NULL, res_str);
    ck_assert_msg(result, "test-15 failed");
  }
  {
    char src[100] = ")(s(x)";
    char res_str[256];
    int result = driver(src, NULL, res_str);
    ck_assert_msg(result, "test-16 failed");
  }
  {
    char src[100] = ".+m)";
    char res_str[256];
    int result = driver(src, NULL, res_str);
    ck_assert_msg(result, "test-17 failed");
  }
}
END_TEST

START_TEST(s21_test_4) {
  double orig = 69;
  char res_str[256];
  char str[255] = "2+(5 * 5 * 5 + 5 + 2 * 2) / (2)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(s21_test_5) {
  double orig = 11.958851;
  char res_str[256];
  char str[255] = "5 + (3 + sin(8/2 - 3.5)) *2";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-6);
}
END_TEST

START_TEST(s21_test_6) {
  double orig = 90;
  char res_str[256];
  char str[255] = "24*4 -3 * 2";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(s21_test_7) {
  double orig = 4;
  char res_str[256];
  char str[255] = "2 - (-2)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(s21_test_8) {
  double orig = -0.41211848524;
  char res_str[256];
  char str[255] = "-sin(9)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-6);
}
END_TEST

START_TEST(s21_test_9) {
  double orig = 2.41211848524;
  char res_str[256];
  char str[255] = "sin(9)-(-2)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-6);
}
END_TEST

START_TEST(s21_test_10) {
  double orig = 0.91113026188;
  char res_str[256];
  char str[255] = "-cos(9)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-6);
}
END_TEST

START_TEST(plus_t) {
  double orig = 0;
  char res_str[256];
  char str[255] = "(-2)+2";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(minus_t) {
  double orig = -4.4;
  char res_str[256];
  char str[255] = "(-2.2)-2.2";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(mult_t) {
  double orig = -4.84;
  char res_str[256];
  char str[255] = "(-2.2)*(+2.2)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(division_t) {
  double orig = -1;
  char res_str[256];
  char str[255] = "(-2.2)/2.2";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq(atof(res_str), orig);
}
END_TEST

START_TEST(degr_t) {
  double orig = 0.17647;
  char res_str[256];
  char str[255] = "(2.2)^(-2.2)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-6);
}
END_TEST

START_TEST(unar_minus_t) {
  double orig = 0.04;
  char res_str[256];
  char str[255] = "5^-2";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-6);
}
END_TEST

START_TEST(sin_t) {
  double orig = sin(2);
  char res_str[256];
  char str[255] = "sin(+2)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-6);
}
END_TEST

START_TEST(cos_t) {
  double orig = cos(2);
  char res_str[256];
  char str[255] = "cos(2)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(tan_t) {
  double orig = tan(2);
  char res_str[256];
  char str[255] = "tan(2)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(asin_t) {
  double orig = asin(1);
  char res_str[256];
  char str[255] = "asin(1)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(acos_t) {
  double orig = acos(1);
  char res_str[256];
  char str[255] = "acos(1)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(atan_t) {
  double orig = atan(1);
  char res_str[256];
  char str[255] = "atan(1)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(ln_t) {
  double orig = log(1);
  char res_str[256];
  char str[255] = "ln(1)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(log_t) {
  double orig = log10(1);
  char res_str[256];
  char str[255] = "log(1)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(sqrt_t) {
  double orig = sqrt(1);
  char res_str[256];
  char str[255] = "sqrt(1)";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(mod_t) {
  double orig = 4;
  char res_str[256];
  char str[255] = "4mod8";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(expression_t) {
  double orig = -30.0721649;
  char res_str[256];
  char str[255] =
      "15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))3-(2+(1+1))(15/"
      "(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_double_eq_tol(atof(res_str), orig, 1e-7);
}
END_TEST

START_TEST(error_1_t) {
  double orig = NAN;
  char res_str[256];
  char str[255] = "()";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_msg(atof(res_str) != atof(res_str) && orig != orig,
                "error-1 failed");
}
END_TEST

START_TEST(error_2_t) {
  double orig = NAN;
  char res_str[256];
  char str[255] = "cos()";
  ck_assert_int_eq(driver(str, "0", res_str), 0);
  ck_assert_msg(atof(res_str) != atof(res_str) && orig != orig,
                "error-2 failed");
}
END_TEST

Suite *suite_smart_calc(void) {
  Suite *s = suite_create("suite_smart_calc");
  TCase *tc = tcase_create("suite_smart_calc");

  tcase_add_test(tc, s21_test_0);
  tcase_add_test(tc, s21_test_1);
  tcase_add_test(tc, s21_test_2);
  tcase_add_test(tc, s21_test_3);
  tcase_add_test(tc, calc_test);
  tcase_add_test(tc, s21_test_4);
  tcase_add_test(tc, s21_test_5);
  tcase_add_test(tc, s21_test_6);
  tcase_add_test(tc, s21_test_7);
  tcase_add_test(tc, s21_test_8);
  tcase_add_test(tc, s21_test_9);
  tcase_add_test(tc, s21_test_10);

  tcase_add_test(tc, plus_t);
  tcase_add_test(tc, minus_t);
  tcase_add_test(tc, division_t);
  tcase_add_test(tc, mult_t);
  tcase_add_test(tc, degr_t);
  tcase_add_test(tc, unar_minus_t);
  tcase_add_test(tc, sin_t);
  tcase_add_test(tc, cos_t);
  tcase_add_test(tc, tan_t);
  tcase_add_test(tc, asin_t);
  tcase_add_test(tc, acos_t);
  tcase_add_test(tc, atan_t);
  tcase_add_test(tc, ln_t);
  tcase_add_test(tc, log_t);
  tcase_add_test(tc, sqrt_t);
  tcase_add_test(tc, mod_t);
  tcase_add_test(tc, expression_t);

  tcase_add_test(tc, error_1_t);
  tcase_add_test(tc, error_2_t);

  suite_add_tcase(s, tc);
  return s;
}