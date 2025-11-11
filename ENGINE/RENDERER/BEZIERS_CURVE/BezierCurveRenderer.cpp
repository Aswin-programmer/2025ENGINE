#include "BezierCurveRenderer.h"

BezierCurveRenderer::BezierCurveRenderer()
	:
	BezierCurveVAO{0},
	BezierCurveVBO{0},
	bezierCurvePoints{}
{
	Initialize();
}

BezierCurveRenderer::~BezierCurveRenderer()
{
	bezierCurvePoints.clear();
	glDeleteBuffers(1, &BezierCurveVBO);
	glDeleteVertexArrays(1, &BezierCurveVAO);
}

void BezierCurveRenderer::Initialize()
{
	glCreateVertexArrays(1, &BezierCurveVAO);

	//Vertex Buffer Configuration
	glCreateBuffers(1, &BezierCurveVBO);
	glNamedBufferStorage(BezierCurveVBO, sizeof(BezierCurvePoint) * 1000, nullptr, GL_DYNAMIC_STORAGE_BIT);

	glVertexArrayVertexBuffer(BezierCurveVAO, 0, BezierCurveVBO, 0, sizeof(BezierCurvePoint));

	glVertexArrayAttribFormat(BezierCurveVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(BezierCurveVAO, 0, 0);
	glEnableVertexArrayAttrib(BezierCurveVAO, 0);
}

void BezierCurveRenderer::Clear()
{
	bezierCurvePoints.clear();
}

void BezierCurveRenderer::Prepare(BezierCurvePoint fixedCtrlPnt1, BezierCurvePoint movableCtrlPnt, BezierCurvePoint fixedCtrlPnt2)
{
	for (float t = 0.0f; t <= 1.0001f; t += 0.01f)
	{
		bezierCurvePoints.push_back(
			Lerp(Lerp(fixedCtrlPnt1, movableCtrlPnt, t)
				, Lerp(movableCtrlPnt, fixedCtrlPnt2, t)
				, t)
		);
	}
}

void BezierCurveRenderer::Render()
{
	glBindVertexArray(BezierCurveVAO);

	glNamedBufferSubData(BezierCurveVBO, 0
		, sizeof(BezierCurvePoint) * bezierCurvePoints.size()
		, bezierCurvePoints.data());

	glDrawArrays(GL_LINE_STRIP, 0, bezierCurvePoints.size());
}

BezierCurvePoint BezierCurveRenderer::Lerp(BezierCurvePoint pt1, BezierCurvePoint pt2, float t)
{
	return BezierCurvePoint(
		(1 - t) * pt1.x + t * pt2.x,
		(1 - t) * pt1.y + t * pt2.y
	);
}
