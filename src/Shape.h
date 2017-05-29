#ifndef DEF_SHAPE
#define DEF_SHAPE

#include <string>

class Shape
{
public:
	Shape();
	Shape(float const& x, float const& y, float const& radius);
	void draw();
	void hide();
	void display();
	void toggle();
	bool isVisible();
	void move(float const& x, float const& y);

private:
	float m_radius;
	float m_xpos;
	float m_ypos;
	bool m_visible;
};

#endif