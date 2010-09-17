/*
 * Copyright 2009-2010, Alexandre Deckner. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "TestRenderer.h"

#include "MathUtils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <OS.h> //pour system time


TestRenderer::TestRenderer(uint32 resolutionX, uint32 resolutionY)
	:
	fTime(0)
{	
	printf("TestRenderer::TestRenderer w%lu, h%lu\n", resolutionX, resolutionY);
}


TestRenderer::~TestRenderer()
{	
}


void
TestRenderer::FrameEvent(bigtime_t deltaTime)
{	
	fTime += deltaTime;
}


void
TestRenderer::RenderClip(const int2d& leftTop, uint32* pixel, uint32 width, uint32 height, size_t bufferWidth)
{
	uint32 r = 0;
	uint32 g = 0;
	uint32 b = 0;

	float floatTime = (float)fTime / 1000000.0;
	//uint32 value = (uint32)floatTime;
	//uint32 value = hsvToRGB(360 * floatTime, 1.0f, 1.0f);
	uint32 value = cosinusHueToRGB(360 * floatTime); 
	
	
	uint32 x = 0;
	uint32 y = 0;
	while (y < height) {			               	  
		while (x < width) {
			//value++;	               	  
			//*pixel = value;
			
			r++;
			if (r >= 255) {
				r = 0;
				g++;
				if (g >= 255) {
					g = 0;
					b++;
					if (b >= 255)
						b = 0;
				}
			}
			*pixel = b + g * 256 + r * 256 * 256;
			
			 
			x++;
			pixel++;
		}
		y++;
		pixel += bufferWidth - width; //return, line feed
		x = 0;
	}	
}
