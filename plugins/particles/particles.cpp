#include "particles.h"
#include "glwidget.h"

Particles::Particles()
    : program(nullptr)
    , billboard_vbo(QOpenGLBuffer::VertexBuffer)
    , particles_position_vbo(QOpenGLBuffer::VertexBuffer)
{
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

    // create buffers
    vao.create();
    billboard_vbo.create();
    billboard_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    particles_position_vbo.create();
    particles_position_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    // load buffers
    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    initBuffers();

    program->enableAttributeArray(0);
    billboard_vbo.bind();
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3*sizeof(GLfloat));


    program->enableAttributeArray(1);
    particles_position_vbo.bind();
    program->setAttributeBuffer(1, GL_FLOAT, 0, 3, 3*sizeof(GLfloat));

    program->release();
}


Particles::~Particles()
{
    cleanup();
}

void Particles::cleanup()
{
    if (program == nullptr) return;
    billboard_vbo.destroy();
    particles_position_vbo.destroy();
    vao.destroy();
    delete program;
    program = nullptr;
}

void Particles::onPluginLoad(){}

bool Particles::paintGL()
{
    GLWidget &g = *glwidget();
    g.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    g.glClearColor(0.3f,0.3f,0.3f,1.0f);
    g.glEnable(GL_DEPTH_TEST);
    g.glDisable(GL_CULL_FACE);

    program->bind();
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);


    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
    const unsigned int NUM_PARTICLES = 10;
    // fill buffer with random points on a sphere
    QRandomGenerator rnd(1234);
    GLfloat points[NUM_PARTICLES][3];
    for (int i = 0; i < 10; ++i) {
        double theta = 2 * M_PI * rnd.generateDouble();
        double phi = qAcos(1 - 2 * rnd.generateDouble());
        double x = qSin(phi) * qCos(theta);
        double y = qSin(phi) * qSin(theta);
        double z = qCos(phi);
        points[i][0] = x;
        points[i][1] = y;
        points[i][2] = z;
        // qDebug() << "got point " << x << "," << y << "," << z;
    }
    particles_position_vbo.bind();
    particles_position_vbo.write(0, points, NUM_PARTICLES*3*sizeof(GLfloat));
    g.glVertexAttribDivisor(0,0); // same mesh
    g.glVertexAttribDivisor(1,1); // particle position 1 per quad

    g.glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, NUM_PARTICLES);

    program->release();
    return true;
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
