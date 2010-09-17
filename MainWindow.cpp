/*
 * Copyright 2009, Alexandre Deckner. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "MainWindow.h"
#include <View.h>
#include <Application.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "TestRenderer.h"
#include "WaveRenderer.h"

#include "MathUtils.h" //pour int2d dans rendercliip


MainWindow::MainWindow(BRect frame, const char* title)
	:
	BDirectWindow(frame, title, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE, 0)
{
	fClipRenderer = new WaveRenderer((uint32)frame.Width(), (uint32)frame.Height());

	
	fConnected = false;
	fConnectionDisabled = false;
	fLocker = new BLocker();
	fClipList = NULL;
	fNumClipRects = 0;
	
	//we need to tell the application server not to draw anything
	//BView* view = new BView(Bounds(), "auxView", B_FOLLOW_ALL, B_WILL_DRAW);
	//view->SetViewColor(B_TRANSPARENT_32_BIT);
	//AddChild(view);
	
	if (!SupportsWindowMode()) {
	   SetFullScreen(true);
	}
	
	fDirty = true;

	if (_CreateRenderThread() != B_OK)
		printf("Error trying to start the render thread!\n");
	//else
	//	Show();
}


MainWindow::~MainWindow()
{
	fConnectionDisabled = true;      // Connection is dying
	Hide();
	Sync();
	
	if (fRenderThread >= 0)
		wait_for_thread(fRenderThread, NULL);

	delete fClipRenderer;

	free(fClipList);
	delete fLocker;
}


uint32
MainWindow::_CreateRenderThread()
{
	fRenderThread = spawn_thread(MainWindow::_RenderThreadEntry,
		"renderThread",	B_NORMAL_PRIORITY, this);

	if (fRenderThread < 0)
		return fRenderThread;

	return resume_thread(fRenderThread);
}


/*void
MainWindow::_StopRenderThread()
{
	LockGL();
	fStopRendering = true;
	UnlockGL();

	if (fRenderThread >= 0)
		wait_for_thread(fRenderThread, NULL);
}*/


bool
MainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
MainWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		default:
			BDirectWindow::MessageReceived(message);
	}
}


void
MainWindow::DirectConnected(direct_buffer_info* info)
{	
   if (!fConnected && fConnectionDisabled) {
      return;
   }
   
   fLocker->Lock();

   switch(info->buffer_state & B_DIRECT_MODE_MASK) {
      case B_DIRECT_START:
         fConnected = true;

      case B_DIRECT_MODIFY:
         // Get clipping information

         if (fClipList) {
            free(fClipList);
            fClipList = NULL;
         }

         fNumClipRects = info->clip_list_count;
         fClipList = (clipping_rect *)
               malloc(fNumClipRects*sizeof(clipping_rect));

         if (fClipList) {
            memcpy(fClipList, info->clip_list,
                  fNumClipRects*sizeof(clipping_rect));
            fBits = (uint8 *) info->bits;
            fRowBytes = info->bytes_per_row;
            fFormat = info->pixel_format;
            fBounds = info->window_bounds;
            fDirty = true;

         }
         break;

      case B_DIRECT_STOP:
         fConnected = false;
         break;
   }
   fLocker->Unlock();
}


int32
MainWindow::_RenderThreadEntry(void* pointer)
{
	return reinterpret_cast<MainWindow*>(pointer)->_RenderLoop();
}


int32
MainWindow::_RenderLoop()
{
	bigtime_t lastFrameTime = system_time();
	while (!fConnectionDisabled) {		
		fLocker->Lock();		
		if (fConnected) {
			if (fDirty) {				
				if (fFormat == B_RGB32) {				
					uint32* bufferStart = (uint32*)fBits;
					uint32  bufferWidth = fRowBytes / sizeof(uint32);
				
					for (uint32 i = 0; i < fNumClipRects; i++) {
						clipping_rect* clip = &fClipList[i];
						
						BPoint wLeftTop(this->Frame().LeftTop());
						int2d leftTop;
						leftTop.x = clip->left - wLeftTop.x;
						leftTop.y = clip->top - wLeftTop.y;
						fClipRenderer->RenderClip(leftTop,
							bufferStart + clip->left + clip->top * bufferWidth,
							clip->right - clip->left + 1,
							clip->bottom - clip->top + 1,
							bufferWidth);
					}
				}
				
				bigtime_t time = system_time();
				fClipRenderer->FrameEvent(time - lastFrameTime);
				lastFrameTime = time;
			}
			fDirty = true; //true == loop
		}
		fLocker->Unlock();
		// Use BWindow or BView APIs here if you want
		snooze(10000);
	}
	return B_OK;
}
