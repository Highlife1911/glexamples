#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include "DitheringOptions.h"

class InputHandling;

namespace globjects
{
	class Texture;
	class Framebuffer;
}

namespace gloperate
{
    class AbstractViewportCapability;
	class ScreenAlignedQuad;
	class InputCapability;
}

class Dithering : public gloperate::Painter
{
public:
	Dithering( gloperate::ResourceManager & resourceManager );
	virtual ~Dithering();

	void setOptionsChanged();

protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;

	void loadTexture();
	void setupFramebuffer();
	void updateUniforms();
	void dither();

protected:
    /* capabilities */
    gloperate::AbstractViewportCapability * m_viewportCapability;
	gloperate::InputCapability * m_inputCapability;

    /* members */
	DitheringOptions m_options;

	globjects::ref_ptr<globjects::Texture> m_dithered;
	globjects::ref_ptr<globjects::Texture> m_comptex;
	globjects::ref_ptr<globjects::Framebuffer> m_fbo;
	globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;
	globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_screen;

	InputHandling * m_inputHandler;

	glm::ivec2 m_textureSize;
	bool m_changed;
};
