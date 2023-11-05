/**********
 * main.cpp
 * Author: Yoshito Nakaue
 * Created: 2023/11/04
 * Last Modified: 2023/11/04
 **********/

#include <iostream>
#include <thread>

#include "GUI.h"

int main(void)
{
    GUI gui;

    while (gui.loop())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}
