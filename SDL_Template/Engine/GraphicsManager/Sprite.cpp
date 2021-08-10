#include "Sprite.h"
#include "Graphics.h"

GraphicsEngine::Sprite::Sprite()
{
}

GraphicsEngine::Sprite::Sprite(const char* path)
    :
    texPath(path)
{
    auto gfx = Graphics::GetInstance();
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw EngineException("SDL Sprite Exception", __FILE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.", __LINE__);
    mpTex.reset(SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf), SDL_DestroyTexture);
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
        throw EngineException("SDL Sprite Exception", __FILE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.", __LINE__);
    mpTex.reset(SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf), SDL_DestroyTexture);
}

GraphicsEngine::Sprite::Sprite(const Sprite& newSurface)
    :
    mWidth(newSurface.mWidth),
    mHeight(newSurface.mHeight),
    mpTex(newSurface.mpTex)
{
    //InitSurface(newSurface.texPath.c_str());
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
}

void GraphicsEngine::Sprite::InitSurface(const char* path)
{
    texPath = path;
    auto gfx = Graphics::GetInstance();
    SDL_Surface* surf = IMG_Load(path);
    if (surf == nullptr)
        throw EngineException("SDL Sprite Exception", __FILE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.", __LINE__);
    mpTex.reset(SDL_CreateTextureFromSurface(gfx->GetRenderer(), surf), SDL_DestroyTexture);
    mWidth = surf->w, mHeight = surf->h;
    SDL_FreeSurface(surf);
}

void GraphicsEngine::Sprite::BlendColor(const GraphicsEngine::Color& c)
{
    SDL_SetTextureColorMod(mpTex.get(), c.c.r, c.c.g, c.c.b);
}

SDL_Texture* GraphicsEngine::Sprite::GetTexture() const
{
    return mpTex.get();
}

int GraphicsEngine::Sprite::GetWidth() const
{
    return mWidth;
}

int GraphicsEngine::Sprite::GetHeight() const
{
    return mHeight;
}
