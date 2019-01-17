#include "particles.h"
#include "glwidget.h"

Particles::Particles()
    : program(nullptr)
    , billboard_vbo(QOpenGLBuffer::VertexBuffer)
    , particles_position_vbo(QOpenGLBuffer::VertexBuffer)
{
    // create buffers
    billboard_vbo.create();
    particles_position_vbo.create();

    // load shaders
    program = new QOpenGLShaderProgram;
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, VSPath) ||
        !program->addShaderFromSourceFile(QOpenGLShader::Fragment, FSPath)) {
        qDebug() << program->log();
    }
    program->bindAttributeLocation("vertex", 0);
    program->bindAttributeLocation("particle", 1);

    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;

    program->bind();

    // load buffers
    billboard_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    particles_position_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    initBuffers();

    program->enableAttributeArray(0);
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3*sizeof(GLfloat));
    program->enableAttributeArray(1);
    program->setAttributeBuffer(1, GL_FLOAT, 0, 3, 3*sizeof(GLfloat));

    program->release();
}


Particles::~Particles()
{
    cleanup();
}

void Particles::cleanup()
{
    billboard_vbo.destroy();
    particles_position_vbo.destroy();
}

void Particles::onPluginLoad()
{
    initBuffers();
}

void Particles::preFrame()
{
    program->bind();
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);
}

void Particles::postFrame()
{
    program->release();
}

bool Particles::paintGL()
{
    return false;
    /*
    GLWidget &g = *glwidget();
    g.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    g.glClearColor(0.1f,0.1f,0.1f,1.0f);
    g.glEnable(GL_DEPTH_TEST);

    program->bind();

    program->release();
    return true;
    */
}

void Particles::initBuffers()
{
    GLfloat billboard_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
    };

    // billboard data
    billboard_vbo.bind();
    billboard_vbo.allocate(billboard_data, sizeof(billboard_data));

    // particles data
    particles_position_vbo.bind();
    particles_position_vbo.allocate(nullptr,
        static_cast<int>(MAX_PARTICLES * 3 * sizeof(GLfloat)));
}
