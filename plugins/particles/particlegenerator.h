#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include <QVector3D>
#include <vector>
using std::vector;

class ParticleGenerator
{
public:
    struct Particle {
        QVector3D pos;
        float life;

        Particle() {
            pos = {0,0,0};
            life = 0.0f;
        }
    };

    ParticleGenerator(unsigned int max_size);
    ~ParticleGenerator();

    // return pointer to particles position
    // 3 floats by particle
    const float* particlesPositions();
    // number of active particles
    unsigned int size() const;

    // generate data
    void generateSphereData(unsigned int count);

private:
    unsigned int max_particles;
    unsigned int nb_particles_alive;
    // container for the particles
    vector<Particle> particles;
    // tmp buffer for particles data
    float *particles_data_buffer;
};

#endif // PARTICLEGENERATOR_H
