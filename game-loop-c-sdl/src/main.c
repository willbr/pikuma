#include <stdio.h>
#include <SDL.h>

#define FALSE 0
#define TRUE 1

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

#define PADDLE_SPEED 400.0f

struct game_object {
    float x;
    float y;
    float width;
    float height;
    float vel_x;
    float vel_y;
} ball, paddle, goal_top, goal_bottom;


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
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            game_is_running = FALSE;
            break;
        case SDLK_LEFT:
            paddle.vel_x = -PADDLE_SPEED;
            break;
        case SDLK_RIGHT:
            paddle.vel_x = +PADDLE_SPEED;
            break;
        }
        break;

    case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            game_is_running = FALSE;
        case SDLK_LEFT:
            // fallthrough
        case SDLK_RIGHT:
            paddle.vel_x = 0;
            break;
        }
        break;
    }
    return;
}


void
setup(void) {
    ball.width  = 15;
    ball.height = 15;
    ball.x = 20;
    ball.y = 20;
    ball.vel_x = 300;
    ball.vel_y = 300;

    paddle.width  = 100;
    paddle.height = 20;
    paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
    paddle.y = WINDOW_HEIGHT - 40;
    paddle.vel_x = 0;
    paddle.vel_y = 0;


    goal_top.width  = WINDOW_WIDTH;
    goal_top.height = 40;
    goal_top.x = 0;
    goal_top.y = -goal_top.height;

    goal_bottom.width  = WINDOW_WIDTH;
    goal_bottom.height = 40;
    goal_bottom.x = 0;
    goal_bottom.y = WINDOW_HEIGHT;
}


int
rects_collide(const struct game_object *a, const struct game_object *b) {
    if (a->x + a->width <= b->x) return 0;
    if (b->x + b->width <= a->x) return 0;

    if (a->y + a->height <= b->y) return 0;
    if (b->y + b->height <= a->y) return 0;

    return 1;
}


void
update(void) {
    int time_to_wait_ms = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait_ms > 0 && time_to_wait_ms <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait_ms);
    }

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    ball.x += ball.vel_x * delta_time;
    ball.y += ball.vel_y * delta_time;

    paddle.x += paddle.vel_x * delta_time;
    paddle.y += paddle.vel_y * delta_time;


    // ball + wall collision
    if (ball.x + ball.width > WINDOW_WIDTH) {
        ball.x = WINDOW_WIDTH - ball.width;
        ball.vel_x = -ball.vel_x;
    }

    if (ball.x < 0) {
        ball.x = 0;
        ball.vel_x = -ball.vel_x;
    }

    // ball + paddle collision
    if (rects_collide(&ball, &paddle)) {
        ball.vel_x = -ball.vel_x;
        ball.vel_y = -ball.vel_y;

        ball.x += ball.vel_x * delta_time;
        ball.y += ball.vel_y * delta_time;
    }

    if (rects_collide(&ball, &goal_top)) {
        ball.vel_y = -ball.vel_y;
        ball.y += ball.vel_y * delta_time;
    }

    if (rects_collide(&ball, &goal_bottom)) {
        ball.vel_y = -ball.vel_y;
        ball.y += ball.vel_y * delta_time;
    }


    // paddle + wall collision
    if (paddle.x + paddle.width > WINDOW_WIDTH) {
        paddle.x = WINDOW_WIDTH - paddle.width;
    }

    if (paddle.x < 0) {
        paddle.x = 0;
    }

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

    SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height
    };

    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
    SDL_RenderFillRect(renderer, &paddle_rect);

    SDL_RenderPresent(renderer);
}


int
main(int argc, char **argv) {
    (void)argc;
    (void)argv;

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

