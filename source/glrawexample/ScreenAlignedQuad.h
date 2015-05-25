#pragma once

#include <globjects/base/ref_ptr.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>

class ScreenAlignedQuad
{
public:
	ScreenAlignedQuad();
	void draw();
	globjects::ref_ptr<globjects::VertexArray> m_vao;
	globjects::ref_ptr<globjects::Buffer> m_buffer;
};