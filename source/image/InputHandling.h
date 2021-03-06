#pragma once

#include <glm/vec2.hpp>

#include <gloperate/input/KeyboardInputHandler.h>

class InputHandling : public gloperate::KeyboardInputHandler
{
public:
	InputHandling();

	void		setPos(glm::vec2 pos);
	glm::vec2	pos() const;
	void		setZoom(float zoom);
	float		zoom() const;

protected:

	virtual void onKeyUp(gloperate::Key) override;

	glm::vec2	m_pos;
	float		m_zoom;
};