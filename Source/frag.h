#version 120

uniform vec4 lightPosition;
varying vec2 textureCoordOut;
varying vec2 _uv;
uniform sampler2D Texture_1;
uniform sampler2D Texture_2;
uniform sampler2D Texture_3;
uniform sampler2D Texture_4;

//uniform sampler2D Texture_1;
uniform float iGlobalTime;
uniform float _x;
uniform float _y;
uniform float _w;
uniform float _h;
uniform float arrFloat[512];
uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;

uniform float _mx;
uniform float _my;

//#define fragColor gl_FragColor
//#define color gl_FragColor
//#define o gl_FragColor


vec2 fragCoord = (_uv * iResolution);
//vec2 p = (_uv * iResolution);










float distanceToSegment(vec2 a, vec2 b, vec2 p)
{
    vec2 pa = p - a;
    vec2 ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba*h);
    
    
}


float getcolor(vec2 uv)
{
    if (uv.x < 0.75 &&  uv.x > 0.25)
        return 1.;
    return 0.;
}





////////////////////////////////////////////////////
////////////////////////////////////////////////////
///
/// vitual machine
////////////////////////////////////////////////////
////////////////////////////////////////////////////





const int    NOOP = 0;
const int mov_eax_xxx = 1;
const int jmp = 2;
const int lea_eax_mesp_xx = 3;
const int push_eax = 4;
const int mov_eax_addr = 5;
const int mov_eax_byte_addr = 6;
const int ret = 7;
const int pop_ebx_mov_mebx_eax = 8;
const int add_esp_xx = 9;
const int pop_ebx_imul_eax_ebx = 10;
const int pop_ebx_add_eax_ebx = 11;
const int call_eax = 12;
const int halt = 13;
const int jz = 14;
const int pop_ebx_equal_eax_ebx = 15;
const int pop_ebx_not_equal_eax_ebx = 16;
const int call_addr = 17;
const int pop_ebx_ebx_less_than_ebx = 18;
const int pop_ebx_ebx_greate_than_ebx = 19;
const int pop_ebx_sub_ebx_eax = 20;
const int pop_ebx_fdiv_ebx_eax = 21;
const int pop_ebx_mod_ebx_eax = 22;
const int pop_ebx_logic_and_ebx_eax = 23;
const int pop_ebx_logic_or_ebx_eax = 24;
const int pop_ebx_bit_and_ebx_eax = 25;
const int pop_ebx_bit_or_ebx_eax = 26;
const int pop_ebx_bit_xor_ebx_eax = 27;
const int pop_ebx_shl_ebx_eax = 28;
const int pop_ebx_shr_ebx_eax = 29;
const int negative_eax = 30;
const int mov_eax_ValueAtEax_negative_eax = 31;
const int bit_negative_eax = 32;
const int mov_eax_ValueAtEax_bit_negative_eax = 33;
const int not_eax = 34;
const int mov_eax_ValueAtEax_not_eax = 35;
const int pop_ebx_ebx_greate_EQUAL_than_ebx = 36;
const int pop_ebx_ebx_less_EQUAL_than_ebx = 37;
const int pop_ebx_mov_mebx_eax_with_offset = 38;
const int call_api = 39;





/////////////////////////////////////////////
/////////////////////////////////////////////
////
////
////vector graphic
/////////////////////////////////////////////
/////////////////////////////////////////////

#define DEFAULT_SHAPE_V 1e+20
#define DEFAULT_CLIP_V -1e+20


const int Replace = 0; // default: replace the new source with the old one
const int Alpha = 1; // alpha-blend the new source on top of the old one
const int Multiply = 2; // multiply the new source with the old one


struct Context {
    // screen position, query position
    // 前 vec2 是 当前shader处理的点的坐标。  后面的vec2是当前鼠标点击的坐标
    vec4 position;
    
    //当前shader处理的点(鼠标点击的坐标）的坐标到要画的线的距离。
    vec2 shape;
    
    // 是否裁剪， 画直线时候 ， 如果当前坐标向量 和 要画的直线向量   pa pb   (pa.x*pb.y - pa.y*pb.x) 是负数 ，那么clip是负数 ，逆时针， 则填充线的颜色。
    vec2 clip;
    
    // 当前缩放的值
    vec2 scale;
    
    //当前线的宽度
    float line_width;
    
    // 是否使用提前相乘，在画直线的时候用到。 目前一直是false。
    bool premultiply;
    
    //当前的模糊值， 比如直线的边缘使用模糊的宽度。
    vec2 blur;
    
    //当前直线的颜色的值。
    vec4 source;
    
    //move to 的值？
    vec2 start_pt;
    
    //最近一次 line_to的值 。
    vec2 last_pt;
    
    //未知
    int source_blend;
    
    //当前是否使用了裁剪
    bool has_clip;
};

Context _stack;
vec3 _color = vec3(1.0);


void init(vec2 fragCoord) {
    vec2 uv = fragCoord.xy / iResolution.xy;
	
    vec2 m = iMouse.xy / iResolution.xy;
    vec2 aspect = vec2(iResolution.x / iResolution.y, 1.0);
    
    //_color.x = _uv.x;
    //_color.y = 0.;
    //_color.z = 0.;
    
    vec2 position = (uv*2.0 - 1.0)*aspect;
    vec2 query_position = (m*2.0 - 1.0)*aspect;
    
    _stack = Context(
                     vec4(position, query_position),
                     vec2(DEFAULT_SHAPE_V),
                     vec2(DEFAULT_CLIP_V),
                     vec2(1.0),
                     1.0,
                     false,
                     vec2(0.005, 1.0),
                     vec4(vec3(0.0), 1.0),
                     vec2(0.0),
                     vec2(0.0),
                     Replace,
                     false
                     );
}



void set_source_rgba(vec4 c) {
    //c *= c;   // why squre ？
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

void set_line_width(float w) {
    _stack.line_width = w;
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


void move_to(vec2 p) {
    _stack.start_pt = p;
    _stack.last_pt = p;
}

void move_to(float x, float y) { move_to(vec2(x, y)); }



vec2 length2(vec4 a) {
    return vec2(length(a.xy), length(a.zw));
}

vec2 dot2(vec4 a, vec2 b) {
    return vec2(dot(a.xy, b), dot(a.zw, b));
}


void add_field(vec2 d) {
    d = d / _stack.scale;
    _stack.shape = min(_stack.shape, d);
}

void add_field(float c) {
    _stack.shape.x = min(_stack.shape.x, c);
}


void add_clip(vec2 d) {
    d = d / _stack.scale;
    _stack.clip = max(_stack.clip, d);
    _stack.has_clip = true;
}




// stroke only
void line_to(vec2 p) {
    vec4 pa = _stack.position - _stack.last_pt.xyxy;
    vec2 ba = p - _stack.last_pt;
    vec2 h = clamp(dot2(pa, ba) / dot(ba, ba), 0.0, 1.0);
    vec2 s = sign(pa.xz*ba.y - pa.yw*ba.x);
    vec2 d = length2(pa - ba.xyxy*h.xxyy);
    add_field(d);
    add_clip(d * s);
    _stack.last_pt = p;
}

void line_to(float x, float y) { line_to(vec2(x, y)); }



////向量a 在 向量 b上的投影。  A = sqrt(a*a)A 表示 a的長度。B 表示 b的長度。
////
////cos(x) = a * b / A * B
////
////a在向量b上的投影 A * cos(x) = A *  (a*b) / A*B = (a*b) / B      // 這個值是一個標量
////
////所以 a在向量b上的投影向量是(a*b) / B * b / B = a*b*b / b*b
////
////即(a*b / b*b)  * b
//
//// stroke only
//void line_to_(vec2 p) {
//    _stack.position = vec4(.3f, .3f, 0.f, 2.f);
//    //p = vec2(.5f, .5f);
//    //_stack.last_pt = vec2(-2.f, 0.f);
//
//    //float xxx = atan2(-1.f, 0.f);
//
//    vec4 pa = _stack.position - _stack.last_pt.xyxy;
//    vec2 ba = p - _stack.last_pt;
//    vec2 pb = dot2(pa, ba);
//    float bb = dot(ba, ba);
//    vec2 h = clamp(dot2(pa, ba) / dot(ba, ba), 0.0f, 1.0f);    // (a*b / b*b)   分別表示兩個標量 ， 第一個算的是當前計算的點，  第二個算的是鼠標所在的點（？ todo 還沒有研究）
//                                                               //這裏需要 clamp 不然下面的  pa - projectionVector 計算不對， 會造成直綫過長
//
//                                                               //vec2 h = dot2(pa, ba) / dot(ba, ba);
//    vec2 ss = pa.xz*ba.y - pa.yw*ba.x;    // a.x * b.y  - a.y * b.x    算兩個夾角逆時針是否超過180 度 ？
//    vec2 s = sign((vec2)ss);
//
//    vec4 projectionVector = ba.xyxy*h.xxyy;
//    vec4 subPlayervector_Projectionvector = pa - projectionVector;
//    vec2 d = length2(subPlayervector_Projectionvector);
//    //vec2 d = length2(pa - h.xxyy);
//    add_field(d);
//    add_clip(d * s);
//    _stack.last_pt = p;
//}


void write_color(vec4 rgba /* 线的颜色 */, float w /* w 在 0，1之间 */) {
    
    // src_a 表示当前颜色需要保留的百分比是多少， 0 表示当前颜色全部保留 。
    // 因为 0 ，表示 rgba的a是0， rgba 是 stack.source 也就是线的颜色    那么表示不用线的颜色（线的alpha为0，完全透明） 。 这是下面的  _color （当前背景颜色） * (1.0 - src_a)  = 当前背景颜色 * 1 ；
    // 因为 0 ，表示 w 为0 .      同上 。
    
    float src_a = w * rgba.a;
    float dst_a = _stack.premultiply ? w : src_a;
    
    // rgb 是 line 的颜色 ， 所以 src_a 为 1， 那么当前点是实心的 。 如果 0< dst_a < 1 那么当前点是模糊的
    _color = _color * (1.0 - src_a) + rgba.rgb * dst_a;
}




float calc_aa_blur(float w /*w 表示现在正在检查的点离要画的直线的边缘有多远 !!!!!
                            如果w是负数 ，表示这个点是直线内的点 。 否则为直线外的点。
                            */) {
                                vec2 blur = _stack.blur;
                                
                                
                                
                                
                                // a= w - x  表示a这个宽度内是需要模糊的 ， 剩下的w 表示不需要模糊的宽度 。
                                // w += blur.x; 执行完后 ， 如果w为负数 ， 说明 这个点是实心的， 没有被模糊。
                                // 如果 w 在 0-blur.x之间， 说明这个点是在模糊范围内的 。
                                // 如果 w 大于  blur.x，   说明这个点在模糊范围之外， 也就是说，这个点不属于当前要画的line， 也就是说，采用背景色。
                                w += blur.x;
                                
                                
                                float wa = clamp(-w*min(iResolution.x, iResolution.y)/**uniform_scale_for_aa()*/, 0.0, 1.0);       // wa 表示当前离 直线边缘的距离  够不够一个像素， 如果不够， 那么这个点的颜色， 就是 线的颜色 * wa 比如0.5， 相当于抗锯齿。
                                float wb = clamp(-w / blur.x + blur.y, 0.0, 1.0);               // 当前离 直线边缘的距离  除以   blur的宽度  ， 如果在blur之间， 就对其 。。。。 同上  // 如果 blur.x为0， 那么就除以 blur.y ,也就是 1， 这就是为什么当blur为0时， 1的意义了。 否则， blur有值， 那么blur.y 就为0.
                                return wa * wb;
                    
                            }


vec2 stroke_shape() {
    return abs(_stack.shape) - _stack.line_width / _stack.scale;
}

void stroke_preserve() {
    float w = stroke_shape().x;
    write_color(_stack.source, calc_aa_blur(w));
}



void new_path() {
    _stack.shape = vec2(DEFAULT_SHAPE_V);
    _stack.clip = vec2(DEFAULT_CLIP_V);
    _stack.has_clip = false;
}


void stroke() {
    stroke_preserve();
    new_path();
}


void fill_preserve() {
	write_color(_stack.source, calc_aa_blur(_stack.shape.x));
	if (_stack.has_clip) {
		write_color(_stack.source, calc_aa_blur(_stack.clip.x));
	}
}

void fill() {
	fill_preserve();
	new_path();
}


void clear() {
    _color = mix(_color, _stack.source.rgb, _stack.source.a);
}


// bezier


// from https://www.shadertoy.com/view/ltXSDB

// Test if point p crosses line (a, b), returns sign of result
float test_cross(vec2 a, vec2 b, vec2 p) {
	return sign((b.y - a.y) * (p.x - a.x) - (b.x - a.x) * (p.y - a.y));
}

// Determine which side we're on (using barycentric parameterization)
float bezier_sign(vec2 A, vec2 B, vec2 C, vec2 p) {
	vec2 a = C - A, b = B - A, c = p - A;
	vec2 bary = vec2(c.x*b.y - b.x*c.y, a.x*c.y - c.x*a.y) / (a.x*b.y - b.x*a.y);
	vec2 d = vec2(bary.y * 0.5, 0.0) + 1.0 - bary.x - bary.y;
	return mix(sign(d.x * d.x - d.y), mix(-1.0, 1.0,
		step(test_cross(A, B, p) * test_cross(B, C, p), 0.0)),
		step((d.x - d.y), 0.0)) * test_cross(A, C, B);
}

// Solve cubic equation for roots
vec3 bezier_solve(float a, float b, float c) {
	float p = b - a*a / 3.0, p3 = p*p*p;
	float q = a * (2.0*a*a - 9.0*b) / 27.0 + c;
	float d = q*q + 4.0*p3 / 27.0;
	float offset = -a / 3.0;
	if (d >= 0.0) {
		float z = sqrt(d);
		vec2 x = (vec2(z, -z) - q) / 2.0;
		vec2 uv = sign(x)*pow(abs(x), vec2(1.0 / 3.0));
		return vec3(offset + uv.x + uv.y);
	}
	float v = acos(-sqrt(-27.0 / p3) * q / 2.0) / 3.0;
	float m = cos(v), n = sin(v)*1.732050808;
	return vec3(m + m, -n - m, n - m) * sqrt(-p / 3.0) + offset;
}

// Find the signed distance from a point to a quadratic bezier curve
float bezier(vec2 A, vec2 B, vec2 C, vec2 p)
{
	B = mix(B + vec2(1e-4), B, abs(sign(B * 2.0 - A - C)));
	vec2 a = B - A, b = A - B * 2.0 + C, c = a * 2.0, d = A - p;
	vec3 k = vec3(3.*dot(a, b), 2.*dot(a, a) + dot(d, b), dot(d, a)) / dot(b, b);
	vec3 t = clamp(bezier_solve(k.x, k.y, k.z), 0.0, 1.0);
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




float _vm();





float pxRange = 2.0;
vec4 bgColor = vec4(0.0, 0.0, 0.0, 1.0f);
vec4 fgColor = vec4(1.0, 1.0, 0.0, 1.0f);

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float lineWidth = 22.0;
float lineHeight = 22.0;
vec2 lines = vec2(lineWidth, lineHeight);

int rows = 20;
int cols = 20;

int cPerRow = 32;
float cPerRowf = 32.0;

vec4 getCharInGrid( int c, int whichRow, int whichCol)
{
    vec2 uv = fragCoord.xy / iResolution.xy;
    float restrictRowBefore = step((1.0 / float(cols)) * float(whichCol), uv.x);
    
    float restrictColBefore = step(uv.y, (1.0 / float(rows)) *float(rows - whichRow));
    
    float restrictRowAfter = step(uv.x, (1.0 / float(cols)) * float(whichCol + 1));
    
    float restrictColAfter = step((1.0 / float(rows)) *float(rows - whichRow - 1), uv.y);
    
    vec2 grid = vec2(rows, cols);
    
    vec2 st = uv*grid;
    
    uv = fract(st);
    
    float x = (1.0 / cPerRowf) *  fract(float(c) / cPerRowf)  * cPerRowf + uv.x / cPerRowf;
    float y = (1.0 / cPerRowf) *  (cPerRowf - float(c / cPerRow) - 1.0) + uv.y / cPerRowf;
    
    //vec2 gx = dFdx(st / 16.0);
    //    vec2 gy = dFdy(st / 16.0);
    
    return gl_FragColor += texture2D(Texture_1, vec2(x, y)).xyzw  * restrictRowAfter * restrictColAfter * restrictRowBefore *restrictColBefore;
}

// uv            left bottom is 0 0
// draw pos     left top    is 0 0

vec4 getChar( int c, float xpos, float ypos, float width, float height)
{
    vec2 uv = fragCoord.xy / iResolution.xy;
    float restrictRowBefore = step(xpos, uv.x);
    
    float restrictColBefore = step(uv.y, ypos);
    
    float restrictRowAfter = step(uv.x, xpos + width);
    
    float restrictColAfter = step(ypos - height, uv.y);
    
    vec2 grid = vec2(1/width, 1/height);
    
    //vec2 st = uv*grid;
    uv.x = uv.x - xpos;
    uv.y = uv.y + ypos;
    vec2 st = uv*grid;
    uv = fract(st);
    
    float x = (1.0 / cPerRowf) *  fract(float(c) / cPerRowf)  * cPerRowf + uv.x / cPerRowf;
    float y = (1.0 / cPerRowf) *  (cPerRowf - float(c / cPerRow) - 1.0) + uv.y / cPerRowf;
    
    //vec2 gx = dFdx(st / 16.0);
    //    vec2 gy = dFdy(st / 16.0);
    
    return gl_FragColor += texture2D(Texture_1, vec2(x, y)).xyzw  * restrictRowAfter * restrictColAfter * restrictRowBefore *restrictColBefore;
}

void show33()
{
	//width: 569height: 1922
	move_to(0.405975, 0.189386);

	line_to(0.309315, 0.593652);

	line_to(0.309315, 0.762747);

	line_to(0.70123, 0.762747);

	line_to(0.70123, 0.593652);

	line_to(0.609842, 0.189386);

	line_to(0.405975, 0.189386);

	move_to(0.323374, 0);

	line_to(0.323374, 0.10666);

	line_to(0.68717, 0.10666);

	line_to(0.68717, 0);

	line_to(0.323374, 0);


};


void show34()
{
	//width: 727height: 1922
	move_to(0.198074, 0.492716);

	line_to(0.129298, 0.637877);

	line_to(0.129298, 0.762747);

	line_to(0.411279, 0.762747);

	line_to(0.411279, 0.637877);

	line_to(0.349381, 0.492716);

	line_to(0.198074, 0.492716);

	move_to(0.65337, 0.492716);

	line_to(0.58597, 0.637877);

	line_to(0.58597, 0.762747);

	line_to(0.86795, 0.762747);

	line_to(0.86795, 0.637877);

	line_to(0.801926, 0.492716);

	line_to(0.65337, 0.492716);


};


void show35()
{
	//width: 1139height: 1922
	move_to(0.0904302, -0.0130073);

	line_to(0.166813, 0.208637);

	line_to(0.0184372, 0.208637);

	line_to(0.0184372, 0.28616);

	line_to(0.193152, 0.28616);

	line_to(0.258121, 0.475026);

	line_to(0.0184372, 0.475026);

	line_to(0.0184372, 0.552549);

	line_to(0.28446, 0.552549);

	line_to(0.360843, 0.775754);

	line_to(0.492537, 0.775754);

	line_to(0.416155, 0.552549);

	line_to(0.692713, 0.552549);

	line_to(0.769096, 0.775754);

	line_to(0.901668, 0.775754);

	line_to(0.825285, 0.552549);

	line_to(0.977173, 0.552549);

	line_to(0.977173, 0.475026);

	line_to(0.798946, 0.475026);

	line_to(0.733099, 0.28616);

	line_to(0.977173, 0.28616);

	line_to(0.977173, 0.208637);

	line_to(0.70676, 0.208637);

	line_to(0.630378, -0.0130073);

	line_to(0.498683, -0.0130073);

	line_to(0.574188, 0.208637);

	line_to(0.298507, 0.208637);

	line_to(0.222125, -0.0130073);

	line_to(0.0904302, -0.0130073);

	move_to(0.324846, 0.28616);

	line_to(0.600527, 0.28616);

	line_to(0.666374, 0.475026);

	line_to(0.389816, 0.475026);

	line_to(0.324846, 0.28616);


};


void show36()
{
	//width: 1139height: 1922
	move_to(0.447761, -0.109781);

	line_to(0.447761, -0.016129);

	curve_to(0.329236, -0.00728408, 0.254609, 0.0150884);
	curve_to(0.18086, 0.0379813, 0.126427, 0.0884495);
	curve_to(0.0728709, 0.138918, 0.0640913, 0.211759);
	line_to(0.223003, 0.229448);

	curve_to(0.24144, 0.154006, 0.287972, 0.118626);
	curve_to(0.354697, 0.0686785, 0.447761, 0.0629553);
	line_to(0.447761, 0.361082);

	curve_to(0.350307, 0.372008, 0.248464, 0.405827);
	curve_to(0.172959, 0.430801, 0.131694, 0.475026);
	curve_to(0.0913082, 0.519251, 0.0913082, 0.575442);
	curve_to(0.0913082, 0.675338, 0.210711, 0.737253);
	curve_to(0.290606, 0.778876, 0.447761, 0.788241);
	line_to(0.447761, 0.832986);

	line_to(0.540825, 0.832986);

	line_to(0.540825, 0.788241);

	curve_to(0.678666, 0.780437, 0.759438, 0.740375);
	curve_to(0.863038, 0.689386, 0.884109, 0.600416);
	line_to(0.720808, 0.585848);

	curve_to(0.70676, 0.640999, 0.661984, 0.670135);
	curve_to(0.618086, 0.699792, 0.540825, 0.709157);
	line_to(0.540825, 0.439126);

	curve_to(0.660228, 0.421436, 0.698859, 0.41155);
	curve_to(0.772608, 0.3923, 0.81914, 0.364724);
	curve_to(0.865672, 0.337149, 0.890255, 0.299168);
	curve_to(0.915716, 0.261186, 0.915716, 0.216962);
	curve_to(0.915716, 0.119667, 0.811238, 0.0546306);
	curve_to(0.70676, -0.0104058, 0.540825, -0.0150884);
	line_to(0.540825, -0.109781);

	line_to(0.447761, -0.109781);

	move_to(0.447761, 0.710198);

	curve_to(0.355575, 0.701873, 0.302019, 0.666493);
	curve_to(0.249342, 0.631113, 0.249342, 0.582726);
	curve_to(0.249342, 0.53486, 0.294118, 0.502601);
	curve_to(0.339772, 0.470343, 0.447761, 0.451093);
	line_to(0.447761, 0.710198);

	move_to(0.540825, 0.0629553);

	curve_to(0.633011, 0.069719, 0.692713, 0.110302);
	curve_to(0.753292, 0.150884, 0.753292, 0.210718);
	curve_to(0.753292, 0.261707, 0.710272, 0.292404);
	curve_to(0.66813, 0.323621, 0.540825, 0.348075);
	line_to(0.540825, 0.0629553);


};


void show37()
{
	//width: 1821height: 1922
	move_to(0.0653487, 0.579605);

	curve_to(0.0653487, 0.66129, 0.108731, 0.718522);
	curve_to(0.152114, 0.775754, 0.234487, 0.775754);
	curve_to(0.310269, 0.775754, 0.359692, 0.724246);
	curve_to(0.409665, 0.673257, 0.409665, 0.573881);
	curve_to(0.409665, 0.477107, 0.359143, 0.424558);
	curve_to(0.309171, 0.372529, 0.235585, 0.372529);
	curve_to(0.162548, 0.372529, 0.113674, 0.424037);
	curve_to(0.0653487, 0.475546, 0.0653487, 0.579605);
	move_to(0.237232, 0.711238);

	curve_to(0.200439, 0.711238, 0.175728, 0.681061);
	curve_to(0.151565, 0.650885, 0.151565, 0.570239);
	curve_to(0.151565, 0.496878, 0.176277, 0.466701);
	curve_to(0.200988, 0.437045, 0.237232, 0.437045);
	curve_to(0.274574, 0.437045, 0.298737, 0.467222);
	curve_to(0.323449, 0.497399, 0.323449, 0.577523);
	curve_to(0.323449, 0.651405, 0.298737, 0.681061);
	curve_to(0.274025, 0.711238, 0.237232, 0.711238);
	move_to(0.237781, -0.0280957);

	line_to(0.678199, 0.775754);

	line_to(0.758375, 0.775754);

	line_to(0.319605, -0.0280957);

	line_to(0.237781, -0.0280957);

	move_to(0.585942, 0.17898);

	curve_to(0.585942, 0.261186, 0.629325, 0.317898);
	curve_to(0.672707, 0.37513, 0.755629, 0.37513);
	curve_to(0.831411, 0.37513, 0.880835, 0.323621);
	curve_to(0.930807, 0.272633, 0.930807, 0.173257);
	curve_to(0.930807, 0.0764828, 0.880286, 0.0239334);
	curve_to(0.830313, -0.0280957, 0.756178, -0.0280957);
	curve_to(0.683141, -0.0280957, 0.634267, 0.0234131);
	curve_to(0.585942, 0.0754422, 0.585942, 0.17898);
	move_to(0.758375, 0.310614);

	curve_to(0.721032, 0.310614, 0.696321, 0.280437);
	curve_to(0.672158, 0.25026, 0.672158, 0.169615);
	curve_to(0.672158, 0.0967742, 0.69687, 0.0665973);
	curve_to(0.721582, 0.0364204, 0.757825, 0.0364204);
	curve_to(0.795717, 0.0364204, 0.819879, 0.0665973);
	curve_to(0.844591, 0.0967742, 0.844591, 0.176899);
	curve_to(0.844591, 0.25078, 0.819879, 0.280437);
	curve_to(0.795168, 0.310614, 0.758375, 0.310614);

};


void show38()
{
	//width: 1366height: 1922
	move_to(0.712299, 0.0900104);

	curve_to(0.647145, 0.0385016, 0.570278, 0.012487);
	curve_to(0.493411, -0.0130073, 0.4041, -0.0130073);
	curve_to(0.239385, -0.0130073, 0.142753, 0.066077);
	curve_to(0.0644217, 0.130593, 0.0644217, 0.210198);
	curve_to(0.0644217, 0.280957, 0.128111, 0.337669);
	curve_to(0.192533, 0.394901, 0.319912, 0.438085);
	curve_to(0.247438, 0.497399, 0.22328, 0.534339);
	curve_to(0.199122, 0.57128, 0.199122, 0.605619);
	curve_to(0.199122, 0.674298, 0.274524, 0.724766);
	curve_to(0.350659, 0.775754, 0.465593, 0.775754);
	curve_to(0.575403, 0.775754, 0.644949, 0.727888);
	curve_to(0.715227, 0.680021, 0.715227, 0.612903);
	curve_to(0.715227, 0.504162, 0.512445, 0.427159);
	line_to(0.704978, 0.252862);

	curve_to(0.737921, 0.298647, 0.756223, 0.359001);
	line_to(0.893119, 0.338189);

	curve_to(0.857979, 0.238293, 0.79795, 0.173777);
	curve_to(0.871889, 0.104058, 0.965593, 0.0567118);
	line_to(0.877013, -0.0176899);

	curve_to(0.797218, 0.0187305, 0.712299, 0.0900104);
	move_to(0.444363, 0.487513);

	curve_to(0.530015, 0.523413, 0.554905, 0.550468);
	curve_to(0.580527, 0.577523, 0.580527, 0.610302);
	curve_to(0.580527, 0.649324, 0.545388, 0.673777);
	curve_to(0.510981, 0.698751, 0.459004, 0.698751);
	curve_to(0.405564, 0.698751, 0.369693, 0.674298);
	curve_to(0.334553, 0.649844, 0.334553, 0.614464);
	curve_to(0.334553, 0.596774, 0.346999, 0.577003);
	curve_to(0.360176, 0.557752, 0.385798, 0.5359);
	line_to(0.444363, 0.487513);

	move_to(0.629575, 0.163892);

	line_to(0.387994, 0.376691);

	curve_to(0.281113, 0.331426, 0.243777, 0.292404);
	curve_to(0.206442, 0.253902, 0.206442, 0.215921);
	curve_to(0.206442, 0.169615, 0.258419, 0.119667);
	curve_to(0.310395, 0.069719, 0.405564, 0.069719);
	curve_to(0.464861, 0.069719, 0.527818, 0.0957336);
	curve_to(0.591508, 0.122268, 0.629575, 0.163892);

};


void show39()
{
	//width: 391height: 1922
	move_to(0.347826, 0.492716);

	line_to(0.230179, 0.635276);

	line_to(0.230179, 0.762747);

	line_to(0.754476, 0.762747);

	line_to(0.754476, 0.635276);

	line_to(0.631714, 0.492716);

	line_to(0.347826, 0.492716);


};


void show40()
{
	//width: 682height: 1922
	move_to(0.702346, -0.224246);

	curve_to(0.483871, -0.126431, 0.332845, 0.00468262);
	curve_to(0.181818, 0.135796, 0.181818, 0.276275);
	curve_to(0.181818, 0.400104, 0.294721, 0.513528);
	curve_to(0.426686, 0.645161, 0.702346, 0.775754);
	line_to(0.891496, 0.775754);

	curve_to(0.714076, 0.667534, 0.656892, 0.621228);
	curve_to(0.567449, 0.549428, 0.516129, 0.471384);
	curve_to(0.453079, 0.374089, 0.453079, 0.275754);
	curve_to(0.453079, 0.0254943, 0.891496, -0.224246);
	line_to(0.702346, -0.224246);


};


void show41()
{
	//width: 682height: 1922
	move_to(0.370968, -0.224246);

	line_to(0.181818, -0.224246);

	curve_to(0.620235, 0.0254943, 0.620235, 0.275754);
	curve_to(0.620235, 0.373569, 0.557185, 0.469823);
	curve_to(0.507331, 0.547867, 0.417889, 0.619667);
	curve_to(0.360704, 0.666493, 0.181818, 0.775754);
	line_to(0.370968, 0.775754);

	curve_to(0.646628, 0.645161, 0.778592, 0.513528);
	curve_to(0.891496, 0.400104, 0.891496, 0.276275);
	curve_to(0.891496, 0.135796, 0.739003, 0.00468262);
	curve_to(0.587977, -0.126431, 0.370968, -0.224246);

};


void show42()
{
	//width: 797height: 1922
	move_to(0.0803011, 0.622789);

	line_to(0.138018, 0.69667);

	curve_to(0.337516, 0.667534, 0.427854, 0.646202);
	curve_to(0.404015, 0.740375, 0.40276, 0.775754);
	line_to(0.584693, 0.775754);

	curve_to(0.580929, 0.724246, 0.555834, 0.646722);
	curve_to(0.685069, 0.673777, 0.851945, 0.69667);
	line_to(0.909661, 0.622789);

	curve_to(0.750314, 0.600937, 0.59724, 0.593652);
	curve_to(0.673777, 0.566077, 0.813049, 0.495317);
	line_to(0.662484, 0.451093);

	curve_to(0.589711, 0.492196, 0.49059, 0.562955);
	curve_to(0.397742, 0.489594, 0.327478, 0.451093);
	line_to(0.179423, 0.495317);

	curve_to(0.324969, 0.569719, 0.387704, 0.593652);
	curve_to(0.225847, 0.60666, 0.0803011, 0.622789);

};


void show43()
{
	//width: 1196height: 1922
	move_to(0.42893, 0.123309);

	line_to(0.42893, 0.332466);

	line_to(0.0953177, 0.332466);

	line_to(0.0953177, 0.419875);

	line_to(0.42893, 0.419875);

	line_to(0.42893, 0.627471);

	line_to(0.57107, 0.627471);

	line_to(0.57107, 0.419875);

	line_to(0.904682, 0.419875);

	line_to(0.904682, 0.332466);

	line_to(0.57107, 0.332466);

	line_to(0.57107, 0.123309);

	line_to(0.42893, 0.123309);


};


void show44()
{
	//width: 569height: 1922
	move_to(0.319859, 0);

	line_to(0.319859, 0.10666);

	line_to(0.680141, 0.10666);

	line_to(0.680141, 0);

	curve_to(0.680141, -0.0587929, 0.609842, -0.094693);
	curve_to(0.539543, -0.131113, 0.386643, -0.150884);
	line_to(0.29877, -0.110822);

	curve_to(0.398946, -0.0978148, 0.446397, -0.0723205);
	curve_to(0.493849, -0.0473465, 0.499121, 0);
	line_to(0.319859, 0);


};


void show45()
{
	//width: 682height: 1922
	move_to(0.0953079, 0.228928);

	line_to(0.0953079, 0.323101);

	line_to(0.906158, 0.323101);

	line_to(0.906158, 0.228928);

	line_to(0.0953079, 0.228928);


};


void show46()
{
	//width: 569height: 1922
	move_to(0.326889, 0);

	line_to(0.326889, 0.10666);

	line_to(0.68717, 0.10666);

	line_to(0.68717, 0);

	line_to(0.326889, 0);


};


void show47()
{
	//width: 569height: 1922
	move_to(0, -0.0130073);

	line_to(0.746924, 0.775754);

	line_to(1, 0.775754);

	line_to(0.254833, -0.0130073);

	line_to(0, -0.0130073);


};


void show48()
{
	//width: 1139height: 1922
	move_to(0.0746269, 0.376171);

	curve_to(0.0746269, 0.511446, 0.121159, 0.593652);
	curve_to(0.168569, 0.676379, 0.260755, 0.721124);
	curve_to(0.353819, 0.765869, 0.494293, 0.765869);
	curve_to(0.597893, 0.765869, 0.676032, 0.740895);
	curve_to(0.75417, 0.716441, 0.805092, 0.669615);
	curve_to(0.856014, 0.623309, 0.884987, 0.556191);
	curve_to(0.91396, 0.489594, 0.91396, 0.376171);
	curve_to(0.91396, 0.241935, 0.867428, 0.159209);
	curve_to(0.820895, 0.0770031, 0.727831, 0.0317378);
	curve_to(0.635645, -0.0130073, 0.494293, -0.0130073);
	curve_to(0.308165, -0.0130073, 0.201932, 0.066077);
	curve_to(0.0746269, 0.16129, 0.0746269, 0.376171);
	move_to(0.23705, 0.376171);

	curve_to(0.23705, 0.188345, 0.310799, 0.125911);
	curve_to(0.385426, 0.0639958, 0.494293, 0.0639958);
	curve_to(0.603161, 0.0639958, 0.67691, 0.126431);
	curve_to(0.751536, 0.188866, 0.751536, 0.376171);
	curve_to(0.751536, 0.564516, 0.67691, 0.626431);
	curve_to(0.603161, 0.688345, 0.492537, 0.688345);
	curve_to(0.38367, 0.688345, 0.318701, 0.633715);
	curve_to(0.23705, 0.563996, 0.23705, 0.376171);

};


void show49()
{
	//width: 1139height: 1922
	move_to(0.669886, 0);

	line_to(0.511853, 0);

	line_to(0.511853, 0.596774);

	curve_to(0.454785, 0.564516, 0.361721, 0.532258);
	curve_to(0.269535, 0.5, 0.195786, 0.483871);
	line_to(0.195786, 0.574402);

	curve_to(0.328358, 0.611342, 0.427568, 0.663892);
	curve_to(0.526778, 0.716441, 0.568042, 0.765869);
	line_to(0.669886, 0.765869);

	line_to(0.669886, 0);


};


void show50()
{
	//width: 1139height: 1922
	move_to(0.90518, 0.0900104);

	line_to(0.90518, 0);

	line_to(0.0544337, 0);

	curve_to(0.0526778, 0.0338189, 0.0728709, 0.0650364);
	curve_to(0.105356, 0.116545, 0.176471, 0.166493);
	curve_to(0.248464, 0.216441, 0.38367, 0.281998);
	curve_to(0.593503, 0.383975, 0.667252, 0.443288);
	curve_to(0.741001, 0.503122, 0.741001, 0.556191);
	curve_to(0.741001, 0.611863, 0.673398, 0.649844);
	curve_to(0.606673, 0.688345, 0.498683, 0.688345);
	curve_to(0.384548, 0.688345, 0.316067, 0.647763);
	curve_to(0.247586, 0.60718, 0.246708, 0.53538);
	line_to(0.0842845, 0.545265);

	curve_to(0.100966, 0.652966, 0.209833, 0.709157);
	curve_to(0.318701, 0.765869, 0.502195, 0.765869);
	curve_to(0.687445, 0.765869, 0.795435, 0.704995);
	curve_to(0.903424, 0.644121, 0.903424, 0.55411);
	curve_to(0.903424, 0.508325, 0.871817, 0.4641);
	curve_to(0.840211, 0.419875, 0.766462, 0.370968);
	curve_to(0.693591, 0.32206, 0.523266, 0.236733);
	curve_to(0.381036, 0.165973, 0.34065, 0.140479);
	curve_to(0.300263, 0.115505, 0.273924, 0.0900104);
	line_to(0.90518, 0.0900104);


};


void show51()
{
	//width: 1139height: 1922
	move_to(0.0755048, 0.201353);

	line_to(0.233538, 0.21384);

	curve_to(0.260755, 0.134235, 0.325724, 0.0988554);
	curve_to(0.391572, 0.0639958, 0.485514, 0.0639958);
	curve_to(0.597015, 0.0639958, 0.673398, 0.109781);
	curve_to(0.750658, 0.155567, 0.750658, 0.223205);
	curve_to(0.750658, 0.287721, 0.679543, 0.329344);
	curve_to(0.608428, 0.371488, 0.498683, 0.371488);
	curve_to(0.453907, 0.371488, 0.387182, 0.361082);
	line_to(0.404741, 0.443288);

	curve_to(0.420544, 0.442248, 0.430202, 0.442248);
	curve_to(0.531168, 0.442248, 0.61194, 0.473465);
	curve_to(0.692713, 0.504683, 0.692713, 0.569719);
	curve_to(0.692713, 0.621228, 0.633889, 0.655047);
	curve_to(0.575066, 0.688866, 0.482002, 0.688866);
	curve_to(0.389816, 0.688866, 0.328358, 0.654527);
	curve_to(0.266901, 0.620187, 0.249342, 0.551509);
	line_to(0.0913082, 0.568158);

	curve_to(0.120281, 0.662331, 0.223003, 0.71384);
	curve_to(0.325724, 0.765869, 0.47849, 0.765869);
	curve_to(0.583845, 0.765869, 0.67252, 0.738814);
	curve_to(0.761194, 0.712279, 0.807726, 0.665973);
	curve_to(0.855136, 0.619667, 0.855136, 0.567638);
	curve_to(0.855136, 0.51821, 0.81036, 0.477627);
	curve_to(0.765584, 0.437045, 0.677788, 0.413111);
	curve_to(0.791923, 0.397503, 0.855136, 0.348075);
	curve_to(0.918349, 0.299168, 0.918349, 0.225286);
	curve_to(0.918349, 0.12539, 0.795435, 0.0556712);
	curve_to(0.67252, -0.0135276, 0.484636, -0.0135276);
	curve_to(0.315189, -0.0135276, 0.202809, 0.0463059);
	curve_to(0.0913082, 0.106139, 0.0755048, 0.201353);

};


void show52()
{
	//width: 1139height: 1922
	move_to(0.581212, 0);

	line_to(0.581212, 0.182622);

	line_to(0.022827, 0.182622);

	line_to(0.022827, 0.26847);

	line_to(0.610184, 0.762747);

	line_to(0.739245, 0.762747);

	line_to(0.739245, 0.26847);

	line_to(0.913082, 0.26847);

	line_to(0.913082, 0.182622);

	line_to(0.739245, 0.182622);

	line_to(0.739245, 0);

	line_to(0.581212, 0);

	move_to(0.581212, 0.26847);

	line_to(0.581212, 0.612383);

	line_to(0.178227, 0.26847);

	line_to(0.581212, 0.26847);


};


void show53()
{
	//width: 1139height: 1922
	move_to(0.0746269, 0.199792);

	line_to(0.240562, 0.208117);

	curve_to(0.258999, 0.136316, 0.325724, 0.0998959);
	curve_to(0.393327, 0.0639958, 0.488147, 0.0639958);
	curve_to(0.602283, 0.0639958, 0.681299, 0.114984);
	curve_to(0.760316, 0.165973, 0.760316, 0.25026);
	curve_to(0.760316, 0.330385, 0.683933, 0.376691);
	curve_to(0.608428, 0.422997, 0.485514, 0.422997);
	curve_to(0.409131, 0.422997, 0.347673, 0.402185);
	curve_to(0.286216, 0.381894, 0.251097, 0.349115);
	line_to(0.102722, 0.360562);

	line_to(0.227392, 0.752341);

	line_to(0.867428, 0.752341);

	line_to(0.867428, 0.662851);

	line_to(0.353819, 0.662851);

	line_to(0.28446, 0.457856);

	curve_to(0.400351, 0.505723, 0.527656, 0.505723);
	curve_to(0.696225, 0.505723, 0.812116, 0.436524);
	curve_to(0.928007, 0.367326, 0.928007, 0.258585);
	curve_to(0.928007, 0.155047, 0.826163, 0.0796046);
	curve_to(0.702371, -0.0130073, 0.488147, -0.0130073);
	curve_to(0.312555, -0.0130073, 0.201054, 0.0452653);
	curve_to(0.0904302, 0.103538, 0.0746269, 0.199792);

};


void show54()
{
	//width: 1139height: 1922
	move_to(0.894644, 0.575963);

	line_to(0.737489, 0.568678);

	curve_to(0.716418, 0.623829, 0.677788, 0.648803);
	curve_to(0.613696, 0.688866, 0.519754, 0.688866);
	curve_to(0.444249, 0.688866, 0.387182, 0.663892);
	curve_to(0.312555, 0.631634, 0.269535, 0.569719);
	curve_to(0.226514, 0.507804, 0.224759, 0.39334);
	curve_to(0.281826, 0.444849, 0.364355, 0.469823);
	curve_to(0.446883, 0.494797, 0.537313, 0.494797);
	curve_to(0.695347, 0.494797, 0.80597, 0.425598);
	curve_to(0.917471, 0.35692, 0.917471, 0.247659);
	curve_to(0.917471, 0.175858, 0.864794, 0.113944);
	curve_to(0.812994, 0.0525494, 0.721686, 0.0197711);
	curve_to(0.630378, -0.0130073, 0.514486, -0.0130073);
	curve_to(0.316945, -0.0130073, 0.192274, 0.0728408);
	curve_to(0.0676032, 0.159209, 0.0676032, 0.35692);
	curve_to(0.0676032, 0.578044, 0.205443, 0.67846);
	curve_to(0.325724, 0.765869, 0.529412, 0.765869);
	curve_to(0.681299, 0.765869, 0.777875, 0.715401);
	curve_to(0.875329, 0.664932, 0.894644, 0.575963);
	move_to(0.249342, 0.247138);

	curve_to(0.249342, 0.198751, 0.283582, 0.154527);
	curve_to(0.318701, 0.110302, 0.381036, 0.0868887);
	curve_to(0.443371, 0.0639958, 0.511853, 0.0639958);
	curve_to(0.61194, 0.0639958, 0.683933, 0.111863);
	curve_to(0.755926, 0.159729, 0.755926, 0.241935);
	curve_to(0.755926, 0.32102, 0.684811, 0.366285);
	curve_to(0.613696, 0.412071, 0.505707, 0.412071);
	curve_to(0.398595, 0.412071, 0.323968, 0.366285);
	curve_to(0.249342, 0.32102, 0.249342, 0.247138);

};


void show55()
{
	//width: 1139height: 1922
	move_to(0.0851624, 0.662851);

	line_to(0.0851624, 0.752862);

	line_to(0.918349, 0.752862);

	line_to(0.918349, 0.680021);

	curve_to(0.795435, 0.602497, 0.674276, 0.473985);
	curve_to(0.553995, 0.345473, 0.488147, 0.209677);
	curve_to(0.440737, 0.113944, 0.427568, 0);
	line_to(0.265145, 0);

	curve_to(0.267779, 0.0900104, 0.324846, 0.217482);
	curve_to(0.381914, 0.344953, 0.488147, 0.463059);
	curve_to(0.595259, 0.581686, 0.71554, 0.662851);
	line_to(0.0851624, 0.662851);


};


void show56()
{
	//width: 1139height: 1922
	move_to(0.317823, 0.413632);

	curve_to(0.219491, 0.434964, 0.172081, 0.474506);
	curve_to(0.124671, 0.514048, 0.124671, 0.569199);
	curve_to(0.124671, 0.652445, 0.225637, 0.709157);
	curve_to(0.326602, 0.765869, 0.494293, 0.765869);
	curve_to(0.662862, 0.765869, 0.765584, 0.707596);
	curve_to(0.868306, 0.649844, 0.868306, 0.566597);
	curve_to(0.868306, 0.513528, 0.820895, 0.473985);
	curve_to(0.774363, 0.434964, 0.678666, 0.413632);
	curve_to(0.797191, 0.390739, 0.858648, 0.33975);
	curve_to(0.920983, 0.288762, 0.920983, 0.218002);
	curve_to(0.920983, 0.120187, 0.804214, 0.05359);
	curve_to(0.687445, -0.0130073, 0.496927, -0.0130073);
	curve_to(0.306409, -0.0130073, 0.18964, 0.05359);
	curve_to(0.0728709, 0.120708, 0.0728709, 0.220604);
	curve_to(0.0728709, 0.295005, 0.136084, 0.344953);
	curve_to(0.200176, 0.395421, 0.317823, 0.413632);
	move_to(0.286216, 0.572321);

	curve_to(0.286216, 0.51821, 0.34504, 0.483871);
	curve_to(0.403863, 0.449532, 0.497805, 0.449532);
	curve_to(0.589113, 0.449532, 0.647059, 0.483351);
	curve_to(0.705882, 0.51769, 0.705882, 0.567118);
	curve_to(0.705882, 0.618626, 0.645303, 0.653486);
	curve_to(0.585601, 0.688866, 0.496049, 0.688866);
	curve_to(0.405619, 0.688866, 0.345917, 0.654527);
	curve_to(0.286216, 0.620187, 0.286216, 0.572321);
	move_to(0.235294, 0.220083);

	curve_to(0.235294, 0.180021, 0.266901, 0.14256);
	curve_to(0.299385, 0.105099, 0.362599, 0.0842872);
	curve_to(0.425812, 0.0639958, 0.498683, 0.0639958);
	curve_to(0.61194, 0.0639958, 0.685689, 0.10718);
	curve_to(0.759438, 0.150364, 0.759438, 0.216962);
	curve_to(0.759438, 0.284599, 0.683055, 0.328824);
	curve_to(0.607551, 0.373049, 0.493415, 0.373049);
	curve_to(0.381914, 0.373049, 0.308165, 0.329344);
	curve_to(0.235294, 0.28564, 0.235294, 0.220083);

};


void show57()
{
	//width: 1139height: 1922
	move_to(0.0983319, 0.176379);

	line_to(0.250219, 0.184703);

	curve_to(0.269535, 0.121228, 0.323968, 0.0926119);
	curve_to(0.378402, 0.0639958, 0.463565, 0.0639958);
	curve_to(0.536435, 0.0639958, 0.590869, 0.0837669);
	curve_to(0.646181, 0.103538, 0.681299, 0.136316);
	curve_to(0.716418, 0.169615, 0.740123, 0.225806);
	curve_to(0.763828, 0.281998, 0.763828, 0.340271);
	curve_to(0.763828, 0.346514, 0.76295, 0.359001);
	curve_to(0.71554, 0.314256, 0.633011, 0.28616);
	curve_to(0.551361, 0.258585, 0.455663, 0.258585);
	curve_to(0.295874, 0.258585, 0.18525, 0.327263);
	curve_to(0.0746269, 0.395942, 0.0746269, 0.508325);
	curve_to(0.0746269, 0.62435, 0.18964, 0.695109);
	curve_to(0.305531, 0.765869, 0.479368, 0.765869);
	curve_to(0.604917, 0.765869, 0.708516, 0.725806);
	curve_to(0.812994, 0.685744, 0.86655, 0.611342);
	curve_to(0.920983, 0.537461, 0.920983, 0.396982);
	curve_to(0.920983, 0.25078, 0.867428, 0.163892);
	curve_to(0.813872, 0.0775234, 0.707638, 0.0322581);
	curve_to(0.602283, -0.0130073, 0.460053, -0.0130073);
	curve_to(0.309043, -0.0130073, 0.213345, 0.0364204);
	curve_to(0.117647, 0.0863684, 0.0983319, 0.176379);
	move_to(0.745391, 0.513007);

	curve_to(0.745391, 0.593652, 0.67252, 0.640999);
	curve_to(0.600527, 0.688345, 0.498683, 0.688345);
	curve_to(0.393327, 0.688345, 0.315189, 0.637357);
	curve_to(0.23705, 0.586368, 0.23705, 0.505203);
	curve_to(0.23705, 0.432362, 0.310799, 0.386576);
	curve_to(0.385426, 0.341311, 0.494293, 0.341311);
	curve_to(0.604039, 0.341311, 0.674276, 0.386576);
	curve_to(0.745391, 0.432362, 0.745391, 0.513007);

};


void show58()
{
	//width: 569height: 1922
	move_to(0.325132, 0.44589);

	line_to(0.325132, 0.552549);

	line_to(0.685413, 0.552549);

	line_to(0.685413, 0.44589);

	line_to(0.325132, 0.44589);

	move_to(0.325132, 0);

	line_to(0.325132, 0.10666);

	line_to(0.685413, 0.10666);

	line_to(0.685413, 0);

	line_to(0.325132, 0);


};


void show59()
{
	//width: 569height: 1922
	move_to(0.319859, 0.44589);

	line_to(0.319859, 0.552549);

	line_to(0.680141, 0.552549);

	line_to(0.680141, 0.44589);

	line_to(0.319859, 0.44589);

	move_to(0.319859, 0);

	line_to(0.319859, 0.10666);

	line_to(0.680141, 0.10666);

	line_to(0.680141, 0);

	curve_to(0.680141, -0.0587929, 0.609842, -0.094693);
	curve_to(0.539543, -0.131113, 0.386643, -0.150884);
	line_to(0.29877, -0.110822);

	curve_to(0.398946, -0.0978148, 0.446397, -0.0723205);
	curve_to(0.493849, -0.0473465, 0.499121, 0);
	line_to(0.319859, 0);


};


void show60()
{
	//width: 1196height: 1922
	move_to(0.0936455, 0.333507);

	line_to(0.0936455, 0.420916);

	line_to(0.905518, 0.634235);

	line_to(0.905518, 0.541103);

	line_to(0.261706, 0.376691);

	line_to(0.905518, 0.210718);

	line_to(0.905518, 0.117586);

	line_to(0.0936455, 0.333507);


};


void show61()
{
	//width: 1196height: 1922
	move_to(0.904682, 0.448491);

	line_to(0.0953177, 0.448491);

	line_to(0.0953177, 0.5359);

	line_to(0.904682, 0.5359);

	line_to(0.904682, 0.448491);

	move_to(0.904682, 0.216962);

	line_to(0.0953177, 0.216962);

	line_to(0.0953177, 0.30437);

	line_to(0.904682, 0.30437);

	line_to(0.904682, 0.216962);


};


void show62()
{
	//width: 1196height: 1922
	move_to(0.905518, 0.333507);

	line_to(0.0936455, 0.117586);

	line_to(0.0936455, 0.210718);

	line_to(0.736622, 0.376691);

	line_to(0.0936455, 0.541103);

	line_to(0.0936455, 0.634235);

	line_to(0.905518, 0.420916);

	line_to(0.905518, 0.333507);


};


void show63()
{
	//width: 1139height: 1922
	move_to(0.414399, 0.187825);

	curve_to(0.413521, 0.206556, 0.413521, 0.215921);
	curve_to(0.413521, 0.271072, 0.43986, 0.311134);
	curve_to(0.459175, 0.341311, 0.502195, 0.372008);
	curve_to(0.533802, 0.394381, 0.615452, 0.437045);
	curve_to(0.697981, 0.480229, 0.722564, 0.505723);
	curve_to(0.747147, 0.531217, 0.747147, 0.561394);
	curve_to(0.747147, 0.616025, 0.675154, 0.657128);
	curve_to(0.603161, 0.698751, 0.498683, 0.698751);
	curve_to(0.397717, 0.698751, 0.330114, 0.66129);
	curve_to(0.262511, 0.623829, 0.24144, 0.544225);
	line_to(0.0790167, 0.555671);

	curve_to(0.100966, 0.662331, 0.208955, 0.719043);
	curve_to(0.317823, 0.775754, 0.496049, 0.775754);
	curve_to(0.684811, 0.775754, 0.797191, 0.71488);
	curve_to(0.90957, 0.654006, 0.90957, 0.567638);
	curve_to(0.90957, 0.51769, 0.870061, 0.475546);
	curve_to(0.830553, 0.433403, 0.71554, 0.373049);
	curve_to(0.638279, 0.332466, 0.614574, 0.313215);
	curve_to(0.590869, 0.293965, 0.579456, 0.268991);
	curve_to(0.568042, 0.244017, 0.566286, 0.187825);
	line_to(0.414399, 0.187825);

	move_to(0.404741, 0);

	line_to(0.404741, 0.10666);

	line_to(0.584723, 0.10666);

	line_to(0.584723, 0);

	line_to(0.404741, 0);


};


void show64()
{
	//width: 2079height: 1922
	move_to(0.558442, 0.0848075);

	curve_to(0.527176, 0.0457856, 0.488215, 0.0218522);
	curve_to(0.449735, -0.00156087, 0.410774, -0.00156087);
	curve_to(0.367965, -0.00156087, 0.327561, 0.0254943);
	curve_to(0.287157, 0.0525494, 0.261664, 0.108741);
	curve_to(0.236652, 0.164932, 0.236652, 0.23205);
	curve_to(0.236652, 0.314776, 0.275613, 0.397503);
	curve_to(0.315055, 0.480749, 0.373256, 0.522373);
	curve_to(0.431457, 0.563996, 0.486291, 0.563996);
	curve_to(0.528139, 0.563996, 0.566138, 0.540062);
	curve_to(0.604137, 0.516649, 0.631554, 0.468262);
	line_to(0.647908, 0.548907);

	line_to(0.734007, 0.548907);

	line_to(0.664743, 0.199792);

	curve_to(0.650313, 0.126951, 0.650313, 0.119147);
	curve_to(0.650313, 0.105099, 0.659933, 0.094693);
	curve_to(0.670034, 0.0848075, 0.683983, 0.0848075);
	curve_to(0.709476, 0.0848075, 0.750842, 0.116545);
	curve_to(0.805676, 0.158169, 0.837422, 0.227888);
	curve_to(0.869649, 0.298127, 0.869649, 0.372529);
	curve_to(0.869649, 0.459417, 0.828283, 0.53486);
	curve_to(0.787398, 0.610302, 0.705628, 0.655567);
	curve_to(0.624339, 0.700832, 0.525734, 0.700832);
	curve_to(0.413179, 0.700832, 0.319865, 0.6436);
	curve_to(0.227032, 0.586889, 0.175565, 0.480229);
	curve_to(0.124579, 0.374089, 0.124579, 0.252341);
	curve_to(0.124579, 0.12487, 0.175565, 0.0327784);
	curve_to(0.227032, -0.0593132, 0.323713, -0.103018);
	curve_to(0.420875, -0.147242, 0.538721, -0.147242);
	curve_to(0.664743, -0.147242, 0.74988, -0.101457);
	curve_to(0.835017, -0.0556712, 0.877345, 0.00988554);
	line_to(0.964406, 0.00988554);

	curve_to(0.939875, -0.0447451, 0.880231, -0.101457);
	curve_to(0.820587, -0.158169, 0.738336, -0.190947);
	curve_to(0.656085, -0.224246, 0.540164, -0.224246);
	curve_to(0.433381, -0.224246, 0.342953, -0.194589);
	curve_to(0.253006, -0.164932, 0.189514, -0.105099);
	curve_to(0.126022, -0.0457856, 0.0937951, 0.0317378);
	curve_to(0.0533911, 0.130073, 0.0533911, 0.244017);
	curve_to(0.0533911, 0.370968, 0.101491, 0.485952);
	curve_to(0.160173, 0.626951, 0.267917, 0.701873);
	curve_to(0.376142, 0.776795, 0.530063, 0.776795);
	curve_to(0.649351, 0.776795, 0.744108, 0.723725);
	curve_to(0.839346, 0.671176, 0.89418, 0.566597);
	curve_to(0.940837, 0.476587, 0.940837, 0.370968);
	curve_to(0.940837, 0.220083, 0.842713, 0.103018);
	curve_to(0.755171, -0.00208117, 0.651275, -0.00208117);
	curve_to(0.618086, -0.00208117, 0.597403, 0.00884495);
	curve_to(0.577201, 0.0197711, 0.567581, 0.0400624);
	curve_to(0.561328, 0.0530697, 0.558442, 0.0848075);
	move_to(0.325637, 0.225806);

	curve_to(0.325637, 0.154527, 0.356902, 0.114984);
	curve_to(0.388167, 0.0754422, 0.428571, 0.0754422);
	curve_to(0.455507, 0.0754422, 0.485329, 0.0926119);
	curve_to(0.515152, 0.110302, 0.542088, 0.144641);
	curve_to(0.569505, 0.17898, 0.586821, 0.23153);
	curve_to(0.604137, 0.284599, 0.604137, 0.337669);
	curve_to(0.604137, 0.408429, 0.571429, 0.447451);
	curve_to(0.539202, 0.486472, 0.492545, 0.486472);
	curve_to(0.46176, 0.486472, 0.434343, 0.469303);
	curve_to(0.407407, 0.452653, 0.381914, 0.415193);
	curve_to(0.356421, 0.377732, 0.341029, 0.324142);
	curve_to(0.325637, 0.270552, 0.325637, 0.225806);

};


void show65()
{
	//width: 1366height: 1922
	move_to(-0.00219619, 0);

	line_to(0.409956, 0.762747);

	line_to(0.562958, 0.762747);

	line_to(1.0022, 0);

	line_to(0.84041, 0);

	line_to(0.715227, 0.231009);

	line_to(0.266471, 0.231009);

	line_to(0.148609, 0);

	line_to(-0.00219619, 0);

	move_to(0.307467, 0.313215);

	line_to(0.671303, 0.313215);

	line_to(0.559297, 0.524454);

	curve_to(0.508053, 0.620708, 0.483163, 0.682622);
	curve_to(0.462665, 0.609261, 0.425329, 0.536941);
	line_to(0.307467, 0.313215);


};


void show66()
{
	//width: 1366height: 1922
	move_to(0.10981, 0);

	line_to(0.10981, 0.762747);

	line_to(0.512445, 0.762747);

	curve_to(0.635432, 0.762747, 0.70937, 0.739334);
	curve_to(0.784041, 0.716441, 0.825769, 0.668054);
	curve_to(0.868228, 0.620187, 0.868228, 0.567638);
	curve_to(0.868228, 0.51873, 0.830893, 0.475546);
	curve_to(0.793558, 0.432362, 0.718155, 0.405827);
	curve_to(0.81552, 0.385536, 0.867496, 0.336629);
	curve_to(0.920205, 0.287721, 0.920205, 0.221124);
	curve_to(0.920205, 0.167534, 0.887994, 0.121228);
	curve_to(0.856515, 0.0754422, 0.809663, 0.0504683);
	curve_to(0.762811, 0.0254943, 0.691801, 0.012487);
	curve_to(0.621523, 0, 0.519034, 0);
	line_to(0.10981, 0);

	move_to(0.25183, 0.442248);

	line_to(0.483895, 0.442248);

	curve_to(0.578331, 0.442248, 0.619326, 0.451093);
	curve_to(0.673499, 0.462539, 0.700586, 0.489074);
	curve_to(0.728404, 0.515609, 0.728404, 0.555671);
	curve_to(0.728404, 0.593652, 0.702782, 0.622268);
	curve_to(0.67716, 0.651405, 0.629575, 0.661811);
	curve_to(0.581991, 0.672737, 0.466325, 0.672737);
	line_to(0.25183, 0.672737);

	line_to(0.25183, 0.442248);

	move_to(0.25183, 0.0900104);

	line_to(0.519034, 0.0900104);

	curve_to(0.587848, 0.0900104, 0.615666, 0.0936524);
	curve_to(0.664715, 0.0998959, 0.697657, 0.114464);
	curve_to(0.7306, 0.129032, 0.75183, 0.156608);
	curve_to(0.77306, 0.184703, 0.77306, 0.221124);
	curve_to(0.77306, 0.263788, 0.742313, 0.295005);
	curve_to(0.711567, 0.326743, 0.656662, 0.33923);
	curve_to(0.602489, 0.352237, 0.5, 0.352237);
	line_to(0.25183, 0.352237);

	line_to(0.25183, 0.0900104);


};


void show67()
{
	//width: 1479height: 1922
	move_to(0.814064, 0.26743);

	line_to(0.945233, 0.241935);

	curve_to(0.903989, 0.117586, 0.796484, 0.0520291);
	curve_to(0.689655, -0.0130073, 0.534821, -0.0130073);
	curve_to(0.374577, -0.0130073, 0.273834, 0.0369407);
	curve_to(0.173766, 0.087409, 0.121028, 0.182622);
	curve_to(0.0689655, 0.277836, 0.0689655, 0.387097);
	curve_to(0.0689655, 0.506243, 0.127789, 0.594693);
	curve_to(0.187289, 0.683663, 0.296146, 0.729448);
	curve_to(0.40568, 0.775754, 0.536849, 0.775754);
	curve_to(0.685598, 0.775754, 0.787018, 0.717482);
	curve_to(0.888438, 0.659209, 0.92833, 0.55359);
	line_to(0.799189, 0.530177);

	curve_to(0.764706, 0.613424, 0.699121, 0.651405);
	curve_to(0.633536, 0.689386, 0.534145, 0.689386);
	curve_to(0.419878, 0.689386, 0.342799, 0.647242);
	curve_to(0.266396, 0.605099, 0.235294, 0.533819);
	curve_to(0.204192, 0.463059, 0.204192, 0.387617);
	curve_to(0.204192, 0.290323, 0.240703, 0.217482);
	curve_to(0.27789, 0.145161, 0.355646, 0.109261);
	curve_to(0.433401, 0.0733611, 0.524003, 0.0733611);
	curve_to(0.634212, 0.0733611, 0.710615, 0.122268);
	curve_to(0.787018, 0.171176, 0.814064, 0.26743);

};


void show68()
{
	//width: 1479height: 1922
	move_to(0.106829, 0);

	line_to(0.106829, 0.762747);

	line_to(0.448276, 0.762747);

	curve_to(0.563895, 0.762747, 0.624746, 0.751821);
	curve_to(0.709939, 0.736733, 0.770115, 0.69719);
	curve_to(0.848546, 0.646202, 0.887086, 0.566597);
	curve_to(0.926302, 0.487513, 0.926302, 0.385536);
	curve_to(0.926302, 0.298647, 0.899932, 0.23153);
	curve_to(0.873563, 0.164412, 0.832319, 0.120187);
	curve_to(0.791075, 0.0764828, 0.741717, 0.0509886);
	curve_to(0.693036, 0.0260146, 0.623394, 0.0130073);
	curve_to(0.554429, 0, 0.464503, 0);
	line_to(0.106829, 0);

	move_to(0.237999, 0.0900104);

	line_to(0.449628, 0.0900104);

	curve_to(0.547667, 0.0900104, 0.60311, 0.104058);
	curve_to(0.659229, 0.118106, 0.69236, 0.1436);
	curve_to(0.739013, 0.179501, 0.764706, 0.239854);
	curve_to(0.791075, 0.300728, 0.791075, 0.387097);
	curve_to(0.791075, 0.506764, 0.739689, 0.57076);
	curve_to(0.688979, 0.635276, 0.615957, 0.657128);
	curve_to(0.563218, 0.672737, 0.446247, 0.672737);
	line_to(0.237999, 0.672737);

	line_to(0.237999, 0.0900104);


};


void show69()
{
	//width: 1366height: 1922
	move_to(0.118594, 0);

	line_to(0.118594, 0.762747);

	line_to(0.894583, 0.762747);

	line_to(0.894583, 0.672737);

	line_to(0.260615, 0.672737);

	line_to(0.260615, 0.439126);

	line_to(0.854319, 0.439126);

	line_to(0.854319, 0.349636);

	line_to(0.260615, 0.349636);

	line_to(0.260615, 0.0900104);

	line_to(0.919473, 0.0900104);

	line_to(0.919473, 0);

	line_to(0.118594, 0);


};


void show70()
{
	//width: 1251height: 1922
	move_to(0.134293, 0);

	line_to(0.134293, 0.762747);

	line_to(0.92486, 0.762747);

	line_to(0.92486, 0.672737);

	line_to(0.289369, 0.672737);

	line_to(0.289369, 0.436524);

	line_to(0.839329, 0.436524);

	line_to(0.839329, 0.346514);

	line_to(0.289369, 0.346514);

	line_to(0.289369, 0);

	line_to(0.134293, 0);


};


void show71()
{
	//width: 1593height: 1922
	move_to(0.529818, 0.299168);

	line_to(0.529818, 0.388658);

	line_to(0.919648, 0.389178);

	line_to(0.919648, 0.106139);

	curve_to(0.829881, 0.0468262, 0.734463, 0.0166493);
	curve_to(0.639046, -0.0130073, 0.538606, -0.0130073);
	curve_to(0.403013, -0.0130073, 0.291902, 0.0348595);
	curve_to(0.181419, 0.0832466, 0.124922, 0.174298);
	curve_to(0.0684244, 0.265349, 0.0684244, 0.377732);
	curve_to(0.0684244, 0.489074, 0.124294, 0.585328);
	curve_to(0.180791, 0.682102, 0.286252, 0.728928);
	curve_to(0.391714, 0.775754, 0.52919, 0.775754);
	curve_to(0.629002, 0.775754, 0.709353, 0.748699);
	curve_to(0.790333, 0.722164, 0.836158, 0.674298);
	curve_to(0.881984, 0.626431, 0.905838, 0.549428);
	line_to(0.795982, 0.524454);

	curve_to(0.775267, 0.582726, 0.744507, 0.616025);
	curve_to(0.713748, 0.649324, 0.656623, 0.669095);
	curve_to(0.599498, 0.689386, 0.529818, 0.689386);
	curve_to(0.446328, 0.689386, 0.385436, 0.668054);
	curve_to(0.324545, 0.647242, 0.28688, 0.612903);
	curve_to(0.249843, 0.578564, 0.229127, 0.537461);
	curve_to(0.193974, 0.466701, 0.193974, 0.383975);
	curve_to(0.193974, 0.281998, 0.236033, 0.213319);
	curve_to(0.278719, 0.144641, 0.359699, 0.111342);
	curve_to(0.440678, 0.0780437, 0.531701, 0.0780437);
	curve_to(0.610797, 0.0780437, 0.686127, 0.103018);
	curve_to(0.761456, 0.128512, 0.800377, 0.157128);
	line_to(0.800377, 0.299168);

	line_to(0.529818, 0.299168);


};


void show72()
{
	//width: 1479height: 1922
	move_to(0.110886, 0);

	line_to(0.110886, 0.762747);

	line_to(0.242055, 0.762747);

	line_to(0.242055, 0.449532);

	line_to(0.757268, 0.449532);

	line_to(0.757268, 0.762747);

	line_to(0.888438, 0.762747);

	line_to(0.888438, 0);

	line_to(0.757268, 0);

	line_to(0.757268, 0.359521);

	line_to(0.242055, 0.359521);

	line_to(0.242055, 0);

	line_to(0.110886, 0);


};


void show73()
{
	//width: 569height: 1922
	move_to(0.335677, 0);

	line_to(0.335677, 0.762747);

	line_to(0.676626, 0.762747);

	line_to(0.676626, 0);

	line_to(0.335677, 0);


};


void show74()
{
	//width: 1024height: 1922
	move_to(0.0576172, 0.216441);

	line_to(0.228516, 0.228928);

	curve_to(0.235352, 0.141519, 0.290039, 0.109261);
	curve_to(0.344727, 0.0770031, 0.441406, 0.0770031);
	curve_to(0.512695, 0.0770031, 0.564453, 0.0941727);
	curve_to(0.616211, 0.111863, 0.635742, 0.141519);
	curve_to(0.655273, 0.171696, 0.655273, 0.237253);
	line_to(0.655273, 0.762747);

	line_to(0.844727, 0.762747);

	line_to(0.844727, 0.242976);

	curve_to(0.844727, 0.147242, 0.800781, 0.094693);
	curve_to(0.757813, 0.0421436, 0.663086, 0.0145682);
	curve_to(0.569336, -0.0130073, 0.442383, -0.0130073);
	curve_to(0.253906, -0.0130073, 0.15332, 0.0447451);
	curve_to(0.0537109, 0.102497, 0.0576172, 0.216441);

};


void show75()
{
	//width: 1366height: 1922
	move_to(0.10981, 0);

	line_to(0.10981, 0.762747);

	line_to(0.25183, 0.762747);

	line_to(0.25183, 0.384495);

	line_to(0.784773, 0.762747);

	line_to(0.977306, 0.762747);

	line_to(0.527086, 0.453694);

	line_to(0.997072, 0);

	line_to(0.809663, 0);

	line_to(0.427526, 0.386056);

	line_to(0.25183, 0.264308);

	line_to(0.25183, 0);

	line_to(0.10981, 0);


};


void show76()
{
	//width: 1139height: 1922
	move_to(0.131694, 0);

	line_to(0.131694, 0.762747);

	line_to(0.302019, 0.762747);

	line_to(0.302019, 0.0900104);

	line_to(0.935909, 0.0900104);

	line_to(0.935909, 0);

	line_to(0.131694, 0);


};


void show77()
{
	//width: 1706height: 1922
	move_to(0.0890973, 0);

	line_to(0.0890973, 0.762747);

	line_to(0.260258, 0.762747);

	line_to(0.463658, 0.222685);

	curve_to(0.491794, 0.147242, 0.504689, 0.109781);
	curve_to(0.519343, 0.151405, 0.55041, 0.23205);
	line_to(0.756155, 0.762747);

	line_to(0.909144, 0.762747);

	line_to(0.909144, 0);

	line_to(0.799531, 0);

	line_to(0.799531, 0.638398);

	line_to(0.549824, 0);

	line_to(0.447245, 0);

	line_to(0.19871, 0.649324);

	line_to(0.19871, 0);

	line_to(0.0890973, 0);


};


void show78()
{
	//width: 1479height: 1922
	move_to(0.105477, 0);

	line_to(0.105477, 0.762747);

	line_to(0.240027, 0.762747);

	line_to(0.760649, 0.163892);

	line_to(0.760649, 0.762747);

	line_to(0.88641, 0.762747);

	line_to(0.88641, 0);

	line_to(0.751859, 0);

	line_to(0.231237, 0.599376);

	line_to(0.231237, 0);

	line_to(0.105477, 0);


};


void show79()
{
	//width: 1593height: 1922
	move_to(0.0621469, 0.371488);

	curve_to(0.0621469, 0.561394, 0.185185, 0.668574);
	curve_to(0.308223, 0.776275, 0.502825, 0.776275);
	curve_to(0.630257, 0.776275, 0.73258, 0.725806);
	curve_to(0.834903, 0.675338, 0.888261, 0.584808);
	curve_to(0.942247, 0.494797, 0.942247, 0.380333);
	curve_to(0.942247, 0.264308, 0.88575, 0.172737);
	curve_to(0.829253, 0.0811655, 0.725675, 0.0338189);
	curve_to(0.622097, -0.0130073, 0.502197, -0.0130073);
	curve_to(0.372254, -0.0130073, 0.269931, 0.0390219);
	curve_to(0.167608, 0.091051, 0.114878, 0.181061);
	curve_to(0.0621469, 0.271072, 0.0621469, 0.371488);
	move_to(0.187696, 0.369927);

	curve_to(0.187696, 0.23205, 0.276836, 0.152445);
	curve_to(0.366604, 0.0733611, 0.501569, 0.0733611);
	curve_to(0.639046, 0.0733611, 0.727558, 0.153486);
	curve_to(0.816698, 0.233611, 0.816698, 0.380853);
	curve_to(0.816698, 0.473985, 0.778406, 0.543184);
	curve_to(0.740741, 0.612903, 0.667294, 0.650885);
	curve_to(0.594476, 0.689386, 0.503453, 0.689386);
	curve_to(0.374137, 0.689386, 0.280603, 0.615505);
	curve_to(0.187696, 0.542144, 0.187696, 0.369927);

};


void show80()
{
	//width: 1366height: 1922
	move_to(0.115666, 0);

	line_to(0.115666, 0.762747);

	line_to(0.520498, 0.762747);

	curve_to(0.627379, 0.762747, 0.683748, 0.755463);
	curve_to(0.762811, 0.746098, 0.816252, 0.719563);
	curve_to(0.869693, 0.693548, 0.901903, 0.646202);
	curve_to(0.934846, 0.598855, 0.934846, 0.542144);
	curve_to(0.934846, 0.444849, 0.847731, 0.377211);
	curve_to(0.760615, 0.310094, 0.532943, 0.310094);
	line_to(0.257687, 0.310094);

	line_to(0.257687, 0);

	line_to(0.115666, 0);

	move_to(0.257687, 0.400104);

	line_to(0.535139, 0.400104);

	curve_to(0.672767, 0.400104, 0.7306, 0.436524);
	curve_to(0.788433, 0.472945, 0.788433, 0.539022);
	curve_to(0.788433, 0.586889, 0.754026, 0.620708);
	curve_to(0.720351, 0.655047, 0.664715, 0.665973);
	curve_to(0.628843, 0.672737, 0.532211, 0.672737);
	line_to(0.257687, 0.672737);

	line_to(0.257687, 0.400104);


};


void show81()
{
	//width: 1593height: 1922
	move_to(0.79661, 0.0816857);

	curve_to(0.881356, 0.0332986, 0.952919, 0.0109261);
	line_to(0.917138, -0.0593132);

	curve_to(0.817954, -0.0296566, 0.719397, 0.0343392);
	curve_to(0.617075, -0.0130073, 0.493409, -0.0130073);
	curve_to(0.368487, -0.0130073, 0.266792, 0.0369407);
	curve_to(0.165097, 0.0868887, 0.109856, 0.177419);
	curve_to(0.0552417, 0.26795, 0.0552417, 0.381374);
	curve_to(0.0552417, 0.494277, 0.110483, 0.586889);
	curve_to(0.165725, 0.679501, 0.26742, 0.727888);
	curve_to(0.369743, 0.776275, 0.49592, 0.776275);
	curve_to(0.623352, 0.776275, 0.725675, 0.725806);
	curve_to(0.827998, 0.675858, 0.881356, 0.585328);
	curve_to(0.935342, 0.495317, 0.935342, 0.381894);
	curve_to(0.935342, 0.287721, 0.900816, 0.212279);
	curve_to(0.86629, 0.137357, 0.79661, 0.0816857);
	move_to(0.528562, 0.210718);

	curve_to(0.634024, 0.186264, 0.702448, 0.137877);
	curve_to(0.809793, 0.219043, 0.809793, 0.381894);
	curve_to(0.809793, 0.474506, 0.7715, 0.543704);
	curve_to(0.733836, 0.612903, 0.660389, 0.650885);
	curve_to(0.587571, 0.689386, 0.496547, 0.689386);
	curve_to(0.360326, 0.689386, 0.270559, 0.611863);
	curve_to(0.180791, 0.53486, 0.180791, 0.381374);
	curve_to(0.180791, 0.23257, 0.269303, 0.152966);
	curve_to(0.358443, 0.0733611, 0.496547, 0.0733611);
	curve_to(0.561833, 0.0733611, 0.619586, 0.0936524);
	curve_to(0.562461, 0.12435, 0.499058, 0.137357);
	line_to(0.528562, 0.210718);


};


void show82()
{
	//width: 1479height: 1922
	move_to(0.108857, 0);

	line_to(0.108857, 0.762747);

	line_to(0.548343, 0.762747);

	curve_to(0.680865, 0.762747, 0.749831, 0.741935);
	curve_to(0.818796, 0.721644, 0.860041, 0.669615);
	curve_to(0.901285, 0.617586, 0.901285, 0.554631);
	curve_to(0.901285, 0.473465, 0.832995, 0.417794);
	curve_to(0.764706, 0.362123, 0.622042, 0.347034);
	curve_to(0.674104, 0.327784, 0.701149, 0.309053);
	curve_to(0.758621, 0.26847, 0.810007, 0.207596);
	line_to(0.982421, 0);

	line_to(0.817444, 0);

	line_to(0.686275, 0.158689);

	curve_to(0.628803, 0.227367, 0.591616, 0.263788);
	curve_to(0.554429, 0.300208, 0.524679, 0.314776);
	curve_to(0.495605, 0.329344, 0.465179, 0.335068);
	curve_to(0.442867, 0.33871, 0.392157, 0.33871);
	line_to(0.240027, 0.33871);

	line_to(0.240027, 0);

	line_to(0.108857, 0);

	move_to(0.240027, 0.426119);

	line_to(0.521974, 0.426119);

	curve_to(0.6119, 0.426119, 0.66261, 0.440167);
	curve_to(0.71332, 0.454735, 0.739689, 0.485952);
	curve_to(0.766058, 0.51769, 0.766058, 0.554631);
	curve_to(0.766058, 0.608741, 0.714672, 0.6436);
	curve_to(0.663962, 0.67846, 0.553753, 0.67846);
	line_to(0.240027, 0.67846);

	line_to(0.240027, 0.426119);


};


void show83()
{
	//width: 1366height: 1922
	move_to(0.0673499, 0.245057);

	line_to(0.201318, 0.253382);

	curve_to(0.210835, 0.19615, 0.245242, 0.159209);
	curve_to(0.280381, 0.122789, 0.353587, 0.0998959);
	curve_to(0.426794, 0.0775234, 0.518302, 0.0775234);
	curve_to(0.599561, 0.0775234, 0.661786, 0.094693);
	curve_to(0.724012, 0.111863, 0.754026, 0.141519);
	curve_to(0.784773, 0.171696, 0.784773, 0.207076);
	curve_to(0.784773, 0.242976, 0.75549, 0.269511);
	curve_to(0.726208, 0.296566, 0.658858, 0.314776);
	curve_to(0.615666, 0.326743, 0.467789, 0.351717);
	curve_to(0.319912, 0.377211, 0.260615, 0.399584);
	curve_to(0.183748, 0.4282, 0.145681, 0.470343);
	curve_to(0.108346, 0.513007, 0.108346, 0.565557);
	curve_to(0.108346, 0.623309, 0.154466, 0.673257);
	curve_to(0.200586, 0.723725, 0.289165, 0.74974);
	curve_to(0.377745, 0.775754, 0.486091, 0.775754);
	curve_to(0.605417, 0.775754, 0.696193, 0.748179);
	curve_to(0.787701, 0.721124, 0.83675, 0.668054);
	curve_to(0.885798, 0.614984, 0.889458, 0.547867);
	line_to(0.753294, 0.540583);

	curve_to(0.742313, 0.612903, 0.678624, 0.649844);
	curve_to(0.615666, 0.686785, 0.491947, 0.686785);
	curve_to(0.363104, 0.686785, 0.303807, 0.652966);
	curve_to(0.245242, 0.619667, 0.245242, 0.572321);
	curve_to(0.245242, 0.531217, 0.286969, 0.504683);
	curve_to(0.327965, 0.478148, 0.500732, 0.450052);
	curve_to(0.674231, 0.422477, 0.738653, 0.401665);
	curve_to(0.832357, 0.370968, 0.877013, 0.323621);
	curve_to(0.921669, 0.276795, 0.921669, 0.215401);
	curve_to(0.921669, 0.154527, 0.872621, 0.100416);
	curve_to(0.823572, 0.0468262, 0.731332, 0.0166493);
	curve_to(0.639824, -0.0130073, 0.52489, -0.0130073);
	curve_to(0.379209, -0.0130073, 0.280381, 0.0171696);
	curve_to(0.182284, 0.0473465, 0.125915, 0.1077);
	curve_to(0.0702782, 0.168574, 0.0673499, 0.245057);

};


void show84()
{
	//width: 1251height: 1922
	move_to(0.42446, 0);

	line_to(0.42446, 0.672737);

	line_to(0.0383693, 0.672737);

	line_to(0.0383693, 0.762747);

	line_to(0.967226, 0.762747);

	line_to(0.967226, 0.672737);

	line_to(0.579536, 0.672737);

	line_to(0.579536, 0);

	line_to(0.42446, 0);


};


void show85()
{
	//width: 1479height: 1922
	move_to(0.757268, 0.762747);

	line_to(0.888438, 0.762747);

	line_to(0.888438, 0.32206);

	curve_to(0.888438, 0.207076, 0.854631, 0.139438);
	curve_to(0.820825, 0.0718002, 0.732252, 0.0291363);
	curve_to(0.644354, -0.0130073, 0.501014, -0.0130073);
	curve_to(0.361731, -0.0130073, 0.273158, 0.0239334);
	curve_to(0.184584, 0.0608741, 0.146721, 0.130593);
	curve_to(0.108857, 0.200832, 0.108857, 0.32206);
	line_to(0.108857, 0.762747);

	line_to(0.240027, 0.762747);

	line_to(0.240027, 0.322581);

	curve_to(0.240027, 0.223205, 0.263692, 0.175858);
	curve_to(0.288032, 0.129032, 0.34618, 0.103538);
	curve_to(0.405003, 0.0780437, 0.48952, 0.0780437);
	curve_to(0.634212, 0.0780437, 0.69574, 0.128512);
	curve_to(0.757268, 0.17898, 0.757268, 0.322581);
	line_to(0.757268, 0.762747);


};


void show86()
{
	//width: 1366height: 1922
	move_to(0.422401, 0);

	line_to(0.00658858, 0.762747);

	line_to(0.160322, 0.762747);

	line_to(0.439239, 0.208637);

	curve_to(0.472914, 0.14204, 0.495608, 0.0837669);
	curve_to(0.520498, 0.146202, 0.553441, 0.208637);
	line_to(0.843338, 0.762747);

	line_to(0.988287, 0.762747);

	line_to(0.568082, 0);

	line_to(0.422401, 0);


};


void show87()
{
	//width: 1933height: 1922
	move_to(0.214175, 0);

	line_to(0.0129333, 0.762747);

	line_to(0.115882, 0.762747);

	line_to(0.231247, 0.262747);

	curve_to(0.249871, 0.184183, 0.263321, 0.10666);
	curve_to(0.292292, 0.228928, 0.297465, 0.247659);
	line_to(0.4418, 0.762747);

	line_to(0.562856, 0.762747);

	line_to(0.671495, 0.376691);

	curve_to(0.712364, 0.233091, 0.730471, 0.10666);
	curve_to(0.744956, 0.17898, 0.768236, 0.272633);
	line_to(0.887222, 0.762747);

	line_to(0.988101, 0.762747);

	line_to(0.780134, 0);

	line_to(0.683394, 0);

	line_to(0.523539, 0.581165);

	curve_to(0.503363, 0.654006, 0.499741, 0.670656);
	curve_to(0.487843, 0.618106, 0.477496, 0.581165);
	line_to(0.316606, 0);

	line_to(0.214175, 0);


};


void show88()
{
	//width: 1366height: 1922
	move_to(0.00658858, 0);

	line_to(0.421669, 0.397503);

	line_to(0.0556369, 0.762747);

	line_to(0.224744, 0.762747);

	line_to(0.419473, 0.567118);

	curve_to(0.480234, 0.506243, 0.505857, 0.473465);
	curve_to(0.541728, 0.515088, 0.590776, 0.560354);
	line_to(0.806735, 0.762747);

	line_to(0.961201, 0.762747);

	line_to(0.584187, 0.403226);

	line_to(0.990483, 0);

	line_to(0.814788, 0);

	line_to(0.544656, 0.272112);

	curve_to(0.521962, 0.295525, 0.497804, 0.323101);
	curve_to(0.461933, 0.281478, 0.446559, 0.265869);
	line_to(0.17716, 0);

	line_to(0.00658858, 0);


};


void show89()
{
	//width: 1366height: 1922
	move_to(0.418009, 0);

	line_to(0.418009, 0.323101);

	line_to(0.00439239, 0.762747);

	line_to(0.17716, 0.762747);

	line_to(0.388726, 0.532778);

	curve_to(0.447291, 0.468262, 0.497804, 0.403746);
	curve_to(0.54612, 0.46358, 0.614934, 0.538502);
	line_to(0.82284, 0.762747);

	line_to(0.988287, 0.762747);

	line_to(0.560029, 0.323101);

	line_to(0.560029, 0);

	line_to(0.418009, 0);


};


void show90()
{
	//width: 1251height: 1922
	move_to(0.0327738, 0);

	line_to(0.0327738, 0.0936524);

	line_to(0.633094, 0.582206);

	curve_to(0.697042, 0.634235, 0.754596, 0.672737);
	line_to(0.100719, 0.672737);

	line_to(0.100719, 0.762747);

	line_to(0.940048, 0.762747);

	line_to(0.940048, 0.672737);

	line_to(0.282174, 0.1436);

	line_to(0.211031, 0.0900104);

	line_to(0.959233, 0.0900104);

	line_to(0.959233, 0);

	line_to(0.0327738, 0);


};


void show91()
{
	//width: 569height: 1922
	move_to(0.244288, -0.211759);

	line_to(0.244288, 0.762747);

	line_to(0.942003, 0.762747);

	line_to(0.942003, 0.685224);

	line_to(0.560633, 0.685224);

	line_to(0.560633, -0.134235);

	line_to(0.942003, -0.134235);

	line_to(0.942003, -0.211759);

	line_to(0.244288, -0.211759);


};


void show92()
{
	//width: 569height: 1922
	move_to(0.746924, -0.0130073);

	line_to(0, 0.775754);

	line_to(0.254833, 0.775754);

	line_to(1, -0.0130073);

	line_to(0.746924, -0.0130073);


};


void show93()
{
	//width: 569height: 1922
	move_to(0.766257, -0.211759);

	line_to(0.0685413, -0.211759);

	line_to(0.0685413, -0.134235);

	line_to(0.449912, -0.134235);

	line_to(0.449912, 0.685224);

	line_to(0.0685413, 0.685224);

	line_to(0.0685413, 0.762747);

	line_to(0.766257, 0.762747);

	line_to(0.766257, -0.211759);


};


void show94()
{
	//width: 961height: 1922
	move_to(0.248699, 0.359001);

	line_to(0.0561915, 0.359001);

	line_to(0.423517, 0.775754);

	line_to(0.574402, 0.775754);

	line_to(0.943809, 0.359001);

	line_to(0.755463, 0.359001);

	line_to(0.498439, 0.669615);

	line_to(0.248699, 0.359001);


};


void show95()
{
	//width: 1139height: 1922
	move_to(-0.0272169, -0.211759);

	line_to(-0.0272169, -0.144121);

	line_to(1.02019, -0.144121);

	line_to(1.02019, -0.211759);

	line_to(-0.0272169, -0.211759);


};


void show96()
{
	//width: 682height: 1922
	move_to(0.681818, 0.621228);

	line_to(0.469208, 0.621228);

	line_to(0.130499, 0.766909);

	line_to(0.483871, 0.766909);

	line_to(0.681818, 0.621228);


};


void show97()
{
	//width: 1139height: 1922
	move_to(0.726953, 0.0681582);

	curve_to(0.639157, 0.0239334, 0.557507, 0.0057232);
	curve_to(0.476734, -0.012487, 0.38367, -0.012487);
	curve_to(0.230026, -0.012487, 0.147498, 0.0317378);
	curve_to(0.0649693, 0.0764828, 0.0649693, 0.145682);
	curve_to(0.0649693, 0.186264, 0.095698, 0.219563);
	curve_to(0.127305, 0.253382, 0.177349, 0.273673);
	curve_to(0.22827, 0.293965, 0.291484, 0.30437);
	curve_to(0.338016, 0.311655, 0.431958, 0.318418);
	curve_to(0.623354, 0.331946, 0.713784, 0.350676);
	curve_to(0.714662, 0.369927, 0.714662, 0.37513);
	curve_to(0.714662, 0.432362, 0.669886, 0.455775);
	curve_to(0.609306, 0.487513, 0.489903, 0.487513);
	curve_to(0.378402, 0.487513, 0.324846, 0.4641);
	curve_to(0.272169, 0.441207, 0.246708, 0.382414);
	line_to(0.0921861, 0.394901);

	curve_to(0.113257, 0.453694, 0.161545, 0.489594);
	curve_to(0.209833, 0.526015, 0.301141, 0.545265);
	curve_to(0.39245, 0.565036, 0.51273, 0.565036);
	curve_to(0.632133, 0.565036, 0.70676, 0.548387);
	curve_to(0.781387, 0.531738, 0.816506, 0.506243);
	curve_to(0.851624, 0.48127, 0.865672, 0.442768);
	curve_to(0.873573, 0.418835, 0.873573, 0.3564);
	line_to(0.873573, 0.23153);

	curve_to(0.873573, 0.100937, 0.883231, 0.066077);
	curve_to(0.893766, 0.0317378, 0.923617, 0);
	line_to(0.75856, 0);

	curve_to(0.733977, 0.0291363, 0.726953, 0.0681582);
	move_to(0.713784, 0.277315);

	curve_to(0.627744, 0.256504, 0.455663, 0.241935);
	curve_to(0.358209, 0.233611, 0.317823, 0.223205);
	curve_to(0.277436, 0.212799, 0.255487, 0.192508);
	curve_to(0.233538, 0.172737, 0.233538, 0.148283);
	curve_to(0.233538, 0.110822, 0.280948, 0.0858481);
	curve_to(0.329236, 0.0608741, 0.421422, 0.0608741);
	curve_to(0.51273, 0.0608741, 0.583845, 0.0842872);
	curve_to(0.654961, 0.108221, 0.688323, 0.149324);
	curve_to(0.713784, 0.181061, 0.713784, 0.242976);
	line_to(0.713784, 0.277315);


};


void show98()
{
	//width: 1139height: 1922
	move_to(0.264267, 0);

	line_to(0.117647, 0);

	line_to(0.117647, 0.762747);

	line_to(0.27568, 0.762747);

	line_to(0.27568, 0.490635);

	curve_to(0.375768, 0.565036, 0.531168, 0.565036);
	curve_to(0.617208, 0.565036, 0.693591, 0.544225);
	curve_to(0.770852, 0.523933, 0.820018, 0.486472);
	curve_to(0.870061, 0.449532, 0.898156, 0.396982);
	curve_to(0.926251, 0.344433, 0.926251, 0.284599);
	curve_to(0.926251, 0.14256, 0.807726, 0.0650364);
	curve_to(0.689201, -0.012487, 0.523266, -0.012487);
	curve_to(0.358209, -0.012487, 0.264267, 0.0691987);
	line_to(0.264267, 0);

	move_to(0.262511, 0.280437);

	curve_to(0.262511, 0.181061, 0.308165, 0.136837);
	curve_to(0.382792, 0.0645161, 0.510097, 0.0645161);
	curve_to(0.613696, 0.0645161, 0.689201, 0.117586);
	curve_to(0.764706, 0.171176, 0.764706, 0.276795);
	curve_to(0.764706, 0.385016, 0.691835, 0.436524);
	curve_to(0.619842, 0.488033, 0.51712, 0.488033);
	curve_to(0.413521, 0.488033, 0.338016, 0.434443);
	curve_to(0.262511, 0.381374, 0.262511, 0.280437);

};


void show99()
{
	//width: 1024height: 1922
	move_to(0.808594, 0.202393);

	line_to(0.981445, 0.190427);

	curve_to(0.953125, 0.0952133, 0.835938, 0.041103);
	curve_to(0.719727, -0.012487, 0.549805, -0.012487);
	curve_to(0.336914, -0.012487, 0.207031, 0.0613944);
	curve_to(0.078125, 0.135796, 0.078125, 0.274194);
	curve_to(0.078125, 0.363684, 0.133789, 0.430801);
	curve_to(0.189453, 0.497919, 0.302734, 0.531217);
	curve_to(0.416992, 0.565036, 0.550781, 0.565036);
	curve_to(0.719727, 0.565036, 0.827148, 0.519251);
	curve_to(0.93457, 0.473985, 0.964844, 0.390219);
	line_to(0.793945, 0.376171);

	curve_to(0.769531, 0.431842, 0.707031, 0.459938);
	curve_to(0.645508, 0.488033, 0.557617, 0.488033);
	curve_to(0.424805, 0.488033, 0.341797, 0.437045);
	curve_to(0.258789, 0.386576, 0.258789, 0.276795);
	curve_to(0.258789, 0.165453, 0.338867, 0.114984);
	curve_to(0.418945, 0.0645161, 0.547852, 0.0645161);
	curve_to(0.651367, 0.0645161, 0.720703, 0.0983351);
	curve_to(0.790039, 0.132154, 0.808594, 0.202393);

};


void show100()
{
	//width: 1139height: 1922
	move_to(0.723442, 0);

	line_to(0.723442, 0.069719);

	curve_to(0.634767, -0.012487, 0.462687, -0.012487);
	curve_to(0.351185, -0.012487, 0.257243, 0.0239334);
	curve_to(0.164179, 0.0603538, 0.112379, 0.12539);
	curve_to(0.0614574, 0.190947, 0.0614574, 0.275754);
	curve_to(0.0614574, 0.358481, 0.107989, 0.425598);
	curve_to(0.154522, 0.493236, 0.247586, 0.529136);
	curve_to(0.34065, 0.565036, 0.455663, 0.565036);
	curve_to(0.539947, 0.565036, 0.605795, 0.543704);
	curve_to(0.671642, 0.522893, 0.712906, 0.489074);
	line_to(0.712906, 0.762747);

	line_to(0.870061, 0.762747);

	line_to(0.870061, 0);

	line_to(0.723442, 0);

	move_to(0.223881, 0.275754);

	curve_to(0.223881, 0.169615, 0.299385, 0.117066);
	curve_to(0.37489, 0.0645161, 0.477612, 0.0645161);
	curve_to(0.581212, 0.0645161, 0.653205, 0.114464);
	curve_to(0.726076, 0.164932, 0.726076, 0.26795);
	curve_to(0.726076, 0.381374, 0.652327, 0.434443);
	curve_to(0.578578, 0.487513, 0.470588, 0.487513);
	curve_to(0.365233, 0.487513, 0.294118, 0.436524);
	curve_to(0.223881, 0.385536, 0.223881, 0.275754);

};


void show101()
{
	//width: 1139height: 1922
	move_to(0.756804, 0.17794);

	line_to(0.920105, 0.165973);

	curve_to(0.881475, 0.0811655, 0.776997, 0.0343392);
	curve_to(0.67252, -0.012487, 0.510097, -0.012487);
	curve_to(0.305531, -0.012487, 0.18525, 0.0619147);
	curve_to(0.0658472, 0.136837, 0.0658472, 0.271592);
	curve_to(0.0658472, 0.41103, 0.187006, 0.488033);
	curve_to(0.308165, 0.565036, 0.501317, 0.565036);
	curve_to(0.688323, 0.565036, 0.806848, 0.489594);
	curve_to(0.925373, 0.414152, 0.925373, 0.277315);
	curve_to(0.925373, 0.268991, 0.924495, 0.252341);
	line_to(0.229148, 0.252341);

	curve_to(0.237928, 0.16129, 0.316067, 0.112903);
	curve_to(0.394205, 0.0645161, 0.510975, 0.0645161);
	curve_to(0.597893, 0.0645161, 0.65935, 0.0915713);
	curve_to(0.720808, 0.118626, 0.756804, 0.17794);
	move_to(0.237928, 0.329344);

	line_to(0.75856, 0.329344);

	curve_to(0.748025, 0.399063, 0.698859, 0.433923);
	curve_to(0.623354, 0.488033, 0.503073, 0.488033);
	curve_to(0.394205, 0.488033, 0.319579, 0.444849);
	curve_to(0.24583, 0.401665, 0.237928, 0.329344);

};


void show102()
{
	//width: 569height: 1922
	move_to(0.31283, 0);

	line_to(0.31283, 0.479709);

	line_to(0.0333919, 0.479709);

	line_to(0.0333919, 0.552549);

	line_to(0.31283, 0.552549);

	line_to(0.31283, 0.611342);

	curve_to(0.31283, 0.667014, 0.346221, 0.694069);
	curve_to(0.391916, 0.730489, 0.506151, 0.752862);
	curve_to(0.622144, 0.775754, 0.829525, 0.775754);
	curve_to(0.963093, 0.775754, 1.12478, 0.766389);
	line_to(1.07733, 0.684703);

	curve_to(0.97891, 0.689906, 0.891037, 0.689906);
	curve_to(0.746924, 0.689906, 0.68717, 0.671696);
	curve_to(0.627416, 0.653486, 0.627416, 0.603538);
	line_to(0.627416, 0.552549);

	line_to(0.991213, 0.552549);

	line_to(0.991213, 0.479709);

	line_to(0.627416, 0.479709);

	line_to(0.627416, 0);

	line_to(0.31283, 0);


};


void show103()
{
	//width: 1139height: 1922
	move_to(0.0895522, -0.0457856);

	line_to(0.243196, -0.0593132);

	curve_to(0.252853, -0.101457, 0.296752, -0.120708);
	curve_to(0.355575, -0.146722, 0.457419, -0.146722);
	curve_to(0.567164, -0.146722, 0.626866, -0.120708);
	curve_to(0.686567, -0.094693, 0.707638, -0.0478668);
	curve_to(0.71993, -0.0192508, 0.719052, 0.0723205);
	curve_to(0.615452, 0, 0.460931, 0);
	curve_to(0.268657, 0, 0.163301, 0.082206);
	curve_to(0.0579456, 0.164412, 0.0579456, 0.279396);
	curve_to(0.0579456, 0.358481, 0.106234, 0.425078);
	curve_to(0.154522, 0.492196, 0.24583, 0.528616);
	curve_to(0.338016, 0.565036, 0.461809, 0.565036);
	curve_to(0.626866, 0.565036, 0.733977, 0.485952);
	line_to(0.733977, 0.552549);

	line_to(0.879719, 0.552549);

	line_to(0.879719, 0.074922);

	curve_to(0.879719, -0.0541103, 0.834943, -0.1077);
	curve_to(0.791045, -0.161811, 0.694469, -0.193028);
	curve_to(0.598771, -0.224246, 0.458297, -0.224246);
	curve_to(0.291484, -0.224246, 0.188762, -0.179501);
	curve_to(0.0860404, -0.135276, 0.0895522, -0.0457856);
	move_to(0.220369, 0.28616);

	curve_to(0.220369, 0.177419, 0.29324, 0.127471);
	curve_to(0.366111, 0.0775234, 0.475856, 0.0775234);
	curve_to(0.584723, 0.0775234, 0.658472, 0.126951);
	curve_to(0.732221, 0.176899, 0.732221, 0.283038);
	curve_to(0.732221, 0.384495, 0.655838, 0.436004);
	curve_to(0.580334, 0.487513, 0.473222, 0.487513);
	curve_to(0.367867, 0.487513, 0.294118, 0.436524);
	curve_to(0.220369, 0.386056, 0.220369, 0.28616);

};


void show104()
{
	//width: 1139height: 1922
	move_to(0.118525, 0);

	line_to(0.118525, 0.762747);

	line_to(0.276558, 0.762747);

	line_to(0.276558, 0.489074);

	curve_to(0.387182, 0.565036, 0.555751, 0.565036);
	curve_to(0.65935, 0.565036, 0.735733, 0.540583);
	curve_to(0.812116, 0.516649, 0.8446, 0.473985);
	curve_to(0.877963, 0.431322, 0.877963, 0.350156);
	line_to(0.877963, 0);

	line_to(0.71993, 0);

	line_to(0.71993, 0.350156);

	curve_to(0.71993, 0.420395, 0.66813, 0.452133);
	curve_to(0.617208, 0.484391, 0.523266, 0.484391);
	curve_to(0.453029, 0.484391, 0.390694, 0.462539);
	curve_to(0.329236, 0.441207, 0.302897, 0.404266);
	curve_to(0.276558, 0.367326, 0.276558, 0.302289);
	line_to(0.276558, 0);

	line_to(0.118525, 0);


};


void show105()
{
	//width: 455height: 1922
	move_to(0.298901, 0.655047);

	line_to(0.298901, 0.762747);

	line_to(0.694506, 0.762747);

	line_to(0.694506, 0.655047);

	line_to(0.298901, 0.655047);

	move_to(0.298901, 0);

	line_to(0.298901, 0.552549);

	line_to(0.694506, 0.552549);

	line_to(0.694506, 0);

	line_to(0.298901, 0);


};


void show106()
{
	//width: 455height: 1922
	move_to(0.294506, 0.654006);

	line_to(0.294506, 0.762747);

	line_to(0.69011, 0.762747);

	line_to(0.69011, 0.654006);

	line_to(0.294506, 0.654006);

	move_to(-0.206593, -0.21436);

	line_to(-0.131868, -0.134755);

	curve_to(-0.0131868, -0.14204, 0.0549451, -0.14204);
	curve_to(0.175824, -0.14204, 0.235165, -0.122789);
	curve_to(0.294506, -0.104058, 0.294506, -0.0280957);
	line_to(0.294506, 0.552549);

	line_to(0.69011, 0.552549);

	line_to(0.69011, -0.0301769);

	curve_to(0.69011, -0.132154, 0.578022, -0.172216);
	curve_to(0.435165, -0.224246, 0.103297, -0.224246);
	curve_to(-0.0571429, -0.224246, -0.206593, -0.21436);

};


void show107()
{
	//width: 1024height: 1922
	move_to(0.132813, 0);

	line_to(0.132813, 0.762747);

	line_to(0.308594, 0.762747);

	line_to(0.308594, 0.327784);

	line_to(0.724609, 0.552549);

	line_to(0.952148, 0.552549);

	line_to(0.555664, 0.347555);

	line_to(0.992188, 0);

	line_to(0.775391, 0);

	line_to(0.432617, 0.282518);

	line_to(0.308594, 0.219043);

	line_to(0.308594, 0);

	line_to(0.132813, 0);


};


void show108()
{
	//width: 455height: 1922
	move_to(0.287912, 0);

	line_to(0.287912, 0.762747);

	line_to(0.683517, 0.762747);

	line_to(0.683517, 0);

	line_to(0.287912, 0);


};


void show109()
{
	//width: 1706height: 1922
	move_to(0.0791325, 0);

	line_to(0.0791325, 0.552549);

	line_to(0.173505, 0.552549);

	line_to(0.173505, 0.475026);

	curve_to(0.202814, 0.515609, 0.251465, 0.540062);
	curve_to(0.300117, 0.565036, 0.362251, 0.565036);
	curve_to(0.431419, 0.565036, 0.475381, 0.539542);
	curve_to(0.51993, 0.514048, 0.538101, 0.468262);
	curve_to(0.611958, 0.565036, 0.730363, 0.565036);
	curve_to(0.822978, 0.565036, 0.872802, 0.519251);
	curve_to(0.922626, 0.473985, 0.922626, 0.379292);
	line_to(0.922626, 0);

	line_to(0.817702, 0);

	line_to(0.817702, 0.348075);

	curve_to(0.817702, 0.404266, 0.807151, 0.42872);
	curve_to(0.797186, 0.453694, 0.770223, 0.468783);
	curve_to(0.743259, 0.483871, 0.706917, 0.483871);
	curve_to(0.641266, 0.483871, 0.59789, 0.444849);
	curve_to(0.554513, 0.406348, 0.554513, 0.32102);
	line_to(0.554513, 0);

	line_to(0.449004, 0);

	line_to(0.449004, 0.359001);

	curve_to(0.449004, 0.421436, 0.423212, 0.452653);
	curve_to(0.397421, 0.483871, 0.338804, 0.483871);
	curve_to(0.294256, 0.483871, 0.256155, 0.463059);
	curve_to(0.21864, 0.442248, 0.201641, 0.402185);
	curve_to(0.184642, 0.362123, 0.184642, 0.286681);
	line_to(0.184642, 0);

	line_to(0.0791325, 0);


};


void show110()
{
	//width: 1139height: 1922
	move_to(0.118525, 0);

	line_to(0.118525, 0.552549);

	line_to(0.260755, 0.552549);

	line_to(0.260755, 0.473985);

	curve_to(0.363477, 0.565036, 0.557507, 0.565036);
	curve_to(0.641791, 0.565036, 0.712028, 0.546826);
	curve_to(0.783143, 0.529136, 0.818262, 0.5);
	curve_to(0.85338, 0.470864, 0.867428, 0.430801);
	curve_to(0.876207, 0.404787, 0.876207, 0.33975);
	line_to(0.876207, 0);

	line_to(0.718174, 0);

	line_to(0.718174, 0.336108);

	curve_to(0.718174, 0.39334, 0.699737, 0.421436);
	curve_to(0.681299, 0.450052, 0.633889, 0.466701);
	curve_to(0.587357, 0.483871, 0.524144, 0.483871);
	curve_to(0.423178, 0.483871, 0.349429, 0.44589);
	curve_to(0.276558, 0.407908, 0.276558, 0.301769);
	line_to(0.276558, 0);

	line_to(0.118525, 0);


};


void show111()
{
	//width: 1139height: 1922
	move_to(0.0597015, 0.276275);

	curve_to(0.0597015, 0.429761, 0.203687, 0.503642);
	curve_to(0.323968, 0.565036, 0.496927, 0.565036);
	curve_to(0.689201, 0.565036, 0.811238, 0.490114);
	curve_to(0.933275, 0.415713, 0.933275, 0.284079);
	curve_to(0.933275, 0.177419, 0.878841, 0.116025);
	curve_to(0.825285, 0.0551509, 0.721686, 0.0213319);
	curve_to(0.618964, -0.012487, 0.496927, -0.012487);
	curve_to(0.301141, -0.012487, 0.179982, 0.0619147);
	curve_to(0.0597015, 0.136316, 0.0597015, 0.276275);
	move_to(0.222125, 0.276275);

	curve_to(0.222125, 0.170135, 0.300263, 0.117066);
	curve_to(0.378402, 0.0645161, 0.496927, 0.0645161);
	curve_to(0.614574, 0.0645161, 0.692713, 0.117586);
	curve_to(0.770852, 0.170656, 0.770852, 0.279396);
	curve_to(0.770852, 0.381894, 0.691835, 0.434443);
	curve_to(0.613696, 0.487513, 0.496927, 0.487513);
	curve_to(0.378402, 0.487513, 0.300263, 0.434964);
	curve_to(0.222125, 0.382414, 0.222125, 0.276275);

};


void show112()
{
	//width: 1139height: 1922
	move_to(0.118525, -0.211759);

	line_to(0.118525, 0.552549);

	line_to(0.262511, 0.552549);

	line_to(0.262511, 0.480749);

	curve_to(0.313433, 0.522893, 0.377524, 0.543704);
	curve_to(0.441615, 0.565036, 0.532924, 0.565036);
	curve_to(0.652327, 0.565036, 0.743635, 0.528616);
	curve_to(0.834943, 0.492196, 0.881475, 0.425598);
	curve_to(0.928007, 0.359521, 0.928007, 0.280437);
	curve_to(0.928007, 0.19563, 0.876207, 0.127471);
	curve_to(0.825285, 0.0598335, 0.726953, 0.0234131);
	curve_to(0.6295, -0.012487, 0.52151, -0.012487);
	curve_to(0.442493, -0.012487, 0.37928, 0.00728408);
	curve_to(0.316945, 0.0270552, 0.276558, 0.057232);
	line_to(0.276558, -0.211759);

	line_to(0.118525, -0.211759);

	move_to(0.261633, 0.273153);

	curve_to(0.261633, 0.166493, 0.334504, 0.115505);
	curve_to(0.407375, 0.0645161, 0.510975, 0.0645161);
	curve_to(0.61633, 0.0645161, 0.690957, 0.117066);
	curve_to(0.766462, 0.170135, 0.766462, 0.280957);
	curve_to(0.766462, 0.386576, 0.692713, 0.439126);
	curve_to(0.619842, 0.491675, 0.517998, 0.491675);
	curve_to(0.417032, 0.491675, 0.338894, 0.435484);
	curve_to(0.261633, 0.379813, 0.261633, 0.273153);

};


void show113()
{
	//width: 1139height: 1922
	move_to(0.712906, -0.211759);

	line_to(0.712906, 0.0587929);

	curve_to(0.676032, 0.0280957, 0.609306, 0.00780437);
	curve_to(0.543459, -0.012487, 0.468832, -0.012487);
	curve_to(0.302897, -0.012487, 0.182616, 0.066077);
	curve_to(0.0632133, 0.144641, 0.0632133, 0.281478);
	curve_to(0.0632133, 0.364724, 0.111501, 0.430801);
	curve_to(0.160667, 0.496878, 0.252853, 0.530697);
	curve_to(0.345917, 0.565036, 0.456541, 0.565036);
	curve_to(0.6295, 0.565036, 0.728709, 0.478668);
	line_to(0.728709, 0.552549);

	line_to(0.870939, 0.552549);

	line_to(0.870939, -0.211759);

	line_to(0.712906, -0.211759);

	move_to(0.225637, 0.277836);

	curve_to(0.225637, 0.171176, 0.301141, 0.117586);
	curve_to(0.376646, 0.0645161, 0.482002, 0.0645161);
	curve_to(0.582968, 0.0645161, 0.655838, 0.114984);
	curve_to(0.728709, 0.165973, 0.728709, 0.269511);
	curve_to(0.728709, 0.379813, 0.651449, 0.435484);
	curve_to(0.575066, 0.491155, 0.471466, 0.491155);
	curve_to(0.368745, 0.491155, 0.296752, 0.439126);
	curve_to(0.225637, 0.387617, 0.225637, 0.277836);

};


void show114()
{
	//width: 682height: 1922
	move_to(0.195015, 0);

	line_to(0.195015, 0.552549);

	line_to(0.432551, 0.552549);

	line_to(0.432551, 0.468783);

	curve_to(0.52346, 0.527575, 0.599707, 0.546306);
	curve_to(0.677419, 0.565036, 0.769795, 0.565036);
	curve_to(0.903226, 0.565036, 1.04106, 0.53486);
	line_to(0.950147, 0.447971);

	curve_to(0.853372, 0.468262, 0.756598, 0.468262);
	curve_to(0.670088, 0.468262, 0.601173, 0.449532);
	curve_to(0.532258, 0.431322, 0.502933, 0.398543);
	curve_to(0.458944, 0.348595, 0.458944, 0.289282);
	line_to(0.458944, 0);

	line_to(0.195015, 0);


};


void show115()
{
	//width: 1024height: 1922
	move_to(0.0615234, 0.164932);

	line_to(0.235352, 0.179501);

	curve_to(0.25, 0.123829, 0.316406, 0.0941727);
	curve_to(0.383789, 0.0645161, 0.503906, 0.0645161);
	curve_to(0.625, 0.0645161, 0.683594, 0.0905307);
	curve_to(0.742188, 0.117066, 0.742188, 0.152445);
	curve_to(0.742188, 0.184183, 0.69043, 0.202393);
	curve_to(0.654297, 0.21488, 0.510742, 0.234131);
	curve_to(0.317383, 0.260146, 0.242188, 0.278876);
	curve_to(0.167969, 0.298127, 0.128906, 0.331426);
	curve_to(0.0908203, 0.365245, 0.0908203, 0.405827);
	curve_to(0.0908203, 0.442768, 0.12207, 0.473985);
	curve_to(0.154297, 0.505723, 0.208984, 0.526535);
	curve_to(0.25, 0.542664, 0.320313, 0.55359);
	curve_to(0.391602, 0.565036, 0.472656, 0.565036);
	curve_to(0.594727, 0.565036, 0.686523, 0.546306);
	curve_to(0.779297, 0.527575, 0.823242, 0.495317);
	curve_to(0.867188, 0.46358, 0.883789, 0.40999);
	line_to(0.711914, 0.397503);

	curve_to(0.700195, 0.440167, 0.643555, 0.4641);
	curve_to(0.587891, 0.488033, 0.485352, 0.488033);
	curve_to(0.364258, 0.488033, 0.3125, 0.466701);
	curve_to(0.260742, 0.445369, 0.260742, 0.416753);
	curve_to(0.260742, 0.398543, 0.282227, 0.383975);
	curve_to(0.303711, 0.368887, 0.349609, 0.359001);
	curve_to(0.375977, 0.353798, 0.504883, 0.335068);
	curve_to(0.691406, 0.308533, 0.764648, 0.291363);
	curve_to(0.838867, 0.274714, 0.880859, 0.242456);
	curve_to(0.922852, 0.210198, 0.922852, 0.162331);
	curve_to(0.922852, 0.115505, 0.871094, 0.0738814);
	curve_to(0.820313, 0.0327784, 0.723633, 0.00988554);
	curve_to(0.626953, -0.012487, 0.504883, -0.012487);
	curve_to(0.302734, -0.012487, 0.196289, 0.0322581);
	curve_to(0.0908203, 0.0770031, 0.0615234, 0.164932);

};


void show116()
{
	//width: 569height: 1922
	move_to(0.927944, 0.0837669);

	line_to(0.973638, 0.00104058);

	curve_to(0.84007, -0.00728408, 0.734622, -0.00728408);
	curve_to(0.56239, -0.00728408, 0.467487, 0.00884495);
	curve_to(0.372583, 0.024974, 0.333919, 0.0509886);
	curve_to(0.295255, 0.0775234, 0.295255, 0.161811);
	line_to(0.295255, 0.479709);

	line_to(0.0632689, 0.479709);

	line_to(0.0632689, 0.552549);

	line_to(0.295255, 0.552549);

	line_to(0.295255, 0.689386);

	line_to(0.609842, 0.745578);

	line_to(0.609842, 0.552549);

	line_to(0.927944, 0.552549);

	line_to(0.927944, 0.479709);

	line_to(0.609842, 0.479709);

	line_to(0.609842, 0.156608);

	curve_to(0.609842, 0.116545, 0.625659, 0.105099);
	curve_to(0.643234, 0.0936524, 0.680141, 0.0868887);
	curve_to(0.718805, 0.0801249, 0.789104, 0.0801249);
	curve_to(0.841828, 0.0801249, 0.927944, 0.0837669);

};


void show117()
{
	//width: 1139height: 1922
	move_to(0.729587, 0);

	line_to(0.729587, 0.0811655);

	curve_to(0.62072, -0.012487, 0.433714, -0.012487);
	curve_to(0.351185, -0.012487, 0.279192, 0.0062435);
	curve_to(0.208077, 0.024974, 0.172959, 0.0530697);
	curve_to(0.138718, 0.0816857, 0.124671, 0.122789);
	curve_to(0.115013, 0.150364, 0.115013, 0.210198);
	line_to(0.115013, 0.552549);

	line_to(0.273047, 0.552549);

	line_to(0.273047, 0.246098);

	curve_to(0.273047, 0.172737, 0.282704, 0.147242);
	curve_to(0.29763, 0.110302, 0.345917, 0.0889698);
	curve_to(0.394205, 0.0681582, 0.46532, 0.0681582);
	curve_to(0.536435, 0.0681582, 0.598771, 0.0894901);
	curve_to(0.661106, 0.111342, 0.686567, 0.148283);
	curve_to(0.712906, 0.185744, 0.712906, 0.256504);
	line_to(0.712906, 0.552549);

	line_to(0.870939, 0.552549);

	line_to(0.870939, 0);

	line_to(0.729587, 0);


};


void show118()
{
	//width: 1024height: 1922
	move_to(0.419922, 0);

	line_to(0.0253906, 0.552549);

	line_to(0.210938, 0.552549);

	line_to(0.433594, 0.221644);

	curve_to(0.469727, 0.168054, 0.5, 0.110302);
	curve_to(0.523438, 0.154006, 0.56543, 0.215401);
	line_to(0.795898, 0.552549);

	line_to(0.976563, 0.552549);

	line_to(0.583984, 0);

	line_to(0.419922, 0);


};


void show119()
{
	//width: 1479height: 1922
	move_to(0.2238, 0);

	line_to(0.0040568, 0.552549);

	line_to(0.129817, 0.552549);

	line_to(0.244084, 0.233611);

	line_to(0.28668, 0.114984);

	curve_to(0.289385, 0.123829, 0.323867, 0.228928);
	line_to(0.438134, 0.552549);

	line_to(0.563218, 0.552549);

	line_to(0.670723, 0.23205);

	line_to(0.706558, 0.126431);

	line_to(0.747803, 0.233091);

	line_to(0.870859, 0.552549);

	line_to(0.989182, 0.552549);

	line_to(0.764706, 0);

	line_to(0.638269, 0);

	line_to(0.524003, 0.330905);

	line_to(0.496281, 0.425078);

	line_to(0.350913, 0);

	line_to(0.2238, 0);


};


void show120()
{
	//width: 1024height: 1922
	move_to(0.0146484, 0);

	line_to(0.393555, 0.287201);

	line_to(0.0429688, 0.552549);

	line_to(0.262695, 0.552549);

	line_to(0.421875, 0.422997);

	curve_to(0.466797, 0.386056, 0.494141, 0.361082);
	curve_to(0.537109, 0.395421, 0.573242, 0.421956);
	line_to(0.748047, 0.552549);

	line_to(0.958008, 0.552549);

	line_to(0.599609, 0.292404);

	line_to(0.985352, 0);

	line_to(0.769531, 0);

	line_to(0.556641, 0.171696);

	line_to(0.5, 0.218002);

	line_to(0.227539, 0);

	line_to(0.0146484, 0);


};


void show121()
{
	//width: 1024height: 1922
	move_to(0.124023, -0.212799);

	line_to(0.104492, -0.12487);

	curve_to(0.162109, -0.133195, 0.205078, -0.133195);
	curve_to(0.263672, -0.133195, 0.298828, -0.122789);
	curve_to(0.333984, -0.112383, 0.356445, -0.0936524);
	curve_to(0.373047, -0.0796046, 0.410156, -0.0239334);
	curve_to(0.415039, -0.016129, 0.425781, -0.00104058);
	line_to(0.0322266, 0.552549);

	line_to(0.22168, 0.552549);

	line_to(0.4375, 0.23257);

	curve_to(0.479492, 0.171696, 0.512695, 0.104579);
	curve_to(0.542969, 0.169095, 0.584961, 0.230489);
	line_to(0.806641, 0.552549);

	line_to(0.982422, 0.552549);

	line_to(0.587891, -0.00936524);

	curve_to(0.524414, -0.100416, 0.489258, -0.134755);
	curve_to(0.442383, -0.181061, 0.381836, -0.202393);
	curve_to(0.321289, -0.224246, 0.237305, -0.224246);
	curve_to(0.186523, -0.224246, 0.124023, -0.212799);

};


void show122()
{
	//width: 1024height: 1922
	move_to(0.0390625, 0);

	line_to(0.0390625, 0.0759625);

	line_to(0.699219, 0.479709);

	curve_to(0.586914, 0.476587, 0.500977, 0.476587);
	line_to(0.078125, 0.476587);

	line_to(0.078125, 0.552549);

	line_to(0.925781, 0.552549);

	line_to(0.925781, 0.490635);

	line_to(0.364258, 0.139958);

	line_to(0.255859, 0.0759625);

	curve_to(0.374023, 0.0806452, 0.477539, 0.0806452);
	line_to(0.957031, 0.0806452);

	line_to(0.957031, 0);

	line_to(0.0390625, 0);


};


void show123()
{
	//width: 684height: 1922
	move_to(0.0833333, 0.318418);

	curve_to(0.195906, 0.319459, 0.266082, 0.33975);
	curve_to(0.337719, 0.360562, 0.361111, 0.396462);
	curve_to(0.384503, 0.432362, 0.385965, 0.519251);
	curve_to(0.387427, 0.606139, 0.394737, 0.633715);
	curve_to(0.407895, 0.677419, 0.442982, 0.703954);
	curve_to(0.479532, 0.730489, 0.532164, 0.746098);
	curve_to(0.584795, 0.762227, 0.666667, 0.770552);
	curve_to(0.722222, 0.775754, 0.847953, 0.775754);
	line_to(0.929825, 0.775754);

	line_to(0.929825, 0.694069);

	line_to(0.884503, 0.694069);

	curve_to(0.732456, 0.694069, 0.682749, 0.674298);
	curve_to(0.633041, 0.655047, 0.633041, 0.587409);
	curve_to(0.633041, 0.451093, 0.616959, 0.415193);
	curve_to(0.590643, 0.359521, 0.526316, 0.329344);
	curve_to(0.46345, 0.299168, 0.327485, 0.275754);
	curve_to(0.488304, 0.251821, 0.559942, 0.202393);
	curve_to(0.633041, 0.153486, 0.633041, 0.0416233);
	curve_to(0.633041, -0.0598335, 0.638889, -0.0790843);
	curve_to(0.650585, -0.114464, 0.697368, -0.128512);
	curve_to(0.745614, -0.14256, 0.884503, -0.14256);
	line_to(0.929825, -0.14256);

	line_to(0.929825, -0.224246);

	line_to(0.847953, -0.224246);

	curve_to(0.704678, -0.224246, 0.640351, -0.215921);
	curve_to(0.546784, -0.203954, 0.48538, -0.176899);
	curve_to(0.423977, -0.150364, 0.404971, -0.109261);
	curve_to(0.387427, -0.0681582, 0.385965, 0.0254943);
	curve_to(0.384503, 0.119147, 0.361111, 0.155047);
	curve_to(0.337719, 0.190947, 0.266082, 0.211759);
	curve_to(0.195906, 0.23257, 0.0833333, 0.233611);
	line_to(0.0833333, 0.318418);


};


void show124()
{
	//width: 532height: 1922
	move_to(0.353383, -0.224246);

	line_to(0.353383, 0.775754);

	line_to(0.648496, 0.775754);

	line_to(0.648496, -0.224246);

	line_to(0.353383, -0.224246);


};


void show125()
{
	//width: 684height: 1922
	move_to(0.915205, 0.318418);

	line_to(0.915205, 0.233611);

	curve_to(0.802632, 0.23257, 0.730994, 0.211759);
	curve_to(0.660819, 0.190947, 0.637427, 0.155047);
	curve_to(0.614035, 0.119667, 0.612573, 0.0327784);
	curve_to(0.611111, -0.0541103, 0.603801, -0.0816857);
	curve_to(0.590643, -0.125911, 0.554094, -0.151925);
	curve_to(0.519006, -0.17846, 0.466374, -0.194069);
	curve_to(0.413743, -0.210198, 0.331871, -0.218522);
	curve_to(0.276316, -0.224246, 0.150585, -0.224246);
	line_to(0.0687134, -0.224246);

	line_to(0.0687134, -0.14256);

	line_to(0.114035, -0.14256);

	curve_to(0.266082, -0.14256, 0.315789, -0.122789);
	curve_to(0.365497, -0.103538, 0.365497, -0.0353798);
	curve_to(0.365497, 0.094693, 0.378655, 0.129553);
	curve_to(0.402047, 0.187305, 0.472222, 0.221644);
	curve_to(0.542398, 0.256504, 0.671053, 0.275754);
	curve_to(0.502924, 0.30437, 0.434211, 0.351197);
	curve_to(0.365497, 0.398543, 0.365497, 0.510406);
	curve_to(0.365497, 0.611863, 0.358187, 0.631634);
	curve_to(0.347953, 0.666493, 0.299708, 0.680021);
	curve_to(0.252924, 0.694069, 0.114035, 0.694069);
	line_to(0.0687134, 0.694069);

	line_to(0.0687134, 0.775754);

	line_to(0.150585, 0.775754);

	curve_to(0.29386, 0.775754, 0.358187, 0.76743);
	curve_to(0.451754, 0.755983, 0.513158, 0.728928);
	curve_to(0.574561, 0.701873, 0.592105, 0.66077);
	curve_to(0.611111, 0.619667, 0.612573, 0.526015);
	curve_to(0.614035, 0.432362, 0.637427, 0.396462);
	curve_to(0.660819, 0.361082, 0.730994, 0.340271);
	curve_to(0.802632, 0.319459, 0.915205, 0.318418);

};


void show126()
{
	//width: 1196height: 1922
	move_to(0.0727425, 0.289802);

	line_to(0.0727425, 0.396462);

	curve_to(0.161371, 0.458897, 0.305184, 0.458897);
	curve_to(0.355351, 0.458897, 0.410535, 0.449532);
	curve_to(0.465719, 0.440687, 0.567726, 0.413632);
	curve_to(0.625418, 0.398543, 0.653846, 0.393861);
	curve_to(0.68311, 0.389178, 0.712375, 0.389178);
	curve_to(0.766722, 0.389178, 0.824415, 0.409469);
	curve_to(0.882943, 0.429761, 0.928094, 0.460458);
	line_to(0.928094, 0.350156);

	curve_to(0.874582, 0.318939, 0.819398, 0.304891);
	curve_to(0.76505, 0.290843, 0.696488, 0.290843);
	curve_to(0.646321, 0.290843, 0.600334, 0.298127);
	curve_to(0.555184, 0.305411, 0.455686, 0.332986);
	curve_to(0.357023, 0.360562, 0.29097, 0.360562);
	curve_to(0.237458, 0.360562, 0.189799, 0.345994);
	curve_to(0.142977, 0.331946, 0.0727425, 0.289802);

};


void show127()
{
	//width: 1536height: 1922
	move_to(0.166667, 0);

	line_to(0.166667, 0.665973);

	line_to(0.833333, 0.665973);

	line_to(0.833333, 0);

	line_to(0.166667, 0);

	move_to(0.1875, 0.0166493);

	line_to(0.8125, 0.0166493);

	line_to(0.8125, 0.649324);

	line_to(0.1875, 0.649324);

	line_to(0.1875, 0.0166493);


};


vec4 getChar2(int c, float xpos, float ypos, float width, float height)
{
	vec2 uv = fragCoord.xy / iResolution.xy;
	float restrictRowBefore = step(xpos, uv.x);

	float restrictColBefore = step(uv.y, ypos);

	float restrictRowAfter = step(uv.x, xpos + width);

	float restrictColAfter = step(ypos - height, uv.y);

	vec2 grid = vec2(1 / width, 1 / height);

	//vec2 st = uv*grid;
	uv.x = uv.x - xpos;
	uv.y = uv.y + ypos;
	vec2 st = uv*grid;
	uv = fract(st);

	float x = (1.0 / cPerRowf) *  fract(float(c) / cPerRowf)  * cPerRowf + uv.x / cPerRowf;
	float y = (1.0 / cPerRowf) *  (cPerRowf - float(c / cPerRow) - 1.0) + uv.y / cPerRowf;

	//vec2 gx = dFdx(st / 16.0);
	//    vec2 gy = dFdy(st / 16.0);

	return gl_FragColor += texture2D(Texture_2, vec2(x, y)).xyzw  * restrictRowAfter * restrictColAfter * restrictRowBefore *restrictColBefore;
}


void main()
{
    //vec2 uv = fragCoord.xy / iResolution.xy;
    //if (uv.y > 0.5)
    //    gl_FragColor = vec4(1.0, 0., 0., 1.);
    //else
    //    gl_FragColor = vec4(1.0, 1., 0., 1.);
    //return;
    
    set_source_rgb(vec3(0.1333f, 0.1333f, 0.1333f));
    clear();
    init(fragCoord);

	set_source_rgba(1., 0.0, 0.0, 1.);
	set_line_width(0.01);
	set_blur(0.001);
	getCharInGrid(50, 3, 1);
	//getChar(50, .5, .5, .05, .1 / 2.);
	//move_to(-1.f, -1.f);
	//line_to(.5f, .5f);
	//line_to(.0f, .5f);
	//curve_to(0.f, 0.5f, -0.5f, 0.f);

	//stroke();
	stroke_preserve();
	//fill_preserve();
	
	

   // _vm();
    gl_FragColor += vec4(_color, 1.0);
    
    //getCharInGrid( 53, 1, 1);
    //   getChar(49, .5, .5, .05, .1/2.);
    
    
    //set_source_rgb(vec3(0.1333f, 0.1333f, 0.1333f));
    //clear();
    //set_source_rgba(vec4(1.f, 0.0f, 0.f, 1.f));
    //set_line_width(0.003);
    //set_blur(0.002);// why 0.0002  the line will dispare?
    
    
    //float gridHeight = 2.0 / rows;
    //float gridWidth = 2.0 / cols;
    //for (int i = 0; i < rows; i++)         // maybe low fps!
    //{
    //        move_to(-1.0, i*gridHeight - 1.0);
    //        line_to(1.0, i*gridHeight - 1.0);
    //}
    //stroke();
    
    //gl_FragColor.rgba = vec4(_color, 1.);
    
    //_vm();
    
    //fragColor = vec4(_color, 1.0);
    
    //    gl_FragColor = texture2D(Texture_1, textureCoordOut);
    
    //    for(int i = 0; i < rows; i ++)    // low fps!!!
    //    {
    //        for(int j = 0; j < 1; j++)
    //        {
    //            gl_FragColor  += getChar(uv, 49 + i * 20 + j, i, j);
    //        }
    //    }
    
    //vec2 uv = fragCoord.xy / iResolution.xy;
    //// show 1 2 3 4 5 in specific grid
    //gl_FragColor += getChar(uv, 49, 1, 1);
    //gl_FragColor += getChar(uv, 49, .5, .5, .05, .1/2.);
    
    //gl_FragColor += getChar(uv, 50, 2, 2);
    //gl_FragColor += getChar(uv, 51, 3, 3);
    //gl_FragColor += getChar(uv, 52, 4, 4);
    //gl_FragColor += getChar(uv, 53, 5, 5);
    
    
    //// show HELLO WORLD! in specific grid
    //gl_FragColor += getChar(uv, 72, 6, 6);
    //gl_FragColor += getChar(uv, 69, 6, 7);
    //gl_FragColor += getChar(uv, 76, 6, 8);
    //gl_FragColor += getChar(uv, 76, 6, 9);
    //gl_FragColor += getChar(uv, 79, 6, 10);
    //gl_FragColor += getChar(uv, 32, 6, 11);
    //gl_FragColor += getChar(uv, 87, 6, 12);
    //gl_FragColor += getChar(uv, 79, 6, 13);
    //gl_FragColor += getChar(uv, 82, 6, 14);
    //gl_FragColor += getChar(uv, 76, 6, 15);
    //gl_FragColor += getChar(uv, 68, 6, 16);
    //gl_FragColor += getChar(uv, 33, 6, 17);
    
    
    
    //// show HELLO WORLD! in specific grid
    //gl_FragColor += getChar(uv, 72, 14, 3);
    //gl_FragColor += getChar(uv, 69, 14, 4);
    //gl_FragColor += getChar(uv, 76, 14, 5);
    //gl_FragColor += getChar(uv, 76, 14, 6);
    //gl_FragColor += getChar(uv, 79, 14, 7);
    //gl_FragColor += getChar(uv, 32, 14, 8);
    //gl_FragColor += getChar(uv, 87, 14, 9);
    //gl_FragColor += getChar(uv, 79, 14, 10);
    //gl_FragColor += getChar(uv, 82, 14, 11);
    //gl_FragColor += getChar(uv, 76, 14, 12);
    //gl_FragColor += getChar(uv, 68, 14, 13);
    //gl_FragColor += getChar(uv, 33, 14, 14);
    
    //// show 1 2 3 4 5 in specific grid
    //gl_FragColor += getChar(uv, 49, 15, 15);
    //gl_FragColor += getChar(uv, 50, 16, 16);
    //gl_FragColor += getChar(uv, 51, 17, 17);
    //gl_FragColor += getChar(uv, 52, 18, 18);
    //gl_FragColor += getChar(uv, 53, 19, 19);
    
    return;
    
    //gl_FragColor = mix(bgColor, fgColor, showc(vec2(0.0, 0.0)));
    //gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    //for (int i = 0; i < lineWidth; i++)
    //{
    //    for (int j = 0; j < lineHeight; j++)
    //    {
    //        gl_FragColor = mix(gl_FragColor, fgColor, showc(vec2(i, j)));
    //    }
    //
    //}
    
    
    
    //vec2 uv = fragCoord.xy / iResolution.xx;
    //float r = getcolor(uv);
    //float v = _vm();
    //fragColor = vec4(1., 1.0, 0., 1.0);
    
    
    
    //float mx = max(iResolution.x, iResolution.y);
    
    //vec2 uv = fragCoord.xy / mx;
    //vec2 center = iResolution.xy / mx * 0.5;
    
    //float cDist = distance(center, uv) * 5.0;
    //float mDist = distance(iMouse.xy / mx, uv) * 10.0;
    //float color = cDist * cDist * mDist;
    
    //fragColor = vec4(color > 1.0, 1, 1, 1);
}


//fragColor = vec4(0.5, 0., 0.5, 1.0);     // 重置shader









float _vm()
{
    const int stackSize = 1024;
    float stack[stackSize];
    int codeSize = int(arrFloat[0]);
    
    int pc = 0;
    int esp = stackSize; // no value
    int op = 0;
    float eax = 0;
    float ebx = 0;
    for (pc = 1; pc < codeSize; )
    {
        op = int(arrFloat[pc++]);
        
        if (op == mov_eax_xxx)
        {
            eax = arrFloat[pc++];
        }
        else if (op == jmp)
        {
            pc = int(arrFloat[pc]);
        }
        else if (op == jz)
        {
            int targetPc = int(arrFloat[pc++]);
            if (eax == 0)
            {
                //printf("\n eax == 0 jump to %d\n", targetPc);
                pc = targetPc;
            }
            else
            {
                //printf("\n eax != 0 do not jump\n");
                //do nothing
            }
        }
        else if (op == lea_eax_mesp_xx)
        {
            eax = esp + arrFloat[pc++];
        }
        else if (op == push_eax)
        {
            stack[--esp] = eax;
        }
        else if (op == mov_eax_addr)
        {
            eax = stack[int(eax)];
        }
        else if (op == mov_eax_byte_addr)
        {
            
        }
        else if (op == ret)
        {
            pc = int(stack[esp]);
            esp++;
        }
        else if (op == pop_ebx_mov_mebx_eax)
        {
            ebx = stack[esp++];
            stack[int(ebx)] = eax;
        }
        else if (op == add_esp_xx)
        {
            esp += int(arrFloat[pc++]);
        }
        else if (op == pop_ebx_imul_eax_ebx)
        {
            ebx = stack[esp++];
            eax = eax * ebx;
        }
        else if (op == pop_ebx_fdiv_ebx_eax)
        {
            ebx = stack[esp++];
            eax = ebx / eax;
        }
        else if (op == pop_ebx_mod_ebx_eax)
        {
            ebx = stack[esp++];
            eax = int(ebx) / int(eax);            // to do , glsl has no mod!!!!!
        }
        else if (op == pop_ebx_logic_and_ebx_eax)
        {
            ebx = stack[esp++];
            if (eax != 0 && ebx != 0)
                eax = 1;
            else
                eax = 0;
        }
        else if (op == pop_ebx_logic_or_ebx_eax)
        {
            ebx = stack[esp++];
            if (eax == 0 && ebx == 0)
                eax = 0;
            else
                eax = 1;
        }
        //else if (op == pop_ebx_bit_and_ebx_eax)
        //{
        //    ebx = stack[esp++];
        //    eax = int(eax) * int(ebx);                 // to do , glsl has no &!!!!!
        //}
        //else if (op == pop_ebx_bit_or_ebx_eax)
        //{
        //    ebx = stack[esp++];
        //    eax = int(eax) + int(ebx);                    // to do , glsl has no |!!!!!
        //}
        //else if (op == pop_ebx_bit_xor_ebx_eax)
        //{
        //    ebx = stack[esp++];
        //    eax = int(eax) * int(ebx);                   // to do , glsl has no ^!!!!!
        //}
        //else if (op == pop_ebx_shl_ebx_eax)
        //{
        //    ebx = stack[esp++];
        //    eax = int(ebx)  * int(eax);                        // to do , glsl has no <<  !!!!!
        //}
        //else if (op == pop_ebx_shr_ebx_eax)
        //{
        //    ebx = stack[esp++];
        //    eax = int(ebx) * int(eax);                 // to do , glsl has no >>  !!!!!
        //}
        else if (op == pop_ebx_add_eax_ebx)
        {
            ebx = stack[esp++];
            eax = eax + ebx;
        }
        else if (op == pop_ebx_sub_ebx_eax)
        {
            ebx = stack[esp++];
            eax = ebx - eax;
        }
        else if (op == pop_ebx_equal_eax_ebx)
        {
            ebx = stack[esp++];
            if (eax == ebx)
            {
                eax = 1;
            }
            else
                eax = 0;
        }
        else if (op == pop_ebx_not_equal_eax_ebx)
        {
            ebx = stack[esp++];
            if (eax != ebx)
            {
                eax = 1;
            }
            else
                eax = 0;
        }
        else if (op == pop_ebx_ebx_less_than_ebx)
        {
            ebx = stack[esp++];
            if (ebx < eax)
            {
                eax = 1;
            }
            else
                eax = 0;
        }
        else if (op == pop_ebx_ebx_greate_than_ebx)
        {
            ebx = stack[esp++];
            if (ebx > eax)
            {
                eax = 1;
            }
            else
                eax = 0;
        }
        else if (op == pop_ebx_ebx_greate_EQUAL_than_ebx)
        {
            ebx = stack[esp++];
            if (ebx >= eax)
            {
                eax = 1;
            }
            else
                eax = 0;
        }
        else if (op == pop_ebx_ebx_less_EQUAL_than_ebx)
        {
            ebx = stack[esp++];
            if (ebx <= eax)
            {
                eax = 1;
            }
            else
                eax = 0;
        }
        
        else if (op == call_eax)
        {
            stack[--esp] = pc;
            pc = int(eax);
            
        }
        else if (op == halt)
        {
            //printf("virtual machine exit with return value is %f\n", eax);
            break;
        }
        else if (op == call_addr)
        {
            
            stack[--esp] = pc + 1; // + 1 because call has 1 argument
            
            pc = int(arrFloat[pc]);
        }
        else if (op == negative_eax)
        {
            eax = -eax;
        }
        else if (op == mov_eax_ValueAtEax_negative_eax)
        {
            eax = stack[int(eax)];
            eax = -eax;
        }
//        else if (op == bit_negative_eax)
//        {
//            eax = ~int(eax);
//        }
        else if (op == not_eax)
        {
            if (eax != 0)
                eax = 0;
            else
                eax = 1;
            
        }
//        else if (op == mov_eax_ValueAtEax_bit_negative_eax)
//        {
//            eax = stack[int(eax)];
//            eax = ~int(eax);
//        }
        else if (op == mov_eax_ValueAtEax_not_eax)
        {
            eax = stack[int(eax)];
            if (eax != 0)
                eax = 0;
            else
                eax = 1;
        }
        else if (op == call_api)
        {
            int index = int(arrFloat[pc++]);
            if (index == 0)
            {
                //int arg1 = stack[esp];
                //            msg(juce::String(stack[esp + 3]) + " " + juce::String(stack[esp + 2]) + " " + juce::String(stack[esp + 1]) + " " + juce::String(stack[esp + 0]));
            }
            else if (index == 1)
            {
                int arg1 = int(stack[esp]);
            }
            else if (index == 2)
            {
                set_source_rgba(stack[esp + 3], stack[esp + 2], stack[esp + 1], stack[esp + 0]);
            }
            else if (index == 3)
            {
                set_line_width(stack[esp + 0]);
            }
            else if (index == 4)
            {
                set_blur(stack[esp + 0]);
            }
            else if (index == 5)
            {
                move_to(stack[esp + 1], stack[esp + 0]);
            }
            else if (index == 6)
            {
                line_to(stack[esp + 1], stack[esp + 0]);
            }
            else if (index == 7)
            {
                stroke();
            }
            else if (index == 8)
            {
                getCharInGrid(int(stack[esp + 2]),  int(stack[esp + 1]), int(stack[esp + 0]));
            }
            else if (index == 9)
            {
                getChar(int(stack[esp + 4]), stack[esp + 3], stack[esp + 2], stack[esp + 1], stack[esp + 0]);
            }
            
            //var = sym_declare(context, "set_source_rgba", 'A');
            //var->apiIndex = 2;
            
            //var = sym_declare(context, "set_line_width", 'A');
            //var->apiIndex = 3;
            
            //var = sym_declare(context, "set_blur", 'A');
            //var->apiIndex = 4;
            
            //var = sym_declare(context, "move_to", 'A');
            //var->apiIndex = 5;
            
            //var = sym_declare(context, "line_to", 'A');
            //var->apiIndex = 6;
            
            //var = sym_declare(context, "stroke", 'A');
            //var->apiIndex = 7;
        }
    }
    return eax;
}
