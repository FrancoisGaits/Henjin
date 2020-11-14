#ifndef HENJIN_GLWIDGET_H
#define HENJIN_GLWIDGET_H

#include <QOpenGLWidget>
#include <QtWidgets/QMessageBox>
#include <QOpenGLFunctions_4_5_Core>
#include <memory>
#include <QKeyEvent>
#include <QDateTime>

#include "scene.h"

class GlWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core {
Q_OBJECT
public:
    explicit GlWidget(QWidget *parent = nullptr);

    ~GlWidget() final;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void changeScene(unsigned sceneNumber);
    void setBloom(bool bloom);
    void setExposure(float exposure);
    void setToneMapping(ToneMapping tm);

    bool getBloom();
    float getExposure();
    ToneMapping getToneMapping();

public slots:
    void cleanup();


protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;



private:
    std::unique_ptr<Scene> _scene;
    bool _lines = false;

    float _sceneExposure = 1.f;

    std::int64_t _lastTime;
    std::int64_t _firstTime;
    float _deltaTime;

};

#endif //HENJIN_GLWIDGET_H
