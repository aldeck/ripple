/*
 * Copyright 2009-2010, Alexandre Deckner. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _WAVE_RENDERER_H
#define _WAVE_RENDERER_H

#include "ClipRenderer.h"
#include "MathUtils.h"


class WaveRenderer : public ClipRenderer {
public:
					WaveRenderer(uint32 resolutionX, uint32 resolutionY);
	virtual			~WaveRenderer();

	virtual void	RenderClip(const int2d& leftTop, uint32* pixel, uint32 width, uint32 height,
						size_t bufferWidth);
						
	virtual void	FrameEvent(bigtime_t deltaTime);
	
protected:
	bigtime_t		fTime;
	float			fFloatTime;
	int64			fFrame;	
	
	void			_AddGaussBump(int32 x, int32 y, float widthFactor, float amplitude);
	void			_AddGaussBump_Brush(int32 x, int32 y, float amplitude);
	
	inline float&	_HeightMap(int32 x, int32 y);
	inline float&	_SpeedMap(int32 x, int32 y);
	
	int2d			fResolution;
	float*			fHeightMap;
	float*			fSpeedMap;
	uint32			fColorTable[360]; 
	float			fGaussBrush[128][128];
};

#endif /* _WAVE_RENDERER_H */
