/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * UnlessSURFACEquired by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <android_native_app_glue.h>

#include <errno.h>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "evaluator.h"
#include "libvisual.h"
#include "lv_video.h"

#define  LOG_TAG    "starvisuals"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/* Set to 1 to enable debug log traces. */
#define DEBUG 0

/* Set to 1 to optimize memory stores when generating plasma. */
#define OPTIMIZE_WRITES  1


static __inline int makeint(double t)
{
  if (t <= 0.0) return 0;
  if (t >= 1.0) return 255;
  return (int)(t*255.0);
}

char init[] = "n=32;t=0;";
char beat[] = "n=n+5";
char frame[] = "t = t - 5;";
char point[] = ""\
"d=i+v*0.02;"\
"r=t+i*PI*20;"\ 
"x=cos(r)*d*0.8;"\
"y=sin(r)*d*0.8";


typedef struct {
    double n, b, x, y, i, v, w, h, red, green, blue, linesize, skip, drawmode, t, d; 

    int needs_init;
    int color_pos;
    int channel_source;
    int blendmode;
    int isBeat;

    VisPalette pal;
    VisInput *input;
    float audiodata[2][2][1024];
    unsigned char blendtable[256][256];

    void *init;
    void *beat;
    void *frame;
    void *point;

} SuperScopePrivate;


typedef enum scope_runnable ScopeRunnable;

enum scope_runnable {
    SCOPE_RUNNABLE_INIT,
    SCOPE_RUNNABLE_FRAME,
    SCOPE_RUNNABLE_BEAT,
    SCOPE_RUNNABLE_POINT,
};

int scope_load_runnable(SuperScopePrivate *priv, ScopeRunnable runnable, char *buf)
{
    switch((int)runnable) {
        case SCOPE_RUNNABLE_INIT:
		Compile(buf, &priv->init);
	break;
	case SCOPE_RUNNABLE_BEAT:
		Compile(buf, &priv->beat);
	break;
	case SCOPE_RUNNABLE_FRAME:
		Compile(buf, &priv->frame);
	break;
	case SCOPE_RUNNABLE_POINT:
		Compile(buf, &priv->point);
	break;
    }
    return 0;
}

int scope_run(SuperScopePrivate *priv, ScopeRunnable runnable)
{
	RESULT result = {0, 0, 0, NULL};

        SetVariableNumeric("n", priv->n);
        SetVariableNumeric("b", priv->b);
        SetVariableNumeric("x", priv->x);
        SetVariableNumeric("y", priv->y);
        SetVariableNumeric("i", priv->i);
        SetVariableNumeric("v", priv->v);
        SetVariableNumeric("w", priv->w);
        SetVariableNumeric("h", priv->h);
        SetVariableNumeric("red", priv->red);
        SetVariableNumeric("green", priv->green);
        SetVariableNumeric("blue", priv->blue);
        SetVariableNumeric("linesize", priv->linesize);
        SetVariableNumeric("skip", priv->skip);
        SetVariableNumeric("drawmode", priv->drawmode);
        SetVariableNumeric("t", priv->t);
        SetVariableNumeric("d", priv->d);
    switch(runnable) {
        case SCOPE_RUNNABLE_INIT:
		Eval(priv->init, &result);
	break;
	case SCOPE_RUNNABLE_BEAT:
		Eval(priv->beat, &result);
	break;
	case SCOPE_RUNNABLE_FRAME:
		Eval(priv->frame, &result);
	break;
	case SCOPE_RUNNABLE_POINT:
		Eval(priv->point, &result);
	break;
    }
    priv->n = R2N(FindVariable("n")->value);
    priv->b = R2N(FindVariable("b")->value);
    priv->x = R2N(FindVariable("x")->value);
    priv->y = R2N(FindVariable("y")->value);
    priv->i = R2N(FindVariable("i")->value);
    priv->v = R2N(FindVariable("v")->value);
    priv->w = R2N(FindVariable("w")->value);
    priv->h = R2N(FindVariable("h")->value);
    priv->red = R2N(FindVariable("red")->value);
    priv->green = R2N(FindVariable("green")->value);
    priv->blue = R2N(FindVariable("blue")->value);
    priv->linesize = R2N(FindVariable("linesize")->value);
    priv->skip = R2N(FindVariable("skip")->value);
    priv->drawmode = R2N(FindVariable("drawmode")->value);
    priv->t = R2N(FindVariable("t")->value);
    priv->d = R2N(FindVariable("d")->value);
    return 0;
}

int avs_gfx_line_ints (void *pixels, int x0, int y0, int x1, int y1, int pitch, int col)
{
	register int dy = y1 - y0;
	register int dx = x1 - x0;
	register int stepx, stepy, stepy_;
	register int fraction;
	register int bp;
	register int x;
	register int y;
	uint32_t color = col;
	uint16_t *buf = pixels;
	uint32_t pbpp = pitch;

	if (dy < 0) {
		dy = -dy;
		stepy_ = -pbpp;
		stepy = -1;
	} else {
		stepy = 1;
		stepy_ = pbpp;
	}

	if (dx < 0) {
		dx = -dx;
		stepx = -1;
	} else {
		stepx = 1;
	}

	dy <<= 1;
	dx <<= 1;

	bp = x0 + y0 * (pbpp);

	*(buf + bp) = color;

	x = x0;
	y = y0;

	if (dx > dy) {
		fraction = dy - (dx >> 1);
		while (x0 != x1) {
			if (fraction >= 0) {
				bp += stepy_;
				fraction -= dx;
			}
			x0 += stepx;
			bp += stepx;
			fraction += dy;

			buf[bp] = color;
		}
	}

	return 0;
}

/* Return current time in milliseconds */
static double now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000. + tv.tv_usec/1000.;
}

/* We're going to perform computations for every pixel of the target
 * bitmap. floating-point operations are very slow on ARMv5, and not
 * too bad on ARMv7 with the exception of trigonometric functions.
 *
 * For better performance on all platforms, we're going to use fixed-point
 * arithmetic and all kinds of tricks
 */

typedef int32_t  Fixed;

#define  FIXED_BITS           16
#define  FIXED_ONE            (1 << FIXED_BITS)
#define  FIXED_AVERAGE(x,y)   (((x) + (y)) >> 1)

#define  FIXED_FROM_INT(x)    ((x) << FIXED_BITS)
#define  FIXED_TO_INT(x)      ((x) >> FIXED_BITS)

#define  FIXED_FROM_FLOAT(x)  ((Fixed)((x)*FIXED_ONE))
#define  FIXED_TO_FLOAT(x)    ((x)/(1.*FIXED_ONE))

#define  FIXED_MUL(x,y)       (((int64_t)(x) * (y)) >> FIXED_BITS)
#define  FIXED_DIV(x,y)       (((int64_t)(x) * FIXED_ONE) / (y))

#define  FIXED_DIV2(x)        ((x) >> 1)
#define  FIXED_AVERAGE(x,y)   (((x) + (y)) >> 1)

#define  FIXED_FRAC(x)        ((x) & ((1 << FIXED_BITS)-1))
#define  FIXED_TRUNC(x)       ((x) & ~((1 << FIXED_BITS)-1))

#define  FIXED_FROM_INT_FLOAT(x,f)   (Fixed)((x)*(FIXED_ONE*(f)))

typedef int32_t  Angle;

#define  ANGLE_BITS              9

#if ANGLE_BITS < 8
#  error ANGLE_BITS must be at least 8
#endif

#define  ANGLE_2PI               (1 << ANGLE_BITS)
#define  ANGLE_PI                (1 << (ANGLE_BITS-1))
#define  ANGLE_PI2               (1 << (ANGLE_BITS-2))
#define  ANGLE_PI4               (1 << (ANGLE_BITS-3))

#define  ANGLE_FROM_FLOAT(x)   (Angle)((x)*ANGLE_PI/M_PI)
#define  ANGLE_TO_FLOAT(x)     ((x)*M_PI/ANGLE_PI)

#if ANGLE_BITS <= FIXED_BITS
#  define  ANGLE_FROM_FIXED(x)     (Angle)((x) >> (FIXED_BITS - ANGLE_BITS))
#  define  ANGLE_TO_FIXED(x)       (Fixed)((x) << (FIXED_BITS - ANGLE_BITS))
#else
#  define  ANGLE_FROM_FIXED(x)     (Angle)((x) << (ANGLE_BITS - FIXED_BITS))
#  define  ANGLE_TO_FIXED(x)       (Fixed)((x) >> (ANGLE_BITS - FIXED_BITS))
#endif

static Fixed  angle_sin_tab[ANGLE_2PI+1];

static void init_angles(void)
{
    int  nn;
    for (nn = 0; nn < ANGLE_2PI+1; nn++) {
        double  radians = nn*M_PI/ANGLE_PI;
        angle_sin_tab[nn] = FIXED_FROM_FLOAT(sin(radians));
    }
}

static __inline__ Fixed angle_sin( Angle  a )
{
    return angle_sin_tab[(uint32_t)a & (ANGLE_2PI-1)];
}

static __inline__ Fixed angle_cos( Angle  a )
{
    return angle_sin(a + ANGLE_PI2);
}

static __inline__ Fixed fixed_sin( Fixed  f )
{
    return angle_sin(ANGLE_FROM_FIXED(f));
}

static __inline__ Fixed  fixed_cos( Fixed  f )
{
    return angle_cos(ANGLE_FROM_FIXED(f));
}

/* Color palette used for rendering the plasma */
#define  PALETTE_BITS   8
#define  PALETTE_SIZE   (1 << PALETTE_BITS)

#if PALETTE_BITS > FIXED_BITS
#  error PALETTE_BITS must be smaller than FIXED_BITS 
#endif

static uint16_t  palette[PALETTE_SIZE];

static uint16_t  make565(int red, int green, int blue)
{
    return (uint16_t)( ((red   << 8) & 0xf800) |
                       ((green << 2) & 0x03e0) |
                       ((blue  >> 3) & 0x001f) );
}

static void init_palette(SuperScopePrivate *priv)
{
    int  nn, mm = 0;
    /* fun with colors */
    for (nn = 0; nn < PALETTE_SIZE/4; nn++) {
        int  jj = (nn-mm)*4*255/PALETTE_SIZE;
        palette[nn] = make565(255, jj, 255-jj);
    }

    for ( mm = nn; nn < PALETTE_SIZE/2; nn++ ) {
        int  jj = (nn-mm)*4*255/PALETTE_SIZE;
        palette[nn] = make565(255-jj, 255, jj);
    }

    for ( mm = nn; nn < PALETTE_SIZE*3/4; nn++ ) {
        int  jj = (nn-mm)*4*255/PALETTE_SIZE;
        palette[nn] = make565(0, 255-jj, 255);
    }

    for ( mm = nn; nn < PALETTE_SIZE; nn++ ) {
        int  jj = (nn-mm)*4*255/PALETTE_SIZE;
        palette[nn] = make565(jj, 0, 255);
    }

    visual_palette_allocate_colors(&priv->pal, PALETTE_SIZE);
    for(nn = 0; nn < priv->pal.ncolors; nn++)
    {
	VisColor col;
        visual_color_from_uint16(&col, palette[nn]);
        priv->pal.colors[nn].r = col.r;
	priv->pal.colors[nn].g = col.g;
        priv->pal.colors[nn].b = col.b;
        priv->pal.colors[nn].a = col.a;
    }

}

static __inline__ uint16_t  palette_from_fixed( Fixed  x )
{
    if (x < 0) x = -x;
    if (x >= FIXED_ONE) x = FIXED_ONE-1;
    int  idx = FIXED_FRAC(x) >> (FIXED_BITS - PALETTE_BITS);
    return palette[idx & (PALETTE_SIZE-1)];
}

/* Angles expressed as fixed point radians */

static void init_tables(SuperScopePrivate *priv)
{
    init_palette(priv);
    init_angles();
}

static void fill_aurora(ANativeWindow_Buffer *buffer, double t)
{
	int h, w;
	int depth = 32;
	switch(buffer->format)
	{
		case WINDOW_FORMAT_RGBA_8888:
			depth = 32;
			break;
		case WINDOW_FORMAT_RGBX_8888:
			depth = 24;
			break;
		case WINDOW_FORMAT_RGB_565:
			depth = 16;
			break;
		
	}

	VisVideo *image, *src, *dst, *scalevid, *final;

	image = visual_video_new();
	visual_video_set_attributes(image, buffer->width, buffer->height, buffer->stride * 2,
		visual_video_depth_enum_from_value(depth));
	visual_video_set_buffer(image, buffer->bits);

	src = visual_bitmap_load_new_video ("/mnt/sdcard/starvisuals/bg.bmp");

	dst = visual_video_new_with_buffer(src->width, src->height, visual_video_depth_enum_from_value(depth));
	visual_video_depth_transform(dst, src);


	VisRectangle rect;

	int delta = (image->height - dst->height);
	h = dst->height + delta;
	delta = (image->width - dst->width);
	w = dst->width + delta;

	visual_rectangle_set(&rect, 0, 0, w, h);

	scalevid = visual_video_new_with_buffer(w, h, 
		visual_video_depth_enum_from_value(depth));
	visual_video_scale(scalevid, dst, VISUAL_VIDEO_SCALE_NEAREST);

	visual_video_composite_set_surface(scalevid, .8 * 256);
        visual_video_composite_set_type(scalevid, VISUAL_VIDEO_COMPOSITE_TYPE_SURFACE);
	visual_video_blit_overlay_rectangle(image, &rect, scalevid, &rect, TRUE);


	visual_video_free_buffer(image);
	visual_video_free_buffer(src);
	visual_video_free_buffer(scalevid);

	return;
}

static void fill_plasma(ANativeWindow_Buffer* buffer, double  t)
{
    //Fixed ft  = FIXED_FROM_FLOAT(t/1000.);
    Fixed yt1 = FIXED_FROM_FLOAT(t/1230.);
    Fixed yt2 = yt1;
    Fixed xt10 = FIXED_FROM_FLOAT(t/3000.);
    Fixed xt20 = xt10;

#define  YT1_INCR   FIXED_FROM_FLOAT(1/100.)
#define  YT2_INCR   FIXED_FROM_FLOAT(1/163.)

    void* pixels = buffer->bits;
    //LOGI("width=%d height=%d stride=%d format=%d", buffer->width, buffer->height,
    //        buffer->stride, buffer->format);

    int  yy;
    for (yy = 0; yy < buffer->height; yy++) {
        uint16_t*  line = (uint16_t*)pixels;
        Fixed      base = fixed_sin(yt1) + fixed_sin(yt2);
        Fixed      xt1 = xt10;
        Fixed      xt2 = xt20;

        yt1 += YT1_INCR;
        yt2 += YT2_INCR;

#define  XT1_INCR  FIXED_FROM_FLOAT(1/173.)
#define  XT2_INCR  FIXED_FROM_FLOAT(1/242.)

#if OPTIMIZE_WRITES
        /* optimize memory writes by generating one aligned 32-bit store
         * for every pair of pixels.
         */
        uint16_t*  line_end = line + buffer->width;

        if (line < line_end) {
            if (((uint32_t)line & 3) != 0) {
                Fixed ii = base + fixed_sin(xt1) + fixed_sin(xt2);

                xt1 += XT1_INCR;
                xt2 += XT2_INCR;

                line[0] = palette_from_fixed(ii >> 2);
                line++;
            }

            while (line + 2 <= line_end) {
                Fixed i1 = base + fixed_sin(xt1) + fixed_sin(xt2);
                xt1 += XT1_INCR;
                xt2 += XT2_INCR;

                Fixed i2 = base + fixed_sin(xt1) + fixed_sin(xt2);
                xt1 += XT1_INCR;
                xt2 += XT2_INCR;

                uint32_t  pixel = ((uint32_t)palette_from_fixed(i1 >> 2) << 16) |
                                   (uint32_t)palette_from_fixed(i2 >> 2);

                ((uint32_t*)line)[0] = pixel;
                line += 2;
            }

            if (line < line_end) {
                Fixed ii = base + fixed_sin(xt1) + fixed_sin(xt2);
                line[0] = palette_from_fixed(ii >> 2);
                line++;
            }
        }
#else /* !OPTIMIZE_WRITES */
        int xx;
        for (xx = 0; xx < buffer->width; xx++) {

            Fixed ii = base + fixed_sin(xt1) + fixed_sin(xt2);

            xt1 += XT1_INCR;
            xt2 += XT2_INCR;

            line[xx] = palette_from_fixed(ii / 4);
        }
#endif /* !OPTIMIZE_WRITES */

        // go to next line
        pixels = (uint16_t*)pixels + buffer->stride;
    }
}

static void fill_starvisuals(SuperScopePrivate *priv, ANativeWindow_Buffer* buffer)
{
    float pcmbuf[BEAT_ADV_MAX];
    int size = BEAT_ADV_MAX/2;
    int isBeat = priv->isBeat;

    int16_t *buf = buffer->bits;

    if(priv->needs_init) {
        priv->needs_init = FALSE;
        scope_run(priv, SCOPE_RUNNABLE_INIT);
    }
    int a, l, lx = 0, ly = 0, x = 0, y = 0;
    int32_t current_color;
    int ws=(priv->channel_source&4)?1:0;
    int xorv=(ws*128)^128;
    //uint16_t fa_data[576];

    if((priv->channel_source&3) >= 2)
    {
        for(x = 0; x < size; x++) {
            pcmbuf[x] = priv->audiodata[ws^1][0][x] / 2 + priv->audiodata[ws^1][1][x] / 2;
        }
    }
    else 
    {
        for(x = 0; x < size; x++) {
            pcmbuf[x] = priv->audiodata[ws^1][priv->channel_source&3][x];
        }
    }
  
    priv->color_pos+=5;

    if(priv->color_pos >= priv->pal.ncolors * 64) 
	priv->color_pos = 0;

    {
        int p = priv->color_pos/64;
        int r = priv->color_pos&63;
        int c1, c2;
        int r1, r2, r3;
        c1 = visual_color_to_uint32(&priv->pal.colors[p]);
        if(p+1 < priv->pal.ncolors)
            c2 = visual_color_to_uint32(&priv->pal.colors[p+1]);
        else 
            c2 = visual_color_to_uint32(&priv->pal.colors[0]);

        r1 = (((c1&255)*(63-r))+((c2&255)*4))/64;
        r2 = ((((c1>>8)&255)*(63-r))+(((c2>>8)&255)*4))/64;
        r3 = ((((c1>>16)&255)*(63-r))+(((c2>>16)&255)*r))/64;

        current_color = r1|(r2<<8)|(r3<<16)|(255<<24);
    }

    priv->h = buffer->height;
    priv->w = buffer->width;
    priv->b = isBeat?1.0:0.0;
    priv->blue = (current_color&0xff)/255.0;
    priv->green = ((current_color>>8)&0xff)/255.0;
    priv->red = ((current_color>>16)&0xff)/255.0;
    priv->skip = 0.0;
    priv->linesize = (double) ((priv->blendmode&0xff0000)>>16);
    priv->drawmode = priv->drawmode ? 1.0 : 0.0;
    scope_run(priv, SCOPE_RUNNABLE_FRAME);

    if (isBeat)
        scope_run(priv, SCOPE_RUNNABLE_BEAT);

    //int candraw=0;
    l = priv->n;
    if (l >= size*size)
        l = size*size - 1;
    if( l == 0 || l == 1)
	l = 2;

    for (a=0; a < l; a++) 
    {
        double r=(a*size)/(double)l;
        double s1=r-(int)r;
/*
        int val1 = (pcmbuf[(int)r] + 1) / 2.0 * size;
        int val2 = (pcmbuf[(int)r+1] + 1) / 2.0  * size;
        double yr=(val1^xorv)*(1.0-s1)+(val2^xorv)*(s1);
*/
        priv->v = 128/(double)size;
        priv->i = a/(double)(l-1);
        priv->skip = 0.0;
        scope_run(priv, SCOPE_RUNNABLE_POINT);

        x = (int)((priv->x + 1.0) * buffer->width * 0.5);
        y = (int)((priv->y + 1.0) * buffer->height * 0.5);


        if (priv->skip >= 0.00001)
            continue;

        uint32_t this_color = makeint(priv->blue) | (makeint(priv->green) << 8) | (makeint(priv->red) << 16) | (255 << 24);

        if (priv->drawmode < 0.00001 && 0) {
            if (y >= 0 && y < buffer->height && x >= 0 && x < buffer->width) {
                //BLEND_LINE(buf+x+y*buffer->width, this_color, priv->blendtable, pipeline->blendmode);
            }
        } else {
            if (a > 0) {
                if (y >= 0 && y < buffer->height && x >= 0 && x < buffer->width &&
                    ly >= 0 && ly < buffer->height && lx >= 0 && lx < buffer->width) {
                        avs_gfx_line_ints(buf, lx, ly, x, y, buffer->width, this_color);
                }
            }
        }
       	lx = x;
        ly = y;
    }


    LOGI("width=%d height=%d stride=%d format=%d", buffer->width, buffer->height,
            buffer->stride, buffer->format);
}

double PI = 3.14159;
double E = 2.71828;
double PHI = 1.618033;

static void function_log(RESULT * result, RESULT * arg1);
static void function_sin(RESULT * result, RESULT * arg1);
static void function_cos(RESULT * result, RESULT * arg1);
static void function_tan(RESULT * result, RESULT * arg1);
static void function_asin(RESULT * result, RESULT * arg1);
static void function_acos(RESULT * result, RESULT * arg1);
static void function_atan(RESULT * result, RESULT * arg1);
static void function_if(RESULT * result, RESULT * arg1, RESULT * arg2, RESULT * arg3);
static void function_div(RESULT * result, RESULT * arg1, RESULT * arg2);
static void function_rand(RESULT * result, RESULT * arg1, RESULT * arg2);

void init_evaluator(SuperScopePrivate *priv)
{

        AddFunction("log", 1, function_log);
        AddFunction("sin", 1, function_sin);
        AddFunction("cos", 1, function_cos);
        AddFunction("tan", 1, function_tan);
        AddFunction("asin", 1, function_asin);
        AddFunction("acos", 1, function_acos);
        AddFunction("atan", 1, function_atan);
        AddFunction("if", 2, function_if);
        AddFunction("div", 2, function_div);
        AddFunction("rand", 1, function_rand);

        SetVariableNumeric ("PI", PI);

        SetVariableNumeric ("E", E);

        SetVariableNumeric ("PHI", PHI);

	scope_load_runnable(priv, SCOPE_RUNNABLE_INIT, init);
	scope_load_runnable(priv, SCOPE_RUNNABLE_BEAT, beat);
	scope_load_runnable(priv, SCOPE_RUNNABLE_FRAME, frame);
	scope_load_runnable(priv, SCOPE_RUNNABLE_POINT, point);

	priv->n = 50;
	priv->b = 0;
	priv->x = 0;
	priv->y = 0;
	priv->i = 0;
	priv->v = 0;
	priv->w = 255;
	priv->h = 255;
	priv->red = 1;
	priv->green = 1;
	priv->blue = 1;
	priv->linesize = 1;
	priv->skip = 0;
	priv->drawmode = 0;
	priv->t = 0;
	priv->d = 0;
	priv->needs_init = TRUE;

        SetVariableNumeric("n", priv->n);
        SetVariableNumeric("b", priv->b);
        SetVariableNumeric("x", priv->x);
        SetVariableNumeric("y", priv->y);
        SetVariableNumeric("i", priv->i);
        SetVariableNumeric("v", priv->v);
        SetVariableNumeric("w", priv->w);
        SetVariableNumeric("h", priv->h);
        SetVariableNumeric("red", priv->red);
        SetVariableNumeric("green", priv->green);
        SetVariableNumeric("blue", priv->blue);
        SetVariableNumeric("linesize", priv->linesize);
        SetVariableNumeric("skip", priv->skip);
        SetVariableNumeric("drawmode", priv->drawmode);
        SetVariableNumeric("t", priv->t);
        SetVariableNumeric("d", priv->d);

}

static void function_log(RESULT * result, RESULT * arg1)
{
        double val = log(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_sin(RESULT * result, RESULT * arg1)
{
        double val = sin(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_cos(RESULT * result, RESULT * arg1)
{
        double val = cos(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_tan(RESULT * result, RESULT * arg1)
{
        double val = tan(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_asin(RESULT * result, RESULT * arg1)
{
        double val = asin(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_acos(RESULT * result, RESULT * arg1)
{
        double val = acos(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_atan(RESULT * result, RESULT * arg1)
{
        double val = atan(R2N(arg1));

        SetResult(&result, R_NUMBER, &val);
}

static void function_if(RESULT * result, RESULT * arg1, RESULT * arg2, RESULT * arg3)
{
        double a = R2N(arg1);
        double b = R2N(arg2);
        double c = R2N(arg3);
        double val = (c != 0.0) ? a : b;

        SetResult(&result, R_NUMBER, &val);
}

static void function_div(RESULT * result, RESULT * arg1, RESULT * arg2)
{
        double a = R2N(arg1);
        double b = R2N(arg2);
        double val = (a == 0) ? 0 : (b / a);
        SetResult(&result, R_NUMBER, &val);
}

static void function_rand(RESULT * result, RESULT * arg1, RESULT * arg2)
{
        int a, b, seed, val;
        a = R2N(arg1);
        b = R2N(arg1);

        seed = time(NULL);

        srand(seed);

        val = (rand() % (b - a)) + a;

        SetResult(&result, R_NUMBER, &val);
}


/* simple stats management */
typedef struct {
    double  renderTime;
    double  frameTime;
} FrameStats;

#define  MAX_FRAME_STATS  200
#define  MAX_PERIOD_MS    1500

typedef struct {
    double  firstTime;
    double  lastTime;
    double  frameTime;

    int         firstFrame;
    int         numFrames;
    FrameStats  frames[ MAX_FRAME_STATS ];
} Stats;

static void
stats_init( Stats*  s )
{
    s->lastTime = now_ms();
    s->firstTime = 0.;
    s->firstFrame = 0;
    s->numFrames  = 0;
}

static void
stats_startFrame( Stats*  s )
{
    s->frameTime = now_ms();
}

static void
stats_endFrame( Stats*  s )
{
    double now = now_ms();
    double renderTime = now - s->frameTime;
    double frameTime  = now - s->lastTime;
    int nn;

    if (now - s->firstTime >= MAX_PERIOD_MS) {
        if (s->numFrames > 0) {
            double minRender, maxRender, avgRender;
            double minFrame, maxFrame, avgFrame;
            int count;

            nn = s->firstFrame;
            minRender = maxRender = avgRender = s->frames[nn].renderTime;
            minFrame  = maxFrame  = avgFrame  = s->frames[nn].frameTime;
            for (count = s->numFrames; count > 0; count-- ) {
                nn += 1;
                if (nn >= MAX_FRAME_STATS)
                    nn -= MAX_FRAME_STATS;
                double render = s->frames[nn].renderTime;
                if (render < minRender) minRender = render;
                if (render > maxRender) maxRender = render;
                double frame = s->frames[nn].frameTime;
                if (frame < minFrame) minFrame = frame;
                if (frame > maxFrame) maxFrame = frame;
                avgRender += render;
                avgFrame  += frame;
            }
            avgRender /= s->numFrames;
            avgFrame  /= s->numFrames;

            LOGI("frame/s (avg,min,max) = (%.1f,%.1f,%.1f) "
                 "render time ms (avg,min,max) = (%.1f,%.1f,%.1f)\n",
                 1000./avgFrame, 1000./maxFrame, 1000./minFrame,
                 avgRender, minRender, maxRender);
        }
        s->numFrames  = 0;
        s->firstFrame = 0;
        s->firstTime  = now;
    }

    nn = s->firstFrame + s->numFrames;
    if (nn >= MAX_FRAME_STATS)
        nn -= MAX_FRAME_STATS;

    s->frames[nn].renderTime = renderTime;
    s->frames[nn].frameTime  = frameTime;

    if (s->numFrames < MAX_FRAME_STATS) {
        s->numFrames += 1;
    } else {
        s->firstFrame += 1;
        if (s->firstFrame >= MAX_FRAME_STATS)
            s->firstFrame -= MAX_FRAME_STATS;
    }

    s->lastTime = now;
}

static void reset_superscope(SuperScopePrivate *priv)
{
	priv->n = 50;
	priv->b = 0;
	priv->x = 0;
	priv->y = 0;
	priv->i = 0;
	priv->v = 0;
	priv->w = 255;
	priv->h = 255;
	priv->red = 1;
	priv->green = 1;
	priv->blue = 1;
	priv->linesize = 1;
	priv->skip = 0;
	priv->drawmode = 0;
	priv->t = 0;
	priv->d = 0;
	priv->needs_init = TRUE;
}

static void priv_init(SuperScopePrivate *priv)
{
	reset_superscope(priv);
}

// ----------------------------------------------------------------------

struct engine {
    struct android_app* app;

    Stats stats;

    int animating;
    int running;
    SuperScopePrivate *priv;
};

static void engine_draw_frame(struct engine* engine) {
    int i;
    if (engine->app->window == NULL) {
        // No window.
        return;
    }

    ANativeWindow_Buffer buffer;
    VisVideo *image;
    int depth = 32;
    switch(buffer.format)
    {
    	case WINDOW_FORMAT_RGBA_8888:
    		depth = 32;
    		break;
    	case WINDOW_FORMAT_RGBX_8888:
    		depth = 24;
    		break;
    	case WINDOW_FORMAT_RGB_565:
    		depth = 16;
    		break;
    	
    }

    if (ANativeWindow_lock(engine->app->window, &buffer, NULL) < 0) {
        LOGW("Unable to lock window buffer");
        return;
    }
    image = visual_video_new();
    visual_video_set_attributes(image, buffer.width, buffer.height, buffer.stride * 2, visual_video_depth_enum_from_value(depth));
    visual_video_set_buffer(image, buffer.bits);
    visual_video_fill_color(image, visual_color_black());

    stats_startFrame(&engine->stats);

    struct timespec t;
    t.tv_sec = t.tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &t);
    int64_t time_ms = (((int64_t)t.tv_sec)*1000000000LL + t.tv_nsec)/1000000;

    visual_input_run(engine->priv->input);
    VisAudio *audio = engine->priv->input->audio;

    VisBuffer pcmbuf1;
    VisBuffer pcmbuf2;
    VisBuffer spmbuf1;
    VisBuffer spmbuf2;
    VisBuffer tmp;

    int size = BEAT_MAX_SIZE/2;

    float data[2][2][size];

    visual_buffer_init_allocate(&tmp, sizeof(float) * size, visual_buffer_destroyer_free);

    /* Left audio */
    visual_buffer_set_data_pair(&pcmbuf1, data[0][0], sizeof(float) * size);

    if(visual_audio_get_sample(audio, &tmp, VISUAL_AUDIO_CHANNEL_LEFT) == VISUAL_OK)

        visual_audio_sample_buffer_mix(&pcmbuf1, &tmp, TRUE, 1.0);

    visual_buffer_set_data_pair(&spmbuf1, &data[1][0], sizeof(float) * size);

    visual_audio_get_spectrum_for_sample (&spmbuf1, &tmp, TRUE);

    /* Right audio */
    visual_buffer_set_data_pair(&pcmbuf2, data[0][1], sizeof(float) * size);

    if(visual_audio_get_sample(audio, &tmp, VISUAL_AUDIO_CHANNEL_LEFT) == VISUAL_OK)

        visual_audio_sample_buffer_mix(&pcmbuf2, &tmp, TRUE, 1.0);

    visual_buffer_set_data_pair(&spmbuf2, data[1][1], sizeof(float) * size);

    visual_audio_get_spectrum_for_sample(&spmbuf2, &tmp, TRUE);

    visual_object_unref(VISUAL_OBJECT(&tmp));
    
    for(i = size - 1; i >= 0; i--) {
    engine->priv->audiodata[0][0][i] = (data[0][0][i] + 1) / 2;
    engine->priv->audiodata[1][0][i] = (data[1][0][i] + 1) / 2;
    engine->priv->audiodata[0][1][i] = (data[0][1][i] + 1) / 2;
    engine->priv->audiodata[1][1][i] = (data[1][1][i] + 1) / 2;
    }

    float beatdata[BEAT_MAX_SIZE];
    unsigned char visdata[BEAT_MAX_SIZE];

    memcpy(beatdata, data[1][0], size * sizeof(float));
    memcpy(beatdata + size, data[1][1], size * sizeof(float));

    for(i = BEAT_MAX_SIZE - 1; i >= 0; i--) {
        visdata[i] = (beatdata[i] + 1) / 2.0 * UCHAR_MAX;
    }

    engine->priv->isBeat = visual_audio_is_beat_with_data(audio, VISUAL_BEAT_ALGORITHM_PEAK, visdata, BEAT_MAX_SIZE);

    /* Now fill the values with a nice little plasma */
    //fill_starvisuals(engine->priv, &buffer);
    fill_plasma(&buffer, time_ms);
    fill_aurora(&buffer, time_ms);

    ANativeWindow_unlockAndPost(engine->app->window);

    stats_endFrame(&engine->stats);
}

static int engine_term_display(struct engine* engine) {
    engine->animating = FALSE;
    engine->running = FALSE;
    return 0;
}

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animating = 1;
        return 1;
    } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
        LOGI("Key event: action=%d keyCode=%d metaState=0x%x",
                AKeyEvent_getAction(event),
                AKeyEvent_getKeyCode(event),
                AKeyEvent_getMetaState(event));
    }

    return 0;
}


static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    if (engine->app->window) switch (cmd) {
        case APP_CMD_INIT_WINDOW:
            reset_superscope(engine->priv);
            if (engine->app->window != NULL) {
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            engine_term_display(engine);
        case APP_CMD_LOST_FOCUS:
            engine->animating = FALSE;
            engine->running = FALSE;
            break;
	case APP_CMD_GAINED_FOCUS:
            reset_superscope(engine->priv);
            engine->animating = TRUE;
            engine_draw_frame(engine);
            break;
	case APP_CMD_WINDOW_RESIZED:
            reset_superscope(engine->priv);
            engine_draw_frame(engine);
            break;
    }
}

void android_main(struct android_app* state) {
    static int init;

    struct engine engine;

    visual_init_path_add( "/mnt/sdcard/starvisuals");

    visual_init(0, NULL);

    
    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    engine.priv = malloc(sizeof(SuperScopePrivate));
    memset(engine.priv, 0, sizeof(SuperScopePrivate));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    priv_init(engine.priv);

    engine.priv->input = visual_input_new("alsa");
    visual_input_realize(engine.priv->input);

    if (!init) {
        init_tables(engine.priv);
        init = 1;
    }

    stats_init(&engine.stats);

    init_evaluator(engine.priv);

    // loop waiting for stuff to do.

    engine.running = TRUE;
    while (engine.running) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                LOGI("Engine thread destroy requested!");
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) {
            engine_draw_frame(&engine);
        }
    }
    visual_mem_free(engine.priv);
    visual_quit();
}
