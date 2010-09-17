/*
 * Copyright 2009, Alexandre Deckner. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _TEST_RENDERER_H
#define _TEST_RENDERER_H

#include "ClipRenderer.h"
#include "MathUtils.h"


class TestRenderer : public ClipRenderer {
public:
					TestRenderer(uint32 resolutionX, uint32 resolutionY);
	virtual			~TestRenderer();

	virtual void	RenderClip(const int2d& leftTop, uint32* pixel, uint32 width, uint32 height,
						size_t bufferWidth);
						
	virtual void	FrameEvent(bigtime_t deltaTime);
	
protected:
	bigtime_t		fTime;
};

#endif /* _TEST_RENDERER_H */
