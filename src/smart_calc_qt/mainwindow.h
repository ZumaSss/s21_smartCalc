#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  double xBegin, xEnd, yBegin, yEnd, h, X, Y;
  int N;

  QVector<double> x, y;

 private slots:
  void input_line();
  void clear_line();
  void get_res();
  void plot();
  void calc_credit();
};
#endif  // MAINWINDOW_H
