#include "files.h"

void openF(Game *game){
    OPENFILENAME ofn;
    HWND hwnd;
    HANDLE hf;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = game->fname;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(game->fname);
    ofn.lpstrFilter = "Image(*.bmp)\0*.BMP\0All\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    GetOpenFileName(&ofn);
}

void load_from_bmp(Game *game){
    int i, j;
    FILE* fp = fopen(game->fname, "r");

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, fp);

    int offset = info[10] | (info[11]<<8) | (info[12]<<16) | (info[13]<<24);
    if((game->image_width = info[18] | (info[19]<<8) | (info[20]<<16) | (info[21]<<24)) > game->win_width)
        game->image_width = game->win_width;
    if((game->image_height = info[22] | (info[23]<<8) | (info[24]<<16) | (info[25]<<24)) > game->win_height)
        game->image_height = game->win_height;

    fseek(fp, offset, SEEK_SET);

    int size = game->image_width * game->image_height * 3;
    game->image = (unsigned char*)malloc(size * sizeof(unsigned char));
    fread(game->image, sizeof(unsigned char), size, fp);
    fclose(fp);

    set_image(game);
}
