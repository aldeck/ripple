/*
 * Copyright 2009, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */
#ifndef _MATH_UTILS_H
#define _MATH_UTILS_H

#include <SupportDefs.h>


class MathUtils
{
public:
	static	float	EaseInOutCubic(float time, float begin, float distance,
						float duration);
	static	float	EaseInOutQuart(float time, float begin, float distance,
						float duration);
	static	float	EaseInOutQuint(float time, float begin, float distance,
						float duration);
	static	float	EaseInOutSine(float time, float begin, float distance,
						float duration);
};

///////////////////////////
struct int2d {
	int x;
	int y;	
};


struct rgb {
	uint8 b;
	uint8 g;
	uint8 r;
	uint8 a;		
};

float gauss(float x);

inline uint32 frgbToUint32(float r, float g, float b);

uint32 hsvToRGB(float hue, float saturation, float value);


uint32 cosinusHueToRGB(float angle);



///////////////////////////

#endif /* _MATH_UTILS_H */
