/**********
 * GUI.h
 * Author: Yoshito Nakaue
 * Created: 2023/11/04
 * Last Modified: 2023/11/04
 **********/

#ifndef GUI_H
#define GUI_H

#include "SDL.h"

class GUI
{
public:
    GUI();
    ~GUI();

    void abort();

private:
    void destroyGUI();

    void destroyWindow();
    void destroyRenderer();

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

#endif  // GUI_H
