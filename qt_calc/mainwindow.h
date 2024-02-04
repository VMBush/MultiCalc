#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include "smartcalc.h"
#include "diagram.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QComboBox>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void raise_error(QString text);

    void calc_switch_click();
    void init_window();
    void init_credit();
    void init_deposit();
    void insert_click();

    void transfer_symbols(QString *str);
    void rtransfer_symbols(QString *str);
    void on_aclear_operation_clicked();

    void on_clear_operation_clicked();
    void on_calc_res_btn_clicked();
    void on_build_graph_btn_clicked();

    void calculate_credit();
    void calculate_ann(double sum, double rate, int term);
    void calculate_diff(double sum, double rate, int term);

    void add_deposit();
    void add_withdraw();
    void clear_deposit_table();
    void calculate_deposit();
    void get_refs_from_table(QVector<double>* refinances);



   private:
    Diagram* diagram;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
