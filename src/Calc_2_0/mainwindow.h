#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h" // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.

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

    void on_graph_area_customContextMenuRequested(const QPoint &pos);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    void showSizeDialog(bool on);
    QWidget * SizeDialog;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
