#ifndef _IGRAPHICSMAC_
#define _IGRAPHICSMAC_

#if defined(__APPLE__) && defined(__LP64__) && !defined(IPLUG_NO_CARBON_SUPPORT)
  #define IPLUG_NO_CARBON_SUPPORT
#endif

// carbon support uses quickdraw methods that have been removed in SDKs > 10.6
#if __MAC_OS_X_VERSION_MAX_ALLOWED > 1060
  #warning Carbon GUIs work best with the 10.6 sdk
#endif

#include "IGraphics.h"
#include "../swell/swell.h"

#ifdef VST3_API
  #define _UINT32 // this is necessary to get VST3 to compile with the 10.6 SDK due to a conflict
#endif

#include <Carbon/Carbon.h>

#ifndef DEFAULT_PATH_OSX
  #define DEFAULT_PATH_OSX "~/Desktop"
#endif

#ifndef IPLUG_NO_CARBON_SUPPORT
  class IGraphicsCarbon;
#endif

#ifndef COCOA_PREFIX
  #define COCOA_PREFIX_AUTO(a) a##__COUNTER__
  #define COCOA_PREFIX COCOA_PREFIX_AUTO(CocoaPre)
#endif

#if defined(VST_API)
  #define API _vst
#elif defined(AU_API)
  #define API _au
#elif defined(RTAS_API)
  #define API _rtas
#elif defined(AAX_API)
  #define API _aax
#elif defined(VST3_API)
  #define API _vst3
#elif defined(SA_API)
#define API _sa
#endif

#define CONCAT3(a,b,c) a##b##c
#define CONCAT2(a,b,c) CONCAT3(a,b,c)
#define CONCAT(cname) CONCAT2(cname,COCOA_PREFIX,API)

#define IGRAPHICS_COCOA CONCAT(IGraphicsCocoa_)
#define IGRAPHICS_NSMENU CONCAT(IGraphicsNSMenu_)
#define IGRAPHICS_MENU_RCVR CONCAT(IGraphicsMenuRcvr_)
#define CUSTOM_COCOA_WINDOW CONCAT(CustomCocoaWindow_)
#define COCOA_FORMATTER CONCAT(CocoaFormatter_)
/// Concrete IGraphics implementation for Mac OS X
class IGraphicsMac : public IGraphics
{
public:
  IGraphicsMac(IPlugBase* pPlug, int w, int h, int refreshFPS);
  virtual ~IGraphicsMac();

  void SetBundleID(const char* bundleID) { mBundleID.Set(bundleID); }

  bool DrawScreen(IRECT* pR);
  bool MeasureIText(IText* pTxt, const char* str, IRECT* pR);

  void* OpenWindow(void* pWindow);
#ifndef IPLUG_NO_CARBON_SUPPORT
  void* OpenWindow(void* pWindow, void* pControl, short leftOffset = 0, short topOffset = 0);
#endif

  void* OpenCocoaWindow(void* pParentView);
#ifndef IPLUG_NO_CARBON_SUPPORT
  void* OpenCarbonWindow(void* pParentWnd, void* pParentControl, short leftOffset, short topOffset);
#endif

  void AttachSubWindow(void* hostWindowRef);
  void RemoveSubWindow();

  void CloseWindow();
  bool WindowIsOpen();
  void Resize(int w, int h);

  void HideMouseCursor();
  void ShowMouseCursor();

  int ShowMessageBox(const char* pText, const char* pCaption, int type);
  void ForceEndUserEdit();

  const char* GetGUIAPI();

  void UpdateTooltips();

  void HostPath(WDL_String* pPath);
  void PluginPath(WDL_String* pPath);
  void DesktopPath(WDL_String* pPath);
  void AppSupportPath(WDL_String* pPath);

  void PromptForFile(WDL_String* pFilename, EFileAction action = kFileOpen, WDL_String* pDir = 0, const char* extensions = "");   // extensions = "txt wav" for example.
  bool PromptForColor(IColor* pColor, const char* prompt = "");

  IPopupMenu* CreateIPopupMenu(IPopupMenu* pMenu, IRECT* pTextRect);
  void CreateTextEntry(IControl* pControl, IText* pText, IRECT* pTextRect, const char* pString, IParam* pParam );

  bool OpenURL(const char* url, const char* msgWindowTitle = 0, const char* confirmMsg = 0, const char* errMsgOnFailure = 0);

  void* GetWindow();

  const char* GetBundleID()  { return mBundleID.Get(); }
  static int GetUserOSVersion();   // Returns a number like 0x1050 (10.5).

protected:
  virtual LICE_IBitmap* OSLoadBitmap(int ID, const char* name);
  
private:
#ifndef IPLUG_NO_CARBON_SUPPORT
  IGraphicsCarbon* mGraphicsCarbon;
#endif
  void* mGraphicsCocoa;   // Can't forward-declare IGraphicsCocoa because it's an obj-C object.

  WDL_String mBundleID;
  
  friend int GetMouseOver(IGraphicsMac* pGraphics);
  
#ifndef IPLUG_NO_CARBON_SUPPORT
  friend class IGraphicsCarbon;
#endif
  
public: //TODO: make this private
  void* mHostNSWindow;
};

inline CFStringRef MakeCFString(const char* cStr)
{
  return CFStringCreateWithCString(0, cStr, kCFStringEncodingUTF8);
}

/// UTF8 char* to CFStringRef string converter structure
struct CFStrLocal
{
  CFStringRef mCFStr;
  CFStrLocal(const char* cStr)
  {
    mCFStr = MakeCFString(cStr);
  }
  ~CFStrLocal()
  {
    CFRelease(mCFStr);
  }
};

/// CFStringRef to UTF8 char* string converter structure
struct CStrLocal
{
  char* mCStr;
  CStrLocal(CFStringRef cfStr)
  {
    int n = CFStringGetLength(cfStr) + 1;
    mCStr = (char*) malloc(n);
    CFStringGetCString(cfStr, mCStr, n, kCFStringEncodingUTF8);
  }
  ~CStrLocal()
  {
    FREE_NULL(mCStr);
  }
};

inline CGRect ToCGRect(int h, IRECT* pR)
{
  int B = h - pR->B;
  return CGRectMake(pR->L, B, pR->W(), B + pR->H());
}

inline int AdjustFontSize(int size)
{
  return int(0.9 * (double)size);
}

#endif
