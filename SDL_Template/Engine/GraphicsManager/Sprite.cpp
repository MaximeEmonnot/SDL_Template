#include "Sprite.h"

GraphicsEngine::Sprite::Sprite()
    :
    tex(nullptr)
{
}

GraphicsEngine::Sprite::Sprite(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    width = surf->w, height = surf->h;
    SDL_FreeSurface(surf);
}

GraphicsEngine::Sprite::Sprite(SDL_Surface* surf, SDL_Renderer* renderer, int width, int height)
    :
    width(width),
    height(height)
{
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(renderer, surf);
}

GraphicsEngine::Sprite::Sprite(const Sprite& newSurface)
{
    SDL_DestroyTexture(tex);
    this->tex = newSurface.tex;
    this->width = newSurface.width;
    this->height = newSurface.height;
}

GraphicsEngine::Sprite& GraphicsEngine::Sprite::operator=(const Sprite& rhs)
{
    tex = rhs.tex;
    width = rhs.width;
    height = rhs.height;
    return *this;
}

GraphicsEngine::Sprite::~Sprite()
{
    SDL_DestroyTexture(tex);
}

void GraphicsEngine::Sprite::InitSurface(const char* path, SDL_Renderer* renderer)
{
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(renderer, surf);
    width = surf->w, height = surf->h;
    SDL_FreeSurface(surf);
}

SDL_Texture* GraphicsEngine::Sprite::GetTexture() const
{
    return tex;
}

int GraphicsEngine::Sprite::GetWidth() const
{
    return width;
}

int GraphicsEngine::Sprite::GetHeight() const
{
    return height;
}
