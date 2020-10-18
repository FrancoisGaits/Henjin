#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    //setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);

    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSamples(3);
    QSurfaceFormat::setDefaultFormat(format);


    ui->setupUi(this);

    glWidget = new GlWidget(ui->centralwidget);
    glWidget->setFocus();



}

void MainWindow::on_actionOpenGl_Info_triggered() {
    std::stringstream message;
    message << "Qt version     : " << qVersion() << std::endl;
    message << "Renderer       : " << glGetString(GL_RENDERER) << std::endl;
    message << "Vendor         : " << glGetString(GL_VENDOR) << std::endl;
    message << "Version        : " << glGetString(GL_VERSION) << std::endl;
    message << "GLSL Version   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    QMessageBox::information(this, "OpenGL Information", message.str().c_str());
}







MainWindow::~MainWindow() {
    delete ui;
    delete glWidget;
}

void MainWindow::on_actionFullScreen_triggered() {
    showFullScreen();
}

void MainWindow::on_actionClose_triggered() {
    QMainWindow::close();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    resizeWindow(event->size().width(),event->size().height());

}

void MainWindow::resizeWindow(int width, int height) {
    glWidget->resize(width,height);

}

void MainWindow::on_actionNormal_triggered() {
    showNormal();
}

void MainWindow::on_actionSet_Surface_Color_triggered() {
    QColor color = QColorDialog::getColor(Qt::white, this);

    if(!color.isValid()) {
        return;
    }

    glm::vec3 newColor(glm::vec3(color.red()/255.f,color.green()/255.f,color.blue()/255.f));

    glWidget->setSurfaceColor(newColor);
}
