#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDebug>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_subtract_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_viewData_clicked();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *model;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
