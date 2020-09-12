#ifndef HENJIN_GLWIDGET_H
#define HENJIN_GLWIDGET_H

#include <QOpenGLWidget>
#include <QtWidgets/QMessageBox>
#include <QOpenGLFunctions_4_1_Core>
#include <memory>

#include "scene.h"

class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {
Q_OBJECT
public:
    explicit GlWidget(QWidget *parent = nullptr);

    ~GlWidget() final;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void cleanup();

protected:
    // OpenGL management
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    std::unique_ptr<Scene> _scene;

};

#endif //HENJIN_GLWIDGET_H
