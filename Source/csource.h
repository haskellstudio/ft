

int main()
{
	set_source_rgba(1.0, 0.0, 0.0, 1.0);
	set_line_width(0.05);
	set_blur(0.01);
	move_to(-0.2, 0.0);
	line_to(0.5, 0.0);
	line_to(0.0, -0.7);
	stroke();
	getCharInGrid( 80, 1, 1);
	getChar(80, 0.7, 0.5, 0.1,  0.1);
}
