#include "mainwindow.h"

void MainWindow::init_deposit() {
  QTableWidget* tbl = ui->deposit_table;
  tbl->setColumnCount(4);
  QStringList names = {"Тип", "Периодичность", "Месяц", "Сумма"};
  tbl->setHorizontalHeaderLabels(names);
  tbl->setColumnWidth(0, 100);
  tbl->setColumnWidth(1, 150);
  tbl->setColumnWidth(2, 70);
  tbl->setColumnWidth(3, 100);

  QComboBox* term = ui->dep_term;
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

  QComboBox* period = ui->dep_period;
  period->addItem("1 month", QVariant(1));
  period->addItem("2 months", QVariant(2));
  period->addItem("3 months", QVariant(3));
  period->addItem("every quart", QVariant(4));
  period->addItem("6 months", QVariant(6));
  period->addItem("1 year", QVariant(12));

  QComboBox* per_dep = ui->dep_per_dep;
  per_dep->addItem("once", QVariant(0));
  per_dep->addItem("per month", QVariant(1));
  per_dep->addItem("per 2 months", QVariant(2));
  per_dep->addItem("per 3 months", QVariant(3));
  per_dep->addItem("per quart", QVariant(4));
  per_dep->addItem("per 6 months", QVariant(6));
  per_dep->addItem("per year", QVariant(12));

  QComboBox* per_wid = ui->dep_per_wid;
  per_wid->addItem("once", QVariant(0));
  per_wid->addItem("per month", QVariant(1));
  per_wid->addItem("per 2 months", QVariant(2));
  per_wid->addItem("per 3 months", QVariant(3));
  per_wid->addItem("per quart", QVariant(4));
  per_wid->addItem("per 6 months", QVariant(6));
  per_wid->addItem("per year", QVariant(12));
}

void MainWindow::add_deposit() {
  QTableWidget* tbl = ui->deposit_table;
  int row_cnt = tbl->rowCount();
  tbl->insertRow(row_cnt);
  tbl->setItem(row_cnt, 0, new QTableWidgetItem("Пополнение"));
  QString per_eng, period;
  per_eng = ui->dep_per_dep->currentText();
  if (per_eng == "once") {
    period = "Разово";
  } else  if (per_eng == "per month") {
    period = "Каждый месяц";
  } else  if (per_eng == "per 2 months") {
    period = "Каждые два месяца";
  } else  if (per_eng == "per 3 months") {
    period = "Каждые 3 месяца";
  } else  if (per_eng == "per quart") {
    period = "Каждый квартал";
  } else  if (per_eng == "per 6 months") {
    period = "Каждые 6 месяцев";
  } else  if (per_eng == "per year") {
    period = "Каждый год";
  }
  //Добавляем к ячейке данные
  QTableWidgetItem* new_item = new QTableWidgetItem(period);
  new_item->setData(Qt::UserRole, ui->dep_per_dep->currentData().toInt());
  tbl->setItem(row_cnt, 1, new_item);

}

void MainWindow::add_withdraw() {
  QTableWidget* tbl = ui->deposit_table;
  int row_cnt = tbl->rowCount();
  tbl->insertRow(row_cnt);
  tbl->setItem(row_cnt, 0, new QTableWidgetItem("Снятие"));
  QString per_eng, period;
  per_eng = ui->dep_per_wid->currentText();
  if (per_eng == "once") {
    period = "Разово";
  } else  if (per_eng == "per month") {
    period = "Каждый месяц";
  } else  if (per_eng == "per 2 months") {
    period = "Каждые два месяца";
  } else  if (per_eng == "per 3 months") {
    period = "Каждые 3 месяца";
  } else  if (per_eng == "per quart") {
    period = "Каждый квартал";
  } else  if (per_eng == "per 6 months") {
    period = "Каждые 6 месяцев";
  } else  if (per_eng == "per year") {
    period = "Каждый год";
  }
  //Добавляем к ячейке данные
  QTableWidgetItem* new_item = new QTableWidgetItem(period);
  new_item->setData(Qt::UserRole, ui->dep_per_wid->currentData().toInt());
  tbl->setItem(row_cnt, 1, new_item);
}

void MainWindow::clear_deposit_table() {
  QTableWidget* tbl = ui->deposit_table;
  tbl->setRowCount(0);
}

void MainWindow::calculate_deposit() {
  bool sumok, int_rate_ok, tax_rate_ok;
  double sum, term, int_rate, tax_rate, payment_per;
  sum = ui->dep_sum->text().toDouble(&sumok);
  term = ui->dep_term->currentData().toDouble();
  int_rate = ui->dep_rate_int->text().toDouble(&int_rate_ok);
  tax_rate = ui->dep_rate_tax->text().toDouble(&tax_rate_ok);
  payment_per = ui->dep_period->currentData().toDouble();
  bool is_capitalize = ui->dep_capit->isChecked();
  bool is_tax = int_rate >= 18;
  QVector<double> refinances;
  if (!sumok || !int_rate_ok || (is_tax && !tax_rate_ok) || sum < 0) {
    raise_error("Неверно введены данные");
  }
  else {
    double interest_amount = 0;
    double tax_amount = 0;
    get_refs_from_table(&refinances);
    int last_recaped = 0;
    // Идем по месяцам и рекапитализируем, если нужно
    for (int cur_m = 1; cur_m <= term; cur_m++) {
      sum += refinances[cur_m - 1];
      if (is_capitalize && cur_m % static_cast<int>(payment_per) == 0) {
        double interest = sum * int_rate / 1200 * (cur_m - last_recaped);
        if (is_tax) {
          double tax = interest * tax_rate / 100;
          interest -= tax;
          tax_amount += tax;
        }
        interest_amount += interest;
        sum += interest;
        last_recaped = cur_m;
      }
    }
    // Досчитываем вклад
    double interest = sum * int_rate / 1200 * (term - last_recaped);
    if (is_tax) {
      double tax = interest * tax_rate / 100;
      interest -= tax;
      tax_amount += tax;
    }
    interest_amount += interest;
    sum += interest;

    ui->dep_fin_amount->setText(QString::number(sum, 'f', 2));
    ui->dep_acc_interest->setText(QString::number(interest_amount, 'f', 2));
    ui->dep_tax_amount->setText(QString::number(tax_amount, 'f', 2));
  }
}

void MainWindow::get_refs_from_table(QVector<double>* refinances) {
  QTableWidget* tbl = ui->deposit_table;
  double term = ui->dep_term->currentData().toDouble();
  // пройдем по всем ячейкам и для каждого месяца запишем пополнения и снятия
  refinances->resize(static_cast<int>(term));
  refinances->fill(0.0);
  for (int i = 0; i < tbl->rowCount(); i++) {
    bool amok, fmok;
    double amount = tbl->item(i, 3)->text().toDouble(&amok);
    double first_month = tbl->item(i, 2)->text().toDouble(&fmok);
    int period = tbl->item(i, 1)->data(Qt::UserRole).toInt();
    QString type = tbl->item(i, 0)->text();
    if (amok && fmok) {
      if (period == 0) {
        if (type == "Пополнение") {
          (*refinances)[first_month - 1] += amount;
        } else if (type == "Снятие") {
          (*refinances)[first_month - 1] -= amount;
        }
      } else {
        for (int cur_mnth = first_month; cur_mnth <= term; cur_mnth += period) {
          if (type == "Пополнение") {
            (*refinances)[cur_mnth - 1] += amount;
          } else if (type == "Снятие") {
            (*refinances)[cur_mnth - 1] -= amount;
          }
        }
      }
    }
  }
}
