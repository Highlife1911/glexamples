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
	, m_targetFramebufferCapability(addCapability(new gloperate::TargetFramebufferCapability()))
	, m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
	, m_inputCapability(addCapability(new gloperate::InputCapability()))
	, m_inputHandler(new InputHandling(this))
	, m_filter(canvas)
{
}

GlrawExample::~GlrawExample() = default;

void GlrawExample::onInitialize()
{
    // create program
    globjects::init();

#ifdef __APPLE__
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

	auto tmp = m_resourceManager.load<globjects::Texture>("data/glrawexample/test.png");
	if (m_filter.addFilter("grayscale", QVariantMap()))
	{
		m_texture = m_filter.process(tmp);
	}

	m_size = glm::vec2(m_viewportCapability->width(), m_viewportCapability->height());
	m_quad = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/glrawexample/screen.frag"), m_texture);
	m_quad->program()->setUniform("size", m_size);

	gl::glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

	m_inputCapability->addKeyboardHandler(m_inputHandler);
}

void GlrawExample::onPaint()
{
    if (m_viewportCapability->hasChanged())
    {
        gl::glViewport(
            m_viewportCapability->x(),
            m_viewportCapability->y(),
            m_viewportCapability->width(),
            m_viewportCapability->height());
		m_size.x = m_viewportCapability->width();
		m_size.y = m_viewportCapability->height();

        m_viewportCapability->setChanged(false);
    }

	gl::glClear(gl::GL_COLOR_BUFFER_BIT);
	m_quad->program()->setUniform("pos", m_inputHandler->getPos());
	m_quad->program()->setUniform("zoom", m_inputHandler->getZoom());
	m_quad->draw();
}
