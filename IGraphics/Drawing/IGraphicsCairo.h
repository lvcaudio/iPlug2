/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#pragma once

#include "IPlugPlatform.h"

#ifdef OS_MAC
  #include "cairo/cairo.h"
  #define __QUICKDRAW__
  #define __HISERVICES__
  #include "cairo/cairo-quartz.h"
#elif defined OS_WIN
  #define CAIRO_WIN32_STATIC_BUILD

  #pragma comment(lib, "cairo.lib")
  #pragma comment(lib, "pixman.lib")
  #pragma comment(lib, "freetype.lib")
  #pragma comment(lib, "libpng.lib")
  #pragma comment(lib, "zlib.lib")

  #include "cairo/src/cairo.h"
  #include "cairo/src/cairo-win32.h"
#else
  #error NOT IMPLEMENTED
#endif

#include "IGraphicsPathBase.h"

/** A Cairo API bitmap
 * @ingroup APIBitmaps */
class CairoBitmap : public APIBitmap
{
public:
  CairoBitmap(cairo_surface_t* pSurface, int scale, float drawScale);
  CairoBitmap(cairo_surface_t* pSurfaceType, int width, int height, int scale, float drawScale);
  virtual ~CairoBitmap();
};

/** IGraphics draw class using Cairo
*   @ingroup DrawClasses */
class IGraphicsCairo : public IGraphicsPathBase
{
public:
  const char* GetDrawingAPIStr() override { return "CAIRO"; }

  IGraphicsCairo(IGEditorDelegate& dlg, int w, int h, int fps, float scale);
  ~IGraphicsCairo();

  void DrawBitmap(const IBitmap& bitmap, const IRECT& dest, int srcX, int srcY, const IBlend* pBlend) override;
      
  void PathClear() override;
  void PathClose() override;
  void PathArc(float cx, float cy, float r, float a1, float a2, EWinding winding) override;
  void PathMoveTo(float x, float y) override;
  void PathLineTo(float x, float y) override;
  void PathCubicBezierTo(float c1x, float c1y, float c2x, float c2y, float x2, float y2) override;
  void PathQuadraticBezierTo(float cx, float cy, float x2, float y2) override;
  void PathStroke(const IPattern& pattern, float thickness, const IStrokeOptions& options, const IBlend* pBlend) override;
  void PathFill(const IPattern& pattern, const IFillOptions& options, const IBlend* pBlend) override;
  
  IColor GetPoint(int x, int y) override;
  void* GetDrawContext() override { return (void*) mContext; }
    
  void EndFrame() override;
  void SetPlatformContext(void* pContext) override;
  void DrawResize() override;

  bool BitmapExtSupported(const char* ext) override;

protected:
  APIBitmap* LoadAPIBitmap(const char* fileNameOrResID, int scale, EResourceLocation location, const char* ext) override;
  APIBitmap* CreateAPIBitmap(int width, int height, int scale, double drawScale) override;

  bool LoadAPIFont(const char* fontID, const PlatformFontPtr& font) override;
    
  int AlphaChannel() const override { return 3; }
  bool FlippedBitmap() const override { return false; }

  void GetLayerBitmapData(const ILayerPtr& layer, RawBitmapData& data) override;
  void ApplyShadowMask(ILayerPtr& layer, RawBitmapData& mask, const IShadow& shadow) override;
    
  void DoMeasureText(const IText& text, const char* str, IRECT& bounds) const override;
  void DoDrawText(const IText& text, const char* str, const IRECT& bounds, const IBlend* pBlend) override;

  void SetCairoSourcePattern(cairo_t* context, const IPattern& pattern, const IBlend* pBlend);
  
private:
    
  void PrepareAndMeasureText(const IText& text, const char* str, IRECT& r, double& x, double & y, cairo_glyph_t*& pGlyphs, int& numGlyphs) const;
    
  void PathTransformSetMatrix(const IMatrix& m) override;
  void SetClipRegion(const IRECT& r) override;
  
  void UpdateCairoContext();
  void UpdateCairoMainSurface(cairo_surface_t* pSurface);

  void UpdateLayer() override { UpdateCairoContext(); }
    
  cairo_surface_t* CreateCairoDataSurface(const APIBitmap* pBitmap, RawBitmapData& data, bool resize);
    
  cairo_t* mContext;
  cairo_surface_t* mSurface;
};
