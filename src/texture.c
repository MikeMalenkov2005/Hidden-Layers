#include <texture.h>

Texture CreateTexture1D(GLsizei levels, GLenum internalFormat, GLsizei width)
{
  Texture texture;
  glCreateTextures(GL_TEXTURE_1D, 1, &texture);
  if (!texture) return (Texture)SDL_SetError("Failed to create a texture (OpenGL %u)", glGetError());
  glTextureStorage1D(texture, levels, internalFormat, width);
  GLenum error = glGetError();
  if (error == GL_NO_ERROR) return texture;
  glDeleteTextures(1, &texture);
  return (Texture)SDL_SetError("Failed to allocate texture storage (OpenGL %u)", error);
}

Texture CreateTexture2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
  Texture texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);
  if (!texture) return (Texture)SDL_SetError("Failed to create a texture (OpenGL %u)", glGetError());
  glTextureStorage2D(texture, levels, internalFormat, width, height);
  GLenum error = glGetError();
  if (error == GL_NO_ERROR) return texture;
  glDeleteTextures(1, &texture);
  return (Texture)SDL_SetError("Failed to allocate texture storage (OpenGL %u)", error);
}

Texture CreateTexture3D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth)
{
  Texture texture;
  glCreateTextures(GL_TEXTURE_3D, 1, &texture);
  if (!texture) return (Texture)SDL_SetError("Failed to create a texture (OpenGL %u)", glGetError());
  glTextureStorage3D(texture, levels, internalFormat, width, height, depth);
  GLenum error = glGetError();
  if (error == GL_NO_ERROR) return texture;
  glDeleteTextures(1, &texture);
  return (Texture)SDL_SetError("Failed to allocate texture storage (OpenGL %u)", error);
}

void SetTextureInterpolation(Texture texture, bool pixels, bool levels)
{
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, pixels ? GL_LINEAR : GL_NEAREST);
  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, pixels ? (levels ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST)
                                                             : (levels ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST));
}

void SetTextureWrapping(Texture texture, GLenum mode)
{
  glTextureParameteri(texture, GL_TEXTURE_WRAP_S, mode);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_T, mode);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_R, mode);
}

Texture LoadTexture2D(SDL_IOStream *src, GLsizei levels, bool closeio)
{
  if (!src) return 0;
  SDL_Surface *surface = IMG_Load_IO(src, closeio);
  if (!surface) return (Texture)SDL_SetError("Failed to load texture data");
  PixelFormatTranslation translation;
  if (!TranslatePixelFormat(surface->format, &translation)) (SDL_DestroySurface(surface), 0);
  Texture texture = CreateTexture2D(levels, translation.internalFormat, surface->w, surface->h);
  if (!texture) return (SDL_DestroySurface(surface), 0);
  if (!SDL_LockSurface(surface)) return (SDL_DestroySurface(surface), glDeleteTextures(1, &texture), 0);
  glTextureSubImage2D(texture, 0, 0, 0, surface->w, surface->h, translation.format, translation.type, surface->pixels);
  glGenerateTextureMipmap(texture);
  SDL_UnlockSurface(surface);
  SDL_DestroySurface(surface);
  return texture;
}

bool TranslatePixelFormat(SDL_PixelFormat format, PixelFormatTranslation *translation)
{
  if (!translation) return SDL_SetError("Invalid parameter");
  switch (format)
  {
  case SDL_PIXELFORMAT_RGB332:
    translation->internalFormat = GL_R3_G3_B2;
    translation->format = GL_BGR;
    translation->type = GL_UNSIGNED_BYTE_3_3_2;
    break;
  case SDL_PIXELFORMAT_RGB565:
    translation->internalFormat = GL_RGB565;
    translation->format = GL_RGB;
    translation->type = GL_UNSIGNED_SHORT_5_6_5;
    break;
  case SDL_PIXELFORMAT_BGR565:
    translation->internalFormat = GL_RGB565;
    translation->format = GL_BGR;
    translation->type = GL_UNSIGNED_SHORT_5_6_5;
    break;
  case SDL_PIXELFORMAT_BGRA4444:
    translation->internalFormat = GL_RGBA4;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_SHORT_4_4_4_4;
    break;
  case SDL_PIXELFORMAT_RGBA4444:
    translation->internalFormat = GL_RGBA4;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_SHORT_4_4_4_4;
    break;
  case SDL_PIXELFORMAT_ABGR4444:
    translation->internalFormat = GL_RGBA4;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
    break;
  case SDL_PIXELFORMAT_ARGB4444:
    translation->internalFormat = GL_RGBA4;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
    break;
  case SDL_PIXELFORMAT_XBGR4444:
    translation->internalFormat = GL_RGB4;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
    break;
  case SDL_PIXELFORMAT_XRGB4444:
    translation->internalFormat = GL_RGB4;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_SHORT_4_4_4_4_REV;
    break;
  case SDL_PIXELFORMAT_BGRA5551:
    translation->internalFormat = GL_RGB5_A1;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_SHORT_5_5_5_1;
    break;
  case SDL_PIXELFORMAT_RGBA5551:
    translation->internalFormat = GL_RGB5_A1;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_SHORT_5_5_5_1;
    break;
  case SDL_PIXELFORMAT_ABGR1555:
    translation->internalFormat = GL_RGB5_A1;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
    break;
  case SDL_PIXELFORMAT_ARGB1555:
    translation->internalFormat = GL_RGB5_A1;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
    break;
  case SDL_PIXELFORMAT_XBGR1555:
    translation->internalFormat = GL_RGB5;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
    break;
  case SDL_PIXELFORMAT_XRGB1555:
    translation->internalFormat = GL_RGB5;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
    break;
  case SDL_PIXELFORMAT_RGB24:
    translation->internalFormat = GL_RGB8;
    translation->format = GL_RGB;
    translation->type = GL_UNSIGNED_BYTE;
    break;
  case SDL_PIXELFORMAT_BGR24:
    translation->internalFormat = GL_RGB8;
    translation->format = GL_BGR;
    translation->type = GL_UNSIGNED_BYTE;
    break;
  case SDL_PIXELFORMAT_BGRA8888:
    translation->internalFormat = GL_RGBA8;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_INT_8_8_8_8;
    break;
  case SDL_PIXELFORMAT_RGBA8888:
    translation->internalFormat = GL_RGBA8;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_INT_8_8_8_8;
    break;
  case SDL_PIXELFORMAT_ABGR8888:
    translation->internalFormat = GL_RGBA8;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_INT_8_8_8_8_REV;
    break;
  case SDL_PIXELFORMAT_ARGB8888:
    translation->internalFormat = GL_RGBA8;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_INT_8_8_8_8_REV;
    break;
  case SDL_PIXELFORMAT_BGRX8888:
    translation->internalFormat = GL_RGB8;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_INT_8_8_8_8;
    break;
  case SDL_PIXELFORMAT_RGBX8888:
    translation->internalFormat = GL_RGB8;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_INT_8_8_8_8;
    break;
  case SDL_PIXELFORMAT_XBGR8888:
    translation->internalFormat = GL_RGB8;
    translation->format = GL_RGBA;
    translation->type = GL_UNSIGNED_INT_8_8_8_8_REV;
    break;
  case SDL_PIXELFORMAT_XRGB8888:
    translation->internalFormat = GL_RGB8;
    translation->format = GL_BGRA;
    translation->type = GL_UNSIGNED_INT_8_8_8_8_REV;
    break;
  default:
    return SDL_SetError("Unsupported pixel format");
  }
  return true;
}
