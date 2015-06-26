#include "InputHandling.h"

#include <algorithm>

namespace
{
	const float MovementFactor = 1.0f;
	const float ZoomFactor = 0.1f;
}

InputHandling::InputHandling()
	: m_zoom(1.0f)
{
}

void InputHandling::onKeyUp(gloperate::Key key)
{
	switch(key)
	{
	case gloperate::KeyW:	
		m_pos.y += MovementFactor; break;
	case gloperate::KeyS:	
		m_pos.y -= MovementFactor; break;
	case gloperate::KeyD:	
		m_pos.x += MovementFactor; break;
	case gloperate::KeyA:	
		m_pos.x -= MovementFactor; break;
	case gloperate::KeyQ:	
		m_zoom += ZoomFactor; break;
	case gloperate::KeyE:	
		m_zoom -= ZoomFactor;
		// Prevent a zooming factor of 0.0f
		m_zoom = std::max(m_zoom, ZoomFactor); break;
	}
}

glm::vec2 InputHandling::pos() const
{
	return m_pos;
}
float InputHandling::zoom() const
{
	return m_zoom;
}