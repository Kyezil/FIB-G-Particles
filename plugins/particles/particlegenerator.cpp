#include "particlegenerator.h"
#include <QRandomGenerator>
#include <QMatrix4x4>
#include <cmath>

ParticleGenerator::ParticleGenerator(unsigned int max_size)
    : max_particles(max_size)
    , nb_particles_alive(0)
    , particles(max_size) // resize vector
    , particles_data_buffer(nullptr)
{
    particles_data_buffer = new float[3*max_size];

}

ParticleGenerator::~ParticleGenerator()
{
    delete[] particles_data_buffer;
    particles_data_buffer = nullptr;
}

const float* ParticleGenerator::particlesPositions()
{
    nb_particles_alive = 0;
    float *data = particles_data_buffer;
    for (const Particle& p : particles) {
        if (p.life > 0) {
            if(++nb_particles_alive > max_particles) {
                throw std::runtime_error("Exceeded max number of particles");
            }
            *data = p.pos.x(); ++data;
            *data = p.pos.y(); ++data;
            *data = p.pos.z(); ++data;
        }
    }
    return particles_data_buffer;
}

unsigned int ParticleGenerator::size() const
{
    return nb_particles_alive;
}

void ParticleGenerator::generateSphereData(unsigned int count)
{
    // fill buffer with random points on a unit sphere
    QRandomGenerator rnd;
    for (unsigned int i = 0; i < count; ++i) {
        double theta = 2 * M_PI * rnd.generateDouble();
        double phi = acos(1 - 2 * rnd.generateDouble());
        float x = static_cast<float>(sin(phi) * cos(theta));
        float y = static_cast<float>(sin(phi) * sin(theta));
        float z = static_cast<float>(cos(phi));
        particles[i].pos = QVector3D(x,y,z);
        particles[i].life = 1.0f;
        particles[i].w = QVector3D(0,100,0);
    }
}

void ParticleGenerator::update(double dt)
{
    // rotation along y axis centered at 0
    for (Particle &p : particles) {
        if (p.life > 0) {
            //qDebug() << "BEFORE" << p.pos << " WITH " << p.w;
            float angle = p.w.length()*dt;
            p.pos = QQuaternion::fromAxisAndAngle(p.w, angle)
                               .rotatedVector(p.pos);
            //qDebug() << "AFTER" << p.pos;
        }
    }
}
