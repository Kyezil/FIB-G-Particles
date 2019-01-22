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
    particles_data_buffer = new float[4*max_size]; // pos + life

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
        if (p.life > LIFE_THRESHOLD) {
            if(++nb_particles_alive > max_particles) {
                throw std::runtime_error("Exceeded max number of particles");
            }
            *data = p.pos.x(); ++data;
            *data = p.pos.y(); ++data;
            *data = p.pos.z(); ++data;
            *data = p.life; ++data;
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
        particles[i].life = 1.0f; // head
        particles[i].w = QVector3D(0,100,0);
    }
}

void ParticleGenerator::update(double dt)
{
    vector<Particle> new_particles;

    for (Particle &p: particles) {
        if (p.life == 1.0f) {
            // create new particle at current position
            Particle q;
            q.pos = p.pos;
            q.life = LIFE_DECAY;
            q.w = QVector3D(0,0,0);
            new_particles.push_back(q);

            float angle = p.w.length()*dt;
            p.pos = QQuaternion::fromAxisAndAngle(p.w, angle)
                               .rotatedVector(p.pos);
        } else {
            p.life *= LIFE_DECAY;
        }
    }

    int k = 0;
    for (int i = 0; k < new_particles.size() and i < particles.size(); ++i) {
        Particle &p = particles[i];
        if (p.life <= LIFE_THRESHOLD) { // recycle old particles
            p = new_particles[k];
            ++k;
        }
    }
}

