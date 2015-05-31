#include "GlrawExample.h"

#include <glm/gtc/constants.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>


#include <widgetzeug/make_unique.hpp>

#include <gloperate/base/RenderTargetType.h>

#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>
#include <gloperate/painter/InputCapability.h>
#include <gloperate/input/KeyboardInputHandler.h>

#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <gloperate-qt/QtTextureLoader.h>


using widgetzeug::make_unique;

GlrawExample::GlrawExample(gloperate::ResourceManager & resourceManager)
	: Painter(resourceManager)
	, m_targetFramebufferCapability(addCapability(new gloperate::TargetFramebufferCapability()))
	, m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
	, m_projectionCapability(addCapability(new gloperate::PerspectiveProjectionCapability(m_viewportCapability)))
	, m_cameraCapability(addCapability(new gloperate::CameraCapability()))
	, m_inputCapability(addCapability(new gloperate::InputCapability()))
{
}

GlrawExample::~GlrawExample() = default;

void GlrawExample::setupProjection()
{
    //static const auto zNear = 0.3f, zFar = 15.f, fovy = 50.f;
}

void GlrawExample::onInitialize()
{
    // create program

    globjects::init();

#ifdef __APPLE__
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif
	gloperate_qt::QtTextureLoader * loader = new gloperate_qt::QtTextureLoader;
	m_texture = loader->load("C:/test.png", nullptr);

	m_pos = new glm::vec2(0.f, 0.f);
	m_size = new glm::vec2(m_viewportCapability->width(), m_viewportCapability->height());
	m_zoom = 1.f;
	m_quad = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/glrawexample/screen.frag"), m_texture);
	m_quad->program()->setUniform("size", *m_size);

	gl::glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

	m_inputHandler = new InputHandling(this);
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
		m_size->x = m_viewportCapability->width();
		m_size->y = m_viewportCapability->height();

        m_viewportCapability->setChanged(false);
    }

	gl::glClear(gl::GL_COLOR_BUFFER_BIT);
	m_quad->program()->setUniform("pos", *m_pos);
	m_quad->program()->setUniform("zoom", m_zoom);
	m_quad->draw();
}
