#include "Surface.h"

Surface::Surface()
    :
    tex(nullptr)
{
}

Surface::Surface(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surf = IMG_Load(path);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    width = surf->w, height = surf->h;
    SDL_FreeSurface(surf);
}

Surface::Surface(SDL_Surface* surf, SDL_Renderer* renderer, int width, int height)
    :
    width(width),
    height(height)
{
    tex = SDL_CreateTextureFromSurface(renderer, surf);
}

Surface::Surface(const Surface& newSurface)
{
    SDL_DestroyTexture(tex);
    this->tex = newSurface.tex;
    this->width = newSurface.width;
    this->height = newSurface.height;
}

Surface& Surface::operator=(const Surface& rhs)
{
    tex = rhs.tex;
    width = rhs.width;
    height = rhs.height;
    return *this;
}

Surface::~Surface()
{
    SDL_DestroyTexture(tex);
}

void Surface::InitSurface(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surf = IMG_Load(path);
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    width = surf->w, height = surf->h;
    SDL_FreeSurface(surf);
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
