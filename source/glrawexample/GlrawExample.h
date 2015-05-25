#pragma once

#include <memory>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>



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
}

class GlrawExample : public gloperate::Painter
{
public:
    GlrawExample(gloperate::ResourceManager & resourceManager);
    virtual ~GlrawExample();

    void setupProjection();

protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;

protected:
    /* capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractPerspectiveProjectionCapability * m_projectionCapability;
    gloperate::AbstractCameraCapability * m_cameraCapability;

    /* members */
	globjects::ref_ptr<globjects::Program> m_program_blur_horizontal;
	globjects::ref_ptr<globjects::Program> m_program_blur_vertical;
	globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;
	globjects::ref_ptr<globjects::Texture> m_texture;
	globjects::ref_ptr<globjects::Framebuffer> m_fbo;
};
