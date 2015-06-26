#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>
#include <gloperate-glraw/TextureFilter.h>

namespace globjects
{
	class Texture;
	class Framebuffer;
}

namespace gloperate
{
    class AbstractViewportCapability;
	class InputCapability;
	class ScreenAlignedQuad;
}

namespace gloperate_qt
{
	class QtOpenGLWindow;
}

class InputHandling;

class GlrawExample : public gloperate::Painter
{
public:
	GlrawExample(gloperate::ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas);
    virtual ~GlrawExample();

protected:
	virtual void onInitialize() override;
    virtual void onPaint() override;

	globjects::Texture* loadTexture(const std::string& path);
	void updateScreenSize();

    /* capabilities */
    gloperate::AbstractViewportCapability * m_viewportCapability;
	gloperate::InputCapability * m_inputCapability;

    /* members */
	globjects::ref_ptr<globjects::Texture> m_texture;
	globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;

	InputHandling * m_inputHandler;

	gloperate_glraw::TextureFilter m_filter;
	glm::vec2 m_size;
};
