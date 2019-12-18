#include "Point2D.h"


Point2D::Point2D()
{
	m_x = 0.0f;
	m_y = 0.0f;
}



Point2D::Point2D(float x, float y)
{
	m_x = x;
	m_y = y;
}


Point2D::~Point2D()
{
}


void Point2D::setPoint(float x, float y) {
	m_x = x;
	m_y = y;

}


float Point2D::getX() { return m_x; };

float Point2D::getY() { return m_y; };