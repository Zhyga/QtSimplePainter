#include "mainwindow.h"
#include "qpainter.h"
#include "ui_mainwindow.h"
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pWidget = new PaintWidget(ui->PrintWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tBarQuit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_tBarSquare_triggered()
{
    pWidget->setSType(pWidget->SQUARE);
}


void MainWindow::on_tBarCircle_triggered()
{
    pWidget->setSType(pWidget->CIRCLE);
}


void MainWindow::on_tBarTriangle_triggered()
{
    pWidget->setSType(pWidget->TRIANGLE);
}


void MainWindow::on_tBarLine_triggered()
{
    pWidget->setSType(pWidget->LINE);
}


void MainWindow::on_tBarMove_triggered()
{
     pWidget->setSType(pWidget->MOVE);
}

