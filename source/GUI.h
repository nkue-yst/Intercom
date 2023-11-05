/**********
 * GUI.h
 * Author: Yoshito Nakaue
 * Created: 2023/11/04
 * Last Modified: 2023/11/04
 **********/

#ifndef GUI_H
#define GUI_H

#include <vector>

#include "SDL.h"

class GUI
{
public:
    GUI();
    ~GUI();

    bool loop();

    void abort();

private:
    void handleEvent();
    void sendImage();
    void resetCanvas();

    void draw();

    void destroyGUI();

    void destroyWindow();
    void destroyRenderer();

    bool m_done;  // Whether the main loop is done or not

    bool m_buttonPressed[2];  // Whether the button is pressed or not

    int m_width;
    int m_height;

    std::vector<std::vector<struct SDL_Point>> m_lines;  // Vector of lines

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

#endif  // GUI_H
