
/**
 * @file main.cpp
 * @brief 
 */

#include "game.hpp"
#include <iostream>

int main() {
    std::cout << "Program started" << std::endl;
    std::cout.flush(); 
    Game game;
    try {
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught" << std::endl;
    }

    return 0;
}
