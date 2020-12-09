#ifndef SWARM_H
#define SWARM_H
#include <iostream>
#include "particle.h"

namespace jbm
{

    class swarm
    {
    private:
        particle *m_pParticles;
        int lastTime;

    public:
        const static int NPARTICLES = 5000;
        swarm();
        particle *const getParticle()
        {
            return m_pParticles;
        };
        void update(int elapsed);
        ~swarm();
    };

    swarm::swarm() : lastTime(0)
    {
        m_pParticles = new particle[NPARTICLES];
    }
    void swarm::update(int elapsed) //we pass the elapsed time, this way the speed doesnt depend on the speed of the while loop
    {
        int interval = elapsed - lastTime;

        for (int i = 0; i < swarm::NPARTICLES; i++)
        {
            m_pParticles[i].update(interval);
        }
        lastTime = elapsed;
    }
    swarm::~swarm()
    {
        delete[] m_pParticles;
    }

} // namespace jbm

#endif