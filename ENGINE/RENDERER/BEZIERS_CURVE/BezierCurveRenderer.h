#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct BezierCurvePoint
{
	BezierCurvePoint(float x_, float y_)
	{
		x = x_;
		y = y_;
	}
	float x, y;
};

class BezierCurveRenderer
{
public:
	BezierCurveRenderer();
	~BezierCurveRenderer();

	void Initialize();

	void Clear();
	void Prepare(BezierCurvePoint fixedCtrlPnt1, BezierCurvePoint movableCtrlPnt, BezierCurvePoint fixedCtrlPnt2);
	void Render();
private:
	GLuint BezierCurveVAO, BezierCurveVBO;
	std::vector<BezierCurvePoint> bezierCurvePoints;

	BezierCurvePoint Lerp(BezierCurvePoint pt1, BezierCurvePoint pt2, float t);
};