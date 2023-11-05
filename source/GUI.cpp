/**********
 * GUI.cpp
 * Author: Yoshito Nakaue
 * Created: 2023/11/04
 * Last Modified: 2023/11/06
 **********/

#include "GUI.h"

#include <chrono>
#include <iostream>

#include "SDL_image.h"

#define FONT_SIZE 100
#define BUTTON_WIDTH 400

GUI::GUI()
    : m_done(false)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG);
    TTF_Init();

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

    /////////////////////
    ///// Load font /////
    /////////////////////
    this->m_font = TTF_OpenFont("../fonts/Ubuntu-Regular.ttf", FONT_SIZE);

    this->m_buttonPressed[0] = false;
    this->m_buttonPressed[1] = false;
}

GUI::~GUI()
{
    this->destroyGUI();

    TTF_Quit();
    IMG_Quit();
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

    TTF_Quit();
    IMG_Quit();
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
        ///// Finger input /////
        ////////////////////////
        case SDL_FINGERDOWN:
            {
                std::vector<SDL_Point> line;
                this->m_lines.push_back(line);    // Add new empty line
            }

        case SDL_FINGERMOTION:
            /////////////////////////
            ///// Drawing event /////
            /////////////////////////
            if (event.tfinger.x * this->m_width < this->m_width - BUTTON_WIDTH)
            {
                SDL_Point point;
                point.x = event.tfinger.x * this->m_width;
                point.y = event.tfinger.y * this->m_height;
                this->m_lines.back().push_back(point);
            }

            ////////////////////////
            ///// Button event /////
            ////////////////////////
            if (this->m_width - BUTTON_WIDTH <= event.tfinger.x * this->m_width && event.tfinger.x * this->m_width <= this->m_width)
            {
                if (0 <= event.tfinger.y * this->m_height && event.tfinger.y * this->m_height <= this->m_height / 2)
                {
                    this->m_buttonPressed[0] = true;
                    this->m_buttonPressed[1] = false;
                }
                else
                {
                    this->m_buttonPressed[0] = false;
                    this->m_buttonPressed[1] = true;
                }
            }
            else
            {
                this->m_buttonPressed[0] = false;
                this->m_buttonPressed[1] = false;
            }

            break;

        case SDL_FINGERUP:
            ////////////////////////
            ///// Button event /////
            ////////////////////////
            if (this->m_width - BUTTON_WIDTH <= event.tfinger.x * this->m_width && event.tfinger.x * this->m_width <= this->m_width)
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
            this->m_buttonPressed[0] = false;
            this->m_buttonPressed[1] = false;
            break;

        default:
            break;
        }
    }
}

void GUI::sendImage()
{
    //////////////////////////
    ///// Create surface /////
    //////////////////////////
    SDL_Surface* surface;
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(
        0,
        this->m_width - BUTTON_WIDTH,
        this->m_height,
        32,
        rmask,
        gmask,
        bmask,
        amask
    );

    if (surface == NULL)
    {
        std::cout << "SDL_CreateRGBSurface failed: " << SDL_GetError() << std::endl;
        this->abort();
    }

    ////////////////////////////////////
    ///// Save surface to bmp file /////
    ////////////////////////////////////
    SDL_Rect rect = { 0, 0, this->m_width - BUTTON_WIDTH, this->m_height };
    SDL_RenderReadPixels(
        this->m_renderer,
        &rect,
        SDL_PIXELFORMAT_ARGB8888,
        surface->pixels,
        surface->pitch
    );

    // Setting file name with timestamp
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char filename[100];
    std::strftime(filename, sizeof(filename), "%Y-%m-%d-%H-%M-%S.jpg", std::localtime(&now_c));

    IMG_SaveJPG(surface, filename, 100);
    SDL_FreeSurface(surface);

    std::cout << "Save image: " << filename << std::endl;

    this->resetCanvas();
}

void GUI::resetCanvas()
{
    this->m_lines.clear();

    std::cout << "Reset canvas" << std::endl;
}

void GUI::draw()
{
    // Clear the screen
    SDL_SetRenderDrawColor(this->m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(this->m_renderer);

    //////////////////////////
    ///// Draw the lines /////
    //////////////////////////
    SDL_SetRenderDrawColor(this->m_renderer, 0, 0, 0, 255);
    for (auto line : this->m_lines)
    {
        SDL_RenderDrawLines(this->m_renderer, line.data(), line.size());
    }

    ///////////////////////
    ///// Send button /////
    ///////////////////////
    if (!this->m_buttonPressed[0])
        SDL_SetRenderDrawColor(this->m_renderer, 0, 0, 255, 255);
    else
        SDL_SetRenderDrawColor(this->m_renderer, 0, 69, 255, 255);
    SDL_Rect rect_btn1 = { this->m_width - BUTTON_WIDTH, 0, BUTTON_WIDTH, this->m_height / 2 };
    SDL_RenderFillRect(this->m_renderer, &rect_btn1);

    // Draw text
    SDL_Surface* surface_text;
    surface_text = TTF_RenderUTF8_Blended_Wrapped(
        this->m_font,
        "Send\n(Call)",
        { 255, 255, 255, 255 },
        0
    );
    SDL_Texture* texture_text = SDL_CreateTextureFromSurface(this->m_renderer, surface_text);
    SDL_FreeSurface(surface_text);

    SDL_Rect rect_text = { this->m_width - BUTTON_WIDTH + 100, this->m_height / 6, 200, this->m_height / 6 };
    SDL_RenderCopy(this->m_renderer, texture_text, NULL, &rect_text);
    SDL_DestroyTexture(texture_text);

    ////////////////////////
    ///// Reset button /////
    ////////////////////////
    if (!this->m_buttonPressed[1])
        SDL_SetRenderDrawColor(this->m_renderer, 255, 0, 0, 255);
    else
        SDL_SetRenderDrawColor(this->m_renderer, 255, 69, 0, 255);
    SDL_Rect rect_btn2 = { this->m_width - BUTTON_WIDTH, this->m_height / 2, BUTTON_WIDTH, this->m_height / 2 };
    SDL_RenderFillRect(this->m_renderer, &rect_btn2);

    // Draw text
    surface_text = TTF_RenderUTF8_Blended_Wrapped(
        this->m_font,
        "Reset",
        { 255, 255, 255, 255 },
        0
    );
    texture_text = SDL_CreateTextureFromSurface(this->m_renderer, surface_text);
    SDL_FreeSurface(surface_text);

    rect_text = { this->m_width - BUTTON_WIDTH + 100, this->m_height / 10 * 7, 200, this->m_height / 10 };
    SDL_RenderCopy(this->m_renderer, texture_text, NULL, &rect_text);
    SDL_DestroyTexture(texture_text);

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
