#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
    hideSizeDialog();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_graph_area_customContextMenuRequested(const QPoint &pos) {
    showSizeDialog();
}

void MainWindow::on_buttonBox_accepted()
{
    hideSizeDialog();
}


void MainWindow::on_buttonBox_rejected()
{
    hideSizeDialog();
}

void MainWindow::hideSizeDialog()
{
    QWidget * SizeDialog = ui->centralwidget->findChild<QWidget*>(QString("SizeDialog"));
    if(SizeDialog){
        SizeDialog->setVisible(false);
    }
}

void MainWindow::showSizeDialog()
{
    QWidget * SizeDialog = ui->centralwidget->findChild<QWidget*>(QString("SizeDialog"));
    if(SizeDialog){
        SizeDialog->setVisible(true);
    }
}

