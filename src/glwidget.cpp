
#include "glwidget.h"

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent){

}

QSize GlWidget::minimumSizeHint() const {
    return {1152, 700};
}

QSize GlWidget::sizeHint() const {
    return {1152, 700};
}

void GlWidget::cleanup() {

}

void GlWidget::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GlWidget::cleanup);

    if (!initializeOpenGLFunctions()) {
        QMessageBox::critical(this, "OpenGL initialization error",
                              "MyOpenGLWidget::initializeGL() : Unable to initialize OpenGL functions");
        exit(1);
    }
    makeCurrent();
    _scene = std::make_unique<Scene>(sizeHint().width(), sizeHint().height());
    doneCurrent();
}

void GlWidget::paintGL() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    _scene->draw(context()->defaultFramebufferObject());
    glFinish();

}

void GlWidget::resizeGL(int width, int height) {

}

GlWidget::~GlWidget() {

}

void GlWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()){
        case Qt::Key_R :
            _scene->resetCamera();
            update();
            break;
        default:

            break;
    }
}

void GlWidget::mouseMoveEvent(QMouseEvent *event) {
    _scene->move(event->x(),event->y());
    update();
}

void GlWidget::mousePressEvent(QMouseEvent *event) {
    unsigned b;
    switch(event->button()) {
        case Qt::LeftButton :
            b = 1;
            break;
        case Qt::RightButton :
            b = 2;
            break;
        default:
            b = 0;
            break;
    }

    _scene->click(b,event->x(),event->y());
}
