/*
 * Copyright 2009, Alexandre Deckner. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

#include "WaveRenderer.h"

#include "MathUtils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <OS.h> //pour system time


WaveRenderer::WaveRenderer(uint32 resolutionX, uint32 resolutionY)
	:
	fTime(0),
	fFloatTime(0),
	fFrame(0)
	
{	
	printf("WaveRenderer::WaveRenderer w%lu, h%lu\n", resolutionX, resolutionY);
	fResolution.x = resolutionX;
	fResolution.y = resolutionY;
	
	fHeightMap = new float[fResolution.x * fResolution.y];
	fSpeedMap = new float[fResolution.x * fResolution.y];

	// initialize the maps
	for(int y = 0; y < fResolution.y; y++){
		for(int x = 0; x < fResolution.x; x++){
			_HeightMap(x, y) = 0;
			_SpeedMap(x, y) = 0;
		}
	}	
	
	// init color table
	for (int i = 0; i < 360; i++) {
		fColorTable[i] = hsvToRGB((float)i, 1.0f, 1.0f);	
	}
	
	// init gauss brush
	int size = 128;
	int centerX = size / 2;
	int centerY = size / 2;
	for(int x = 0; x < size; x++){
		for(int y = 0; y < size; y++){
			float dX = x - centerX;
			float dY = y - centerY;
			float dist = 0.1 * sqrt(dX * dX + dY * dY); 
			//printf("TestRenderer::_AddGaussBump dist %f\n", dist);
			fGaussBrush[x][y] += 2.0 * gauss(dist);
		}
	}
	
	// put a few bumps
	srand(system_time());	
	for (int i = 0; i < 3; i++) {
		int a = 48;
		int bx = fResolution.x - 2 * 48;
		int by = fResolution.y - 2 * 48;
		int cx = a + (int)((double)rand() / ((double)RAND_MAX + 1) * bx);		
		int cy = a + (int)((double)rand() / ((double)RAND_MAX + 1) * by);
		float widthFactor = 0.001 + ((double)rand() / ((double)RAND_MAX + 1) * 0.1);
		_AddGaussBump(cx, cy, widthFactor, 3.0);
	}	
}


WaveRenderer::~WaveRenderer()
{	
	delete [] fHeightMap;
	delete [] fSpeedMap;
}


inline float&
WaveRenderer::_HeightMap(int32 x, int32 y)
{
	return fHeightMap[x + y * fResolution.x];
}


inline float&
WaveRenderer::_SpeedMap(int32 x, int32 y)
{
	return fSpeedMap[x + y * fResolution.x];
}


void			
WaveRenderer::_AddGaussBump(int32 centerX, int32 centerY, float widthFactor, float amplitude)
{
	//printf("TestRenderer::_AddGaussBump cx %li cy %li\n", centerX, centerY);	
	for(int x = 0; x < fResolution.x; x++){
		for(int y = 0; y < fResolution.y; y++){
			float dX = x - centerX;
			float dY = y - centerY;
			float dist = widthFactor * sqrt(dX * dX + dY * dY); 
			//printf("TestRenderer::_AddGaussBump dist %f\n", dist);
			_HeightMap(x, y) += amplitude * gauss(dist);
		}
	}
}


// c'est un set gauss bump
void			
WaveRenderer::_AddGaussBump_Brush(int32 centerX, int32 centerY, float amplitude)
{
	//printf("TestRenderer::_AddGaussBump cx %li cy %li\n", centerX, centerY);	
	int size = 128;
	for(int x = 0; x < size; x++){
		for(int y = 0; y < size; y++){
			//float currentVal = _SpeedMap(centerX - size / 2 + x, centerY - size / 2 + y);
			_SpeedMap(centerX - size / 2 + x, centerY - size / 2 + y) += amplitude * fGaussBrush[x][y]; 
		}
	}
}


void
WaveRenderer::FrameEvent(bigtime_t deltaTime)
{
	//_AddGaussBump_Brush(200, 200, GenericAudioFilterNode::fMarcel * 0.1);
	
	//return;
	
	fTime += deltaTime;	
	//if (fTime >= 10000000)
	//	fTime = 0;
	fFloatTime = (float)fTime / 10000000.0;
	fFrame ++;
	
	
	
	float k = 1.0;
	float f = 0.999;
	
	float rac = 0.36;
	float ntermes = 4 + 4 * rac;

	for(int y = 1; y < fResolution.y - 1; y++){
		for(int x = 1; x < fResolution.x - 1; x++){
		float a
			= _HeightMap(x + 1, y) + _HeightMap(x, y + 1)
			+ _HeightMap(x - 1, y) + _HeightMap(x, y - 1);
		
		float b
			= (_HeightMap(x + 1, y + 1) + _HeightMap(x - 1, y + 1)
			+ _HeightMap(x - 1, y - 1) + _HeightMap(x + 1, y - 1)) * rac;
		
		a = k * ((a + b) / ntermes - _HeightMap(x, y));
			
		_SpeedMap(x, y) = f * _SpeedMap(x, y) + a;
		
		/*a=h[x+1][y]+h[x][y+1]+h[x-1][y]+h[x][y-1]; //works
		b=h[x+1][y+1]+h[x-1][y+1]+h[x-1][y-1]+h[x+1][y-1];
		b*=rac;
		a=((a+b)/ntermes)-h[x][y];
		a*=k[x][y];
		c=0;
		b=v[x][y]*f[x][y];
		v[x][y]=b+a+c;*/
		}
	}
	
	//float period = 256.0f; //256 frames
	//float factor = 2 * 3.14159 * (float)fFrame / period;
	//_AddGaussBump_Brush(256 + 64, 128 + 64, cos(factor) * 0.0005);
	
		
		
		
	//calcul des nouvelles positions
	//par rapport aux nouvelles vitesses
	for(int y = 0; y < fResolution.y; y++){	
		for(int x = 0; x < fResolution.x; x++){
			_HeightMap(x, y) += _SpeedMap(x, y);			
		}
	}
	
	//_AddGaussBump_Brush(200, 200, GenericAudioFilterNode::fMarcel * 0.1);	
}




void
WaveRenderer::RenderClip(const int2d& leftTop, uint32* pixel, uint32 width, uint32 height, size_t bufferWidth)
{
	/*uint32 r = 0;
	uint32 g = 0;
	uint32 b = 0;*/
	//uint32 value = (uint32)(fTime / 10000);
	//uint32 value = hsvToRGB(360 * fFloatTime, 1.0f, 1.0f);
	//uint32 value = cosinusHueToRGB(360 * fFloatTime); 
	
	
	uint32 x = 0;
	uint32 y = 0;
	while (y < height) {			               	  
		while (x < width) {
			//value++;	               	  
			//*pixel = value;
			
			/*r++;
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
			*pixel = b + g * 256 + r * 256 * 256 ; */
			
			*pixel = fColorTable[(int)(360.0f + 240.0f + 360.0f * 2.0 * _HeightMap(leftTop.x + x, leftTop.y + y)) % 360];//hsvToRGB(180.0f + 360.0f * h[y][x], 1.0, 1.0);
			 
			x++;
			pixel++;
		}
		y++;
		pixel += bufferWidth - width; //return, line feed
		x = 0;
	}	
}
