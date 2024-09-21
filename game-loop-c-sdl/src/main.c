#include <stdio.h>
#include <SDL.h>

#define FALSE 0
#define TRUE 1

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

struct ball {
    float x;
    float y;
    float width;
    float height;
} ball;


int game_is_running = FALSE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int last_frame_time = 0;

int
init_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initialising SDL.\n");
        return FALSE;
    }

    window = SDL_CreateWindow(
            "Pong", // title
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_BORDERLESS
            );

    if (window == NULL) {
        fprintf(stderr, "Error creating window\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(
            window,
            -1, // default display driver
            0 // flags
            );

    if (renderer == NULL) {
        fprintf(stderr, "Error creating renderer\n");
        return FALSE;
    }

    return TRUE;
}


void
destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void
process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        game_is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            game_is_running = FALSE;
        }
        break;
    }
    return;
}


void
setup(void) {
    ball.x = 20;
    ball.y = 20;
    ball.width  = 15;
    ball.height = 15;
}


void
update(void) {
    //SDL_Delay();

    // while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    int time_to_wait_ms = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait_ms > 0 && time_to_wait_ms <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait_ms);
    }

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    ball.x += 50 * delta_time;
    ball.y += 50 * delta_time;
    return;
}


void
render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_RenderPresent(renderer);
}


int
main(int argc, char **argv) {
    puts("Built: " __DATE__ ", " __TIME__);

    game_is_running = init_window();

    setup();

    while (game_is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}

