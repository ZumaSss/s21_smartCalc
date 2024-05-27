#include "mainwindow.h"

#include "./ui_mainwindow.h"

extern "C" {
#include "../s21_smart_calc.h"
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->lineEdit_x->setValidator(new QDoubleValidator());
  ui->lineEdit_x_begin->setValidator(new QDoubleValidator());
  ui->lineEdit_x_end->setValidator(new QDoubleValidator());
  ui->lineEdit_y_begin->setValidator(new QDoubleValidator());
  ui->lineEdit_y_end->setValidator(new QDoubleValidator());

  ui->lineEdit_sum_credit->setValidator(new QIntValidator());
  ui->lineEdit_term->setValidator(new QIntValidator());
  ui->lineEdit_stake->setValidator(new QDoubleValidator());

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_open_bracket, SIGNAL(clicked()), this,
          SLOT(input_line()));
  connect(ui->pushButton_close_bracket, SIGNAL(clicked()), this,
          SLOT(input_line()));
  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(input_line()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(input_line()));

  connect(ui->pushButton_clear, SIGNAL(clicked()), this, SLOT(clear_line()));

  connect(ui->pushButton_res, SIGNAL(clicked()), this, SLOT(get_res()));

  connect(ui->pushButton_plot, SIGNAL(clicked()), this, SLOT(plot()));

  connect(ui->pushButton_credit, SIGNAL(clicked()), this, SLOT(calc_credit()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::input_line() {
  QPushButton* button = (QPushButton*)sender();
  QString new_line = (ui->lineEdit->text() + button->text());

  ui->lineEdit->setText(new_line);
}

void MainWindow::clear_line() { ui->lineEdit->clear(); }

void MainWindow::get_res() {
  double result;
  QByteArray byteArray = ui->lineEdit->text().toLatin1();
  char* string_c = byteArray.data();
  char* res_str = new char[256];
  const char* input_x = NULL;

  if (!ui->lineEdit_x->text().isEmpty()) {
    byteArray = ui->lineEdit_x->text().toLatin1();
    input_x = byteArray.constData();
  }

  switch (driver(string_c, input_x, res_str)) {
    case 0:
      result = std::stod(res_str);
      ui->lineEdit->setText(QString::number(result, 'g', 15));
      break;
    case 1:
      ui->lineEdit->setText("INVALID INPUT!");
      break;
    case 2:
      ui->lineEdit->setText("INPUT X!");
      break;
  }

  delete[] res_str;
}

void MainWindow::plot() {
  if (!ui->lineEdit->text().isEmpty() and
      ui->lineEdit->text() != "Input expression!") {
    if (!ui->lineEdit_x_begin->text().isEmpty() and
        !ui->lineEdit_x_end->text().isEmpty() and
        !ui->lineEdit_y_begin->text().isEmpty() and
        !ui->lineEdit_y_end->text().isEmpty()) {
      if (!ui->label_error->text().isEmpty()) {
        ui->label_error->clear();
      }

      QByteArray byteArray = ui->lineEdit->text().toLatin1();
      QByteArray byteArrayX;
      char* string_c = byteArray.data();
      char* res_str = new char[256];
      char* input_x = NULL;
      x.clear();
      y.clear();
      ui->widget->clearGraphs();

      if (driver(string_c, (char*)"1", res_str) == 1) {
        ui->lineEdit->setText("INVALID INPUT!");
        delete[] res_str;
        return;
      }

      xBegin = ui->lineEdit_x_begin->text().toDouble();
      xEnd = ui->lineEdit_x_end->text().toDouble();
      yBegin = ui->lineEdit_y_begin->text().toDouble();
      yEnd = ui->lineEdit_y_end->text().toDouble();

      N = 10000;
      h = (xEnd - xBegin) / N;
      ui->widget->xAxis->setRange(xBegin, xEnd);
      ui->widget->yAxis->setRange(yBegin, yEnd);

      for (X = xBegin; X <= xEnd and h != 0; X += h) {
        x.push_back(X);
        byteArrayX = QByteArray::number(X);
        input_x = byteArrayX.data();
        driver(string_c, input_x, res_str);
        Y = std::stod(res_str);
        if (std::isfinite(Y) and (Y <= yEnd and Y >= yBegin))
          y.push_back(Y);
        else
          y.push_back(std::numeric_limits<double>::quiet_NaN());
      }

      ui->widget->addGraph();
      ui->widget->graph(0)->addData(x, y);
      ui->widget->replot();

      delete[] res_str;
    } else {
      ui->label_error->setText("Input D(x) and E(x)!");
    }
  } else {
    ui->lineEdit->setText("Input expression!");
  }
}

void MainWindow::calc_credit() {
  if (!ui->lineEdit_sum_credit->text().isEmpty() and
      !ui->lineEdit_stake->text().isEmpty() and
      !ui->lineEdit_term->text().isEmpty()) {
    if (!ui->label_credit_error->text().isEmpty())
      ui->label_credit_error->clear();
    ui->textEdit_monthly_fee->clear();
    ui->lineEdit_total_pay->clear();
    ui->lineEdit_overpay->clear();

    int sum_credit = ui->lineEdit_sum_credit->text().toInt();
    int term = ui->lineEdit_term->text().toInt();
    double stake = ui->lineEdit_stake->text().toDouble();
    double monthly_fee, total_pay, overpay, month_stake, main_fee, percent_fee,
        body_credit;
    if (ui->comboBox->currentText() == "Аннуитентный") {
      month_stake = stake / (100 * 12);
      monthly_fee =
          std::round((sum_credit *
                      (month_stake / (1 - std::pow(1 + month_stake, -term)))) *
                     100) /
          100;
      total_pay = term * monthly_fee;
      overpay = total_pay - sum_credit;

      ui->textEdit_monthly_fee->setText(QString::number(monthly_fee, 'f', 2));
    } else {
      body_credit = sum_credit;
      total_pay = 0;
      main_fee = body_credit / term;

      for (int i = 1; i <= term; i++) {
        percent_fee = body_credit / 12 * (stake / 100);
        monthly_fee = std::round((main_fee + percent_fee) * 100) / 100;
        ui->textEdit_monthly_fee->setText(
            ui->textEdit_monthly_fee->toPlainText() + QString::number(i) +
            ") ");
        ui->textEdit_monthly_fee->setText(
            ui->textEdit_monthly_fee->toPlainText() +
            QString::number(monthly_fee, 'f', 2) + "\n");

        body_credit -= main_fee;
        total_pay += monthly_fee;
      }
      overpay = total_pay - sum_credit;
    }
    ui->lineEdit_total_pay->setText(QString::number(total_pay, 'f', 2));
    ui->lineEdit_overpay->setText(QString::number(overpay, 'f', 2));

    ui->label_credit_error->setText("Кредит рассчитан!");
  } else
    ui->label_credit_error->setText("Введите все начальные параметры!");
}
