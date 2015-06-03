#pragma once

#include <glm/vec2.hpp>

#include <gloperate/input/KeyboardInputHandler.h>

class GlrawExample;

class InputHandling : public gloperate::KeyboardInputHandler
{
public:
	InputHandling(GlrawExample * handle);
	glm::vec2 getPos() const;
	float getZoom() const;

protected:
	void onKeyUp(gloperate::Key) override;

	GlrawExample * m_handle;

	glm::vec2 m_pos;
	float m_zoom;
};