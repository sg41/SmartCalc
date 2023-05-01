#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  SizeDialog = ui->centralwidget->findChild<QWidget*>(QString("SizeDialog"));
  showSizeDialog(false);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_graph_area_customContextMenuRequested(const QPoint &pos) {
    showSizeDialog(true);
}

void MainWindow::on_buttonBox_accepted()
{
    showSizeDialog(false);
}


void MainWindow::on_buttonBox_rejected()
{
    showSizeDialog(false);
}

void MainWindow::showSizeDialog(bool on)
{
    if(SizeDialog){
        SizeDialog->setVisible(on);
    }
}

