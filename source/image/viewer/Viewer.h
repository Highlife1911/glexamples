#pragma once

#include <memory>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include <gloperate-glraw/TextureFilter.h>

#include "ViewerOptions.h"

namespace globjects
{
	class Texture;
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

class Viewer : public gloperate::Painter
{
public:
	Viewer(gloperate::ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas);
    virtual ~Viewer() = default;

protected:
	virtual void onInitialize() override;
    virtual void onPaint() override;

	void loadTexture();
	void updateScreenSize();
	bool initializePipeline();

    /* capabilities */
    gloperate::AbstractViewportCapability * m_viewportCapability;
	gloperate::InputCapability * m_inputCapability;

    /* members */
	globjects::ref_ptr<globjects::Texture> m_texture;
	globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;

	gloperate_glraw::TextureFilter m_filter;

	ViewerOptions m_options;

	InputHandling * m_inputHandler;
};
