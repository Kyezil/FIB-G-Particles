#ifndef PARTICLES_H
#define PARTICLES_H

#include "plugin.h"
#include <QOpenGLShader>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QRandomGenerator>
#include <QString>
#include <QtMath>

class Particles: public QObject, public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Plugin")
    Q_INTERFACES(Plugin)

public:
    const unsigned int MAX_PARTICLES = 10000;

    const QString VSPath = ":/shaders/simple.vert";
    const QString FSPath = ":/shaders/simple.frag";

    Particles();
    ~Particles();
    void onPluginLoad();

    bool paintGL();
private:
    const QVector3D billboard_data[4] = {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {-0.5f,  0.5f, 0.0f},
        {0.5f,  0.5f, 0.0f}
    };

    void initBuffers();
    void cleanup();
    void sendBillboardData();

    QOpenGLShaderProgram *program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer billboard_vbo;
    QOpenGLBuffer particles_position_vbo;
};

#endif
