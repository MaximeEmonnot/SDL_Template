#include "Sprite.h"

Sprite::Sprite()
    :
    tex(nullptr)
{
}

Sprite::Sprite(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    width = surf->w, height = surf->h;
    SDL_FreeSurface(surf);
}

Sprite::Sprite(SDL_Surface* surf, SDL_Renderer* renderer, int width, int height)
    :
    width(width),
    height(height)
{
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(renderer, surf);
}

Sprite::Sprite(const Sprite& newSurface)
{
    SDL_DestroyTexture(tex);
    this->tex = newSurface.tex;
    this->width = newSurface.width;
    this->height = newSurface.height;
}

Sprite& Sprite::operator=(const Sprite& rhs)
{
    tex = rhs.tex;
    width = rhs.width;
    height = rhs.height;
    return *this;
}

Sprite::~Sprite()
{
    SDL_DestroyTexture(tex);
}

void Sprite::InitSurface(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    width = surf->w, height = surf->h;
    SDL_FreeSurface(surf);
}

SDL_Texture* Sprite::GetTexture() const
{
    return tex;
}

int Sprite::GetWidth() const
{
    return width;
}

int Sprite::GetHeight() const
{
    return height;
}
