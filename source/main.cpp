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

    std::this_thread::sleep_for(std::chrono::seconds(5));
}
