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
		op = int (arrFloat[pc++]);
	
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
			int targetPc = int (arrFloat[pc++]);
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
			eax = stack[int (eax)];
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
			eax = int(ebx) / int (eax);            // to do , glsl has no mod!!!!!
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
		else if (op == pop_ebx_bit_and_ebx_eax)
		{
			ebx = stack[esp++];
			eax = int(eax) * int(ebx);                 // to do , glsl has no &!!!!!
		}
		else if (op == pop_ebx_bit_or_ebx_eax)
		{
			ebx = stack[esp++];
			eax = int(eax) + int(ebx);					// to do , glsl has no |!!!!!
		}
			else if (op == pop_ebx_bit_xor_ebx_eax)
		{
			ebx = stack[esp++];
			eax = int(eax) * int (ebx);                   // to do , glsl has no ^!!!!!
		}
		else if (op == pop_ebx_shl_ebx_eax)
		{
			ebx = stack[esp++];
			eax = int(ebx)  * int(eax);						// to do , glsl has no <<  !!!!!
		}
		else if (op == pop_ebx_shr_ebx_eax)
		{
			ebx = stack[esp++];
			eax = int(ebx) * int(eax);                 // to do , glsl has no >>  !!!!!
		}
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
			int index = int (arrFloat[pc++]);
			if (index == 0)
			{
				//int arg1 = stack[esp];
	//			msg(juce::String(stack[esp + 3]) + " " + juce::String(stack[esp + 2]) + " " + juce::String(stack[esp + 1]) + " " + juce::String(stack[esp + 0]));
			}
			else if (index == 1)
			{
				int arg1 = int( stack[esp]);
			}
		}
	}
	return eax;
}

void main() 
{
	vec2 uv = fragCoord.xy / iResolution.xx;
	 
	//float r = getcolor(uv); 

	float v = _vm();
	fragColor = vec4(v, 0, 0., 1.0);
}


