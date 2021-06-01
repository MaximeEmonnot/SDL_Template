#include "Sprite.h"
#include "Graphics.h"

GraphicsEngine::Sprite::Sprite()
    :
    mpTex(nullptr)
{
}

GraphicsEngine::Sprite::Sprite(const char* path)
{
    auto gfx = Graphics::GetInstance();
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.", __LINE__);
    mpTex = SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf);
    mWidth = surf->w, mHeight = surf->h;
    SDL_FreeSurface(surf);
}

GraphicsEngine::Sprite::Sprite(SDL_Surface* surf, int width, int height)
    :
    mWidth(width),
    mHeight(height)
{
    auto gfx = Graphics::GetInstance();
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.", __LINE__);
    mpTex = SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf);
}

GraphicsEngine::Sprite::Sprite(const Sprite& newSurface)
{
    SDL_DestroyTexture(mpTex);
    this->mpTex = newSurface.mpTex;
    this->mWidth = newSurface.mWidth;
    this->mHeight = newSurface.mHeight;
}

GraphicsEngine::Sprite& GraphicsEngine::Sprite::operator=(const Sprite& rhs)
{
    mpTex = rhs.mpTex;
    mWidth = rhs.mWidth;
    mHeight = rhs.mHeight;
    return *this;
}

GraphicsEngine::Sprite::~Sprite()
{
    SDL_DestroyTexture(mpTex);
}

void GraphicsEngine::Sprite::InitSurface(const char* path)
{
    auto gfx = Graphics::GetInstance();
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw SDLException("SDL Sprite Exception", __FILE__, __LINE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.");
    mpTex = SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf);
    mWidth = surf->w, mHeight = surf->h;
    SDL_FreeSurface(surf);
}

SDL_Texture* GraphicsEngine::Sprite::GetTexture() const
{
    return mpTex;
}

int GraphicsEngine::Sprite::GetWidth() const
{
    return mWidth;
}

int GraphicsEngine::Sprite::GetHeight() const
{
    return mHeight;
}
