#include "hal.h"
#include "device/video.h"
#include "device/palette.h"

#include <string.h>
#include <stdlib.h>

int get_fps();
static inline int min(int a, int b) {
	return a < b ? a : b;
}

void SDL_BlitSurface(SDL_Surface *src, SDL_Rect *srcrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
	int sx, sy, dx, dy, sw, dw, sh, dh; 

	if(srcrect!=NULL) 
	{
			sx = srcrect->x;
			sy = srcrect->y;
			sw = min(srcrect->w, src->w-sx);
			sh = min(srcrect->h, src->h-sy);
	} else {
				sx = 0;
				sy = 0;
				sw = src->w;
				sh = src->h;
			}

	if(dstrect != NULL) {
			dx = dstrect->x;
			dy = dstrect->y;
			dw = min(dstrect->w, dst->w - dx);
			dh = min(dstrect->h, dst->h - dy);
		} else {
				dx = 0;
				dy = 0;
				dw = dst->w;
				dh = dst->h;
			}

	int width = min(dw,sw);
	int height = min(dh,sh);

	int i,j;
	for(i = 0; i < height; i++)	{
			for(j = 0; j < width; j++)	{
						dst->pixels[(dy+i)*dst->w+dx+j] = src->pixels[(sy+i)*src->w+sx+j];
					}
		}
}

void SDL_FillRect(SDL_Surface *dst, SDL_Rect *dstrect, uint32_t color) {
	assert(dst);
	assert(color <= 0xff);
	int dstx = dstrect->x, dsty = dstrect->y, dstw = dstrect->w, dsth = dstrect->h;
	void * dststart = dst->pixels + dsty * dst->pitch + dstx;
	int j;
	for(j = 0; j < dsth; j++)
		memset(dststart + j * dst->pitch, color, dstw);

	/* TODO: Fill the rectangle area described by ``dstrect''
	 * in surface ``dst'' with color ``color''. If dstrect is
	 * NULL, fill the whole surface.
	 */
		Log("%s", __func__);
}

void SDL_UpdateRect(SDL_Surface *screen, int x, int y, int w, int h) {
	assert(screen);
	assert(screen->pitch == 320);
	if(screen->flags & SDL_HWSURFACE) {
		if(x == 0 && y == 0) {
			/* Draw FPS */
			vmem = VMEM_ADDR;
			char buf[80];
			sprintf(buf, "%dFPS", get_fps());
			draw_string(buf, 0, 0, 10);
		} else {
			vmem = VMEM_ADDR;
			int i, j;
			for(i = y; i < (y + h); i++) {
				for(j = x; j < (x + w); j++)
					vmem[i * screen->w + j] = screen->pixels[i * screen->w + j];
			}
		}
		return;
	}

}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, 
		int firstcolor, int ncolors) {
	assert(s);
	assert(s->format);
	assert(s->format->palette);
	assert(firstcolor == 0);

	if(s->format->palette->colors == NULL || s->format->palette->ncolors != ncolors) {
		if(s->format->palette->ncolors != ncolors && s->format->palette->colors != NULL) {
			/* If the size of the new palette is different 
			 * from the old one, free the old one.
			 */
			free(s->format->palette->colors);
		}

		/* Get new memory space to store the new palette. */
		s->format->palette->colors = malloc(sizeof(SDL_Color) * ncolors);
		assert(s->format->palette->colors);
	}

	/* Set the new palette. */
	s->format->palette->ncolors = ncolors;
	memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

	if(s->flags & SDL_HWSURFACE) {
		/* TODO: Set the VGA palette by calling write_palette(). */
		write_palette(colors, ncolors); 
	}
		Log("%s", __func__);
}

/* ======== The following functions are already implemented. ======== */

void SDL_SoftStretch(SDL_Surface *src, SDL_Rect *srcrect, 
		SDL_Surface *dst, SDL_Rect *dstrect) {
	assert(src && dst);
	int x = (srcrect == NULL ? 0 : srcrect->x);
	int y = (srcrect == NULL ? 0 : srcrect->y);
	int w = (srcrect == NULL ? src->w : srcrect->w);
	int h = (srcrect == NULL ? src->h : srcrect->h);

	assert(dstrect);
	if(w == dstrect->w && h == dstrect->h) {
		/* The source rectangle and the destination rectangle
		 * are of the same size. If that is the case, there
		 * is no need to stretch, just copy. */
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		SDL_BlitSurface(src, &rect, dst, dstrect);
	}
	else {
		/* No other case occurs in NEMU-PAL. */
		assert(0);
	}
}

SDL_Surface* SDL_CreateRGBSurface(uint32_t flags, int width, int height, int depth,
		uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask) {
	SDL_Surface *s = malloc(sizeof(SDL_Surface));
	assert(s);
	s->format = malloc(sizeof(SDL_PixelFormat));
	assert(s);
	s->format->palette = malloc(sizeof(SDL_Palette));
	assert(s->format->palette);
	s->format->palette->colors = NULL;

	s->format->BitsPerPixel = depth;

	s->flags = flags;
	s->w = width;
	s->h = height;
	s->pitch = (width * depth) >> 3;
	s->pixels = (flags & SDL_HWSURFACE ? (void *)VMEM_ADDR : malloc(s->pitch * height));
	assert(s->pixels);

	return s;
}

SDL_Surface* SDL_SetVideoMode(int width, int height, int bpp, uint32_t flags) {
	return SDL_CreateRGBSurface(flags,  width, height, bpp,
			0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

void SDL_FreeSurface(SDL_Surface *s) {
	if(s != NULL) {
		if(s->format != NULL) {
			if(s->format->palette != NULL) {
				if(s->format->palette->colors != NULL) {
					free(s->format->palette->colors);
				}
				free(s->format->palette);
			}

			free(s->format);
		}
		
		if(s->pixels != NULL && s->pixels != VMEM_ADDR) {
			free(s->pixels);
		}

		free(s);
	}
}

