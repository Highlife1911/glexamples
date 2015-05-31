#include "InputHandling.h"

#include "GlrawExample.h"

InputHandling::InputHandling(GlrawExample * handle) :
m_handle(handle)
{
}

void InputHandling::onKeyUp(gloperate::Key key)
{
	if (key == gloperate::KeyW)
	{
		m_handle->m_pos->y += 1.f;
	}
	if (key == gloperate::KeyS)
	{
		m_handle->m_pos->y -= 1.f;
	}
	if (key == gloperate::KeyD)
	{
		m_handle->m_pos->x += 1.f;
	}
	if (key == gloperate::KeyA)
	{
		m_handle->m_pos->x -= 1.f;
	}
	if (key == gloperate::KeyQ)
	{
		m_handle->m_zoom += 0.1f;
	}
	if (key == gloperate::KeyE)
	{
		m_handle->m_zoom -= 0.1f;
	}
}