#include "particlegenerator.h"

ParticleGenerator::ParticleGenerator(unsigned int max_size)
    : nb_particles_alive(0)
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

const float* ParticleGenerator::particlesPositions() const
{
    return particles_data_buffer;
}

unsigned int ParticleGenerator::size() const
{
    return nb_particles_alive;
}
