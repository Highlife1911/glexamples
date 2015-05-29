#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include <gloperate-qt/QtTextureLoader.h>


namespace globjects
{
	class Texture;
	class Framebuffer;
}

namespace gloperate
{
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;

	class ScreenAlignedQuad;
}

class Dithering : public gloperate::Painter
{
public:
	Dithering( gloperate::ResourceManager & resourceManager );
	virtual ~Dithering();

	int chunkSize() const;
	void setChunkSize(int size);


protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;

protected:
    /* capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;

    /* members */
	const gloperate_qt::QtTextureLoader m_loader;

	globjects::ref_ptr<globjects::Texture> m_texture;
	globjects::ref_ptr<globjects::Texture> m_comptex;
	globjects::ref_ptr<globjects::Framebuffer> m_fbo;
	globjects::ref_ptr<globjects::Framebuffer> m_fbo2;
	gloperate::ScreenAlignedQuad * m_quad;
	gloperate::ScreenAlignedQuad * m_quad2;
	gloperate::ScreenAlignedQuad * m_copyquad;

	glm::ivec2 m_textureSize;
	int m_chunkSize;
};
