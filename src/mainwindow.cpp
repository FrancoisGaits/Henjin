#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    QSurfaceFormat format;
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
//    format.setSamples(3);
//    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);


    ui->setupUi(this);

    glWidget = new GlWidget(ui->centralwidget);
    glWidget->resize(glWidget->sizeHint());
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


void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    resizeWindow(event->size().width(),event->size().height());

}

void MainWindow::resizeWindow(int width, int height) {
    glWidget->resize(width,height);

}




MainWindow::~MainWindow() {
    delete ui;
}



void MainWindow::on_actionAnim_triggered() {
    glWidget->changeScene(0);
}

void MainWindow::on_actionScene_1_triggered() {
    glWidget->changeScene(1);
}

void MainWindow::on_actionScene_2_triggered() {
    glWidget->changeScene(2);
}


void MainWindow::on_actionScene_3_triggered() {
    glWidget->changeScene(3);
}

void MainWindow::on_actionBloom_triggered() {
    bool ok;
    QStringList l = {"On", "Off"};
    QString text = QInputDialog::getItem(this, "Set Bloom", "Bloom : ", l, !glWidget->getBloom(), false, &ok);

    if (ok) {
        glWidget->setBloom(text == "On");
    }
}

void MainWindow::on_actionExposure_triggered() {
    bool ok;
    double exp = QInputDialog::getDouble(this, "Set Exposure", "Exposure", glWidget->getExposure(),0.1,100.,1,&ok,Qt::WindowFlags(),0.1);

    if(ok) {
        glWidget->setExposure(static_cast<float>(exp));
    }
}

void MainWindow::on_actionToneMapping_triggered() {
    bool ok;
    QStringList l = { "None", "Filmic", "Luminance Only Filmic", "Reinhard"};
    QString text = QInputDialog::getItem(this, "Choose tone mapping", "Tone Mapping : ", l, glWidget->getToneMapping(), false, &ok);

    if (ok) {
        ToneMapping tm;
        if(text == "Filmic") {
            tm = FILMIC;
        } else if (text == "Luminance Only Filmic") {
            tm = FILMIC_LUMIN;
        } else if (text == "Reinhard") {
            tm = REINHARD;
        } else {
            tm = NONE;
        }

        glWidget->setToneMapping(tm);
    }
}

void MainWindow::on_actionBloom_Intensity_triggered() {
    bool ok;
    double bloomIntensity = QInputDialog::getDouble(this, "Set Bloom Intensity", "Bloom Intensity", glWidget->getBloomIntensity(),0.01,5.,2,&ok,Qt::WindowFlags(),0.01);

    if(ok) {
        glWidget->setBloomIntensity(static_cast<float>(bloomIntensity));
    }
}

void MainWindow::on_actionScene_Info_triggered() {
    std::string tone;
    switch(glWidget->getToneMapping()) {
        case NONE :
            tone = "None";
            break;
        case FILMIC:
            tone = "Filmic";
            break;
        case FILMIC_LUMIN:
            tone = "Filmic Luminance Only";
            break;
        case REINHARD:
            tone = "Reinhard";
            break;
    }

    std::stringstream message;
    message << "Bloom : " << (glWidget->getBloom() ? "On" : "Off") << std::endl;
    message << "Bloom Intensity : " << glWidget->getBloomIntensity() << std::endl;
    message << "Exposure : " << glWidget->getExposure() << std::endl;
    message << "Tone Mapping : " << tone << std::endl;
    QMessageBox::information(this, "Scene Informations", message.str().c_str());
}

void MainWindow::on_actionAnimation_Off_triggered() {
    glWidget->toggleAnimation();
}

void MainWindow::on_actionWeights_Off_triggered() {
    glWidget->toggleWeights();
}

void MainWindow::on_actionToggle_GPU_triggered() {
    glWidget->toggleGPU();
}


