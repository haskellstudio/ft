#version 120 

uniform vec4 lightPosition;
varying vec2 textureCoordOut;
varying vec2 _uv;
uniform sampler2D Texture_1;

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

#define fragColor gl_FragColor 
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





const int	NOOP = 0;
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
//	_stack.position = vec4(.3f, .3f, 0.f, 2.f);
//	//p = vec2(.5f, .5f);
//	//_stack.last_pt = vec2(-2.f, 0.f);
//
//	//float xxx = atan2(-1.f, 0.f);
//
//	vec4 pa = _stack.position - _stack.last_pt.xyxy;
//	vec2 ba = p - _stack.last_pt;
//	vec2 pb = dot2(pa, ba);
//	float bb = dot(ba, ba);
//	vec2 h = clamp(dot2(pa, ba) / dot(ba, ba), 0.0f, 1.0f);    // (a*b / b*b)   分別表示兩個標量 ， 第一個算的是當前計算的點，  第二個算的是鼠標所在的點（？ todo 還沒有研究）
//															   //這裏需要 clamp 不然下面的  pa - projectionVector 計算不對， 會造成直綫過長 
//
//															   //vec2 h = dot2(pa, ba) / dot(ba, ba);  
//	vec2 ss = pa.xz*ba.y - pa.yw*ba.x;    // a.x * b.y  - a.y * b.x    算兩個夾角逆時針是否超過180 度 ？
//	vec2 s = sign((vec2)ss);
//
//	vec4 projectionVector = ba.xyxy*h.xxyy;
//	vec4 subPlayervector_Projectionvector = pa - projectionVector;
//	vec2 d = length2(subPlayervector_Projectionvector);
//	//vec2 d = length2(pa - h.xxyy);
//	add_field(d);
//	add_clip(d * s);
//	_stack.last_pt = p;
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
	// w -= blur.x; 执行完后 ， 如果w为负数 ， 说明 这个点是实心的， 没有被模糊。 
	// 如果 w 在 0-blur.x之间， 说明这个点是在模糊范围内的 。
	// 如果 w 大于  blur.x，   说明这个点在模糊范围之外， 也就是说，这个点不属于当前要画的line， 也就是说，采用背景色。
	w -= blur.x;


	float wa = clamp(-w*min(iResolution.x, iResolution.y)/**uniform_scale_for_aa()*/, 0.0, 1.0);       // wa 表示当前离 直线边缘的距离  够不够一个像素， 如果不够， 那么这个点的颜色， 就是 线的颜色 * wa 比如0.5， 相当于抗锯齿。
	float wb = clamp(-w / blur.x + blur.y, 0.0, 1.0);               // 当前离 直线边缘的距离  除以   blur的宽度  ， 如果在blur之间， 就对其 。。。。 同上
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

void clear() {
	_color = mix(_color, _stack.source.rgb, _stack.source.a);
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

vec4 getChar(vec2 uv,  int c , int whichRow, int whichCol)
{
    
    float restrictRowBefore = step((1.0 / float(cols)) * float (whichCol ) ,uv.x );
    
    float restrictColBefore = step(  uv.y , (1.0 / float(rows)) *float (rows - whichRow));
    
    float restrictRowAfter = step(uv.x, (1.0 / float(cols)) * float (whichCol  + 1));
    
    float restrictColAfter = step( (1.0 / float(rows)) *float (rows - whichRow - 1), uv.y);
    
    vec2 grid = vec2(rows, cols);
    
    vec2 st = uv*grid;
    
    uv = fract (st);
    
    float x = (1.0 / cPerRowf) *  fract ( float(c) / cPerRowf)  * cPerRowf + uv.x /cPerRowf ;
    float y = (1.0/cPerRowf) *  (cPerRowf- float(c/cPerRow) - 1.0 ) + uv.y/cPerRowf ;
    
    vec2 gx = dFdx( st/16.0 );
    vec2 gy = dFdy( st/16.0 );
    
    return texture2D( Texture_1, vec2(x, y)).xyzw  * restrictRowAfter * restrictColAfter * restrictRowBefore *restrictColBefore ;
}


void main() 
{
	init(fragCoord);

	set_source_rgb(vec3(0.1333f, 0.1333f, 0.1333f));
	clear();
    set_source_rgba(vec4(1.f, 0.0f, 0.f, 1.f) );
    set_line_width(0.0005);
    set_blur(0.002);

 //   move_to(.0f, 0.9f);
    ////curve_to(0.f, 0.5f, -0.5f, 0.f);
  //  line_to(1.f, .0f);
    //close_path();
  
        //0-2  xia dao shang
    // 2/rows  every grid width
    float gridHeight = 2.0/ rows;
    float gridWidth  = 2.0 / cols;
    for(int i = 0; i < rows; i ++)
    {
       // for(int j = 0; j < cols; j++)
        {
            move_to(-1.0, i*gridHeight-1.0);
            line_to(1.0,  i*gridHeight-1.0);
        }
    }
  
    
    stroke();
    
    gl_FragColor.rgba = vec4(_color, 1.);
	//_vm();

	//fragColor = vec4(_color, 1.0);

//	gl_FragColor = texture2D(Texture_1, textureCoordOut);
	
	
	// show 1 2 3 4 5 in specific grid
    gl_FragColor  += getChar(uv, 49, 1, 1);
    gl_FragColor  += getChar(uv, 50, 2, 2);
    gl_FragColor  += getChar(uv, 51, 3, 3);
    gl_FragColor  += getChar(uv, 52, 4, 4);
    gl_FragColor  += getChar(uv, 53, 5, 5);
    
    
    // show HELLO WORLD! in specific grid
    gl_FragColor  += getChar(uv, 72, 6, 6);
    gl_FragColor  += getChar(uv, 69, 6, 7);
    gl_FragColor  += getChar(uv, 76, 6, 8);
    gl_FragColor  += getChar(uv, 76, 6, 9);
    gl_FragColor  += getChar(uv, 79, 6, 10);
    gl_FragColor  += getChar(uv, 32, 6,11);
    gl_FragColor  += getChar(uv, 87, 6, 12);
    gl_FragColor  += getChar(uv, 79, 6, 13);
    gl_FragColor  += getChar(uv, 82, 6, 14);
    gl_FragColor  += getChar(uv, 76, 6, 15);
    gl_FragColor  += getChar(uv, 68, 6, 16);
    gl_FragColor  += getChar(uv, 33, 6, 17);
    
    
    
    // show HELLO WORLD! in specific grid
    gl_FragColor  += getChar(uv, 72, 14, 3);
    gl_FragColor  += getChar(uv, 69, 14, 4);
    gl_FragColor  += getChar(uv, 76, 14, 5);
    gl_FragColor  += getChar(uv, 76, 14, 6);
    gl_FragColor  += getChar(uv, 79, 14, 7);
    gl_FragColor  += getChar(uv, 32, 14, 8);
    gl_FragColor  += getChar(uv, 87, 14, 9);
    gl_FragColor  += getChar(uv, 79, 14, 10);
    gl_FragColor  += getChar(uv, 82, 14, 11);
    gl_FragColor  += getChar(uv, 76, 14, 12);
    gl_FragColor  += getChar(uv, 68, 14, 13);
    gl_FragColor  += getChar(uv, 33, 14, 14);
    
    // show 1 2 3 4 5 in specific grid
    gl_FragColor  += getChar(uv, 49, 15, 15);
    gl_FragColor  += getChar(uv, 50, 16, 16);
    gl_FragColor  += getChar(uv, 51, 17, 17);
    gl_FragColor  += getChar(uv, 52, 18, 18);
    gl_FragColor  += getChar(uv, 53, 19, 19);
    
	return;
	
	//gl_FragColor = mix(bgColor, fgColor, showc(vec2(0.0, 0.0)));
	//gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	
	//for (int i = 0; i < lineWidth; i++)
	//{
	//	for (int j = 0; j < lineHeight; j++)
	//	{
	//		gl_FragColor = mix(gl_FragColor, fgColor, showc(vec2(i, j)));
	//	}
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
		//	ebx = stack[esp++];
		//	eax = int(eax) * int(ebx);                 // to do , glsl has no &!!!!!
		//}
		//else if (op == pop_ebx_bit_or_ebx_eax)
		//{
		//	ebx = stack[esp++];
		//	eax = int(eax) + int(ebx);					// to do , glsl has no |!!!!!
		//}
		//else if (op == pop_ebx_bit_xor_ebx_eax)
		//{
		//	ebx = stack[esp++];
		//	eax = int(eax) * int(ebx);                   // to do , glsl has no ^!!!!!
		//}
		//else if (op == pop_ebx_shl_ebx_eax)
		//{
		//	ebx = stack[esp++];
		//	eax = int(ebx)  * int(eax);						// to do , glsl has no <<  !!!!!
		//}
		//else if (op == pop_ebx_shr_ebx_eax)
		//{
		//	ebx = stack[esp++];
		//	eax = int(ebx) * int(eax);                 // to do , glsl has no >>  !!!!!
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
		else if (op == bit_negative_eax)
		{
			eax = ~int(eax);
		}
		else if (op == not_eax)
		{
			if (eax != 0)
				eax = 0;
			else
				eax = 1;

		}
		else if (op == mov_eax_ValueAtEax_bit_negative_eax)
		{
			eax = stack[int(eax)];
			eax = ~int(eax);
		}
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
				//			msg(juce::String(stack[esp + 3]) + " " + juce::String(stack[esp + 2]) + " " + juce::String(stack[esp + 1]) + " " + juce::String(stack[esp + 0]));
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

