#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QDialog>
#include <QVector>
#include "smartcalc.h"

namespace Ui {
class Diagram;
}

class Diagram : public QDialog {
  Q_OBJECT

 public:
  explicit Diagram(QWidget *parent = nullptr, QString expression = "");
  ~Diagram();

 private slots:
  void on_resize_clicked();

 private:
  Ui::Diagram *ui;
  QString expression;
  void build();
};

#endif  // DIAGRAM_H
