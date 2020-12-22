#ifndef HENJIN_MAINWINDOW_H
#define HENJIN_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QInputDialog>
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
    void on_actionScene_Info_triggered();

    void on_actionAnim_triggered();
    void on_actionScene_1_triggered();
    void on_actionScene_2_triggered();
    void on_actionScene_3_triggered();

    void on_actionBloom_triggered();
    void on_actionExposure_triggered();
    void on_actionToneMapping_triggered();
    void on_actionBloom_Intensity_triggered();

    void on_actionAnimation_Off_triggered();
    void on_actionWeights_Off_triggered();
    void on_actionToggle_GPU_triggered();

    void on_actionWeights_Settings_triggered();

};

#endif //HENJIN_MAINWINDOW_H_H
