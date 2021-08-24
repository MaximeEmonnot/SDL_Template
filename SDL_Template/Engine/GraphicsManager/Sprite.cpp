#include "Sprite.h"
#include "Graphics.h"

GraphicsEngine::Sprite::Sprite(const std::string& path)
{
    std::shared_ptr<SDL_Surface> surf(IMG_Load(path.c_str()), SDL_FreeSurface);
    if (surf == nullptr)
        throw EngineException("SDL Sprite Exception", __FILE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.", __LINE__);
    mpTex.reset(SDL_CreateTextureFromSurface(Graphics::GetInstance().GetRenderer().get(), surf.get()), SDL_DestroyTexture);
    mWidth = surf->w, mHeight = surf->h;
}

GraphicsEngine::Sprite::Sprite(std::shared_ptr<SDL_Surface> surf, int width, int height)
    :
    mWidth(width),
    mHeight(height)
{
    if (surf == nullptr)
        throw EngineException("SDL Sprite Exception", __FILE__, "An error has been caught during SDL Surface initialisation.\nPlease check sprite path.", __LINE__);
    mpTex.reset(SDL_CreateTextureFromSurface(Graphics::GetInstance().GetRenderer().get(), surf.get()), SDL_DestroyTexture);
}

GraphicsEngine::Sprite::Sprite(const Sprite& newSurface)
    :
    mWidth(newSurface.mWidth),
    mHeight(newSurface.mHeight),
    mpTex(newSurface.mpTex)
{
}

GraphicsEngine::Sprite& GraphicsEngine::Sprite::operator=(const Sprite& rhs)
{
    mpTex = rhs.mpTex;
    mWidth = rhs.mWidth;
    mHeight = rhs.mHeight;
    return *this;
}

void GraphicsEngine::Sprite::BlendColor(const GraphicsEngine::Color& c)
{
    SDL_SetTextureColorMod(mpTex.get(), c.c.r, c.c.g, c.c.b);
}

std::shared_ptr<SDL_Texture> GraphicsEngine::Sprite::GetTexture() const
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
