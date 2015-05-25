#include "ScreenAlignedQuad.h"

#include <glbinding/gl/enum.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Texture.h>

ScreenAlignedQuad::ScreenAlignedQuad()
{
	static const std::array<glm::vec2, 4> raw{ {
			glm::vec2(+1.f, -1.f)
			, glm::vec2(+1.f, +1.f)
			, glm::vec2(-1.f, -1.f)
			, glm::vec2(-1.f, +1.f)
		} };

	m_vao = new globjects::VertexArray;
	m_buffer = new globjects::Buffer();
	m_buffer->setData(raw, gl::GL_STATIC_DRAW); //needed for some drivers

	auto binding = m_vao->binding(0);
	binding->setAttribute(0);
	binding->setBuffer(m_buffer, 0, sizeof(glm::vec2));
	binding->setFormat(2, gl::GL_FLOAT, gl::GL_FALSE, 0);
	m_vao->enable(0);
}

void ScreenAlignedQuad::draw()
{
	m_vao->bind();
	m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 4);
	m_vao->unbind();
}