/*
 * Copyright 2009, Alexandre Deckner. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <DirectWindow.h>
#include <Locker.h>


class ClipRenderer;


class MainWindow : public BDirectWindow {
public:
					MainWindow(BRect frame, const char* title);
					~MainWindow();

	virtual bool	QuitRequested();
	virtual void	MessageReceived(BMessage* message);
	virtual void	DirectConnected(direct_buffer_info* info);

protected:
	uint32			_CreateRenderThread();
	//void			_StopRenderThread();
	static int32	_RenderThreadEntry(void* pointer);
	int32			_RenderLoop();
	
	ClipRenderer*	fClipRenderer;
	
	uint8*			fBits;
	int32			fRowBytes;
	color_space		fFormat;
	clipping_rect	fBounds;

	uint32			fNumClipRects;
	clipping_rect*	fClipList;

	bool			fDirty;
	bool			fConnected;
	bool			fConnectionDisabled;
	BLocker*		fLocker;
	thread_id		fRenderThread;
};

#endif /* _MAINWINDOW_H */
