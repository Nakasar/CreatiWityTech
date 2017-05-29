#include "Shape.h"

#include <SDL2/SDL_opengl.h>
#include <math.h>

using namespace std;

Shape::Shape() : m_radius(1.0), m_xpos(0), m_ypos(0), m_visible(false)
{

}

Shape::Shape(float const& x, float const& y, float const& radius) : m_radius(radius), m_xpos(x), m_ypos(y), m_visible(false)
{

}

bool Shape::isVisible()
{
	return m_visible;
}

void Shape::draw()
{
	// Draw shape only if visibility is set to True
	if (m_visible)
	{
		int splices = 20;

		float pi2 = 3.1416f * 2.0f;

		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(m_xpos, m_ypos);
		for (int i = 0; i <= splices; i++)
		{
			glVertex2f(
				m_xpos + (m_radius * cos(i * pi2 / splices)),
				m_ypos + (m_radius * sin(i * pi2 / splices))
			);
		}
		glEnd();
	}
}

void Shape::display()
{
	m_visible = true;
}

void Shape::hide()
{
	m_visible = false;
}

void Shape::toggle()
{
	m_visible = !m_visible;
}

void Shape::move(float const& x, float const& y)
{
	m_xpos += x;
	m_ypos += y;
}
