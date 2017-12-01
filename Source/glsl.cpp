/*
  ==============================================================================

    glsl.cpp
    Created: 15 Nov 2017 5:59:04pm
    Author:  secci

  ==============================================================================
*/

//#define GLM_SWIZZLE 

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#define min glm::min
#define max glm::max


using namespace glm;
extern vec4 fragColor ;
extern vec2 iResolution ;



#ifdef GLSLSANDBOX
#ifdef GL_ES
#endif
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
#define iTime time
#define iResolution resolution
#define iMouse mouse
#endif
float  getTime()
{
	//int j = ::GetTickCount() % 100;
	//float i = float(j) / 100;
	return 1.;
	
}
#define iTime getTime()

vec2 iMouse = vec2(0.5f, 0.5f);
// interface
//////////////////////////////////////////////////////////

// set color source for stroke / fill / clear
void set_source_rgba(vec4 c);
void set_source_rgba(float r, float g, float b, float a);
void set_source_rgb(vec3 c);
void set_source_rgb(float r, float g, float b);
void set_source_linear_gradient(vec3 color0, vec3 color1, vec2 p0, vec2 p1);
void set_source_linear_gradient(vec4 color0, vec4 color1, vec2 p0, vec2 p1);
void set_source_radial_gradient(vec3 color0, vec3 color1, vec2 p, float r);
void set_source_radial_gradient(vec4 color0, vec4 color1, vec2 p, float r);
//void set_source(sampler2D image);
// control how source changes are applied
const int Replace = 0; // default: replace the new source with the old one
const int Alpha = 1; // alpha-blend the new source on top of the old one
const int Multiply = 2; // multiply the new source with the old one
void set_source_blend_mode(int mode);
// if enabled, blends using premultiplied alpha instead of
// regular alpha blending.
void premultiply_alpha(bool enable);

// set line width in normalized units for stroke
void set_line_width(float w);
// set line width in pixels for stroke
void set_line_width_px(float w);
// set blur strength for strokes in normalized units
void set_blur(float b);

// add a circle path at P with radius R
void circle(vec2 p, float r);
void circle(float x, float y, float r);
// add an ellipse path at P with radii RW and RH
void ellipse(vec2 p, vec2 r);
void ellipse(float x, float y, float rw, float rh);
// add a rectangle at O with size S
void rectangle(vec2 o, vec2 s);
void rectangle(float ox, float oy, float sx, float sy);
// add a rectangle at O with size S and rounded corner of radius R
void rounded_rectangle(vec2 o, vec2 s, float r);
void rounded_rectangle(float ox, float oy, float sx, float sy, float r);

// set starting point for curves and lines to P
void move_to(vec2 p);
void move_to(float x, float y);
// draw straight line from starting point to P,
// and set new starting point to P
void line_to(vec2 p);
void line_to(float x, float y);
// draw quadratic bezier curve from starting point
// over B1 to B2 and set new starting point to B2
void curve_to(vec2 b1, vec2 b2);
void curve_to(float b1x, float b1y, float b2x, float b2y);
// connect current starting point with first
// drawing point.
void close_path();

// clear screen in the current source color
void clear();
// fill paths and clear the path buffer
void fill();
// fill paths and preserve them for additional ops
void fill_preserve();
// stroke paths and clear the path buffer
void stroke_preserve();
// stroke paths and preserve them for additional ops
void stroke();
// clears the path buffer
void new_path();

// source channel for texture font
#define font_texture_source iChannel1
// draw a letter with the given texture coordinate
void letter(ivec2 l);
void letter(int lx, int ly);

// return rgb color for given hue (0..1)
vec3 hue(float hue);
// return rgb color for given hue, saturation and lightness
vec3 hsl(float h, float s, float l);
vec4 hsl(float h, float s, float l, float a);

// rotate the context by A in radians
void rotate(float a);
// uniformly scale the context by S
void scale(float s);
// non-uniformly scale the context by S
void scale(vec2 s);
void scale(float sx, float sy);
// translate the context by offset P
void translate(vec2 p);
void translate(float x, float y);
// clear all transformations for the active context
void identity_matrix();
// transform the active context by the given matrix
void transform(mat3 mtx);
// set the transformation matrix for the active context
void set_matrix(mat3 mtx);

// return the active query position for in_fill/in_stroke
// by default, this is the mouse position
vec2 get_query();
// set the query position for subsequent calls to
// in_fill/in_stroke; clears the query path
void set_query(vec2 p);
// true if the query position is inside the current path
bool in_fill();
// true if the query position is inside the current stroke
bool in_stroke();

// return the transformed coordinate of the current pixel
vec2 get_origin();
// draw a 1D graph from coordinate p, result f(p.x),
// and gradient1D(f,p.x)
void graph(vec2 p, float f_x, float df_x);
// draw a 2D graph from coordinate p, result f(p),
// and gradient2D(f,p)
void graph(vec2 p, float f_x, vec2 df_x);
// adds a custom distance field as path
// this field will not be testable by queries
void add_field(float c);

// returns a gradient for 1D graph function f at position x
#define gradient1D(f,x) (f(x + get_gradient_eps()) - f(x - get_gradient_eps())) / (2.0*get_gradient_eps())
// returns a gradient for 2D graph function f at position x
#define gradient2D(f,x) vec2(f(x + vec2(get_gradient_eps(),0.0)) - f(x - vec2(get_gradient_eps(),0.0)),f(x + vec2(0.0,get_gradient_eps())) - f(x - vec2(0.0,get_gradient_eps()))) / (2.0*get_gradient_eps())
// draws a 1D graph at the current position
#define graph1D(f) { vec2 pp = get_origin(); graph(pp, f(pp.x), gradient1D(f,pp.x)); }
// draws a 2D graph at the current position
#define graph2D(f) { vec2 pp = get_origin(); graph(pp, f(pp), gradient2D(f,pp)); }

// represents the current drawing context
// you usually don't need to change anything here
struct Context {
	// screen position, query position
	vec4 position;
	vec2 shape;
	vec2 clip;
	vec2 scale;
	float line_width;
	bool premultiply;
	vec2 blur;
	vec4 source;
	vec2 start_pt;
	vec2 last_pt;
	int source_blend;
	bool has_clip;
};

// save current stroke width, starting
// point and blend mode from active context.
Context _save();
#define save(name) Context name = _save();
// restore stroke width, starting point
// and blend mode to a context previously returned by save()
void restore(Context ctx);

// draws a half-transparent debug gradient for the
// active path
void debug_gradient();
void debug_clip_gradient();
// returns the gradient epsilon width
float get_gradient_eps();

// your draw calls here
//////////////////////////////////////////////////////////

float myf(float x) {
	return sin(x) * cos((x + iTime * 0.2) * 20.0);
}

float myf2(vec2 x) {
	float r = length(x);
	float a = atan(x.y, x.x);
	return r - 1.0 + 0.5 * sin(3.0*a + 2.0*r*r);
}

void shield_shape() {
	move_to(0.2, 0.2);
	line_to(0.0, 0.3);
	line_to(-0.2, 0.2);
	curve_to(-0.2, -0.05, 0.0, -0.2);
	curve_to(0.2, -0.05, 0.2, 0.2);
}


// implementation
//////////////////////////////////////////////////////////

vec2 aspect;
vec2 uv;
vec2 position;
vec2 query_position;
float ScreenH;
float AA;
float AAINV;

//////////////////////////////////////////////////////////

float det(vec2 a, vec2 b) { return a.x*b.y - b.x*a.y; }

//////////////////////////////////////////////////////////

vec3 hue(float hue) {

	//vec3 a = mod(vec3(2.1f , 3.2f, 2.1f), 1.2);

	//vec3 b = (hue * 6.0f + vec3(0.0f, 4.0f, 2.0f));
	//vec3 c = mod(vec3(2.1f, 3.2f, 2.1f), 6.);

	return 
		clamp(
		abs(
			mod(
				(vec3)(hue * 6.0f + vec3(0.0f, 4.0f, 2.0f)),  6.0f)
			- 3.0f) 
			- 1.0f,
		
		0.0f, 
		1.0f);
}

vec3 hsl(float h, float s, float l) {
	vec3 rgb = hue(h);
	return l + s * (rgb - 0.5f) * (1.0f - abs(2.0f * l - 1.0f));
}

vec4 hsl(float h, float s, float l, float a) {
	return vec4(hsl(h, s, l), a);
}

//////////////////////////////////////////////////////////

#define DEFAULT_SHAPE_V 1e+20
#define DEFAULT_CLIP_V -1e+20

Context _stack;

void init(vec2 fragCoord) {
	uv = fragCoord.xy / iResolution.xy;
	vec2 m = iMouse.xy / iResolution.xy;

	position = (uv*2.0f - 1.0f)*aspect;
	query_position = (m*2.0f - 1.0f)*aspect;

	_stack = Context{
		vec4(position, query_position),  //position
		vec2(DEFAULT_SHAPE_V),//shape
		vec2(DEFAULT_CLIP_V),//clip
		vec2(1.0),//scale
		1.0,//line_width
		false,//premultiply
		vec2(0.0, 1.0),//blur
		vec4(vec3(0.0), 1.0),//source
		vec2(0.0),//start_pt
		vec2(0.0),//last_pt
		Replace,//source_blend
		false// has_clip
	};


}

vec3 _color = vec3(1.0);

vec2 get_origin() {
	return _stack.position.xy;
}

vec2 get_query() {
	return _stack.position.zw;
}

void set_query(vec2 p) {
	_stack.position.zw = p;
	_stack.shape.y = DEFAULT_SHAPE_V;
	_stack.clip.y = DEFAULT_CLIP_V;
}

Context _save() {
	return _stack;
}

void restore(Context ctx) {
	// preserve shape
	vec2 shape = _stack.shape;
	vec2 clip = _stack.clip;
	bool has_clip = _stack.has_clip;
	// preserve source
	vec4 source = _stack.source;
	_stack = ctx;
	_stack.shape = shape;
	_stack.clip = clip;
	_stack.source = source;
	_stack.has_clip = has_clip;
}

mat3 mat2x3_invert(mat3 s)
{
	float d = det(s[0].xy, s[1].xy);
	d = (d != 0.0) ? (1.0 / d) : d;

	return mat3(
		s[1].y*d, -s[0].y*d, 0.0,
		-s[1].x*d, s[0].x*d, 0.0,
		det(s[1].xy, s[2].xy)*d,
		det(s[2].xy, s[0].xy)*d,
		1.0);
}

void identity_matrix() {
	_stack.position = vec4(position, query_position);
	_stack.scale = vec2(1.0);
}

void set_matrix(mat3 mtx) {
	mtx = mat2x3_invert(mtx);
	_stack.position.xy = (vec2)(mtx * vec3(position, 1.0)).xy;
	_stack.position.zw = (vec2)(mtx * vec3(query_position, 1.0)).xy;
	_stack.scale = vec2(glm::length((vec2)mtx[0].xy), glm::length((vec2)mtx[1].xy));
}

void transform(mat3 mtx) {
	mtx = mat2x3_invert(mtx);
	_stack.position.xy = (vec2)(mtx * vec3(_stack.position.xy, 1.0)).xy;
	_stack.position.zw = (vec2)(mtx * vec3(_stack.position.zw, 1.0)).xy;
	_stack.scale *= vec2(length((vec2)mtx[0].xy), length((vec2)mtx[1].xy));
}

void rotate(float a) {
	float cs = cos(a), sn = sin(a);
	transform(mat3(
		cs, sn, 0.0,
		-sn, cs, 0.0,
		0.0, 0.0, 1.0));
}

void scale(vec2 s) {
	transform(mat3(s.x, 0.0, 0.0, 0.0, s.y, 0.0, 0.0, 0.0, 1.0));
}

void scale(float sx, float sy) {
	scale(vec2(sx, sy));
}

void scale(float s) {
	scale(vec2(s));
}

void translate(vec2 p) {
	transform(mat3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, p.x, p.y, 1.0));
}

void translate(float x, float y) { translate(vec2(x, y)); }

void clear() {
	//if (_color.r != 1 || _color.g != 1 || _color.b != 1 )
	//{
	//	_color = mix(_color, (vec3)_stack.source.rgb, _stack.source.a);  // this is why we need clear color !
	//}
	float x = mix(_color, (vec3)_stack.source.rgb, _stack.source.a).x;

	vec3 c = (vec3)mix(_color, (vec3)_stack.source.rgb, _stack.source.a);
	_color = c;
}

void blit(  vec4 & dest) {
	dest = vec4(glm::sqrt(_color), 1.0f);
}

void blit( vec3 &dest) {
	dest = _color;
}

void add_clip(vec2 d) {
	d = d / _stack.scale;
	_stack.clip = max(_stack.clip, d);
	_stack.has_clip = true;
}

void add_field(vec2 d) {
	d = d / _stack.scale;
		_stack.shape = min(_stack.shape, d);
}

void add_field(float c) {
	_stack.shape.x = min(_stack.shape.x, c);

}

void new_path() {
	_stack.shape = vec2(DEFAULT_SHAPE_V);
	_stack.clip = vec2(DEFAULT_CLIP_V);
	_stack.has_clip = false;
}

void debug_gradient() {
	vec2 d = _stack.shape;
	_color = mix(_color,
		hsl(d.x * 6.0,
			1.0, (d.x >= 0.0) ? 0.5 : 0.3),
		0.5);
}

void debug_clip_gradient() {
	vec2 d = _stack.clip;
	_color = mix(_color,
		hsl(d.x * 6.0,
			1.0, (d.x >= 0.0) ? 0.5 : 0.3),
		0.5);
}

void set_blur(float b) {
	if (b == 0.0) {
		_stack.blur = vec2(0.0, 1.0);
	}
	else {
		_stack.blur = vec2(
			b,
			0.0);
	}
}

void write_color(vec4 rgba, float w) {
	float src_a = w * rgba.a;
	float dst_a = _stack.premultiply ? w : src_a;
	
	_color = _color * (1.0f - src_a) + rgba.rgb * dst_a;
	if (_isnan(_color.x))
	{
		_isnan(1);
	}
}

void premultiply_alpha(bool enable) {
	_stack.premultiply = enable;
}

float min_uniform_scale() {
	return min(_stack.scale.x, _stack.scale.y);
}

float uniform_scale_for_aa() {
	return min(1.0f, _stack.scale.x / _stack.scale.y);
}

float calc_aa_blur(float w) {
	
	vec2 blur = _stack.blur;
	w -= blur.x;
	float wa = clamp(-w*AA*uniform_scale_for_aa(), 0.0f, 1.0f);
	float wb = clamp(-w / blur.x + blur.y, 0.0f, 1.0f);
	float r = wa * wb;
	if (_isnan(r))
	{
		_isnan(1);
	}
	if (r != .0)
	{
		_isnan(1);
	}
	return r;
	return w;
}

void fill_preserve() {

	//write_color(_stack.source, calc_aa_blur(_stack.shape.x  ));
	if (_stack.has_clip) {
			write_color(_stack.source, calc_aa_blur(_stack.clip.x));
	}
}

void fill() {
	fill_preserve();
	new_path();
}

void set_line_width(float w) {
	_stack.line_width = w;
}

void set_line_width_px(float w) {
	_stack.line_width = w*min_uniform_scale() * AAINV;
}

float get_gradient_eps() {
	return (1.0 / min_uniform_scale()) * AAINV;
}

vec2 stroke_shape() {
	return abs(_stack.shape) - _stack.line_width / _stack.scale;
}
/*
float w = stroke_shape().x;
write_color(_stack.source, calc_aa_blur(w));
*/
void stroke_preserve() {
	float w = stroke_shape().x;

	if (w > -0.01 && w < -.005)   //  width 0.01
	{
		calc_aa_blur(_stack.shape.x);
		
	}


	float calb = calc_aa_blur(w);

	
	write_color(_stack.source, calb);
}

void stroke() {
	stroke_preserve();
	new_path();
}

bool in_fill() {
	return (_stack.shape.y <= 0.0);
}

bool in_stroke() {
	float w = stroke_shape().y;
	return (w <= 0.0);
}

void set_source_rgba(vec4 c) {
	//c.rgb *= c.rgb;
	c *= c;
	if (_stack.source_blend == Multiply) {
		_stack.source *= c;
	}
	else if (_stack.source_blend == Alpha) {
		float src_a = c.a;
		float dst_a = _stack.premultiply ? 1.0 : src_a;
		_stack.source =
			vec4(_stack.source.rgb * (1.0f - src_a) + c.rgb * dst_a,
				max(_stack.source.a, c.a));
	}
	else {
		_stack.source = c;
	}
}

void set_source_rgba(float r, float g, float b, float a) {
	set_source_rgba(vec4(r, g, b, a));
}

void set_source_rgb(vec3 c) {
	set_source_rgba(vec4(c, 1.0));
}

void set_source_rgb(float r, float g, float b) { set_source_rgb(vec3(r, g, b)); }

//void set_source(sampler2D image) {
//	set_source_rgba(texture(image, _stack.position.xy));
//}

void set_source_linear_gradient(vec4 color0, vec4 color1, vec2 p0, vec2 p1) {
	vec2 pa = _stack.position.xy - p0;
	vec2 ba = p1 - p0;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0f, 1.0f);
	set_source_rgba(mix(color0, color1, h));
}

void set_source_linear_gradient(vec3 color0, vec3 color1, vec2 p0, vec2 p1) {
	set_source_linear_gradient(vec4(color0, 1.0), vec4(color1, 1.0), p0, p1);
}

void set_source_radial_gradient(vec4 color0, vec4 color1, vec2 p, float r) {
	float h = clamp(length(_stack.position.xy - p) / r, 0.0f, 1.0f);
	set_source_rgba(mix(color0, color1, h));
}

void set_source_radial_gradient(vec3 color0, vec3 color1, vec2 p, float r) {
	set_source_radial_gradient(vec4(color0, 1.0f), vec4(color1, 1.0f), p, r);
}

void set_source_blend_mode(int mode) {
	_stack.source_blend = mode;
}

vec2 length2(vec4 a) {
	return vec2(length((vec2)a.xy), length((vec2)a.zw));
}

vec2 dot2(vec4 a, vec2 b) {
	return vec2(dot((vec2)a.xy, b), dot((vec2)a.zw, b));
}

//void letter(ivec2 l) {
//	vec2 p = vec2(l);
//	vec3 tx;
//	vec2 ip;
//	float d;
//	int ic;
//	ip = vec2(l);
//	p += clamp(_stack.position.xy, 0.0f, 1.0f);
//	ic = 0x21 + int(mod(16.f + ip.x + 2.f * ip.y, 94.f));
//	tx = texture(font_texture_source, mod((vec2(mod(float(ic), 16.f),
//		15.f - floor(float(ic) / 16.f)) + fract(p)) * (1.f / 16.f), 1.f)).gba - 0.5f;
//	d = tx.b + 1.f / 256.f;
//	add_field(d / min_uniform_scale());
//}
//
//void letter(int lx, int ly) {
//	letter(ivec2(lx, ly));
//}

void rounded_rectangle(vec2 o, vec2 s, float r) {
	s = (s * 0.5f);
	r = min(r, min(s.x, s.y));
	o += s;
	s -= r;
	vec4 d = abs(o.xyxy - _stack.position) - s.xyxy;
	vec4 dmin = min(d, 0.0f);
	vec4 dmax = max(d, 0.0f);
	vec2 df = max((vec2)dmin.xz, (vec2)dmin.yw) + length2(dmax);
	add_field(df - r);
}

void rounded_rectangle(float ox, float oy, float sx, float sy, float r) {
	rounded_rectangle(vec2(ox, oy), vec2(sx, sy), r);
}

void rectangle(vec2 o, vec2 s) {
	rounded_rectangle(o, s, 0.0);
}

void rectangle(float ox, float oy, float sx, float sy) {
	rounded_rectangle(vec2(ox, oy), vec2(sx, sy), 0.0);
}

void circle(vec2 p, float r) {
	vec4 c = _stack.position - p.xyxy;
	add_field(vec2(length((vec2)c.xy), length((vec2)c.zw)) - r);
}
void circle(float x, float y, float r) { circle(vec2(x, y), r); }

// from https://www.shadertoy.com/view/4sS3zz
float sdEllipse(vec2 p,   vec2& ab)
{
	p = abs(p); if (p.x > p.y) { p = p.yx; ab = ab.yx; }

	float l = ab.y*ab.y - ab.x*ab.x;
	if (l == 0.0) {
		return length(p) - ab.x;
	}

	float m = ab.x*p.x / l;
	float n = ab.y*p.y / l;
	float m2 = m*m;
	float n2 = n*n;

	float c = (m2 + n2 - 1.0) / 3.0;
	float c3 = c*c*c;

	float q = c3 + m2*n2*2.0;
	float d = c3 + m2*n2;
	float g = m + m*n2;

	float co;

	if (d<0.0)
	{
		float p = acos(q / c3) / 3.0f;
		float s = cos(p);
		float t = sin(p)*glm::sqrt(3.0f);
		float rx = glm::sqrt(-c*(s + t + 2.0f) + m2);
		float ry = glm::sqrt(-c*(s - t + 2.0f) + m2);
		co = (ry + sign(l)*rx + abs(g) / (rx*ry) - m) / 2.0;
	}
	else
	{
		float h = 2.0*m*n*glm::sqrt(d);
		float s = sign(q + h)*pow(abs(q + h), 1.0 / 3.0f);
		float u = sign(q - h)*pow(abs(q - h), 1.0 / 3.0f);
		float rx = -s - u - c*4.0f + 2.0f*m2;
		float ry = (s - u)*glm::sqrt(3.0f);
		float rm = glm::sqrt(rx*rx + ry*ry);
		float p = ry / glm::sqrt(rm - rx);
		co = (p + 2.0f*g / rm - m) / 2.0f;
	}

	float si = glm::sqrt(1.0f - co*co);

	vec2 r = vec2(ab.x*co, ab.y*si);

	return length(r - p) * sign(p.y - r.y);
}

void ellipse(vec2 p, vec2 r) {
	vec4 c = _stack.position - p.xyxy;
	add_field(vec2(sdEllipse(c.xy, r), sdEllipse(c.zw, r)));
}

void ellipse(float x, float y, float rw, float rh) {
	ellipse(vec2(x, y), vec2(rw, rh));
}

void move_to(vec2 p) {
	_stack.start_pt = p;
	_stack.last_pt = p;
}

void move_to(float x, float y) { move_to(vec2(x, y)); }

// stroke only
void line_to(vec2 p) {
	vec4 pa = _stack.position - _stack.last_pt.xyxy;
	vec2 ba = p - _stack.last_pt;
	vec2 h = clamp(dot2(pa, ba) / dot(ba, ba), 0.0f, 1.0f);
	vec2 s = sign(pa.xz*ba.y - pa.yw*ba.x);
	vec2 d = length2(pa - ba.xyxy*h.xxyy);
	if (s.x > 0.)
	{
		add_field(d);
	}
	else
		add_field(d);
	add_clip(d * s);
	_stack.last_pt = p;
}


// stroke only
void line_to_(vec2 p) {
	_stack.position = vec4(.3f, .3f, 0.f, 2.f);
	//p = vec2(.5f, .5f);
	//_stack.last_pt = vec2(-2.f, 0.f);

	//float xxx = atan2(-1.f, 0.f);
	
	vec4 pa = _stack.position - _stack.last_pt.xyxy;
	vec2 ba = p - _stack.last_pt;
	vec2 pb = dot2(pa, ba);
	float bb = dot(ba, ba);
	//vec2 h = clamp(dot2(pa, ba) / dot(ba, ba), 0.0f, 1.0f);
		vec2 h = dot2(pa, ba) / dot(ba, ba);
	vec2 ss = pa.xz*ba.y - pa.yw*ba.x;
	vec2 s = sign((vec2)ss);

	vec4 projectionVector = ba.xyxy*h.xxyy;
	vec4 subPlayervector_Projectionvector = pa - projectionVector;
	vec2 d = length2(subPlayervector_Projectionvector);
	//vec2 d = length2(pa - h.xxyy);
	add_field(d);
	add_clip(d * s);
	_stack.last_pt = p;
}




void line_to(float x, float y) { line_to(vec2(x, y)); }

void close_path() {
	line_to(_stack.start_pt);
}

// from https://www.shadertoy.com/view/ltXSDB

// Test if point p crosses line (a, b), returns sign of result
float test_cross(vec2 a, vec2 b, vec2 p) {
	return sign((b.y - a.y) * (p.x - a.x) - (b.x - a.x) * (p.y - a.y));
}

// Determine which side we're on (using barycentric parameterization)
float bezier_sign(vec2 A, vec2 B, vec2 C, vec2 p) {
	vec2 a = C - A, b = B - A, c = p - A;
	vec2 bary = vec2(c.x*b.y - b.x*c.y, a.x*c.y - c.x*a.y) / (a.x*b.y - b.x*a.y);
	vec2 d = vec2(bary.y * 0.5f, 0.0) + 1.0f - bary.x - bary.y;
	return mix(sign(d.x * d.x - d.y), mix(-1.0f, 1.0f,
		step(test_cross(A, B, p) * test_cross(B, C, p), 0.0f)),
		step((d.x - d.y), 0.0f)) * test_cross(A, C, B);
}

// Solve cubic equation for roots
vec3 bezier_solve(float a, float b, float c) {
	float p = b - a*a / 3.0f, p3 = p*p*p;
	float q = a * (2.0f*a*a - 9.0*b) / 27.0f + c;
	float d = q*q + 4.0f*p3 / 27.0f;
	float offset = -a / 3.0f;
	if (d >= 0.0f) {
		float z = glm::sqrt(d);
		vec2 x = (vec2(z, -z) - q) / 2.0f;
		vec2 uv = sign(x)*pow(abs(x), vec2(1.0f / 3.0f));
		return vec3(offset + uv.x + uv.y);
	}
	float v = acos(-glm::sqrt(-27.0f / p3) * q / 2.0f) / 3.0f;
	float m = cos(v), n = sin(v)*1.732050808f;
	return vec3(m + m, -n - m, n - m) * glm::sqrt(-p / 3.0f) + offset;
}

// Find the signed distance from a point to a quadratic bezier curve
float bezier(vec2 A, vec2 B, vec2 C, vec2 p)
{
	B = mix(B + vec2(1e-4), B, abs(sign(B * 2.0f - A - C)));
	vec2 a = B - A, b = A - B * 2.0f + C, c = a * 2.0f, d = A - p;
	vec3 k = vec3(3.f*dot(a, b), 2.f*dot(a, a) + dot(d, b), dot(d, a)) / dot(b, b);
	vec3 t = clamp(bezier_solve(k.x, k.y, k.z), 0.0f, 1.0f);
	vec2 pos = A + (c + b*t.x)*t.x;
	float dis = length(pos - p);
	pos = A + (c + b*t.y)*t.y;
	dis = min(dis, length(pos - p));
	pos = A + (c + b*t.z)*t.z;
	dis = min(dis, length(pos - p));
	return dis * bezier_sign(A, B, C, p);
}

void curve_to(vec2 b1, vec2 b2) {
	vec2 shape = vec2(
		bezier(_stack.last_pt, b1, b2, _stack.position.xy),
		bezier(_stack.last_pt, b1, b2, _stack.position.zw));
	add_field(abs(shape));
	add_clip(shape);
	_stack.last_pt = b2;
}

void curve_to(float b1x, float b1y, float b2x, float b2y) {
	curve_to(vec2(b1x, b1y), vec2(b2x, b2y));
}

void graph(vec2 p, float f_x, float df_x) {
	add_field(abs(f_x - p.y) / glm::sqrt(1.0f + (df_x * df_x)));
}

void graph(vec2 p, float f_x, vec2 df_x) {
	add_field(abs(f_x) / length(df_x));
}



void paint() {

	// traingle !

	set_source_rgb(vec3(1.0, 1.0, 1.));
	clear();



	//example 1 ellipse 
	/*
	ellipse(0.0, 0.0, 0.3, 0.5);
	set_line_width(0.01);
	set_source_rgba(0., 1., 0., 1.);
	//stroke_preserve();   if use this , part of che ellipse and circle are the same color!!!!
	//fill();
	stroke();
	*/



	//example 2 circle 
	/*
	circle(0.3 + 0.3*(sin(1.2)*0.5 + 0.5), 0.0, 0.2);
	set_line_width(0.01);
	set_source_rgba(0., 0., 1., 1.);
	stroke();
	*/



	//example 3 line 
	/*	
	set_source_rgba(1., 0.0, 0.0, 1.);
	set_line_width(0.01);

	set_blur(0.001);
	//scale(.5f);
	move_to(.0f, .0f);
	line_to(.0f, .1f);
	line_to(0.1f, .0f);
	close_path();
	if (_stack.position.y < -.2f)
	{
		
		fill_preserve();
	}
	else
	{
		fill_preserve();
	}*/
	
	
	//stroke_preserve();


	//example 4 curve

	/**/
	set_source_rgba(1., 0.0, 0.0, 1.);
	set_line_width(0.01);
	set_blur(0.0001);
	move_to(.0f, 0.0f);

	//curve_to(0.f, 0.5f, -0.5f, 0.f);
	line_to(.5f, .0f);
	//close_path();
	//stroke();
	fill_preserve();
	


	//float liushidu = 0.6283185306;  // 36 du
	//translate(-1.0f, 0.4f);
	//scale(0.5f);
//	rotate(radians(30.0f));	
	//stroke();
	//for (int i = 1; i < 6; ++i) {
	//	float a0 = liushidu*((float)i*2.f -1);//
	//	//a0 = radians((float(i) - 0.5f) * 72.f);
	//	float a1 = liushidu*((float)i* 2.f ) ;//
	//	//a1 = radians(float(i) * 72.f);
	//	//curve_to(
	//	//	cos(a0)*0.5f, sin(a0)*0.5f,
	//	//	cos(a1)*0.5f, sin(a1)*0.5f);
	//	float ax, ay, xx, xy;
	//	ax = cos(a0);
	//	ay = sin(a0);
	//	xx = cos(a1);
	//	xy = sin(a1);
	//	curve_to(
	//		ax, ay,
	//		xx, xy);
	//}

	//for (int i = 1; i < 6; ++i) {
	//	float a0 = radians((float(i) - 0.5f) * 360.0f / 5.0f);
	//	a0 = radians((float(i) - 0.5f) * 72.f);
	//	float a1 = radians(float(i) * 360.0f / 5.0f);
	//	curve_to(
	//		cos(a0)*0.5f, sin(a0)*0.5f,
	//		cos(a1)*0.5f, sin(a1)*0.5f);
	//}


	//set_line_width_px(0.1f);
	//bool tri_active = in_stroke();
	//set_source_rgba(hsl(tri_active ? 0.1f : 0.5f, 1.0f, 0.5f, 0.5f));
	//fill_preserve();
	//stroke();
	/**/




}




// Test if point p crosses line (a, b), returns sign of result
float testCross(vec2 a, vec2 b, vec2 p) {
	return sign((b.y - a.y) * (p.x - a.x) - (b.x - a.x) * (p.y - a.y));
}

// Determine which side we're on (using barycentric parameterization)
float signBezier(vec2 A, vec2 B, vec2 C, vec2 p)
{
	vec2 a = C - A, b = B - A, c = p - A;
	vec2 bary = vec2(c.x*b.y - b.x*c.y, a.x*c.y - c.x*a.y) / (a.x*b.y - b.x*a.y);
	vec2 d = vec2(bary.y * 0.5f, 0.0f) + 1.0f - bary.x - bary.y;
	return mix(sign(d.x * d.x - d.y), mix(-1.0f, 1.0f,
		step(testCross(A, B, p) * testCross(B, C, p), 0.0f)),
		step((d.x - d.y), 0.0f)) * testCross(A, C, B);
}

// Solve cubic equation for roots
vec3 solveCubic(float a, float b, float c)
{
	float p = b - a*a / 3.0f, p3 = p*p*p;
	float q = a * (2.0f*a*a - 9.0f*b) / 27.0f + c;
	float d = q*q + 4.0f*p3 / 27.0f;
	float offset = -a / 3.0f;
	if (d >= 0.0f) {
		float z = sqrt(d);
		vec2 x = (vec2(z, -z) - q) / 2.0f;
		vec2 uv = sign(x)*pow(abs(x), vec2(1.0f / 3.0f));
		return vec3(offset + uv.x + uv.y);
	}
	float v = acos(-sqrt(-27.0f / p3) * q / 2.0f) / 3.0f;
	float m = cos(v), n = sin(v)*1.732050808f;
	return vec3(m + m, -n - m, n - m) * sqrt(-p / 3.0f) + offset;
}

// Find the signed distance from a point to a bezier curve
float sdBezier(vec2 A, vec2 B, vec2 C, vec2 p)
{
	B = mix(B + vec2(1e-4), B, abs(sign(B * 2.0f - A - C)));
	vec2 a = B - A, b = A - B * 2.0f + C, c = a * 2.0f, d = A - p;
	vec3 k = vec3(3.*dot(a, b), 2.*dot(a, a) + dot(d, b), dot(d, a)) / dot(b, b);
	vec3 t = clamp(solveCubic(k.x, k.y, k.z), 0.0f, 1.0f);
	vec2 pos = A + (c + b*t.x)*t.x;
	float dis = length(pos - p);
	pos = A + (c + b*t.y)*t.y;
	dis = min(dis, length(pos - p));
	pos = A + (c + b*t.z)*t.z;
	dis = min(dis, length(pos - p));
	return dis * signBezier(A, B, C, p);
}
void mainImage(vec2 gl_FragCoord)
{
	gl_FragCoord.y = iResolution.y - gl_FragCoord.y;



	vec2 uv = gl_FragCoord.xy() / iResolution.xy();
	uv = uv - vec2(0.5f);
	uv.x *= iResolution.x / iResolution.y;




	aspect = vec2(iResolution.x / iResolution.y, 1.0);
	ScreenH = min(iResolution.x, iResolution.y);
	AA = ScreenH*0.4;
	AAINV = 1.0 / AA;

	init(gl_FragCoord);

	paint();

	blit(fragColor);



	/*
	float aspect = iResolution.x / iResolution.y;
	vec2 p = (2.0f*gl_FragCoord.xy - iResolution.xy) / iResolution.y;
	p.x *= aspect;
	vec2 m = vec3(-.5f, -.5f, .0f);
	m.x *= aspect;



	// Define the control points of our curve
	vec2 A = vec2(0.0f, -0.6f), C = vec2(0.0f, +0.6f), B = (4.0f * m - A - C) / 2.0f; 



	// Render the control points
	float d = min(distance(p, A), (min(distance(p, m), distance(p, C))));  //B, m
	if (d < 0.04f)
	{
		fragColor = vec4(1.f, 1.f, 0.f, 1.f); 
		return; 
	}



	// Get the signed distance to bezier curve
	d = sdBezier(A, B, C, p);
	if(d>0)
		fragColor = vec4(fract(d*10.f),0, 0,1.0f);
	if(d == 0)
		fragColor = vec4(0.f, 0.f, 1.f, 1.0f);
	if (d < 0)
		fragColor = vec4(0.f, 0.f, fract(abs(d)*10.f), 1.0f);
	return;
	*/
}
