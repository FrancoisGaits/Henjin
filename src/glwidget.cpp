
#include "glwidget.h"

GlWidget::GlWidget(QWidget *parent) : QOpenGLWidget(parent){
    _lastTime = QDateTime::currentMSecsSinceEpoch();
    _firstTime = _lastTime;

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
    if(_lines) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    _scene->draw(context()->defaultFramebufferObject(), _deltaTime, static_cast<float>(_lastTime - _firstTime)/1000.f);
    glFinish();

    std::int64_t time = QDateTime::currentMSecsSinceEpoch();

    _deltaTime = static_cast<float>(time - _lastTime)/1000.f;
    _lastTime = time;

    update();
}

void GlWidget::resizeGL(int width, int height) {
    _scene->resize(width,height);
}

GlWidget::~GlWidget() {

}

void GlWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()){
        case Qt::Key_R :
            _scene->resetCamera();
            break;

        case Qt::Key_N :
            _scene->reloadShader();
            break;

        case Qt::Key_W :
            _lines = !_lines;
            break;

        case Qt::Key_Z:
        case Qt::Key_Q:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_Shift:
        case Qt::Key_Space:
            if(!event->isAutoRepeat())
                _scene->cameraKeyEvent(event);
            break;


        default:

            break;
    }
}

void GlWidget::mouseMoveEvent(QMouseEvent *event) {
    _scene->move(event->x(),event->y(),_deltaTime);
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

void GlWidget::keyReleaseEvent(QKeyEvent *event) {
    switch(event->key()){
        case Qt::Key_Z:
        case Qt::Key_Q:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_Shift:
        case Qt::Key_Space:
            if(!event->isAutoRepeat())
                _scene->cameraKeyReleaseEvent(event);
            break;


        default:

            break;
    }
}

void GlWidget::changeScene(unsigned sceneNumber) {
    _firstTime = QDateTime::currentMSecsSinceEpoch();
    makeCurrent();
    _scene->changeScene(sceneNumber);
    doneCurrent();
}
