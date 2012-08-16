/*
 * Copyright (c) 2012 ASMlover. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list ofconditions and the following disclaimer.
 *
 *    notice, this list of conditions and the following disclaimer in
 *  * Redistributions in binary form must reproduce the above copyright
 *    the documentation and/or other materialsprovided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define INITGUID
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <ddraw.h>
#include "../inc/common.h"
#include "../inc/wg_window.h"
#include "../inc/wg_draw.h"

#include "demo.h"


#define WND_CLASS_NAME        TEXT("WND32FREAKOUT")
#define WINDOW_WIDTH          (640)
#define WINDOW_HEIGHT         (480)

#define GAME_STATE_INIT       (0)
#define GAME_STATE_START      (1)
#define GAME_STATE_RUN        (2)
#define GAME_STATE_STOP       (3)
#define GAME_STATE_EXIT       (4)

/**! block defines */
#define NUM_BLOCK_ROWS        (6)
#define NUM_BLOCK_COLS        (8)
#define BLOCK_WIDTH           (64)
#define BLOCK_HEIGHT          (16)
#define BLOCK_ORIGIN_X        (8)
#define BLOCK_ORIGIN_Y        (8)
#define BLOCK_X_GAP           (80)
#define BLOCK_Y_GAP           (32)

/**! paddle defines */
#define PADDLE_START_X        (WG_SCREEN_WIDTH / 2 - 16)
#define PADDLE_START_Y        (WG_SCREEN_HEIGHT - 32)
#define PADDLE_WIDTH          (32)
#define PADDLE_HEIGHT         (8)
#define PADDLE_COLOR          (191)

/**! ball defines */
#define BALL_START_Y          (WG_SCREEN_HEIGHT / 2)
#define BALL_SIZE             (4)

struct Freakout {
  int gameState;    /**!< save the game state         */
  int paddleX;      /**!< tracks position of paddle   */
  int paddleY;
  int ballX;        /**!< tracks position of ball     */
  int ballY;
  int ballDX;       /**!< velocity of ball            */
  int ballDY;
  int score;        /**!< the score                   */
  int level;        /**!< the current level           */
  int blocksHit;    /**!< tracks number of blocks hit */

  HWND main_hwnd;
  BYTE blocks[NUM_BLOCK_ROWS][NUM_BLOCK_COLS];  /**!< this contains the game grid daat */
};


static void 
freakoutInit(struct Freakout* object) 
{
  memset(object, 0, sizeof(struct Freakout));
  object->gameState = GAME_STATE_INIT;
  object->level     = 1;
}

static void 
freakoutInitBlocks(struct Freakout* object)
{
  int row, col;

  for (row = 0; row < NUM_BLOCK_ROWS; ++row) {
    for (col = 0; col < NUM_BLOCK_COLS; ++col)
      object->blocks[row][col] = row * 16 + col * 3 + 16;
  }
}

static void 
freakoutDrawBlocks(struct Freakout* freakObject, void* drawObject)
{
  int row, col;
  int x1 = BLOCK_ORIGIN_X, y1 = BLOCK_ORIGIN_Y;

  for (row = 0; row < NUM_BLOCK_ROWS; ++row) {
    x1 = BLOCK_ORIGIN_X;
    for (col = 0; col < NUM_BLOCK_COLS; ++col) {
      if (0 != freakObject->blocks[row][col]) {
        wgDrawRectangle(drawObject, x1 - 4, y1 + 4, x1 + BLOCK_WIDTH - 4, y1 + BLOCK_HEIGHT + 4, 0);
        wgDrawRectangle(drawObject, x1, y1, x1 + BLOCK_WIDTH, y1 + BLOCK_HEIGHT, freakObject->blocks[row][col]);
      }
      x1 += BLOCK_X_GAP;
    }
    y1 += BLOCK_Y_GAP;
  }
}

static void 
freakoutProcessBall(struct Freakout* freakObject)
{
  int row, col;
  int x1 = BLOCK_ORIGIN_X, y1 = BLOCK_ORIGIN_Y;
  int ballCX = freakObject->ballX + (BALL_SIZE / 2), ballCY = freakObject->ballY + (BALL_SIZE / 2);

  if (freakObject->ballY > (WG_SCREEN_HEIGHT / 2) && freakObject->ballDY > 0) {
    int x = freakObject->ballX + (BALL_SIZE / 2);
    int y = freakObject->ballY + (BALL_SIZE / 2);

    if ((x >= freakObject->paddleX && x <= freakObject->paddleX + PADDLE_WIDTH)
      && (y >= freakObject->paddleY && y <= freakObject->paddleY + PADDLE_HEIGHT)) {
      freakObject->ballDY = -freakObject->ballDY;
      freakObject->ballY += freakObject->ballDY;

      if (WG_KEYDOWN(VK_RIGHT))
        freakObject->ballDX -= (rand() % 3);
      else if (WG_KEYDOWN(VK_LEFT))
        freakObject->ballDX += (rand() % 3);
      else 
        freakObject->ballDX += (-1 + rand() % 3);

      if (freakObject->blocksHit >= (NUM_BLOCK_ROWS * NUM_BLOCK_COLS)) {
        freakObject->gameState = GAME_STATE_START;
        ++freakObject->level;
      }
      MessageBeep(MB_OK);
      return;
    }
  }

  for (row = 0; row < NUM_BLOCK_ROWS; ++row) {
    x1 = BLOCK_ORIGIN_X;
    for (col = 0; col < NUM_BLOCK_COLS; ++col) {
      if (0 != freakObject->blocks[row][col]) {
        if ((ballCX > x1) && (ballCX < x1 + BLOCK_WIDTH) 
          && (ballCY > y1) && (ballCY < y1 + BLOCK_HEIGHT)) {
          freakObject->blocks[row][col] = 0;
          ++freakObject->blocksHit;
          freakObject->ballDY = -freakObject->ballDY;
          freakObject->ballDX += (-1 + rand() % 3);
          MessageBeep(MB_OK);

          freakObject->score += 5 * (freakObject->level + abs(freakObject->ballDX));
          return;
        }
      }
      x1 += BLOCK_X_GAP;
    }
    y1 += BLOCK_Y_GAP;
  }
}

static int 
gameInit(struct Freakout* freakObject, void* drawObject) 
{
  return Result_Success;
}

static int 
gameStop(struct Freakout* freakObject, void* drawObject)
{
  return Result_Success;
}

static int 
gameMain(struct Freakout* freakObject, void* drawObject, void* clockObject)
{
  TCHAR buffer[128];

  if (GAME_STATE_INIT == freakObject->gameState) {
    wgDrawInit(drawObject, WG_SCREEN_WIDTH, WG_SCREEN_HEIGHT, WG_SCREEN_BPP);

    srand(wgClockStart(clockObject));
    freakObject->paddleX = PADDLE_START_X;
    freakObject->paddleY = PADDLE_START_Y;
    freakObject->ballX  = 8 + rand() % (WG_SCREEN_WIDTH - 16);
    freakObject->ballY  = BALL_START_Y;
    freakObject->ballDX = -4 + rand() % (8 + 1);
    freakObject->ballDY = 6 + rand() % 2;

    freakObject->gameState = GAME_STATE_START;
  }
  else if (GAME_STATE_START == freakObject->gameState) {
    freakoutInitBlocks(freakObject);
    freakObject->blocksHit = 0;
    freakObject->gameState = GAME_STATE_RUN;
  }
  else if (GAME_STATE_RUN == freakObject->gameState) {
    wgClockStart(clockObject);
    wgDrawRectangle(drawObject, 0, 0, WG_SCREEN_WIDTH - 1, WG_SCREEN_HEIGHT - 1, 200);

    if (WG_KEYDOWN(VK_RIGHT)) {
      freakObject->paddleX += 8;
      if (freakObject->paddleX > (WG_SCREEN_WIDTH - PADDLE_WIDTH))
        freakObject->paddleX = (WG_SCREEN_WIDTH - PADDLE_WIDTH);
    }
    else if (WG_KEYDOWN(VK_LEFT)) {
      freakObject->paddleX -= 8;
      if (freakObject->paddleX < 0)
        freakObject->paddleX = 0;
    }

    freakoutDrawBlocks(freakObject, drawObject);

    freakObject->ballX += freakObject->ballDX;
    freakObject->ballY += freakObject->ballDY;

    if (freakObject->ballX > (WG_SCREEN_WIDTH - BALL_SIZE) || freakObject->ballX < 0) {
      freakObject->ballDX = -freakObject->ballDX;
      freakObject->ballX += freakObject->ballDX;
    }

    if (freakObject->ballY < 0) {
      freakObject->ballDY = -freakObject->ballDY;
      freakObject->ballY += freakObject->ballDY;
    }
    else if (freakObject->ballY > (WG_SCREEN_HEIGHT - BALL_SIZE)) {
      freakObject->ballDY = -freakObject->ballDY;
      freakObject->ballY += freakObject->ballDY;
      freakObject->score -= 100;
    }

    if (freakObject->ballDX > 8)
      freakObject->ballDX = 8;
    else if (freakObject->ballDX < -8)
      freakObject->ballDX = -8;

    freakoutProcessBall(freakObject);

    wgDrawRectangle(drawObject, freakObject->paddleX - 8, freakObject->paddleY + 8, 
      freakObject->paddleX + PADDLE_WIDTH - 8, freakObject->paddleY + PADDLE_HEIGHT + 8, 0);
    wgDrawRectangle(drawObject, freakObject->paddleX, freakObject->paddleY, 
      freakObject->paddleX + PADDLE_WIDTH, freakObject->paddleY + PADDLE_HEIGHT, PADDLE_COLOR);
    wgDrawRectangle(drawObject, freakObject->ballX - 4, freakObject->ballY + 4, 
      freakObject->ballX + BALL_SIZE - 4, freakObject->ballY + BALL_SIZE + 4, 0);
    wgDrawRectangle(drawObject, freakObject->ballX, freakObject->ballY, 
      freakObject->ballX + BALL_SIZE, freakObject->ballY + BALL_SIZE, 255);

    _stprintf(buffer, TEXT("F R E A K O U T       Score: %d       Level: %d"), freakObject->score, freakObject->level);
    wgDrawTextGdi(drawObject, 8, WG_SCREEN_HEIGHT - 16, buffer, 127);

    wgDrawFlip(drawObject);
    wgClockWait(clockObject, 30);

    if (WG_KEYDOWN(VK_ESCAPE)) {
      PostMessage(freakObject->main_hwnd, WM_DESTROY, 0, 0);
      freakObject->gameState = GAME_STATE_STOP;
    }
  }
  else if (GAME_STATE_STOP == freakObject->gameState)
    freakObject->gameState = GAME_STATE_EXIT;

  return Result_Success;
}


static LRESULT CALLBACK 
wndProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) 
{
  switch (msg) {
  case WM_CREATE:
    break;
  case WM_PAINT:
    {
      PAINTSTRUCT ps;
      HDC hDC = BeginPaint(h, &ps);

      EndPaint(h, &ps);
    } break;
  case WM_DESTROY:
    PostQuitMessage(0); 
    break;
  }

  return DefWindowProc(h, msg, wp, lp);
}


void 
demo_freakout(void* arg) 
{
  HINSTANCE hInst = GetModuleHandle(NULL);
  WNDCLASSEX wc = {0};
  HWND hWnd;
  MSG  msg;
  void* drawObject;
  void* clockObject;
  struct Freakout freakObject;

  UNUSED_PARAM(arg);
  fprintf(stdout, "Entry -> %s -> 0x%p\n", __FUNCSIG__, hInst);

  wc.cbSize         = sizeof(wc);
  wc.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
  wc.hInstance      = hInst;
  wc.lpfnWndProc    = wndProc;
  wc.lpszClassName  = WND_CLASS_NAME;
  wc.style          = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
  if (!RegisterClassEx(&wc))
    return;

  if (NULL == (hWnd = CreateWindowEx(0, WND_CLASS_NAME, WND_CLASS_NAME, 
    WS_POPUP | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
    GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInst, NULL)))
    return;

  ShowCursor(FALSE);
  freakoutInit(&freakObject);
  freakObject.main_hwnd = hWnd;
  drawObject  = wgDrawCreate(hWnd, hInst);
  clockObject = wgClockCreate();

  gameInit(&freakObject, drawObject);
  while (TRUE) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      if (WM_QUIT == msg.message)
        break;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    gameMain(&freakObject, drawObject, clockObject);
  }
  gameStop(&freakObject, drawObject);

  wgClockRelease(&clockObject);
  wgDrawRelease(&drawObject);

  ShowCursor(TRUE);
}
