#pragma once

#include <memory>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/Painter.h>

#include <reflectionzeug/FilePath.h>

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

class InputHandling;

class ViewerExample : public gloperate::Painter
{
public:
	ViewerExample(gloperate::ResourceManager & resourceManager);
    virtual ~ViewerExample() = default;

protected:
	virtual void onInitialize() override;
    virtual void onPaint() override;

	void loadTexture();
	void updateScreenSize();

    /* capabilities */
    gloperate::AbstractViewportCapability * m_viewportCapability;
	gloperate::InputCapability * m_inputCapability;

    /* members */
	globjects::ref_ptr<globjects::Texture> m_texture;
	globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;

	InputHandling * m_inputHandler;

	reflectionzeug::FilePath m_imagePath;
	bool m_changed;

	void setImagePath(const reflectionzeug::FilePath & path);
	reflectionzeug::FilePath imagePath() const;
};
