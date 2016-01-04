/*
    Copyright (c) 2016 Micha Mettke

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1.  The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2.  Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3.  This notice may not be removed or altered from any source distribution.
*/
#ifndef ZR_H_
#define ZR_H_

#ifdef __cplusplus
extern "C" {
#endif
/*
 * ==============================================================
 *
 *                          CONSTANTS
 *
 * ===============================================================
 */
#define ZR_UTF_INVALID 0xFFFD
#define ZR_UTF_SIZE 4
/* describes the number of bytes a glyph consists of*/
#define ZR_INPUT_MAX 16
/* defines the max number of bytes to be added as text input in one frame */
#define ZR_MAX_COLOR_STACK 32
/* defines the number of temporary configuration color changes that can be stored */
#define ZR_MAX_ATTRIB_STACK 32
/* defines the number of temporary configuration attribute changes that can be stored */
#define ZR_MAX_FONT_STACK 32
/* defines the number of temporary configuration user font changes that can be stored */
#define ZR_MAX_FONT_HEIGHT_STACK 32
/* defines the number of temporary configuration font height changes that can be stored */
#define ZR_MAX_NUMBER_BUFFER 64
/*
 * ==============================================================
 *
 *                          DEFINES
 *
 * ===============================================================
 */
#define ZR_COMPILE_WITH_FIXED_TYPES 1
/* setting this define to 1 adds header <stdint.h> for fixed sized types
 * if 0 each type has to be set to the correct size*/
#define ZR_COMPILE_WITH_ASSERT 1
/* setting this define to 1 adds header <assert.h> for the assert macro
  IMPORTANT: it also adds the standard library assert so only use it if wanted */
#define ZR_COMPILE_WITH_VERTEX_BUFFER 1
/* setting this define to 1 adds a vertex draw command list backend to this library,
  which allows you to convert queue commands into vertex draw commands.
  If you do not want or need a default backend you can set this flag to zero
  and the module of the library will not be compiled */
#define ZR_COMPILE_WITH_FONT 1
/* setting this define to 1 adds the `stb_truetype` and `stb_rect_pack` header
 to this library and provides a default font for font loading and rendering.
 If you already have font handling or do not want to use this font handler
 you can just set this define to zero and the font module will not be compiled
 and the two headers will not be needed. */
/*
* ==============================================================
 *
 *                          BASIC
 *
 * ===============================================================
 */
#if ZR_COMPILE_WITH_FIXED_TYPES
#include <stdint.h>
typedef uint32_t zr_uint;
typedef uint32_t zr_hash;
typedef uint64_t zr_ulong;
typedef uint64_t zr_size;
typedef uintptr_t zr_ptr;
typedef uint32_t zr_flags;
typedef uint32_t zr_rune;
typedef uint8_t zr_byte;
#else
typedef unsigned int zr_uint;
typedef unsigned int zr_hash;
typedef unsigned long zr_ulong;
typedef unsigned long zr_size;
typedef zr_size zr_ptr;
typedef unsigned int zr_flags;
typedef unsigned int zr_rune;
typedef unsigned char zr_byte;
#endif

#if ZR_COMPILE_WITH_ASSERT
#ifndef ZR_ASSERT
#include <assert.h>
#define ZR_ASSERT(expr) assert(expr)
#endif
#else
#define ZR_ASSERT(expr)
#endif

/* Callbacks */
struct zr_user_font;
struct zr_edit_box;
struct zr_user_font_glyph;

/*================================================================
 *                          UTILITY
 * ===============================================================*/
enum {zr_false, zr_true};
struct zr_color {zr_byte r,g,b,a;};
struct zr_vec2 {float x,y;};
struct zr_vec2i {short x, y;};
struct zr_rect {float x,y,w,h;};
struct zr_recti {short x,y,w,h;};
typedef char zr_glyph[ZR_UTF_SIZE];
typedef union {void *ptr; int id;} zr_handle;
struct zr_image {zr_handle handle; unsigned short w, h; unsigned short region[4];};
struct zr_scroll {unsigned short x, y;};

/* pointer */
#define zr_ptr_add(t, p, i) ((t*)((void*)((zr_byte*)(p) + (i))))
#define zr_ptr_sub(t, p, i) ((t*)((void*)((zr_byte*)(p) - (i))))
#define zr_ptr_add_const(t, p, i) ((const t*)((const void*)((const zr_byte*)(p) + (i))))
#define zr_ptr_sub_const(t, p, i) ((const t*)((const void*)((const zr_byte*)(p) - (i))))

/* math */
struct zr_rect zr_get_null_rect(void);
struct zr_rect zr_rect(float x, float y, float w, float h);
struct zr_vec2 zr_vec2(float x, float y);

/* UTF-8 */
zr_size zr_utf_decode(const char*, zr_rune*, zr_size);
zr_size zr_utf_encode(zr_rune, char*, zr_size);
zr_size zr_utf_len(const char*, zr_size byte_len);

/* color */
struct zr_color zr_rgba(zr_byte r, zr_byte g, zr_byte b, zr_byte a);
struct zr_color zr_rgb(zr_byte r, zr_byte g, zr_byte b);
struct zr_color zr_rgba_f(float r, float g, float b, float a);
struct zr_color zr_rgb_f(float r, float g, float b);
struct zr_color zr_hsv(zr_byte h, zr_byte s, zr_byte v);
struct zr_color zr_hsv_f(float h, float s, float v);
struct zr_color zr_hsva(zr_byte h, zr_byte s, zr_byte v, zr_byte a);
struct zr_color zr_hsva_f(float h, float s, float v, float a);
struct zr_color zr_rgba32(zr_uint);
zr_uint zr_color32(struct zr_color);
void zr_colorf(float *r, float *g, float *b, float *a, struct zr_color);
void zr_color_hsv(int *out_h, int *out_s, int *out_v, struct zr_color);
void zr_color_hsv_f(float *out_h, float *out_s, float *out_v, struct zr_color);
void zr_color_hsva(int *out_h, int *out_s, int *out_v, int *out_a, struct zr_color);
void zr_color_hsva_f(float *out_h, float *out_s, float *out_v,
                    float *out_a, struct zr_color);

/* image */
zr_handle zr_handle_ptr(void*);
zr_handle zr_handle_id(int);
struct zr_image zr_image_ptr(void*);
struct zr_image zr_image_id(int);
struct zr_image zr_subimage_ptr(void*, unsigned short w, unsigned short h, struct zr_rect);
struct zr_image zr_subimage_id(int, unsigned short w, unsigned short h, struct zr_rect);
int zr_image_is_subimage(const struct zr_image* img);

/* ==============================================================
 *
 *                          MEMORY BUFFER
 *
 * ===============================================================*/
/*  A basic (double)-buffer API with linear allocation and resetting as only
    freeing policy. The buffers main purpose is to control all memory management inside
    the GUI toolkit and still leave memory control as much as possible in the hand
    of the user. The memory is provided in three different ways.
    The first way is to use a fixed size block of memory to be filled up.
    Biggest advantage is a simple memory model. Downside is that if the buffer
    is full there is no way to accesses more memory, which fits target application
    with a GUI with roughly known memory consumptions.
    The second way to manage memory is by extending the fixed size block by querying
    information from the buffer about the used size and needed size and allocate new
    memory if the buffer is full. While this approach is still better than just using
    a fixed size memory block the reallocation still has one invalid frame as consquence
    since the used memory information is only available at the end of the frame which leads
    to the last way of handling memory.
    The last and most complicated way of handling memory is by allocator callbacks.
    The user hereby registers callbacks to be called to allocate and free
    memory if needed. While this solves most allocation problems it causes some
    loss of flow control on the user side.

    USAGE
    ----------------------------
    To instantiate the buffer you either have to call the fixed size or allocator
    initialization function and provide a memory block in the first case and
    an allocator in the second case.
    To allocate memory from the buffer you would call zr_buffer_alloc with a request
    memory block size as well as an alignment for the block. Finally to reset the memory
    at the end of the frame and when the memory buffer inside the buffer is no longer
    needed you would call zr_buffer_reset. To free all memory that has been allocated
    by an allocator if the buffer is no longer being used you have to call
    zr_buffer_clear.
*/
struct zr_memory_status {
    void *memory;
    /* pointer to the currently used memory block inside the referenced buffer */
    unsigned int type;
    /* type of the buffer which is either fixed size or dynamic */
    zr_size size;
    /* total size of the memory block */
    zr_size allocated;
    /* allocated amount of memory */
    zr_size needed;
    /* memory size that would have been allocated if enough memory was present */
    zr_size calls;
    /* number of allocation calls referencing this buffer */
};

struct zr_allocator {
    zr_handle userdata;
    /* handle to your own allocator */
    void*(*alloc)(zr_handle, zr_size);
    /* allocation function pointer */
    /* reallocation pointer of a previously allocated memory block */
    void(*free)(zr_handle, void*);
    /* callback function pointer to finally free all allocated memory */
};

enum zr_allocation_type {
    ZR_BUFFER_FIXED,
    /* fixed size memory buffer */
    ZR_BUFFER_DYNAMIC
    /* dynamically growing buffer */
};

enum zr_buffer_allocation_type {
    ZR_BUFFER_FRONT,
    /* allocate memory from the front of the buffer */
    ZR_BUFFER_BACK,
    /* allocate memory from the back of the buffer */
    ZR_BUFFER_MAX
};

struct zr_buffer_marker {
    int active;
    /* flag indiciation if the marker was set */
    zr_size offset;
    /* offset of the marker inside the buffer */
};

struct zr_memory {void *ptr;zr_size size;};
struct zr_buffer {
    struct zr_buffer_marker marker[ZR_BUFFER_MAX];
    /* buffer marker to free a buffer to a certain offset */
    struct zr_allocator pool;
    /* allocator callback for dynamic buffers */
    enum zr_allocation_type type;
    /* memory management type */
    struct zr_memory memory;
    /* memory and size of the current memory block */
    float grow_factor;
    /* growing factor for dynamic memory management */
    zr_size allocated;
    /* total amount of memory allocated */
    zr_size needed;
    /* total amount of memory allocated if enough memory would have been present */
    zr_size calls;
    /* number of allocation calls */
    zr_size size;
    /* current size of the buffer */
};

void zr_buffer_init(struct zr_buffer*, const struct zr_allocator*, zr_size initial_size);
void zr_buffer_init_fixed(struct zr_buffer*, void *memory, zr_size size);
void zr_buffer_info(struct zr_memory_status*, struct zr_buffer*);
void zr_buffer_free(struct zr_buffer*);
void *zr_buffer_memory(struct zr_buffer*);
zr_size zr_buffer_total(struct zr_buffer*);

/* ==============================================================
 *
 *                          FONT
 *
 * ===============================================================*/
/*  Font handling in this library can be achived in three different ways.
    The first and simplest ways is by just using your font handling mechanism
    and provide a simple callback for text string width calculation with
    `zr_user_font`. This requires the default drawing output
    and is not possible for the optional vertex buffer output.

    The second way of font handling is by using the same `zr_user_font` struct
    to referencing a font as before but providing a second callback for
    `zr_user_font_glyph` querying which is used for text drawing in the optional vertex
    buffer output. In addition to the callback it is also required to provide
    a texture atlas from the font to draw.

    The final and most complex way is to use the optional font baker
    and font handling function, which requires two additional headers for
    TTF font baking. While the previous two methods did no need any functions
    outside callbacks and are therefore rather simple to handle, the final
    font handling method quite complex and you need to handle the complex
    font baking API. The reason why it is complex is because there are multible
    ways of using the API. For example it must be possible to use the font
    for default command output as well as vertex buffer output. So for example
    texture coordinates can either be UV for vertex buffer output or absolute pixel
    for drawing function based on pixels. Furthermore it is possible to incoperate
    custom user data into the resulting baked image (for example a white pixel for the
    vertex buffer output). In addition and probably the most complex aspect of
    the baking API was to incoperate baking of multible fonts into one image.

    In general the font baking API can be understood as having a number of
    loaded in memory TTF-fonts, font baking configuration and optional custom
    render data as input, while the output is made of font specific data, a big
    glyph array of all baked glyphs and the baked image. The API
    was designed that way to have a typical file format and not
    a perfectly ready in memory library instance of a font. The reason is more
    control and seperates the font baking code from the in library used font format.
*/
typedef zr_size(*zr_text_width_f)(zr_handle, float, const char*, zr_size);
typedef void(*zr_query_font_glyph_f)(zr_handle, float, struct zr_user_font_glyph*,
                                    zr_rune codepoint, zr_rune next_codepoint);

#if ZR_COMPILE_WITH_VERTEX_BUFFER
struct zr_user_font_glyph {
    struct zr_vec2 uv[2];
    /* texture coordinates */
    struct zr_vec2 offset;
    /* offset between top left and glyph */
    float width, height;
    /* size of the glyph  */
    float xadvance;
    /* offset to the next glyph */
};
#endif

struct zr_user_font {
    zr_handle userdata;
    /* user provided font handle */
    float height;
    /* max height of the font */
    zr_text_width_f width;
    /* font string width in pixel callback */
#if ZR_COMPILE_WITH_VERTEX_BUFFER
    zr_query_font_glyph_f query;
    /* font glyph callback to query drawing info */
    zr_handle texture;
    /* texture handle to the used font atlas or texture */
#endif
};

#ifdef ZR_COMPILE_WITH_FONT
enum zr_font_coord_type {
    ZR_COORD_UV,
    /* texture coordinates inside font glyphs are clamped between 0.0f and 1.0f */
    ZR_COORD_PIXEL
    /* texture coordinates inside font glyphs are in absolute pixel */
};

struct zr_baked_font {
    float height;
    /* height of the font  */
    float ascent, descent;
    /* font glyphs ascent and descent  */
    zr_rune glyph_offset;
    /* glyph array offset inside the font glyph baking output array  */
    zr_rune glyph_count;
    /* number of glyphs of this font inside the glyph baking array output */
    const zr_rune *ranges;
    /* font codepoint ranges as pairs of (from/to) and 0 as last element */
};

struct zr_font_config {
    void *ttf_blob;
    /* pointer to loaded TTF file memory block */
    zr_size ttf_size;
    /* size of the loaded TTF file memory block */
    float size;
    /* bake pixel height of the font */
    zr_rune oversample_h, oversample_v;
    /* rasterize at hight quality for sub-pixel position */
    int pixel_snap;
    /* align very character to pixel boundry (if true set oversample (1,1)) */
    enum zr_font_coord_type coord_type;
    /* baked glyph texture coordinate format with either pixel or UV coordinates */
    struct zr_vec2 spacing;
    /* extra pixel spacing between glyphs  */
    const zr_rune *range;
    /* list of unicode ranges (2 values per range, zero terminated) */
    struct zr_baked_font *font;
    /* font to setup in the baking process  */
};

struct zr_font_glyph {
    zr_rune codepoint;
    /* unicode codepoint */
    float xadvance;
    /* xoffset to the next character  */
    float x0, y0, x1, y1;
    /* glyph bounding points in pixel inside the glyph image with top left and bottom right */
    float u0, v0, u1, v1;
    /* texture coordinates either in pixel or clamped (0.0 - 1.0) */
};

struct zr_font {
    float size;
    /* pixel height of the font */
    float scale;
    /* scale factor for different font size */
    float ascent, descent;
    /* font ascent and descent  */
    struct zr_font_glyph *glyphs;
    /* font glyph array  */
    const struct zr_font_glyph *fallback;
    /* fallback glyph */
    zr_rune fallback_codepoint;
    /* fallback glyph codepoint */
    zr_rune glyph_count;
    /* font glyph array size */
    const zr_rune *ranges;
    /* glyph unicode ranges in the font */
    zr_handle atlas;
    /* font image atlas handle */
};

/* some language glyph codepoint ranges */
const zr_rune *zr_font_default_glyph_ranges(void);
const zr_rune *zr_font_chinese_glyph_ranges(void);
const zr_rune *zr_font_cyrillic_glyph_ranges(void);
const zr_rune *zr_font_korean_glyph_ranges(void);

/* font baking functions (need to be called sequentially top to bottom) */
void zr_font_bake_memory(zr_size *temporary_memory, int *glyph_count,
                            struct zr_font_config*, int count);
/*  this function calculates the needed memory for the baking process
    Input:
    - array of configuration for every font that should be baked into one image
    - number of configuration fonts in the array
    Output:
    - amount of memory needed in the baking process
    - total number of glyphs that need to be allocated
*/
int zr_font_bake_pack(zr_size *img_memory, int *img_width, int *img_height,
                            struct zr_recti *custom_space,
                            void *temporary_memory, zr_size temporary_size,
                            const struct zr_font_config*, int font_count);
/*  this function packs together all glyphs and optional space into one
    total image space and returns the needed image width and height.
    Input:
    - NULL or custom space inside the image with width and height (will be updated!)
    - temporary memory block that will be used in the baking process
    - size of the temporary memory block
    - array of configuration for every font that should be baked into one image
    - number of configuration fonts in the array
    Output:
    - calculated resulting size of the image in bytes
    - pixel width of the resulting image
    - pixel height of the resulting image
    - custom space bounds with position and size inside image which can be filled by the user
*/
void zr_font_bake(void *image_memory, int image_width, int image_height,
                    void *temporary_memory, zr_size temporary_memory_size,
                    struct zr_font_glyph*, int glyphs_count,
                    const struct zr_font_config*, int font_count);
/*  this function bakes all glyphs into the pre-allocated image and
    fills a glyph array with information.
    Input:
    - image memory buffer to bake the glyph into
    - pixel width/height of the image
    - temporary memory block that will be used in the baking process
    - size of the temporary memory block
    Output:
    - image filled with glyphs
    - filled glyph array
*/
void zr_font_bake_custom_data(void *img_memory, int img_width, int img_height,
                                struct zr_recti img_dst, const char *texture_data_mask,
                                int tex_width, int tex_height, char white, char black);
/*  this function bakes custom data in string format with white, black and zero
    alpha pixels into the font image. The zero alpha pixel is represented as
    any character beside the black and zero pixel character.
    Input:
    - image memory buffer to bake the custom data into
    - image size (width/height) of the image in pixels
    - custom texture data in string format
    - texture size (width/height) of the custom image content
    - character representing a white pixel in the texture data format
    - character representing a black pixel in the texture data format
    Output:
    - image filled with custom texture data
*/
void zr_font_bake_convert(void *out_memory, int image_width, int image_height, const void *in_memory);
/*  this function converts the alpha8 baking input image into a preallocated rgba8 output image.*/
void zr_font_init(struct zr_font*, float pixel_height, zr_rune fallback_codepoint,
                    struct zr_font_glyph*, const struct zr_baked_font*, zr_handle atlas);
struct zr_user_font zr_font_ref(struct zr_font*);
const struct zr_font_glyph* zr_font_find_glyph(struct zr_font*, zr_rune unicode);

#endif

/* ===============================================================
 *
 *                          RENDERING
 *
 * ===============================================================*/
/*  This library was designed to be render backend agnostic so it does
    not draw anything to the screen. Instead all drawn primitives, widgets
    are made of, are buffered into memory and make up a command queue.
    Each frame therefore fills the command buffer with draw commands
    that then need to be executed by the user and his own render backend.
    After that the command buffer needs to be cleared and a new frame can be started.

    The reason for buffering simple primitives as draw commands instead of
    directly buffering a hardware accessible format with vertex and element
    buffer was to support native render backends like X11 and Win32.
    That being said it is possible to convert the command buffer into a
    hardware accessible format to support hardware based rendering as well.
*/
enum zr_command_type {
    ZR_COMMAND_NOP,
    ZR_COMMAND_SCISSOR,
    ZR_COMMAND_LINE,
    ZR_COMMAND_CURVE,
    ZR_COMMAND_RECT,
    ZR_COMMAND_CIRCLE,
    ZR_COMMAND_ARC,
    ZR_COMMAND_TRIANGLE,
    ZR_COMMAND_TEXT,
    ZR_COMMAND_IMAGE
};

/* command base and header of every comand inside the buffer */
struct zr_command {
    enum zr_command_type type;
    /* the type of the current command */
    zr_size next;
    /* absolute base pointer offset to the next command */
};

struct zr_command_scissor {
    struct zr_command header;
    short x, y;
    unsigned short w, h;
};

struct zr_command_line {
    struct zr_command header;
    struct zr_vec2i begin;
    struct zr_vec2i end;
    struct zr_color color;
};

struct zr_command_curve {
    struct zr_command header;
    struct zr_vec2i begin;
    struct zr_vec2i end;
    struct zr_vec2i ctrl[2];
    struct zr_color color;
};

struct zr_command_rect {
    struct zr_command header;
    unsigned int rounding;
    short x, y;
    unsigned short w, h;
    struct zr_color color;
};

struct zr_command_circle {
    struct zr_command header;
    short x, y;
    unsigned short w, h;
    struct zr_color color;
};

struct zr_command_arc {
    struct zr_command header;
    short cx, cy;
    unsigned short r;
    float a[2];
    struct zr_color color;
};

struct zr_command_triangle {
    struct zr_command header;
    struct zr_vec2i a;
    struct zr_vec2i b;
    struct zr_vec2i c;
    struct zr_color color;
};

struct zr_command_image {
    struct zr_command header;
    short x, y;
    unsigned short w, h;
    struct zr_image img;
};

struct zr_command_text {
    struct zr_command header;
    const struct zr_user_font *font;
    struct zr_color background;
    struct zr_color foreground;
    short x, y;
    unsigned short w, h;
    float height;
    zr_size length;
    char string[1];
};

enum zr_command_clipping {
    ZR_CLIPPING_OFF = zr_false,
    ZR_CLIPPING_ON = zr_true
};

struct zr_command_buffer {
    struct zr_buffer *base;
    /* memory buffer to store the command */
    struct zr_rect clip;
    /* current clipping rectangle */
    int use_clipping;
    /* flag if the command buffer should clip commands */
    zr_size begin, end, last;
};

#if ZR_COMPILE_WITH_VERTEX_BUFFER
typedef unsigned short zr_draw_index;
typedef zr_uint zr_draw_vertex_color;

enum zr_anti_aliasing {
    ZR_ANTI_ALIASING_OFF = zr_false,
    /* renderes all primitives without anti-aliasing  */
    ZR_ANTI_ALIASING_ON
    /* renderes all primitives with anti-aliasing  */
};

struct zr_draw_vertex {
    struct zr_vec2 position;
    struct zr_vec2 uv;
    zr_draw_vertex_color col;
};

struct zr_draw_command {
    unsigned int elem_count;
    /* number of elements in the current draw batch */
    struct zr_rect clip_rect;
    /* current screen clipping rectangle */
    zr_handle texture;
    /* current texture to set */
};

struct zr_draw_null_texture {
    zr_handle texture;
    /* texture handle to a texture with a white pixel */
    struct zr_vec2 uv;
    /* coordinates to the white pixel in the texture  */
};

/* drawing routines for custom widgets */
void zr_draw_scissor(struct zr_command_buffer*, struct zr_rect);
void zr_draw_line(struct zr_command_buffer*, float, float, float, float, struct zr_color);
void zr_draw_curve(struct zr_command_buffer*, float, float, float, float,  float, float,
                    float, float, struct zr_color);
void zr_draw_rect(struct zr_command_buffer*, struct zr_rect, float rounding, struct zr_color);
void zr_draw_circle(struct zr_command_buffer*, struct zr_rect, struct zr_color c);
void zr_draw_arc(struct zr_command_buffer*, float cx, float cy, float radius, float a_min,
                    float a_max, struct zr_color);
void zr_draw_triangle(struct zr_command_buffer*, float, float, float, float, float, float, struct zr_color);
void zr_draw_image(struct zr_command_buffer*, struct zr_rect, struct zr_image*);
void zr_draw_text(struct zr_command_buffer*, struct zr_rect, const char*, zr_size,
                    const struct zr_user_font*, struct zr_color, struct zr_color);

#endif
/* ===============================================================
 *
 *                          GUI
 *
 * ===============================================================*/
enum zr_keys {
    ZR_KEY_SHIFT,
    ZR_KEY_DEL,
    ZR_KEY_ENTER,
    ZR_KEY_TAB,
    ZR_KEY_BACKSPACE,
    ZR_KEY_COPY,
    ZR_KEY_CUT,
    ZR_KEY_PASTE,
    ZR_KEY_LEFT,
    ZR_KEY_RIGHT,
    ZR_KEY_MAX
};

/* every used mouse button */
enum zr_buttons {
    ZR_BUTTON_LEFT,
    ZR_BUTTON_MIDDLE,
    ZR_BUTTON_RIGHT,
    ZR_BUTTON_MAX
};

struct zr_mouse_button {
    int down;
    /* current button state */
    unsigned int clicked;
    /* button state change */
    struct zr_vec2 clicked_pos;
    /* mouse position of last state change */
};

struct zr_mouse {
    struct zr_mouse_button buttons[ZR_BUTTON_MAX];
    /* mouse button states */
    struct zr_vec2 pos;
    /* current mouse position */
    struct zr_vec2 prev;
    /* mouse position in the last frame */
    struct zr_vec2 delta;
    /* mouse travelling distance from last to current frame */
    float scroll_delta;
    /* number of steps in the up or down scroll direction */
};

struct zr_key {
    int down;
    unsigned int clicked;
};

struct zr_keyboard {
    struct zr_key keys[ZR_KEY_MAX];
    /* state of every used key */
    char text[ZR_INPUT_MAX];
    /* utf8 text input frame buffer */
    zr_size text_len;
    /* text input frame buffer length in bytes */
};

struct zr_input {
    struct zr_keyboard keyboard;
    /* current keyboard key + text input state */
    struct zr_mouse mouse;
    /* current mouse button and position state */
};

/* query input state */
int zr_input_has_mouse_click_in_rect(const struct zr_input*,enum zr_buttons, struct zr_rect);
int zr_input_has_mouse_click_down_in_rect(const struct zr_input*, enum zr_buttons,
                                        struct zr_rect, int down);
int zr_input_is_mouse_click_in_rect(const struct zr_input*, enum zr_buttons, struct zr_rect);
int zr_input_any_mouse_click_in_rect(const struct zr_input*, struct zr_rect);
int zr_input_is_mouse_prev_hovering_rect(const struct zr_input*, struct zr_rect);
int zr_input_is_mouse_hovering_rect(const struct zr_input*, struct zr_rect);
int zr_input_mouse_clicked(const struct zr_input*, enum zr_buttons, struct zr_rect);
int zr_input_is_mouse_down(const struct zr_input*, enum zr_buttons);
int zr_input_is_mouse_pressed(const struct zr_input*, enum zr_buttons);
int zr_input_is_mouse_released(const struct zr_input*, enum zr_buttons);
int zr_input_is_key_pressed(const struct zr_input*, enum zr_keys);
int zr_input_is_key_released(const struct zr_input*, enum zr_keys);
int zr_input_is_key_down(const struct zr_input*, enum zr_keys);

/* ==============================================================
 *                          STYLE
 * ===============================================================*/
enum zr_style_colors {
    ZR_COLOR_TEXT,
    ZR_COLOR_TEXT_HOVERING,
    ZR_COLOR_TEXT_ACTIVE,
    ZR_COLOR_WINDOW,
    ZR_COLOR_HEADER,
    ZR_COLOR_BORDER,
    ZR_COLOR_BUTTON,
    ZR_COLOR_BUTTON_HOVER,
    ZR_COLOR_BUTTON_ACTIVE,
    ZR_COLOR_TOGGLE,
    ZR_COLOR_TOGGLE_HOVER,
    ZR_COLOR_TOGGLE_CURSOR,
    ZR_COLOR_SELECTABLE,
    ZR_COLOR_SELECTABLE_HOVER,
    ZR_COLOR_SELECTABLE_TEXT,
    ZR_COLOR_SLIDER,
    ZR_COLOR_SLIDER_CURSOR,
    ZR_COLOR_SLIDER_CURSOR_HOVER,
    ZR_COLOR_SLIDER_CURSOR_ACTIVE,
    ZR_COLOR_PROGRESS,
    ZR_COLOR_PROGRESS_CURSOR,
    ZR_COLOR_PROGRESS_CURSOR_HOVER,
    ZR_COLOR_PROGRESS_CURSOR_ACTIVE,
    ZR_COLOR_PROPERTY,
    ZR_COLOR_PROPERTY_HOVER,
    ZR_COLOR_PROPERTY_ACTIVE,
    ZR_COLOR_INPUT,
    ZR_COLOR_INPUT_CURSOR,
    ZR_COLOR_INPUT_TEXT,
    ZR_COLOR_COMBO,
    ZR_COLOR_HISTO,
    ZR_COLOR_HISTO_BARS,
    ZR_COLOR_HISTO_HIGHLIGHT,
    ZR_COLOR_PLOT,
    ZR_COLOR_PLOT_LINES,
    ZR_COLOR_PLOT_HIGHLIGHT,
    ZR_COLOR_SCROLLBAR,
    ZR_COLOR_SCROLLBAR_CURSOR,
    ZR_COLOR_SCROLLBAR_CURSOR_HOVER,
    ZR_COLOR_SCROLLBAR_CURSOR_ACTIVE,
    ZR_COLOR_TABLE_LINES,
    ZR_COLOR_TAB_HEADER,
    ZR_COLOR_SCALER,
    ZR_COLOR_COUNT
};

enum zr_style_rounding {
    ZR_ROUNDING_BUTTON,
    ZR_ROUNDING_SLIDER,
    ZR_ROUNDING_PROGRESS,
    ZR_ROUNDING_CHECK,
    ZR_ROUNDING_INPUT,
    ZR_ROUNDING_PROPERTY,
    ZR_ROUNDING_CHART,
    ZR_ROUNDING_SCROLLBAR,
    ZR_ROUNDING_MAX
};

enum zr_style_properties {
    ZR_PROPERTY_ITEM_SPACING,
    ZR_PROPERTY_ITEM_PADDING,
    ZR_PROPERTY_PADDING,
    ZR_PROPERTY_SCALER_SIZE,
    ZR_PROPERTY_SCROLLBAR_SIZE,
    ZR_PROPERTY_SIZE,
    ZR_PROPERTY_MAX
};

enum zr_style_header_align {
    ZR_HEADER_LEFT,
    ZR_HEADER_RIGHT
};

enum zr_style_components {
    ZR_DEFAULT_COLOR = 0x01,
    /* default all colors inside the configuration struct */
    ZR_DEFAULT_PROPERTIES = 0x02,
    /* default all properites inside the configuration struct */
    ZR_DEFAULT_ROUNDING = 0x04,
    /* default all rounding values inside the configuration struct */
    ZR_DEFAULT_ALL = 0xFFFF
    /* default the complete configuration struct */
};

struct zr_saved_property {
    enum zr_style_properties type;
    /* identifier of the current modified property */
    struct zr_vec2 value;
    /* property value that has been saveed */
};

struct zr_saved_color {
    enum zr_style_colors type;
    /* identifier of the current modified color */
    struct zr_color value;
    /* color value that has been saveed */
};

struct zr_saved_font {
    struct zr_user_font value;
    /* user font reference */
    int font_height_begin;
    /* style font height stack begin */
    int font_height_end;
    /* user font height stack end */
};

struct zr_style_mod_stack  {
    zr_size property;
    /* current property stack pushing index */
    struct zr_saved_property properties[ZR_MAX_ATTRIB_STACK];
    /* saved property stack */
    int color;
    /* current color stack pushing index */
    struct zr_saved_color colors[ZR_MAX_COLOR_STACK];
    /* saved color stack */
    int font;
    /* current font stack pushing index */
    struct zr_saved_font fonts[ZR_MAX_FONT_STACK];
    /* saved user font stack */
    int font_height;
    /* current font stack pushing index */
    float font_heights[ZR_MAX_FONT_HEIGHT_STACK];
    /* saved user font stack */
};

struct zr_style_header {
    enum zr_style_header_align align;
    /* header content alignment */
    zr_rune close_symbol;
    /* header close icon unicode rune */
    zr_rune minimize_symbol;
    /* header minimize icon unicode rune */
    zr_rune maximize_symbol;
    /* header maximize icon unicode rune */
};

struct zr_style {
    struct zr_user_font font;
    /* the from the user provided font */
    float rounding[ZR_ROUNDING_MAX];
    /* rectangle widget rounding */
    struct zr_style_header header;
    /* window header style */
    struct zr_vec2 properties[ZR_PROPERTY_MAX];
    /* configuration properties to modify the style */
    struct zr_color colors[ZR_COLOR_COUNT];
    /* configuration color to modify color */
    struct zr_style_mod_stack stack;
    /* modification stack */
};

/*===============================================================
 *                          EDIT BOX
 * ===============================================================*/
typedef int(*zr_filter)(zr_rune unicode);
typedef void(*zr_paste_f)(zr_handle, struct zr_edit_box*);
typedef void(*zr_copy_f)(zr_handle, const char*, zr_size size);

/* filter function */
int zr_filter_default(zr_rune unicode);
int zr_filter_ascii(zr_rune unicode);
int zr_filter_float(zr_rune unicode);
int zr_filter_decimal(zr_rune unicode);
int zr_filter_hex(zr_rune unicode);
int zr_filter_oct(zr_rune unicode);
int zr_filter_binary(zr_rune unicode);

/* editbox */
struct zr_edit_box;
void zr_edit_box_clear(struct zr_edit_box*);
void zr_edit_box_add(struct zr_edit_box*, const char*, zr_size);
void zr_edit_box_remove(struct zr_edit_box*);
char *zr_edit_box_get(struct zr_edit_box*);
const char *zr_edit_box_get_const(struct zr_edit_box*);
void zr_edit_box_at(struct zr_edit_box*, zr_size pos, zr_glyph, zr_size*);
void zr_edit_box_at_cursor(struct zr_edit_box*, zr_glyph, zr_size*);
char zr_edit_box_at_char(struct zr_edit_box*, zr_size pos);
void zr_edit_box_set_cursor(struct zr_edit_box*, zr_size pos);
zr_size zr_edit_box_get_cursor(struct zr_edit_box *eb);
zr_size zr_edit_box_len_char(struct zr_edit_box*);
zr_size zr_edit_box_len(struct zr_edit_box*);

/*==============================================================
 *                          WINDOW
 * =============================================================*/
#define ZR_UNDEFINED (-1.0f)
#define ZR_FLAG(x) (1 << (x))

enum zr_modify {
    ZR_FIXED = zr_false,
    ZR_MODIFIABLE = zr_true
};

enum zr_symbol {
    ZR_SYMBOL_X,
    ZR_SYMBOL_UNDERSCORE,
    ZR_SYMBOL_CIRCLE,
    ZR_SYMBOL_CIRCLE_FILLED,
    ZR_SYMBOL_RECT,
    ZR_SYMBOL_RECT_FILLED,
    ZR_SYMBOL_TRIANGLE_UP,
    ZR_SYMBOL_TRIANGLE_DOWN,
    ZR_SYMBOL_TRIANGLE_LEFT,
    ZR_SYMBOL_TRIANGLE_RIGHT,
    ZR_SYMBOL_PLUS,
    ZR_SYMBOL_MINUS,
    ZR_SYMBOL_MAX
};

enum zr_widget_status {
    ZR_INACTIVE,
    ZR_HOVERED,
    ZR_ACTIVE
};

enum zr_collapse_states {
    ZR_MINIMIZED = zr_false,
    ZR_MAXIMIZED = zr_true
};

enum zr_widget_state {
    ZR_WIDGET_INVALID,
    /* The widget cannot be seen and is completly out of view */
    ZR_WIDGET_VALID,
    /* The widget is completly inside the window can be updated */
    ZR_WIDGET_ROM
    /* The widget is partially visible and cannot be updated */
};

enum zr_text_align {
    ZR_TEXT_LEFT,
    /* text is aligned on the left */
    ZR_TEXT_CENTERED,
    /* text is aligned in the center */
    ZR_TEXT_RIGHT
    /* text is aligned on the right */
};

enum zr_button_behavior {
    ZR_BUTTON_DEFAULT,
    /* default push button behavior */
    ZR_BUTTON_REPEATER
    /* repeater behavior will trigger as long as button is down */
};

enum zr_edit_flags {
    ZR_EDIT_READ_ONLY   = ZR_FLAG(0),
    ZR_EDIT_CURSOR      = ZR_FLAG(1),
    ZR_EDIT_SELECTABLE  = ZR_FLAG(2),
    ZR_EDIT_CLIPBOARD   = ZR_FLAG(3),
    ZR_EDIT_MULTILINE   = ZR_FLAG(4)
};

enum zr_edit_types {
    ZR_EDIT_SIMPLE = 0,
    ZR_EDIT_FIELD = (ZR_EDIT_CURSOR|ZR_EDIT_SELECTABLE|ZR_EDIT_CLIPBOARD),
    ZR_EDIT_BOX = (ZR_EDIT_CURSOR|ZR_EDIT_SELECTABLE|ZR_EDIT_CLIPBOARD|ZR_EDIT_MULTILINE)
};

enum zr_chart_type {
    ZR_CHART_LINES,
    /* Line chart with each data point being connected with its previous and next node */
    ZR_CHART_COLUMN,
    /* Histogram with value represented as bars */
    ZR_CHART_MAX
};

enum zr_chart_event {
    ZR_CHART_HOVERING = 0x01,
    /* mouse hoveres over current value */
    ZR_CHART_CLICKED = 0x02
    /* mouse click on current value */
};

enum zr_popup_type {
    ZR_POPUP_STATIC,
    /* static fixed height non growing popup */
    ZR_POPUP_DYNAMIC
    /* dynamically growing popup with maximum height */
};

enum zr_layout_format {
    ZR_DYNAMIC, /* row layout which scales with the window */
    ZR_STATIC /* row layout with fixed pixel width */
};

enum zr_layout_node_type {
    ZR_LAYOUT_NODE,
    /* a node is a space which can be minimized or maximized */
    ZR_LAYOUT_TAB
    /* a tab is a node with a header */
};

struct zr_chart {
    enum zr_chart_type type;
    /* chart type with either line or column chart */
    float x, y;
    /* chart canvas space position */
    float w, h;
    /* chart canvas space size */
    float min, max, range;
    /* min and max value for correct scaling of values */
    struct zr_vec2 last;
    /* last line chart point to connect to. Only used by the line chart */
    zr_size index;
    /* current chart value index*/
    zr_size count;
    /* number of values inside the chart */
};

enum zr_row_layout_type {
    ZR_LAYOUT_DYNAMIC_FIXED,
    /* fixed widget ratio width window layout */
    ZR_LAYOUT_DYNAMIC_ROW,
    /* immediate mode widget specific widget width ratio layout */
    ZR_LAYOUT_DYNAMIC_FREE,
    /* free ratio based placing of widget in a local space  */
    ZR_LAYOUT_DYNAMIC,
    /* retain mode widget specific widget ratio width*/
    ZR_LAYOUT_STATIC_FIXED,
    /* fixed widget pixel width window layout */
    ZR_LAYOUT_STATIC_ROW,
    /* immediate mode widget specific widget pixel width layout */
    ZR_LAYOUT_STATIC_FREE,
    /* free pixel based placing of widget in a local space  */
    ZR_LAYOUT_STATIC
    /* retain mode widget specific widget pixel width layout */
};

struct zr_row_layout {
    enum zr_row_layout_type type;
    /* type of the row layout */
    zr_size index;
    /* index of the current widget in the current window row */
    float height;
    /* height of the current row */
    zr_size columns;
    /* number of columns in the current row */
    const float *ratio;
    /* row widget width ratio */
    float item_width, item_height;
    /* current width of very item */
    float item_offset;
    /* x positon offset of the current item */
    float filled;
    /* total fill ratio */
    struct zr_rect item;
    /* item bounds */
};

struct zr_menu {
    float x, y, w, h;
    /* menu bounds */
    struct zr_scroll offset;
    /* saved window scrollbar offset */
};

enum zr_window_flags {
    ZR_WINDOW_BORDER        = ZR_FLAG(0),
    /* Draws a border around the window to visually seperate the window from the background */
    ZR_WINDOW_BORDER_HEADER = ZR_FLAG(1),
    /* Draws a border between window header and body */
    ZR_WINDOW_MOVABLE      = ZR_FLAG(2),
    /* The moveable flag inidicates that a window can be moved by user input or by
     * dragging the window header */
    ZR_WINDOW_SCALABLE     = ZR_FLAG(3),
    /* The scaleable flag indicates that a window can be scaled by user input
     * by dragging a scaler icon at the button of the window */
    ZR_WINDOW_CLOSABLE     = ZR_FLAG(4),
    /* adds a closeable icon into the header */
    ZR_WINDOW_MINIMIZABLE   = ZR_FLAG(5),
    /* adds a minimize icon into the header */
    ZR_WINDOW_DYNAMIC       = ZR_FLAG(6),
    /* special type of window which grows up in height while being filled to a
     * certain maximum height. It is mainly used for combo boxes/menus but can be
     * used to create perfectly fitting windows as well */
    ZR_WINDOW_NO_SCROLLBAR  = ZR_FLAG(7),
    /* Removes the scrollbar from the window */
    ZR_WINDOW_TITLE         = ZR_FLAG(8)
    /* Removes the scrollbar from the window */
};

struct zr_popup_buffer {
    zr_size begin;
    /* begin of the subbuffer */
    zr_size parent;
    /* last entry before the sub buffer*/
    zr_size last;
    /* last entry in the sub buffer*/
    zr_size end;
    /* end of the subbuffer */
    int active;
};

struct zr_layout {
    zr_flags flags;
    /* window flags modifing its behavior */
    struct zr_rect bounds;
    /* position and size of the window in the os window */
    struct zr_scroll *offset;
    /* window scrollbar offset */
    float at_x, at_y, max_x;
    /* index position of the current widget row and column  */
    float width, height;
    /* size of the actual useable space inside the window */
    float footer_h;
    /* height of the window footer space */
    float header_h;
    /* height of the window footer space */
    struct zr_rect clip;
    /* window clipping rect */
    struct zr_menu menu;
    /* window menubar bounds */
    struct zr_row_layout row;
    /* currently used window row layout */
    struct zr_chart chart;
    /* chart state */
    struct zr_popup_buffer popup_buffer;
    /* output command buffer queuing all popup drawing calls */
    struct zr_command_buffer *buffer;
    struct zr_layout *parent;
};

/*==============================================================
 *                          CONTEXT
 * =============================================================*/
struct zr_clipboard {
    zr_handle userdata;
    /* user memory for callback */
    zr_paste_f paste;
    /* paste callback for the edit box  */
    zr_copy_f copy;
    /* copy callback for the edit box  */
};

struct zr_canvas {
    enum zr_anti_aliasing AA;
    /* flag indicating if anti-aliasing should be used to render primtives */
    struct zr_draw_null_texture null;
    /* texture with white pixel for easy primitive drawing */
    struct zr_rect clip_rect;
    /* current clipping rectangle */
    /* cosine/sine calculation callback since this library does not use libc  */
    struct zr_buffer *buffer;
    /* buffer to store draw commands and temporarily store path */
    struct zr_buffer *vertexes;
    /* buffer to store each draw vertex */
    struct zr_buffer *elements;
    /* buffer to store each draw element index */
    unsigned int element_count;
    /* total number of elements inside the elements buffer */
    unsigned int vertex_count;
    /* total number of vertexes inside the vertex buffer */
    zr_size cmd_offset;
    /* offset to the first command in the buffer */
    unsigned int cmd_count;
    /* number of commands inside the buffer  */
    unsigned int path_count;
    /* current number of points inside the path */
    unsigned int path_offset;
    /* offset to the first point in the buffer */
    struct zr_vec2 circle_vtx[12];
    /* small lookup table for fast circle drawing */
};

struct zr_context {
    unsigned int seq;
    struct zr_input input;
    struct zr_style style;
    struct zr_buffer memory;
    struct zr_clipboard clip;
    void *pool;

#if ZR_COMPILE_WITH_VERTEX_BUFFER
    struct zr_canvas canvas;
#endif

    int build;
    struct zr_window *begin;
    struct zr_window *end;
    struct zr_window *active;
    struct zr_window *current;
    struct zr_window *freelist;
    unsigned int count;
};

/*--------------------------------------------------------------
 *                          CONTEXT
 * -------------------------------------------------------------*/
int zr_init_fixed(struct zr_context*, void *memory, zr_size size, const struct zr_user_font*);
int zr_init_custom(struct zr_context*, struct zr_buffer *cmds,
                    struct zr_buffer *pool, const struct zr_user_font*);
int zr_init(struct zr_context*, struct zr_allocator*, const struct zr_user_font*);
void zr_clear(struct zr_context*);
void zr_free(struct zr_context*);

/* window */
int zr_begin(struct zr_context*, struct zr_layout*, const char *title,
            struct zr_rect bounds, unsigned int flags);
void zr_end(struct zr_context*);

struct zr_rect zr_window_get_bounds(const struct zr_context*);
struct zr_vec2 zr_window_get_position(const struct zr_context*);
struct zr_vec2 zr_window_get_size(const struct zr_context*);
float zr_window_get_width(const struct zr_context*);
float zr_window_get_height(const struct zr_context*);
struct zr_rect zr_window_get_content_region(struct zr_context*);
struct zr_vec2 zr_window_get_content_region_min(struct zr_context*);
struct zr_vec2 zr_window_get_content_region_max(struct zr_context*);
struct zr_vec2 zr_window_get_content_region_size(struct zr_context*);
struct zr_command_buffer* zr_window_get_canvas(struct zr_context*);
int zr_window_has_focus(const struct zr_context*);
int zr_window_is_collapsed(struct zr_context*, const char *name);
int zr_window_is_closed(struct zr_context*, const char *name);
int zr_window_is_active(struct zr_context*, const char *name);

void zr_window_set_bounds(struct zr_context*, struct zr_rect);
void zr_window_set_position(struct zr_context*, struct zr_vec2);
void zr_window_set_size(struct zr_context*, struct zr_vec2);
void zr_window_collapse(struct zr_context *ctx, const char *name, enum zr_collapse_states);
void zr_window_collapse_if(struct zr_context *ctx, const char *name, enum zr_collapse_states, int cond);
void zr_window_set_focus(struct zr_context *ctx, const char *name);

/*--------------------------------------------------------------
 *                      DRAWING
 * -------------------------------------------------------------*/
/* command drawing */
#define zr_command(t, c) ((const struct zr_command_##t*)c)
#define zr_foreach(c, ctx) for((c)=zr__begin(ctx); (c)!=0; (c)=zr__next(ctx, c))
const struct zr_command* zr__next(struct zr_context*, const struct zr_command*);
const struct zr_command* zr__begin(struct zr_context*);

/* vertex command drawing */
#define zr_draw_foreach(cmd,ctx, b) for((cmd)=zr__draw_begin(ctx, b); (cmd)!=0; (cmd)=zr__draw_next(cmd, b, ctx))
void zr_convert(struct zr_context*, struct zr_buffer *cmds,
                struct zr_buffer *vertexes, struct zr_buffer *elements,
                struct zr_draw_null_texture , enum zr_anti_aliasing,
                float line_thickness, unsigned int circle_segment_count);
const struct zr_draw_command* zr__draw_begin(const struct zr_context*, const struct zr_buffer*);
const struct zr_draw_command* zr__draw_next(const struct zr_draw_command*,
                                        const struct zr_buffer*, const struct zr_context*);

/*--------------------------------------------------------------
 *                      INPUT
 * -------------------------------------------------------------*/
void zr_input_begin(struct zr_context*);
void zr_input_motion(struct zr_context*, int x, int y);
void zr_input_key(struct zr_context*, enum zr_keys, int down);
void zr_input_button(struct zr_context*, enum zr_buttons, int x, int y, int down);
void zr_input_scroll(struct zr_context*, float y);
void zr_input_glyph(struct zr_context*, const zr_glyph);
void zr_input_char(struct zr_context*, char);
void zr_input_unicode(struct zr_context *in, zr_rune unicode);
void zr_input_end(struct zr_context*);

/*--------------------------------------------------------------
 *                      STYLE
 * -------------------------------------------------------------*/
void zr_load_default_style(struct zr_context*, zr_flags);
void zr_set_font(struct zr_context*, const struct zr_user_font*);
struct zr_vec2 zr_get_property(const struct zr_context*, enum zr_style_properties);
struct zr_color zr_get_color(const struct zr_context*, enum zr_style_colors);
const char *zr_get_color_name(enum zr_style_colors);
const char *zr_get_rounding_name(enum zr_style_rounding);
const char *zr_get_property_name(enum zr_style_properties);

/* temporarily modify a style value and save the old value in a stack*/
void zr_push_property(struct zr_context*, enum zr_style_properties, struct zr_vec2);
void zr_push_color(struct zr_context*, enum zr_style_colors, struct zr_color);
void zr_push_font(struct zr_context*, struct zr_user_font font);
void zr_push_font_height(struct zr_context*, float font_height);

/* restores a previously saved style value */
void zr_pop_color(struct zr_context*);
void zr_pop_property(struct zr_context*);
void zr_pop_font(struct zr_context*);
void zr_pop_font_height(struct zr_context*);

/* resets the style back into the beginning state */
void zr_reset_colors(struct zr_context*);
void zr_reset_properties(struct zr_context*);
void zr_reset_font(struct zr_context*);
void zr_reset_font_height(struct zr_context*);
void zr_reset(struct zr_context*);

/*--------------------------------------------------------------
 *                      Layout
 * -------------------------------------------------------------*/
void zr_layout_peek(struct zr_rect *bounds, struct zr_context*);

/* columns based layouting with generated position and width and fixed height*/
void zr_layout_row_dynamic(struct zr_context*, float height, zr_size cols);
void zr_layout_row_static(struct zr_context*, float height, zr_size item_width, zr_size cols);

/* widget layouting with custom widget width and fixed height */
void zr_layout_row_begin(struct zr_context*, enum zr_layout_format,
                        float row_height, zr_size cols);
void zr_layout_row_push(struct zr_context*, float value);
void zr_layout_row_end(struct zr_context*);
void zr_layout_row(struct zr_context*, enum zr_layout_format, float height,
                    zr_size cols, const float *ratio);

/* layouting with custom position and size of widgets */
void zr_layout_space_begin(struct zr_context*, enum zr_layout_format,
                            float height, zr_size widget_count);
void zr_layout_space_push(struct zr_context*, struct zr_rect);
void zr_layout_space_end(struct zr_context*);

struct zr_rect zr_layout_space_bounds(struct zr_context*);
struct zr_vec2 zr_layout_space_to_screen(struct zr_context*, struct zr_vec2);
struct zr_vec2 zr_layout_space_to_local(struct zr_context*, struct zr_vec2);
struct zr_rect zr_layout_space_rect_to_screen(struct zr_context*, struct zr_rect);
struct zr_rect zr_layout_space_rect_to_local(struct zr_context*, struct zr_rect);

/* group layout */
int zr_group_begin(struct zr_context*, struct zr_layout*, const char *title, zr_flags);
void zr_group_end(struct zr_context *ctx);

/* tree layout */
int zr_layout_push(struct zr_context*, enum zr_layout_node_type, const char *title);
void zr_layout_pop(struct zr_context*);

/*--------------------------------------------------------------
 *                      Widgets
 * -------------------------------------------------------------*/
enum zr_widget_state zr_widget(struct zr_rect*, const struct zr_context*);
enum zr_widget_state zr_widget_fitting(struct zr_rect*, struct zr_context*);
void zr_spacing(struct zr_context*, zr_size cols);
void zr_seperator(struct zr_context*);

/* content output widgets */
void zr_text(struct zr_context*, const char*, zr_size, enum zr_text_align);
void zr_text_colored(struct zr_context*, const char*, zr_size, enum zr_text_align,
                    struct zr_color);
void zr_text_wrap(struct zr_context*, const char*, zr_size);
void zr_text_wrap_colored(struct zr_context*, const char*, zr_size, struct zr_color);
void zr_label(struct zr_context*, const char*, enum zr_text_align);
void zr_label_colored(struct zr_context*, const char*, enum zr_text_align, struct zr_color);
void zr_label_wrap(struct zr_context*, const char*);
void zr_label_colored_wrap(struct zr_context*, const char*, struct zr_color);
void zr_image(struct zr_context*, struct zr_image);

/* toggle value  */
int zr_check(struct zr_context*, const char*, int active);
void zr_checkbox(struct zr_context*, const char*, int *active);
void zr_radio(struct zr_context*, const char*, int *active);
int zr_option(struct zr_context*, const char*, int active);
int zr_selectable(struct zr_context*, const char*, enum zr_text_align, int *value);
int zr_select(struct zr_context*, const char*, enum zr_text_align, int value);

/* buttons (push/press) */
int zr_button_text(struct zr_context*, const char*, enum zr_button_behavior);
int zr_button_color(struct zr_context*, struct zr_color, enum zr_button_behavior);
int zr_button_symbol(struct zr_context*, enum zr_symbol, enum zr_button_behavior);
int zr_button_image(struct zr_context*, struct zr_image img, enum zr_button_behavior);
int zr_button_text_symbol(struct zr_context*, enum zr_symbol, const char*,
                            enum zr_text_align, enum zr_button_behavior);
int zr_button_text_image(struct zr_context*, struct zr_image img, const char*,
                            enum zr_text_align align, enum zr_button_behavior);

/* simple value modifier by sliding */
void zr_progress(struct zr_context*, zr_size *cur, zr_size max, int modifyable);
void zr_slider_float(struct zr_context*, float min, float *val, float max, float step);
void zr_slider_int(struct zr_context*, int min, int *val, int max, int step);
float zr_slide_float(struct zr_context*, float min, float val, float max, float step);
int zr_slide_int(struct zr_context*, int min, int val, int max, int step);

/* extended value modifier by dragging, increment/decrement and text input */
void zr_property_float(struct zr_context *layout, const char *name,
                        float min, float *val, float max, float step, float inc_per_pixel);
void zr_property_int(struct zr_context *layout, const char *name,
                    int min, int *val, int max, int step, int inc_per_pixel);
float zr_propertyf(struct zr_context *layout, const char *name, float min, float val, float max,
                    float step, float inc_per_pixel);
int zr_propertyi(struct zr_context *layout, const char *name, int min, int val, int max,
                int step, int inc_per_pixel);

/* text manipulation */
int zr_edit_string(struct zr_context*, zr_flags, char *buffer, zr_size *len, zr_size max, zr_filter);
int zr_edit_buffer(struct zr_context*, zr_flags, struct zr_buffer*, zr_filter);

/* simple chart */
void zr_chart_begin(struct zr_context*, enum zr_chart_type, zr_size num, float min, float max);
zr_flags zr_chart_push(struct zr_context*, float);
void zr_chart_end(struct zr_context*);

/*--------------------------------------------------------------
 *                      Popups
 * -------------------------------------------------------------*/
int zr_popup_begin(struct zr_context*, struct zr_layout*, enum zr_popup_type,
                    const char *title, zr_flags, struct zr_rect bounds);
void zr_popup_close(struct zr_context*);
void zr_popup_end(struct zr_context*);

/* abstract combo box */
int zr_combo_begin_text(struct zr_context*, struct zr_layout*, const char *id,
                        const char *selected, int max_height);
int zr_combo_begin_color(struct zr_context*, struct zr_layout*, const char *id,
                        struct zr_color color, int max_height);
int zr_combo_begin_image(struct zr_context*, struct zr_layout*, const char *id,
                        struct zr_image img,  int max_height);
int zr_combo_begin_icon(struct zr_context*, struct zr_layout*, const char *id,
                        const char *selected, struct zr_image img, int height);
int zr_combo_item(struct zr_context*, const char*, enum zr_text_align);
int zr_combo_item_icon(struct zr_context*, struct zr_image, const char*, enum zr_text_align align);
int zr_combo_item_symbol(struct zr_context*, enum zr_symbol symbol, const char*, enum zr_text_align align);
void zr_combo_close(struct zr_context*);
void zr_combo_end(struct zr_context*);

/* contextual menu */
int zr_contextual_begin(struct zr_context*, struct zr_layout*, zr_flags flags,
                        struct zr_vec2 size, struct zr_rect trigger_bounds);
int zr_contextual_item(struct zr_context*, const char*, enum zr_text_align align);
int zr_contextual_item_icon(struct zr_context*, struct zr_image, const char*, enum zr_text_align);
int zr_contextual_item_symbol(struct zr_context*, enum zr_symbol, const char*, enum zr_text_align);
void zr_contextual_close(struct zr_context*);
void zr_contextual_end(struct zr_context*);

/* tooltip */
void zr_tooltip(struct zr_context*, const char *text);
int zr_tooltip_begin(struct zr_context*, struct zr_layout*, float width);
void zr_tooltip_end(struct zr_context*);

/*--------------------------------------------------------------
 *                          MENU
 * -------------------------------------------------------------*/
void zr_menubar_begin(struct zr_context*);
void zr_menubar_end(struct zr_context*);

int zr_menu_text_begin(struct zr_context*, struct zr_layout*, const char *title, float width);
int zr_menu_icon_begin(struct zr_context*, struct zr_layout*, const char *id, struct zr_image, float width);
int zr_menu_symbol_begin(struct zr_context*, struct zr_layout*, const char *id,  enum zr_symbol, float width);
int zr_menu_item(struct zr_context*, enum zr_text_align align, const char*);
int zr_menu_item_icon(struct zr_context*, struct zr_image, const char*, enum zr_text_align);
int zr_menu_item_symbol(struct zr_context*, enum zr_symbol, const char*, enum zr_text_align);
void zr_menu_close(struct zr_context*);
void zr_menu_end(struct zr_context*);

#ifdef __cplusplus
}
#endif

#endif /* ZR_H_ */
