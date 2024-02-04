#include "mainwindow.h"

void MainWindow::init_credit() {
  QTableWidget* tbl = ui->credit_table;

  tbl->setColumnCount(4);
  QStringList names = {"Месяц", "Платеж", "Погашено", "Остаток"};
  tbl->setHorizontalHeaderLabels(names);

  QComboBox* term = ui->cr_term;
  term->addItem("1 month", QVariant(1));
  term->addItem("3 months", QVariant(3));
  term->addItem("6 months", QVariant(6));
  term->addItem("9 months", QVariant(9));
  term->addItem("1 year", QVariant(12));
  term->addItem("1.5 years", QVariant(18));
  term->addItem("2 years", QVariant(24));
  term->addItem("3 years", QVariant(36));
  term->addItem("4 years", QVariant(48));
  term->addItem("5 years", QVariant(60));
  term->addItem("6 years", QVariant(72));
  term->addItem("7 years", QVariant(84));
  term->addItem("10 years", QVariant(120));
  term->addItem("15 years", QVariant(180));
  term->addItem("20 years", QVariant(240));
  term->addItem("25 years", QVariant(300));
  term->addItem("30 years", QVariant(360));

  QComboBox* type = ui->cr_type;
  type->addItem("Annoitent", QVariant("ann"));
  type->addItem("Differential", QVariant("diff"));

}


void MainWindow::calculate_credit() {
  bool sumok, rateok = false;
  QString type;
  double sum, rate, term = 0;

  sum = ui->cr_sum->text().toDouble(&sumok);
  rate = ui->cr_rate->text().toDouble(&rateok);
  term = ui->cr_term->currentData().toDouble();
  type = ui->cr_type->currentData().toString();
  if (sum < 0 || rate < 0 || !(sumok && rateok)) {
    raise_error("Неверно введены данные");
  } else if (type == "ann") {
    calculate_ann(sum, rate, term);
  } else if (type == "diff") {
    calculate_diff(sum, rate, term);
  }
}

void MainWindow::calculate_ann(double sum, double rate, int term) {
  QTableWidget* table = ui->credit_table;
  table->setRowCount(term);
  table->verticalHeader()->setVisible(false);
  double sum_orig = sum;
  double payment = 0;
  double monthly_payment;
  double monthly_rate = rate / 1200;
  for (int month = 1; month <= term; month++) {
    monthly_payment = sum * monthly_rate / (1 - pow(1 + monthly_rate, -1*(term - month + 1)));
    monthly_payment = round(monthly_payment * 100) / 100;
    payment += monthly_payment;
    sum = sum * (1 + monthly_rate) - monthly_payment;
    sum = round(sum * 100) / 100;
    table->setItem(month - 1, 0, new QTableWidgetItem(QString::number(month)));
    table->setItem(month - 1, 1, new QTableWidgetItem(QString::number(monthly_payment, 'f', 2)));
    table->setItem(month - 1, 2, new QTableWidgetItem(QString::number(payment, 'f', 2)));
    table->setItem(month - 1, 3, new QTableWidgetItem(QString::number(sum, 'f', 2)));
  }
  table->resizeColumnsToContents();
  for (int col = 0; col < 4; ++col) {
    int currentWidth = table->columnWidth(col);
    table->setColumnWidth(col, currentWidth + 15);
  }
  ui->cr_total->setText(QString::number(payment, 'f', 2));
  ui->cr_overp->setText(QString::number(payment - sum_orig, 'f', 2));
}


void MainWindow::calculate_diff(double sum, double rate, int term) {
  QTableWidget* table = ui->credit_table;
  table->setRowCount(term);
  table->verticalHeader()->setVisible(false);
  double sum_orig = sum;
  double payment = 0;
  double monthly_payment;
  double monthly_rate = rate / 1200;
  double main_part = sum / term;
  for (int month = 1; month <= term; month++) {
    monthly_payment = main_part + sum * monthly_rate;
    monthly_payment = round(monthly_payment * 100) / 100;
    payment += monthly_payment;
    sum = sum * (1 + monthly_rate) - monthly_payment;
    sum = round(sum * 100) / 100;
    table->setItem(month - 1, 0, new QTableWidgetItem(QString::number(month)));
    table->setItem(month - 1, 1, new QTableWidgetItem(QString::number(monthly_payment, 'f', 2)));
    table->setItem(month - 1, 2, new QTableWidgetItem(QString::number(payment, 'f', 2)));
    table->setItem(month - 1, 3, new QTableWidgetItem(QString::number(sum, 'f', 2)));
  }
  table->resizeColumnsToContents();
  for (int col = 0; col < 4; ++col) {
    int currentWidth = table->columnWidth(col);
    table->setColumnWidth(col, currentWidth + 15);
  }
  ui->cr_total->setText(QString::number(payment, 'f', 2));
  ui->cr_overp->setText(QString::number(payment - sum_orig, 'f', 2));
}

