#ifndef __WG_DRAW_HEADER_H__
#define __WG_DRAW_HEADER_H__

#define WG_SCREEN_WIDTH         (640)
#define WG_SCREEN_HEIGHT        (480)
#define WG_SCREEN_BPP           (8)     /**!< bits per pixel */
#define WG_MAX_COLORS           (256)   /**!< maximum colors */

#define WG_KEYDOWN(vk_code)     ((GetAsyncKeyState((vk_code)) & 0x8000) ? 1 : 0)
#define WG_KEYUP(vk_code)       ((GetAsyncKeyState((vk_code)) & 0x8000) ? 0 : 1)

/**! initializes a direct draw struct object */
#define WG_DDRAWSTRUCT_INIT(s)  { memset(&(s), 0, sizeof(s)); (s).dwSize = sizeof(s); }

/**! direct draw functions */
extern void* wgDrawCreate(HWND main_hwnd, HINSTANCE main_instance);
extern void wgDrawRelease(void** object);

extern int wgDrawInit(void* object, int width, int height, int bpp);
extern void wgDrawUninit(void* object);
extern LPDIRECTDRAWCLIPPER wgDrawAttachClipper(void* object, LPDIRECTDRAWSURFACE7 lpdds, int numRects, LPRECT clipList);
extern int wgDrawFlip(void* object);
extern int wgDrawFillSurface(void* object, LPDIRECTDRAWSURFACE7 lpdds, int color);
extern int wgDrawRectangle(void* object, int left, int top, int right, int bottom, int color);
extern int wgDrawTextGdi(void* object, int x, int y, LPCTSTR text, int color);


extern void* wgClockCreate(void);
extern void wgClockRelease(void** object);
extern DWORD wgClockStart(void* object);
extern DWORD wgClockGet(void* object);
extern DWORD wgClockWait(void* object, DWORD count);

#endif  /* __WG_DRAW_HEADER_H__ */
