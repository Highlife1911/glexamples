#pragma once

#include "InputHandling.h"

#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <gloperate/painter/Painter.h>

#include <gloperate\primitives\ScreenAlignedQuad.h>

#include <gloperate-glraw/TextureFilter.h>

namespace globjects
{
    class Program;
}

namespace gloperate
{
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;
	class InputCapability;
}

namespace gloperate_qt
{
	class QtOpenGLWindow;
}

class GlrawExample : public gloperate::Painter
{
public:
	GlrawExample(gloperate::ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas);
    virtual ~GlrawExample();

	void setupProjection();
protected:
	virtual void onInitialize() override;
    virtual void onPaint() override;

public:
	glm::vec2 m_size;

protected:
    /* capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractPerspectiveProjectionCapability * m_projectionCapability;
    gloperate::AbstractCameraCapability * m_cameraCapability;
	gloperate::InputCapability * m_inputCapability;

    /* members */
	globjects::ref_ptr<globjects::Program> m_program_blur_horizontal;
	globjects::ref_ptr<globjects::Program> m_program_blur_vertical;
	globjects::ref_ptr<globjects::Texture> m_texture;
	globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;

	InputHandling * m_inputHandler;
	gloperate_glraw::TextureFilter m_filter;
};
