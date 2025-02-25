/*
 ==============================================================================

 This file is part of the iPlug 2 library. Copyright (C) the iPlug 2 developers.

 See LICENSE.txt for  more info.

 ==============================================================================
*/

#import <UIKit/UIKit.h>
#include "IGraphicsIOS.h"

inline CGRect ToCGRect(IGraphics* pGraphics, const IRECT& bounds)
{
  float scale = pGraphics->GetDrawScale();
  float x = floor(bounds.L * scale);
  float y = floor(bounds.T * scale);
  float x2 = ceil(bounds.R * scale);
  float y2 = ceil(bounds.B * scale);
  
  return CGRectMake(x, y, x2 - x, y2 - y);
}

inline UIColor* ToUIColor(const IColor& c)
{
  return [UIColor colorWithRed:(double) c.R / 255.0 green:(double) c.G / 255.0 blue:(double) c.B / 255.0 alpha:(double) c.A / 255.0];
}

inline IColor FromUIColor(const UIColor* c)
{
  CGFloat r,g,b,a;
  [c getRed:&r green:&g blue:&b alpha:&a];
  return IColor(a * 255., r * 255., g * 255., b * 255.);
}

@interface IGraphicsIOS_View : UIScrollView <UITextFieldDelegate, UIScrollViewDelegate>
{  
@public
  IGraphicsIOS* mGraphics;
  UITextField* mTextField;
  int mTextFieldLength;
}
- (id) initWithIGraphics: (IGraphicsIOS*) pGraphics;
- (BOOL) isOpaque;
- (BOOL) acceptsFirstResponder;
- (BOOL) delaysContentTouches;
- (void) removeFromSuperview;
- (IPopupMenu*) createPopupMenu: (const IPopupMenu&) menu : (CGRect) bounds;
- (void) createTextEntry: (int) paramIdx : (const IText&) text : (const char*) str : (int) length : (CGRect) areaRect;
- (void) endUserInput;
- (void) showMessageBox: (const char*) str : (const char*) caption : (EMsgBoxType) type : (IMsgBoxCompletionHanderFunc) completionHandler;
- (void) getTouchXY: (CGPoint) pt x: (float*) pX y: (float*) pY;
@property (readonly) CAMetalLayer* metalLayer;
@property (nonatomic, strong) CADisplayLink *displayLink;

@end

#ifdef IGRAPHICS_IMGUI
#import <MetalKit/MetalKit.h>

@interface IGRAPHICS_IMGUIVIEW : MTKView
{
  IGraphicsIOS_View* mView;
}
@property (nonatomic, strong) id <MTLCommandQueue> commandQueue;
- (id) initWithIGraphicsView: (IGraphicsIOS_View*) pView;
@end
#endif
