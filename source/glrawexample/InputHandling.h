#pragma once

#include <gloperate/input/KeyboardInputHandler.h>

class GlrawExample;

class InputHandling : public gloperate::KeyboardInputHandler
{
public:
	InputHandling(GlrawExample * handle);
	glm::vec2 getPos();
	float getZoom();

protected:
	void onKeyUp(gloperate::Key) override;

protected:
	glm::vec2 m_pos;
	float m_zoom;

	GlrawExample * m_handle;
};