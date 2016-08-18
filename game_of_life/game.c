#include "game.h"

Uint32 push_ev(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

void init(Game *game){
    short i, j;

    game->screen_width = game->win_width/game->scale;
    game->screen_height = game->win_height/game->scale;
    game->id = NULL;

    printf("screen_width: %d\tscreen_height: %d\t win_width: %d\t win_height: %d\n",game->screen_width, game->screen_height, game->win_width, game->win_height);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    game->screen = SDL_SetVideoMode(game->win_width, game->win_height, 0, SDL_ANYFORMAT );
    if (!game->screen) {
        fprintf(stderr, "Could not create Window!, Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_WM_SetCaption("Game of Life", "Game of Life");

    game->table = (unsigned char**)malloc(game->screen_height * sizeof(unsigned char*));
    if(!game->table){
        fprintf(stderr,"Failed to allocate memory for table!, Error: %s\n", SDL_GetError());
        return;
    }
    game->table[0] = (unsigned char*)malloc(game->screen_height * game->screen_width * sizeof(unsigned char));
    if(!game->table[0]){
        fprintf(stderr,"Failed to allocate memory for table[0]!, Error: %s\n", SDL_GetError());
        return;
    }
    for(i=1; i<game->screen_height; ++i)
        game->table[i] = game->table[i-1] + game->screen_width;

//    for(i=0;i<game->screen_height;++i){
//        for(j=0;j<game->screen_width;++j)
//            printf("[%d,%d]: %d\t",i,j,game->table[i][j]);
//        printf("\n");
//    }
    clear_table(game);
}

void destruct(Game *game){
    free(game->table[0]);
    free(game->table);
    game->table = NULL;
    free(game->image);
    game->image = NULL;
}

int comp(int a, int b, unsigned char invert){
    if(invert)
        return a > b;
    else
        return a < b;
}

void clear_table(Game *game){
    int i, j;

    for(i=0;i<game->screen_height;i++){
        for(j=0;j<(game->screen_width);++j){
            game->table[i][j] = 0;
        }
    }
}

void draw_table(Game const *game){
    short i, j;

    if(!game->screen)
        printf("kuki");

    boxColor(game->screen, 0, 0, game->win_width, game->win_height, GRAY);
    for(i=0;i<game->screen_height;++i){
        for(j=0;j<game->screen_width;++j){
//            if(game->table[i][j] == 1){
//                filledCircleColor(game->screen, j * game->scale + game->scale/2, i * game->scale + game->scale/2, game->scale/2, BLACK);
//                if(game->scale > 3)
//                    aacircleColor(game->screen, j * game->scale + game->scale/2, i * game->scale + game->scale/2, game->scale/2, BLACK);
//            }
            if(game->table[i][j] == 1)
                boxColor(game->screen, j * game->scale, i * game->scale, (j+1) * game->scale, (i+1) * game->scale, BLACK);
//                rectangleColor(game->screen, j * game->scale, i * game->scale, j * game->scale + game->scale, i * game->scale + game->scale, WHITE);
        }
    }
}

void make_step(Game *game, short y, short x){
    char neighbors=0;
    char i, j;

    for(i=-1;i<2;++i){
        for(j=-1;j<2;++j){
            if(y+i < 0 || x+j < 0 || y+i >  (game->screen_height - 1) || x+j > (game->screen_width - 1) || (i==0 && j==0))
                continue;
            neighbors += game->table[y+i][x+j] & 1;
        }
    }

    if((neighbors < 2 || neighbors > 3) && game->table[y][x] == 1)
        game->table[y][x] = 3;
    else if(game->table[y][x] == 0 && neighbors == 3)
        game->table[y][x] = 2;
}

void step(Game *game){
    short i, j;

    for(i=0;i<game->screen_height;++i){
        for(j=0;j<game->screen_width;++j){
            make_step(game, i ,j);
        }
    }

    for(i=0;i<game->screen_height;++i){
        for(j=0;j<game->screen_width;++j){
            if(game->table[i][j] == 2)
                game->table[i][j] = 1;
            else if(game->table[i][j] == 3)
                game->table[i][j] = 0;
        }
    }
}

void set_image(Game* game){
    short i, j;

    clear_table(game);

    for(i=0;i<game->image_height;i+=game->scale){
        for(j=0;j<game->image_width;j+=game->scale){
            if(comp(game->image[i * game->image_width + j + 2], game->treshold, game->invert))
                game->table[((game->image_height-i-1) + (game->win_height/2) - (game->image_height/2))/game->scale][(j+(game->win_width/2)-(game->image_width/2))/game->scale] = 1;
        }
    }
}
