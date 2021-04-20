#include "Surface.h"

Surface::Surface(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surf = IMG_Load(path);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    width = surf->w, height = surf->h;
    SDL_FreeSurface(surf);
}

Surface::~Surface()
{
    SDL_DestroyTexture(tex);
}

SDL_Texture* Surface::GetTexture() const
{
    return tex;
}

int Surface::GetWidth() const
{
    return width;
}

int Surface::GetHeight() const
{
    return height;
}
