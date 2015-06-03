#include "InputHandling.h"

#include "GlrawExample.h"

InputHandling::InputHandling(GlrawExample * handle) :
m_handle(handle),
m_zoom(1.f)
{
}

void InputHandling::onKeyUp(gloperate::Key key)
{
	if (key == gloperate::KeyW)
	{
		m_pos.y += 1.f;
	}
	if (key == gloperate::KeyS)
	{
		m_pos.y -= 1.f;
	}
	if (key == gloperate::KeyD)
	{
		m_pos.x += 1.f;
	}
	if (key == gloperate::KeyA)
	{
		m_pos.x -= 1.f;
	}
	if (key == gloperate::KeyQ)
	{
		m_zoom += 0.1f;
	}
	if (key == gloperate::KeyE)
	{
		m_zoom -= 0.1f;
	}
}

glm::vec2 InputHandling::getPos() const
{
	return m_pos;
}
float InputHandling::getZoom() const
{
	return m_zoom;
}