#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    ui->tableView->setModel(model);

    model->setHorizontalHeaderLabels({"Employee",
                                     "Age",
                                     "Role"});

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //Sql database initialization
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("Table.sqlite");

    if(!db.open())
    {
        QMessageBox::warning(this,"Error","DB open failed");
        return;
    }

    QSqlQuery query(db);

    QString tableQuery = "CREATE TABLE IF NOT EXISTS Employees"
                         "(Employee Text,"
                         "AGE INTEGER,"
                         "Role Text)";

    if(!query.exec(tableQuery))
    {
        QMessageBox::warning(this,"Error",query.lastError().text());
        return;
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_add_clicked()
{
    model->insertRow(model->rowCount());
}

void MainWindow::on_pushButton_subtract_clicked()
{
    model->removeRow(ui->tableView->currentIndex().row());
}


void MainWindow::on_pushButton_Save_clicked()
{
   QModelIndexList selectedRows =
           ui->tableView->selectionModel()->selectedRows();

   qDebug()<<selectedRows.count();

   if(selectedRows.empty())
   {
       QMessageBox::warning(this,"Error","Select at least one row");
       return;
   }

   QSqlQuery query(db);

   query.prepare("INSERT INTO Employees"
                 "(Employee,AGE,Role)"
                 "VALUES"
                 "(:employee,:age,:role)");


   for(const QModelIndex& index : selectedRows)
   {
       int row = index.row();

       query.bindValue(":employee",model->index(row,0).data());
       query.bindValue(":age",model->index(row,1).data());
       query.bindValue(":role",model->index(row,2).data());

       if(!query.exec())
           qDebug()<<query.lastError().text();
   }

   QMessageBox::information(this,"Success",QString("%1 row(s) Uploaded").arg(selectedRows.count()));
}

void MainWindow::on_pushButton_delete_clicked()
{
    QModelIndexList selectedRows =
            ui->tableView->selectionModel()->selectedRows();

    if (selectedRows.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Select at least one row");
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM Employees WHERE Employee = :employee");

    // Delete from bottom to top so row indexes remain valid
    std::sort(selectedRows.begin(),selectedRows.end(),
              [](const QModelIndex &a,const QModelIndex &b)
    {
        return a.row() > b.row();
    });

    for (const QModelIndex &index : selectedRows)
    {
        int row = index.row();
        QVariant employeeId = model->index(row, 0).data();

        query.bindValue(":employee", employeeId);

        if (!query.exec())
        {
            qDebug() << query.lastError().text();
            QMessageBox::warning(this, "Error", "Database deletion failed");
            return;
        }

        model->removeRow(row);
    }

    QMessageBox::information(
        this, "Success",
        QString("%1 row(s) deleted").arg(selectedRows.count()));
}

void MainWindow::on_pushButton_viewData_clicked()
{
    QSqlQuery query(db);

    if(!query.exec("SELECT * FROM Employees"))
    {
        QMessageBox::warning(this, "Error", query.lastError().text());
               return;
    }

    model->removeRows(0,model->rowCount());

    while(query.next())
    {
        QList<QStandardItem *> rowItems;

        rowItems << new QStandardItem(query.value(0).toString());
        rowItems << new QStandardItem(query.value(1).toString());
        rowItems << new QStandardItem(query.value(2).toString());

        model->appendRow(rowItems);
    }
}
