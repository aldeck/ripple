/*
 * Copyright 2009-10, Alexandre Deckner. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _CLIP_RENDERER_H
#define _CLIP_RENDERER_H

#include <SupportDefs.h>
#include "MathUtils.h"


class ClipRenderer {
public:
//	TODO: virtual destructor in a pure interface class?
	virtual void	RenderClip(const int2d& leftTop, uint32* pixel, uint32 width, uint32 height,
						size_t bufferWidth) = 0;
						
	virtual void	FrameEvent(bigtime_t deltaTime) = 0;
};

#endif /* _CLIP_RENDERER_H */
