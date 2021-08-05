#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QRect>
#include <QPixmap>
#include <QPoint>
#include <QMouseEvent>
#include "paintwidget.h"

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
    void on_tBarQuit_triggered();
    void on_tBarSquare_triggered();

    void on_tBarCircle_triggered();

    void on_tBarTriangle_triggered();

    void on_tBarLine_triggered();

    void on_tBarMove_triggered();

private:
    PaintWidget* pWidget;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
