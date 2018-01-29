
int main()
{
	set_source_rgba(1.0, 0.0, 0.0, 1.0);
	set_line_width(0.01);
	set_blur(0.005);
	move_to(0.0, 0.0);
	line_to(0.5, 0.0);
	stroke();
}