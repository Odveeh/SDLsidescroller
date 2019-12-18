#pragma once


/* kompositioharjoitusta
https://www.learncpp.com/cpp-tutorial/102-composition/
*/



class Point2D
{
private:
	float m_x;
	float m_y;

public:
	Point2D();
	Point2D(float x, float y);
	~Point2D();

	void setPoint(float x, float y);

	float getX();
	float getY();
};

