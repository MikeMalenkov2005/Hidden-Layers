#ifndef TEXTURE_H
#define TEXTURE_H

#include <framework.h>

typedef GLenum Texture;

typedef struct PixelFormatTranslation
{
  GLenum internalFormat;
  GLenum format;
  GLenum type;
} PixelFormatTranslation;

Texture CreateTexture1D(GLsizei levels, GLenum internalFormat, GLsizei width);

Texture CreateTexture2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);

Texture CreateTexture3D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);

void SetTextureInterpolation(Texture texture, bool pixels, bool levels);

void SetTextureWrapping(Texture texture, GLenum mode);

Texture LoadTexture2D(SDL_IOStream *src, GLsizei levels, bool closeio);

bool TranslatePixelFormat(SDL_PixelFormat format, PixelFormatTranslation *translation);

#endif
