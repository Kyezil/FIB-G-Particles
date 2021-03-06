#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H
#include <QVector3D>
#include <QQuaternion>
#include <vector>
using std::vector;

class ParticleGenerator
{
public:
    constexpr const static float LIFE_DECAY = 0.9f;
    constexpr const static float LIFE_THRESHOLD = 0.1f;

    struct Particle {
        QVector3D pos;
        QVector3D w; // rotation speed
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

    // update positions
    void update(double dt);

private:
    unsigned int max_particles;
    unsigned int nb_particles_alive;
    // container for the particles
    vector<Particle> particles;

    // tmp buffer for particles data
    float *particles_data_buffer;
};

#endif // PARTICLEGENERATOR_H
