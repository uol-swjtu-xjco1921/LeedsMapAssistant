/*
  * Test whether SDL is available.
  * 
*/
// Headers
#include <SDL2/SDL.h>
#include <stdio.h>
// Main function
int main(int argc, char* argv[])
{
    // Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return(1);

    // Delay 2 seconds
    SDL_Delay(2000);
    printf("SDL has installed successfully\n");
    // Quit SDL
    SDL_Quit();

    // Return
    return 0;
}