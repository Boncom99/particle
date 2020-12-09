#ifndef PARTICLE_H
#define PARTICLE_H
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "screen.h"

namespace jbm
{
    //there will be SO MANY particles !!
    struct particle //diference of struct and class is that in a struct, the members are public by default, whereas in a class, they are private
    {
        //public:
        double m_x;
        double m_y;
        double m_speed;
        double m_direction;

    private:
        void init();

    public:
        particle();
        void update(int interval);
        ~particle();
    };

    particle::particle() : m_x(0), m_y(0)
    {
        //m_x = ((2.0 * rand()) / RAND_MAX) - 1;
        //m_y = ((2.0 * rand()) / RAND_MAX) - 1;
        m_direction = (2.0 * M_PI * rand()) / RAND_MAX;
        m_speed = ((0.04 * rand()) / RAND_MAX);
        m_speed *= m_speed;
    }
    void particle::init()
    {
        m_x = 0;
        m_y = 0;
        m_direction = (2.0 * M_PI * rand()) / RAND_MAX;
        m_speed = ((0.04 * rand()) / RAND_MAX);
        m_speed *= m_speed;
    }
    void particle::update(int interval)
    {
        m_direction += interval * 0.0002;
        double xspeed = m_speed * cos(m_direction);
        double yspeed = m_speed * sin(m_direction);
        m_x += xspeed * interval; //this way the speed doesnt depend on the speed of the while loop .
        m_y += yspeed * interval;
        if (m_x <= -1.0 || m_x >= 1.0 || m_y <= -1.0 || m_y >= 1.0) //if the particle go out of screen
        {
            init();
        }
        if (rand() < RAND_MAX / 200)
        {
            init();
        }
    }
    particle::~particle()
    {
    }
} // namespace jbm
#endif