#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

const int SCREEN_WIDTH{800};
const int SCREEN_HEIGHT{600};
const int NUM_BARS{100};
const int BAR_WIDTH = SCREEN_WIDTH / NUM_BARS;
const int DELAY_MS = 10;

void drawArray(SDL_Renderer *renderer, const std::vector<int> &arr)
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // draw bars
    for (size_t i = 0; i < arr.size(); i++)
    {
        int ht = arr[i] * (SCREEN_HEIGHT / NUM_BARS);
        SDL_Rect bar = {static_cast<int>(i) * BAR_WIDTH, SCREEN_HEIGHT - ht, BAR_WIDTH - 1, ht};

        // set bar color
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
        SDL_RenderFillRect(renderer, &bar);
    }

    // present renderer to screen
    SDL_RenderPresent(renderer);
}

int partition(std::vector<int> &arr, int low, int high, SDL_Renderer *renderer)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
            drawArray(renderer, arr);
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
        }
    }
    std::swap(arr[i + 1], arr[high]);
    drawArray(renderer, arr);
    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
    return i + 1;
}

void quicksort(std::vector<int> &arr, int low, int high, SDL_Renderer *renderer)
{
    if (low < high)
    {
        int pivot = partition(arr, low, high, renderer);
        quicksort(arr, low, pivot - 1, renderer);
        quicksort(arr, pivot + 1, high, renderer);
    }
}

int main()
{
    // intialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // create window
    SDL_Window *window = SDL_CreateWindow("Quick Sort Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "WIndow could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize array with random values
    std::vector<int> arr(NUM_BARS);
    for (int i = 0; i < NUM_BARS; ++i)
    {
        arr[i] = i + 1;
    }
    std::random_shuffle(arr.begin(), arr.end());

    // Draw initial unsorted array
    drawArray(renderer, arr);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Sort the array with visualization
    quicksort(arr, 0, NUM_BARS - 1, renderer);

    std::cout << "Array sorted!" << std::endl;

    // Wait for user to close the window
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}