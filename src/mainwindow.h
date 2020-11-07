#ifndef HENJIN_MAINWINDOW_H
#define HENJIN_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "glwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() final;

private:
    Ui::MainWindow *ui;
    GlWidget *glWidget;

    void resizeWindow(int width, int height);

private slots:
    void resizeEvent(QResizeEvent* event) override;
    void on_actionOpenGl_Info_triggered();

};

#endif //HENJIN_MAINWINDOW_H_H
