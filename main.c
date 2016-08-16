#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <stdio.h>

#define WHITE 0xFFFFFFFF
#define GREEN 0x22FF88FF
#define BLACK 0x000000FF

#define DELAY 100
#define SCALE 10
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define SCREEN_WIDTH WIN_WIDTH / SCALE
#define SCREEN_HEIGHT WIN_HEIGHT /SCALE

char table[SCREEN_HEIGHT][SCREEN_WIDTH];
char temp[SCREEN_HEIGHT][SCREEN_WIDTH];
SDL_Event ev;
SDL_TimerID id;
SDL_Surface *screen;

Uint32 idozit(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

void load_from_file(char *fileName){
    FILE *fp=fopen(fileName, "r");
    char temp[300];
    char c;
    short i=0, j=0;

    do{
        c=fgetc(fp);
        if(c == '\n'){
            j=0;
            ++i;
            continue;
        }
        if(c == ' ')
            table[i][j] = 0;
        else
            table[i][j] = 1;
        ++j;
    }while(!feof(fp));

    fclose(fp);
}

void init(){
    short i, j;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    screen=SDL_SetVideoMode(WIN_WIDTH, WIN_HEIGHT, 0, SDL_HWACCEL);
    if (!screen) {
        fprintf(stderr, "Nem sikerult megnyitni az ablakot!\n");
        exit(1);
    }
    SDL_WM_SetCaption("SDL peldaprogram", "SDL peldaprogram");

    for(i=0;i<SCREEN_HEIGHT;++i){
        for(j=0;j<SCREEN_WIDTH;++j){
            table[i][j]=0;
            temp[i][j]=0;
        }
    }
}

void draw_table(){
    short i, j;

    boxColor(screen, 0, 0, WIN_WIDTH, WIN_HEIGHT, BLACK);

    for(i=0;i<SCREEN_HEIGHT;++i){
        for(j=0;j<SCREEN_WIDTH;++j){
            if(table[i][j] == 1)
                boxColor(screen, j * SCALE, i * SCALE, j * SCALE + SCALE, i * SCALE + SCALE, GREEN);
        }
    }
}

void make_step(short y, short x){
    char neighbors=0;
    char i, j;

    for(i=-1;i<2;++i){
        for(j=-1;j<2;++j){
            if(i==0 && j==0)
                continue;
            if(y+i < 0 || x+j < 0 || y+i >  SCREEN_HEIGHT-1 || x+j > SCREEN_WIDTH-1)
                continue;
            if(table[y+i][x+j] == 1)
                neighbors++;
        }
    }

    if(neighbors < 2 || neighbors > 3)
        temp[y][x] = 0;
    else if(neighbors == 2 && table[y][x] == 0)
            temp[y][x] = 0;
        else
            temp[y][x] = 1;
}

void step(){
    short i, j;

    for(i=0;i<SCREEN_HEIGHT;++i){
        for(j=0;j<SCREEN_WIDTH;++j){
            make_step(i ,j);
        }
    }

    for(i=0;i<SCREEN_HEIGHT;++i){
        for(j=0;j<SCREEN_WIDTH;++j){
            table[i][j] = temp[i][j];
        }
    }
}

int main(int argc, char *argv[]) {
    char quit=0;
    short i, j;
    char click=0;

    init();

    while (!quit) {
        SDL_WaitEvent(&ev);

        switch(ev.type){
            case SDL_QUIT:
                quit=1;
                break;
            case SDL_USEREVENT:
                step();
                break;
            case SDL_MOUSEBUTTONDOWN:
                click=1;
                table[ev.button.y/SCALE][ev.button.x/SCALE] = 1;
                break;
            case SDL_MOUSEMOTION:
                if(click)
                    table[ev.motion.y/SCALE][ev.motion.x/SCALE] = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                click=0;
                break;
            case SDL_KEYDOWN:
                switch(ev.key.keysym.sym){
                    case SDLK_1:
                        SDL_RemoveTimer(id);
                        break;
                    case SDLK_2:
                        if(!id)
                            id = SDL_AddTimer(DELAY, idozit, NULL);
                        break;
                    case SDLK_3:
                        load_from_file("monalisa.txt");
                        break;
                    case SDLK_SPACE:
                        if(id){
                            SDL_RemoveTimer(id);
                            id = NULL;
                        }
                        step();
                        break;
                    case SDLK_ESCAPE:
                        quit = 1;
                        break;
                }
            default:
                break;
        }
        draw_table();
        SDL_Flip(screen);
    }

    /* ablak bezarasa */
    SDL_Quit();

    return 0;
}
