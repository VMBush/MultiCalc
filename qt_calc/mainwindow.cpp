#include "mainwindow.h"
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  init_window();
  connect(ui->btn_calc_default, SIGNAL(clicked()), this, SLOT(calc_switch_click()));
  connect(ui->btn_calc_credit, SIGNAL(clicked()), this, SLOT(calc_switch_click()));
  connect(ui->btn_calc_deposit, SIGNAL(clicked()), this, SLOT(calc_switch_click()));
  connect(ui->btn_calc_deposit, SIGNAL(clicked()), this, SLOT(calc_switch_click()));

  for (QPushButton* button : {ui->num_0, ui->num_1, ui->num_2, ui->num_3, ui->num_4,
                              ui->num_5, ui->num_6, ui->num_7, ui->num_8, ui->num_9,
                              ui->num_dot, ui->num_x, ui->mod_operation, ui->div_operation,
                              ui->mul_operation, ui->minus_operation, ui->plus_operation, ui->ln_operation,
                              ui->log_operation, ui->pow_operation, ui->sqrt_operation, ui->sin_operation,
                              ui->cos_operation, ui->tan_operation, ui->asin_operation, ui->acos_operation,
                              ui->atan_operation, ui->left_br_operation, ui->righ_br_operation}) {
    connect(button, SIGNAL(clicked()), this, SLOT(insert_click()));
  }

  connect(ui->cr_calculate, SIGNAL(clicked()), this, SLOT(calculate_credit()));

  connect(ui->add_deposit, SIGNAL(clicked()), this, SLOT(add_deposit()));
  connect(ui->add_withdraw, SIGNAL(clicked()), this, SLOT(add_withdraw()));
  connect(ui->dep_calculate, SIGNAL(clicked()), this, SLOT(calculate_deposit()));
  connect(ui->dep_clear_table, SIGNAL(clicked()), this, SLOT(clear_deposit_table()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::init_window()
{
  ui->calc_stack->setCurrentWidget(ui->p1_defaul_calc);
  //ui->calc_stack->isHidden = true;

  ui->btn_calc_default->setProperty("page_num", 1);
  ui->btn_calc_credit->setProperty("page_num", 2);
  ui->btn_calc_deposit->setProperty("page_num", 3);

  ui->expression->setProperty("default_style", ui->expression->styleSheet());

  setFixedSize(800, 700);
  init_credit();
  init_deposit();
}

void MainWindow::calc_switch_click()
{
  QLayoutItem *item;
  QPushButton *button;
  QHBoxLayout *layout = ui->calc_type_handler;
  for (int i = 0; i < layout->count(); i++) {
    // Получаем указатель на элемент в заданной позиции
    item = layout->itemAt(i);
    // Проверяем, является ли элемент кнопкой
    if (item->widget() && item->widget()->isWidgetType() && item->widget()->inherits("QPushButton")) {
      button = (QPushButton *)item->widget();
      button->setChecked(false);
    }
  }

  button = (QPushButton *)sender();
  button->setChecked(true);
  QWidget* pages[] = {ui->p1_defaul_calc, ui->p2_credit_calc, ui->p3_deposit_calc};
  ui->calc_stack->setCurrentWidget(pages[button->property("page_num").toInt() - 1]);
}

void MainWindow::insert_click()
{
  QString dest_str = ui->expression->text();
  QString input_str = ((QPushButton *)sender())->text();

          // если есть текстовый вывод, сбрасываем значение
  if (dest_str == "ERROR" || dest_str == "INF" || dest_str == "-INF") {
    ui->expression->setText("0");
    dest_str = "0";
  }

          // если посылаем функцию, отрезаем часть с x)
  if (input_str.length() > 1) {
    input_str = input_str.mid(0, input_str.length() - 2);
  }

  transfer_symbols(&dest_str);
  transfer_symbols(&input_str);
  char * dest = new char[dest_str.length() + 1];
  qstrcpy(dest, dest_str.toUtf8().constData());
  char * input = new char[input_str.length() + 1];
  qstrcpy(input, input_str.toUtf8().constData());

  int validate_flag = insertion_validator(dest, input);

  if (validate_flag) {
    ui->expression->setStyleSheet(ui->expression->property("default_style").toString());
    rtransfer_symbols(&input_str);
    // обработка пустой строки
    if (dest_str == "0" && !strchr("^%/*+-,", *input)) {
      ui->expression->setText(input_str);
    } else {
      // если мы меняем оператор и в конце строки стоит корень
      if (strchr("^%/*+-", input[0]) && strchr("v", dest[dest_str.length() - 1])) {
        //убираем корни
        while (strchr("v", dest[dest_str.length() - 1])) {
          dest_str = dest_str.mid(0, dest_str.length() - 1);
        }
      }
      // если мы меняем оператор
      if (strchr("^%/*+-", input[0]) && strchr("^v%/*+-", dest[dest_str.length() - 1])) {
        dest_str = dest_str.mid(0, dest_str.length() - 1);
        //qstrcpy(dest, dest_str.toUtf8().constData());
      }
      rtransfer_symbols(&dest_str);
      // если строка оказалась пустой
      if (dest_str.length() == 0) {
        dest_str = "0";
      }
      ui->expression->setText(dest_str + input_str);
    }
  } else {
    ui->expression->setStyleSheet(ui->expression->styleSheet() + "background-color: #FFC0CB;");
  }

  delete[] dest;
  delete[] input;

}

void MainWindow::transfer_symbols(QString *str)
{
  str->replace("÷", "/");
  str->replace("×", "*");
  str->replace("√", "v");
}

void MainWindow::rtransfer_symbols(QString *str)
{
  str->replace("/", "÷");
  str->replace("*", "×");
  str->replace("v", "√");
}

void MainWindow::on_aclear_operation_clicked()
{
  ui->expression->setStyleSheet(ui->expression->property("default_style").toString());
  ui->expression->setText("0");
}

void MainWindow::on_clear_operation_clicked()
{
  ui->expression->setStyleSheet(ui->expression->property("default_style").toString());
  QString dest_str = ui->expression->text();
  transfer_symbols(&dest_str);
  char * dest = new char[dest_str.length() + 1];
  qstrcpy(dest, dest_str.toUtf8().constData());

  delete_last_symbol(dest);

  dest_str = QString(dest);
  rtransfer_symbols(&dest_str);
  ui->expression->setText(dest_str);
  delete[] dest;
}


void MainWindow::on_calc_res_btn_clicked()
{
  QString dest_str = ui->expression->text();
  transfer_symbols(&dest_str);
  char * dest = new char[dest_str.length() + 1];
  qstrcpy(dest, dest_str.toUtf8().constData());

          // возможен ли рассчет
  if (!calculate_validator(dest)) {
    ui->expression->setStyleSheet(ui->expression->styleSheet() + "background-color: #FFC0CB;");
  } else {
    // дозакрываем скобки
    while (dest_str.count("(") > dest_str.count(")")) {
      dest_str += ")";
    }
    //заменяем символы
    transfer_symbols(&dest_str);
    // подсчет польки
    delete[] dest;
    dest = new char[dest_str.length() + 1];
    qstrcpy(dest, dest_str.toUtf8().constData());

    char *result = solve_expression(dest);
    ui->expression->setText(result);
    free(result);
  }


  delete[] dest;
}


void MainWindow::on_build_graph_btn_clicked()
{
  QString dest_str = ui->expression->text();
  transfer_symbols(&dest_str);
  char * dest = new char[dest_str.length() + 1];
  qstrcpy(dest, dest_str.toUtf8().constData());

          // возможен ли рассчет
  if (!graphic_validator(dest)) {
    ui->expression->setStyleSheet(ui->expression->styleSheet() + "background-color: #FFC0CB;");
  } else {
    // дозакрываем скобки
    while (dest_str.count("(") > dest_str.count(")")) {
      dest_str += ")";
    }
    //заменяем символы
    transfer_symbols(&dest_str);

    diagram = new Diagram(this, dest_str);
    diagram->show();
    QCoreApplication::processEvents();
  }


  delete[] dest;

}

void MainWindow::raise_error(QString text) {
  QMessageBox msgBox;
  msgBox.setStyleSheet("background-color: #3b2f2f; color: #ffffff;");
  msgBox.setText(text);
  msgBox.setWindowTitle("Ошибка");
  msgBox.setIcon(QMessageBox::Critical);

  QPushButton* okButton = msgBox.addButton(tr("Ок"), QMessageBox::ActionRole);
  okButton->setStyleSheet("background-color: #ff9900; color: #ffffff; border-radius: 5px; height: 25px; width: 30px;");

  msgBox.exec();
}

