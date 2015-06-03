#include "InputHandling.h"

#include "GlrawExample.h"

InputHandling::InputHandling(GlrawExample * handle) :
m_handle(handle),
m_pos(glm::vec2(0.f,0.f)),
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

glm::vec2 InputHandling::getPos()
{
	return m_pos;
}
float InputHandling::getZoom()
{
	return m_zoom;
}