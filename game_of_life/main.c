#include "game.h"

int main(int argc, char *argv[]) {
    short i, j;

    State state = EDIT;
    Mouse mouse = NOPE;
    Game game = {0,
                0,
                NULL,
                NULL,
                NULL,
                3,
                115,
                0,
                100,
                750,
                600};

    init(&game);

    while (state != QUIT) {
        SDL_WaitEvent(&game.ev);

        switch(game.ev.type){
            case SDL_QUIT:
                state = QUIT;
                break;
            case SDL_USEREVENT:
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(game.ev.button.button == SDL_BUTTON_LEFT)
                    mouse = LMOUSE;
                else if(game.ev.button.button == SDL_BUTTON_RIGHT)
                    mouse = RMOUSE;
                break;
            case SDL_MOUSEMOTION:
                if(mouse == LMOUSE)
                    mouse = LMOTION;
                else if(mouse == RMOUSE)
                    mouse = RMOTION;
                break;
            case SDL_MOUSEBUTTONUP:
                mouse = NOPE;
                break;
            case SDL_KEYDOWN:
                switch(game.ev.key.keysym.sym){
                    case SDLK_1:
                        state = EDIT;
                        break;
                    case SDLK_2:
                        if(!game.id)
                            game.id = SDL_AddTimer(game.delay, push_ev, NULL);
                        state = PLAY;
                        break;
                    case SDLK_3:
                        openF(&game);
                        state = LOAD;
                        break;
                    case SDLK_4:
                        clear_table(&game);
                        state = EDIT;
                        break;
                    case SDLK_w:
                        game.treshold+=5;
                        set_image(&game);
                        break;
                    case SDLK_s:
                        game.treshold-=5;
                        set_image(&game);
                        break;
                    case SDLK_d:
                        game.invert = !game.invert;
                        set_image(&game);
                        break;
                    case SDLK_SPACE:
                        if(game.id){
                            SDL_RemoveTimer(game.id);
                            game.id = NULL;
                        }
                        state = STEP;
                        break;
                    case SDLK_ESCAPE:
                        state = QUIT;
                        break;
                }
            default:
                break;
        }

        switch(state){
            case IDLE:
                break;
            case EDIT:
                if(game.id){
                    SDL_RemoveTimer(game.id);
                    game.id = NULL;
                }
                if(mouse != NOPE){
                    if(mouse > 2){
                        game.table[game.ev.motion.y/game.scale][game.ev.motion.x/game.scale] = mouse - 3;
                    }
                    else
                        game.table[game.ev.button.y/game.scale][game.ev.button.x/game.scale] = mouse - 1;
                }
                break;
            case STEP:
                step(&game);
                state = EDIT;
                break;
            case PLAY:
                if(game.ev.type == SDL_USEREVENT)
                    step(&game);
                break;
            case LOAD:
                load_from_bmp(&game);
                state = EDIT;
                break;
            default:
                break;
        }
        draw_table(&game);
        SDL_Flip(game.screen);
    }

    printf("sizeof(timer): %d",sizeof(SDL_Event));

    destruct(&game);
    SDL_Quit();

    return 0;
}
