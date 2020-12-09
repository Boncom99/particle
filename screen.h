#ifndef SCREEN_H
#define SCREEN_H
#include <iostream>
#include <SDL2/SDL.h>

//Usable in any SDL program. has nothing particluar about particles

namespace jbm
{ //josep boncompte moya

    class screen
    {
    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;
        Uint32 *m_buffer1;
        Uint32 *m_buffer2;

    public:
        const static int SCREEN_WIDTH = 1410;
        const static int SCREEN_HEIGHT = 840;
        screen();
        bool init();
        bool processEvents();
        void update();
        void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
        //void clear();
        void boxBlur();
        void close();
        ~screen();
    };

    screen::screen() : m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), m_buffer2(NULL)
    {
    }
    bool screen::init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            return false;
        }
        //create window
        m_window = SDL_CreateWindow("particle simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (m_window == NULL)
        {
            SDL_Quit();
            return false;
        }
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
        m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
        if (m_renderer == NULL)
        {
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }
        if (m_texture == NULL)
        {
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            SDL_Quit();
            return false;
        }
        m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];                //int with 32 bites. one int for each pixel ( 4bytes * 8 = 32 bites)
        m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];                //int with 32 bites. one int for each pixel ( 4bytes * 8 = 32 bites)
        memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)); //inicialitzar totes els elements de buffer amb el color blanc (255 = 0xFF en exadecimal) or black (0);
        memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)); //inicialitzar totes els elements de buffer amb el color blanc (255 = 0xFF en exadecimal) or black (0);
        //buffer[30000] = 0xFFFFFFFF;
        //each pare of FF is Read Green Blue Alpha
        for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
        {

            //m_buffer[i] = 0xFFFFFFFF;
            m_buffer1[i] = 0x00000000;
        }

        return true;
    }

    bool screen::processEvents()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) //while we still have events to process
        {
            if (event.type == SDL_QUIT)
            { //clickar la creu de tancarr la finestre
                return false;
            }
        }
        return true;
    }
    void screen::update()
    {
        SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
        SDL_RenderPresent(m_renderer);
    }
    void screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
    {
        //TODO   Check in main.cc where we call setpixel and make sure the pixels are inside the correct range instead of checkin it here in the if
        if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) //UNefficient !!!! there are a lot of pixel!
        {
            return;
        }

        Uint32 color = 0;
        color += red;
        color <<= 8;
        color += green;
        color <<= 8;
        color += blue;
        color <<= 8;
        color += 0xFF;

        m_buffer1[y * SCREEN_WIDTH + x] = color;
    }
    /*  void screen::clear()
    {
        memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)); //inicialitzar totes els elements de buffer amb el color blanc (255 = 0xFF en exadecimal) or black (0);
    }*/
    void screen::boxBlur()
    {
        //Swap the buffers, so pixel info in in buffer 2 and we are drawing on buffer1
        Uint32 *temp = m_buffer1;
        m_buffer1 = m_buffer2;
        m_buffer2 = temp;

        for (int y = 0; y < SCREEN_HEIGHT; y++)
        {
            for (int x = 0; x < SCREEN_WIDTH; x++)
            {
                /* 
                0 0 0
                0 1 0
                0 0 0
                */
                int redTotal = 0;
                int greenTotal = 0;
                int blueTotal = 0;
                for (int row = -1; row <= 1; row++)
                {
                    for (int col = -1; col <= 1; col++)
                    {
                        int currentx = x + col;
                        int currenty = y + row;
                        if (currentx >= 0 && currentx < SCREEN_WIDTH && currenty >= 0 && currenty < SCREEN_HEIGHT)
                        {
                            Uint32 color = m_buffer2[currenty * SCREEN_WIDTH + currentx];
                            Uint8 red = color >> 24;
                            Uint8 green = color >> 16;
                            Uint8 blue = color >> 8;
                            redTotal += red;
                            greenTotal += green;
                            blueTotal += blue;
                        }
                    }
                }
                Uint8 red = redTotal / 9;
                Uint8 green = greenTotal / 9;
                Uint8 blue = blueTotal / 9;
                setPixel(x, y, red, green, blue);
            }
        }
    }
    void screen::close()
    {
        delete[] m_buffer1;
        delete[] m_buffer2;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    screen::~screen()
    {
    }
} // namespace jbm
#endif
