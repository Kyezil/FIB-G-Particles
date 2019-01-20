#ifndef PARTICLES_H
#define PARTICLES_H

#include "plugin.h"
#include "particlegenerator.h"

#include <QOpenGLShader>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QRandomGenerator>
#include <QString>
#include <QtMath>
#include <QTimer>
#include <QElapsedTimer>

class Particles: public QObject, public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Plugin")
    Q_INTERFACES(Plugin)

public:
    const static unsigned int MAX_PARTICLES = 10000;
    const static unsigned int START_PARTICLES = 10;
    constexpr const static float PARTICLE_SIZE = 1.0f;
    constexpr const static float SPHERE_RADIUS = 2.0f;

    const QString VSPath = ":/shaders/simple.vert";
    const QString FSPath = ":/shaders/simple.frag";

    Particles();
    ~Particles();
    void onPluginLoad();

    bool paintGL();

private:
    QTimer *timer;
    QElapsedTimer etimer;

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

    ParticleGenerator generator;
};

#endif
