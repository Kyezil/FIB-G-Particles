#ifndef PARTICLES_H
#define PARTICLES_H

#include "plugin.h"
#include <QOpenGLShader>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QString>

class Particles: public QObject, public Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "Plugin")
    Q_INTERFACES(Plugin)

public:
    const unsigned int MAX_PARTICLES = 1000;

    const QString VSPath = ":/shaders/simple.vert";
    const QString FSPath = ":/shaders/simple.frag";

    Particles();
    ~Particles();
    void onPluginLoad();
    void preFrame();
    void postFrame();

    bool paintGL();
private:
    void initBuffers();
    void cleanup();

    QOpenGLShaderProgram *program;
    QOpenGLBuffer billboard_vbo;
    QOpenGLBuffer particles_position_vbo;
};

#endif
