/**********
 * GUI.cpp
 * Author: Yoshito Nakaue
 * Created: 2023/11/04
 * Last Modified: 2023/11/04
 **********/

#include "GUI.h"

#include <iostream>

GUI::GUI()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    /////////////////////////////////////////////////////////
    ///// Create a window that covers the entire screen /////
    /////////////////////////////////////////////////////////
    SDL_Rect rect;
    if (SDL_GetDisplayBounds(0, &rect) != 0)
    {
        std::cout << "SDL_GetDisplayBounds failed: " << SDL_GetError() << std::endl;
        this->abort();
    }

    std::cout << "Display size: Width -> " << rect.w << ", Height -> " << rect.h << std::endl;

    this->m_window = SDL_CreateWindow(
        "INTERCOM",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        rect.w,
        rect.h,
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS
    );

    if (this->m_window == NULL)
    {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        this->abort();
    }

    std::cout << "Window created" << std::endl;

    /////////////////////////////
    ///// Create a renderer /////
    /////////////////////////////
    this->m_renderer = SDL_CreateRenderer(
        this->m_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (this->m_renderer == NULL)
    {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        this->abort();
    }

    std::cout << "Renderer created" << std::endl;
}

GUI::~GUI()
{
    this->destroyGUI();
    SDL_Quit();
}

void GUI::abort()
{
    this->destroyGUI();

    SDL_Quit();
    exit(1);
}

void GUI::destroyGUI()
{
    this->destroyRenderer();
    this->destroyWindow();
}

void GUI::destroyWindow()
{
    if (this->m_window != NULL)
    {
        SDL_DestroyWindow(this->m_window);

        std::cout << "Window destroyed" << std::endl;
    }
} 

void GUI::destroyRenderer()
{
    if (this->m_renderer != NULL)
    {
        SDL_DestroyRenderer(this->m_renderer);
        
        std::cout << "Renderer destroyed" << std::endl;
    }
}
