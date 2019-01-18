#include "particles.h"
#include "glwidget.h"

Particles::Particles()
    : program(nullptr)
    , billboard_vbo(QOpenGLBuffer::VertexBuffer)
    , particles_position_vbo(QOpenGLBuffer::VertexBuffer)
    , generator(MAX_PARTICLES)
{
    // create timer
    timer = new QTimer(this);

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

void Particles::onPluginLoad(){
    generator.generateSphereData(50);
    connect(timer, SIGNAL(timeout()), glwidget(), SLOT(update()));
    timer->start(0);

}

bool Particles::paintGL()
{
    GLWidget &g = *glwidget();
    g.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    g.glClearColor(0.0f,0.0f,0.0f,1.0f);
    g.glDisable(GL_CULL_FACE);
    g.glDisable(GL_DEPTH_TEST);
    // additive blending
    g.glEnable(GL_BLEND);
    g.glBlendFunc(GL_ONE, GL_ONE);

    program->bind();
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);
    program->setUniformValue("modelViewMatrix", camera()->viewMatrix());

    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);

    // update particles
    generator.update(0.1);
    particles_position_vbo.bind();
    particles_position_vbo.write(0, generator.particlesPositions(),
                                 generator.size() * 3 * sizeof(GLfloat));
    // update billboard
    sendBillboardData();

    g.glVertexAttribDivisor(0,0); // same mesh
    g.glVertexAttribDivisor(1,1); // particle position 1 per quad

    g.glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, generator.size());

    program->release();
    return true;
}

void Particles::initBuffers()
{
    // billboard data
    billboard_vbo.bind();
    billboard_vbo.allocate(billboard_data, sizeof(billboard_data));

    // particles data
    particles_position_vbo.bind();
    particles_position_vbo.allocate(nullptr,
        static_cast<int>(MAX_PARTICLES * 3 * sizeof(GLfloat)));
}

void Particles::sendBillboardData()
{
    // remove orientation of view matrix
    QMatrix4x4 Vinv = camera()->viewMatrix().inverted();

    QVector3D billboard_data_rotated[4];
    for (int i = 0; i < 4; ++i) {
        QVector4D p = Vinv * QVector4D(billboard_data[i], 0.0f);
        billboard_data_rotated[i] = p.toVector3D();
    }

    // send new data
    billboard_vbo.bind();
    billboard_vbo.write(0, billboard_data_rotated,
                        sizeof(billboard_data_rotated));
}
