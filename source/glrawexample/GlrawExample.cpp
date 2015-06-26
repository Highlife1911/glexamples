#include "GlrawExample.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>

#include <gloperate/input/KeyboardInputHandler.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/InputCapability.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>

#include "InputHandling.h"

GlrawExample::GlrawExample(gloperate::ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas)
	: Painter(resourceManager)
	, m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
	, m_inputCapability(addCapability(new gloperate::InputCapability()))
	, m_filter(canvas)
	, m_inputHandler(new InputHandling())
{
}

GlrawExample::~GlrawExample() = default;

void GlrawExample::onInitialize()
{
	globjects::init();

#ifdef __APPLE__
	Shader::clearGlobalReplacements();
	Shader::globalReplace("#version 140", "#version 150");

	debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

	m_inputCapability->addKeyboardHandler(m_inputHandler);

	m_texture = loadTexture("data/glrawexample/test.png");
	m_texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_BORDER);
	m_texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_BORDER);
	m_quad = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/glrawexample/screen.frag"), m_texture);
	updateScreenSize();
}

globjects::Texture* GlrawExample::loadTexture(const std::string& path)
{
	globjects::ref_ptr<globjects::Texture> tex = m_resourceManager.load<globjects::Texture>(path);
	// TODO: Remove filter code because this is a load only viewer without filtering.
	if(m_filter.addFilter("grayscale", QVariantMap()))
	{
		return m_filter.process(tex);
	}
	return tex;
}

void GlrawExample::updateScreenSize()
{
	m_size = { m_viewportCapability->width(), m_viewportCapability->height() };
	m_quad->program()->setUniform("size", m_size);
}

void GlrawExample::onPaint()
{
	if(m_viewportCapability->hasChanged())
	{
		gl::glViewport(
			m_viewportCapability->x(),
			m_viewportCapability->y(),
			m_viewportCapability->width(),
			m_viewportCapability->height());

		updateScreenSize();
		m_viewportCapability->setChanged(false);
	}

	m_quad->program()->setUniform("pos", m_inputHandler->pos());
	m_quad->program()->setUniform("zoom", m_inputHandler->zoom());
	m_quad->draw();
}
