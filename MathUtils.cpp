/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */

// Some math utils, useful for animation


#include "MathUtils.h"

#include <math.h>

///////////////////////////
#define PI 3.14159


float
gauss(float x)
{
	return exp(-0.5f * x * x) / sqrt(2 * PI); 	
}


inline uint32
frgbToUint32(float r, float g, float b)
{
	rgb color = {(uint8)(b*255.0f), (uint8)(g*255.0f), (uint8)(r*255.0f), 0};
	//printf("r%i g%i b%i\n", color.r, color.g, color.b);
	//printf("r%f g%f b%f\n", r, g, b);
	
	//return (uint32)(b * 256.0f) + (256 * (uint32)(g * 256.0f)) + (256 * 256 * (uint32)(r * 256.0f));
	return *(uint32*)&color;	
}


uint32
hsvToRGB(float hue, float saturation, float value)
{
	float f = hue / 60.0f - floor(hue / 60.0f);
	float p = value * (1 - saturation);
	float q = value * (1 - f * saturation);
	float t = value * (1 - (1 - f) * saturation);
	  
	int32 intHue = (int32)floor(hue / 60.0f) % 6;
	//printf("intHue %li\n", intHue);
	switch (intHue) {
		case (0):
			return frgbToUint32(value, t, p);
		case (1):
			return frgbToUint32(q, value, p);
		case (2):
			return frgbToUint32(p, value, t);	
		case (3):
			return frgbToUint32(p, q, value);
		case (4):
			return frgbToUint32(t, p, value);
		case (5):
			return frgbToUint32(value, p, q);
		default:
			return 0;	
	}  
}


uint32
cosinusHueToRGB(float angle)
{
	float rad1 = 2 * 3.14159 * angle / 360;
	float rad2 = 2 * 3.14159 * (angle + 120) / 360;
	float rad3 = 2 * 3.14159 * (angle + 240) / 360;
	float r = cos(rad1);
	float g = cos(rad2);
	float b = cos(rad3);
	
	return frgbToUint32(r > 0 ? r : 0, g > 0 ? g : 0, b > 0 ? b : 0);
	//return frgbToUint32(r *r, g * g, b * b);
	
}  




///////////////////////////


float
MathUtils::EaseInOutCubic(float time, float start, float distance,
	float duration)
{
	time /= duration  / 2.0;
	if (time < 1.0)
		return distance / 2.0 * time * time * time + start;
	time -= 2.0;
	return distance / 2.0 * (time * time * time + 2.0) + start;
}


float
MathUtils::EaseInOutQuart(float time, float start, float distance,
	float duration)
{
	time /= duration  / 2;

	if (time < 1)
		return distance / 2 * time * time * time * time + start;

	time -= 2;

	return -distance / 2 * (time * time * time * time - 2) + start;
}


float
MathUtils::EaseInOutQuint(float time, float start, float distance,
	float duration)
{
	time /= duration  / 2;
	if (time < 1)
		return distance / 2 * time * time * time * time * time + start;
	time -= 2;
	return distance / 2 *(time * time * time * time * time + 2) + start;
}


float
MathUtils::EaseInOutSine(float time, float start, float distance,
	float duration)
{
	return -distance / 2 * (cos(3.14159 * time / distance) - 1) + start;
}
