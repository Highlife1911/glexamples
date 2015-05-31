#pragma once

#include <gloperate/input/KeyboardInputHandler.h>

class GlrawExample;

class InputHandling : public gloperate::KeyboardInputHandler
{
public:
	InputHandling(GlrawExample * handle);
protected:
	void onKeyUp(gloperate::Key) override;

protected:
	GlrawExample * m_handle;
};