/**********
 * GUI.cpp
 * Author: Yoshito Nakaue
 * Created: 2023/11/04
 * Last Modified: 2023/11/04
 **********/

#include "GUI.h"

#include <iostream>

GUI::GUI()
    : m_done(false)
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

    this->m_width = rect.w;
    this->m_height = rect.h;

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

bool GUI::loop()
{
    static int64_t frame = 0;
    ++frame;

    this->draw();
    this->handleEvent();

    return !this->m_done;
}

void GUI::abort()
{
    this->destroyGUI();

    SDL_Quit();
    exit(1);
}

void GUI::handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            this->m_done = true;
            break;

        ////////////////////////
        ///// Button input /////
        ////////////////////////
        case SDL_FINGERUP:
            if (this->m_width - 400 <= event.tfinger.x * this->m_width && event.tfinger.x * this->m_width <= this->m_width)
            {
                if (0 <= event.tfinger.y * this->m_height && event.tfinger.y * this->m_height <= this->m_height / 2)
                {
                    this->sendImage();
                }
                else
                {
                    this->resetCanvas();
                }
            }
            break;

        default:
            break;
        }
    }
}

void GUI::sendImage()
{
    std::cout << "Send image" << std::endl;
}

void GUI::resetCanvas()
{
    std::cout << "Reset canvas" << std::endl;
}

void GUI::draw()
{
    // Clear the screen
    SDL_SetRenderDrawColor(this->m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(this->m_renderer);

    ///////////////////////
    ///// Send button /////
    ///////////////////////
    SDL_SetRenderDrawColor(this->m_renderer, 0, 69, 255, 255);
    SDL_Rect rect_btn1 = { this->m_width - 400, 0, 400, this->m_height / 2 };
    SDL_RenderFillRect(this->m_renderer, &rect_btn1);

    ////////////////////////
    ///// Reset button /////
    ////////////////////////
    SDL_SetRenderDrawColor(this->m_renderer, 255, 69, 0, 255);
    SDL_Rect rect_btn2 = { this->m_width - 400, this->m_height / 2, 400, this->m_height / 2 };
    SDL_RenderFillRect(this->m_renderer, &rect_btn2);

    // Swap buffers
    SDL_RenderPresent(this->m_renderer);
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
