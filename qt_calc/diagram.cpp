#include "diagram.h"

#include "ui_diagram.h"

Diagram::Diagram(QWidget *parent, QString expression) : QDialog(parent), ui(new Ui::Diagram) {
  ui->setupUi(this);
  this->expression = expression;
  build();
}

Diagram::~Diagram() { delete ui; }

void Diagram::build() {
  double xmin, xmax, ymin, ymax;
  bool xminok, xmaxok, yminok, ymaxok = false;
  xmin = ui->xmin_txt->text().toDouble(&xminok);
  xmax = ui->xmax_txt->text().toDouble(&xmaxok);
  ymin = ui->ymin_txt->text().toDouble(&yminok);
  ymax = ui->ymax_txt->text().toDouble(&ymaxok);
  if (xmin >= xmax || ymin >= ymax || !(xminok && xmaxok && yminok && ymaxok)) {
    QMessageBox msgBox;
    msgBox.setStyleSheet("background-color: #3b2f2f; color: #ffffff;");
    msgBox.setText("Недопустимое значение осей функции");
    msgBox.setWindowTitle("Ошибка");
    msgBox.setIcon(QMessageBox::Critical);

    QPushButton* okButton = msgBox.addButton(tr("Ок"), QMessageBox::ActionRole);
    okButton->setStyleSheet("background-color: #ff9900; color: #ffffff; border-radius: 5px; height: 25px; width: 30px;");

    msgBox.exec();
  } else {
    double h = (xmax - xmin) / 500;
    QVector<double> x, y;

    for (double x_c = xmin; x_c <= xmax; x_c += h) {
      QString exp_c = expression;
      exp_c.replace("x", QString::number(x_c));
      char * dest = new char[exp_c.length() + 1];
      qstrcpy(dest, exp_c.toUtf8().constData());
      QString res = solve_expression(dest);
      if (res != "inf" && res != "-inf" && res != "ERROR") {
        x.push_back(x_c);
        res.replace(",", ".");
        y.push_back(res.toDouble());
      }
    }

    // добавляем один график в widget
    ui->graphic->addGraph();

            // говорим, что отрисовать нужно график по нашим двум массивам x и y
    ui->graphic->graph(0)->setData(x, y);

            // устанавливаем область, которая будет показываться на графике
    ui->graphic->xAxis->setRange(xmin, xmax);
    ui->graphic->yAxis->setRange(ymin, ymax);

            // задаем возможность зумировать график
    ui->graphic->setInteraction(QCP::iRangeZoom, true);
    ui->graphic->setInteraction(QCP::iRangeDrag, true);

            // задаем цвет точки и толщину линии
    ui->graphic->graph(0)->setPen(QColor(61, 82, 62, 255));
    QPen graphPen = ui->graphic->graph(0)->pen();
    graphPen.setWidth(2);
    ui->graphic->graph(0)->setPen(graphPen);

            // перерисуем график на нашем graphic
    ui->graphic->replot();
  }
}

void Diagram::on_resize_clicked()
{
  build();
}

