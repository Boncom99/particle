#include <iostream>
#include <math.h>
#include <time.h>
#include "screen.h"
#include "particle.h"
#include "swarm.h"

using namespace std;
using namespace jbm;

int main()
{
    srand(time(NULL)); //seed number for random generator. dont want the rand depending on time.
    screen screen;
    if (screen.init() == false)
    {
        cout << "error init" << endl;
    }

    swarm swarm;

    //game loop
    int h = screen::SCREEN_HEIGHT;
    int w = screen::SCREEN_WIDTH;
    int h2 = h / 2;
    while (true)
    {
        int elapsed = SDL_GetTicks(); //number of milisecons from the program started

        // screen.clear(); //NO need because we are gona blur
        swarm.update(elapsed); // not the best efficient way to do it. better if implement the update code inside the for() loop
        //Change color
        unsigned char red = (unsigned char)((1 + sin(elapsed * 0.000041)) / 2.0 * 215 + 40);
        unsigned char green = (unsigned char)((1 + sin(elapsed * 0.000021)) / 2.0 * 200);
        unsigned char blue = (unsigned char)((1 + sin(elapsed * 0.000037)) / 2.0 * 215 + 40);

        const particle *const pParticles = swarm.getParticle();
        for (int i = 0; i < swarm::NPARTICLES; i++)
        {
            particle particle = pParticles[i];

            int x = w * (particle.m_x + 1) / 2;
            int y = w * particle.m_y / 2 + h2;
            screen.setPixel(x, y, red, green, blue); //TODO check if the coordinates are in the correct range
            //screen.setPixel(x, y, 255, 255, 255);    //TODO check if the coordinates are in the correct range
        }
        screen.boxBlur();
        screen.update();
        if (screen.processEvents() == false)
            break;
    }
    //Destroy window, render, texture.
    screen.close();

    return 0;
}