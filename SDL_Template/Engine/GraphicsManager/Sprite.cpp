#include "Sprite.h"
#include "Graphics.h"

GraphicsEngine::Sprite::Sprite()
    :
    tex(nullptr)
{
}

GraphicsEngine::Sprite::Sprite(const char* path)
{
    auto gfx = Graphics::GetInstance();
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf);
    width = surf->w, height = surf->h;
    SDL_FreeSurface(surf);
}

GraphicsEngine::Sprite::Sprite(SDL_Surface* surf, int width, int height)
    :
    width(width),
    height(height)
{
    auto gfx = Graphics::GetInstance();
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf);
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

void GraphicsEngine::Sprite::InitSurface(const char* path)
{
    auto gfx = Graphics::GetInstance();
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    tex = SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf);
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
