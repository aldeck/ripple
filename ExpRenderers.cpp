/*
 * Copyright 2009, Alexandre Deckner. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 * 		Alexandre Deckner <alex@zappotek.com>
 */


//note on pourrait tout dessiner dans une boucle en "levant" le crayon hors des clipping rects
//par exemple en calculant l'inverse des clipping rects
//mais bon ca rajoute des if a l'insterieur du rendu.. donc moins performant
//mais peut etre plus sympa pour un debutant


void
_RenderClippingRect_rgb(uint32* pixel, uint32 width, uint32 height, size_t bufferWidth)
{
	uint32 r = 0;
	uint32 g = 0;
	uint32 b = 0;
	//uint32 value = 0;
	
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
			*pixel = b + g * 256 + r * 256 * 256 ;  
			x++;
			pixel++;
		}
		y++;
		pixel += bufferWidth - width; //return, line feed
		x = 0;
	}	
}


void
_RenderClippingRect(uint32* pixel, uint32 width, uint32 height, size_t bufferWidth)
{
	uint32 value = 0;
	
	uint32 x = 0;
	uint32 y = 0;
	while (y < height) {			               	  
		while (x < width) {
			value += 16;	               	  
			*pixel = value;			
			x++;
			pixel++;
		}
		y++;
		pixel += bufferWidth - width; //return, line feed
		x = 0;
	}	
}

// line fill
//while (y < height) {
//	//uint32 value = y;
//	value++;	               	  
//   memset(pixel, value, width * sizeof(pixel)); //memset sets bytes
//   y++;
//   pixel += bufferWidth;
//}
