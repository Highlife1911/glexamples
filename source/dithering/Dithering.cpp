#include "Dithering.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/boolean.h>
#include <glbinding/gl/types.h>

#include <globjects/globjects.h>
#include <globjects/logging.h>
#include <globjects/DebugMessage.h>
#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/InputCapability.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>

#include "InputHandling.h"

Dithering::Dithering( gloperate::ResourceManager & resourceManager )
:   Painter(resourceManager)
,   m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
,	m_inputCapability(addCapability(new gloperate::InputCapability()))
,	m_options(this)
,	m_inputHandler(new InputHandling())
,	m_changed(false)
{
}

Dithering::~Dithering() = default;

void Dithering::loadTexture()
{
	m_dithered = m_resourceManager.load<globjects::Texture>(m_options.imagePathString());
	if (m_dithered == nullptr)
	{
		globjects::fatal() << "Couldn't load image: " << m_options.imagePathString() << " !";
	}

	m_dithered->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_BORDER);
	m_dithered->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_BORDER);
	m_dithered->setParameter(gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
	m_dithered->setParameter(gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);

	m_textureSize.x = m_dithered->getLevelParameter(0, gl::GL_TEXTURE_WIDTH);
	m_textureSize.y = m_dithered->getLevelParameter(0, gl::GL_TEXTURE_HEIGHT);

	assert(m_textureSize.x % m_options.chunkSize() == 0);
	assert(m_textureSize.y % m_options.chunkSize() == 0);

	m_dithered->bindImageTexture(0, 0, gl::GL_FALSE, 0, gl::GL_READ_WRITE, gl::GL_RGBA8);

	if (m_quad)
	{
		m_quad->setTexture(m_dithered);
	}
	if (m_screen)
	{
		m_screen->setTexture(m_dithered);
	}
}

void Dithering::setupFramebuffer()
{
	m_comptex = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
	m_comptex->image2D(0, gl::GL_RGB, m_textureSize.x / m_options.chunkSize(), m_textureSize.y / m_options.chunkSize(), 0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, nullptr);
	m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_comptex, 0);
}

void Dithering::updateUniforms()
{
	m_quad->program()->setUniform("chunk_size", m_options.chunkSize());
	m_quad->program()->setUniform("num_colors", m_options.greyscalePalette());
	m_quad->program()->setUniform("grey", m_options.formulaData());
}

void Dithering::dither()
{
	m_fbo->bind();
	m_quad->draw();
	m_fbo->unbind();
}

void Dithering::onInitialize()
{
    globjects::init();

#ifdef __APPLE__
	globjects::Shader::clearGlobalReplacements();
	globjects::Shader::globalReplace("#version 140", "#version 150");

	globjects::debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

	m_inputCapability->addKeyboardHandler(m_inputHandler);

	loadTexture();

	m_fbo = new globjects::Framebuffer;
	m_quad = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/dithering/dither.frag"), m_dithered);
	m_screen = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/dithering/screen.frag"), m_dithered);

	setupFramebuffer();
	updateUniforms();

    gl::glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

	dither();
}

void Dithering::onPaint()
{
    if (m_viewportCapability->hasChanged())
    {
        gl::glViewport(
            m_viewportCapability->x(),
            m_viewportCapability->y(),
            m_viewportCapability->width(),
            m_viewportCapability->height());

		glm::vec2 size = { m_viewportCapability->width(), m_viewportCapability->height() };
		m_screen->program()->setUniform("size", size);

        m_viewportCapability->setChanged(false);
    }

	if (m_changed)
	{
		loadTexture();
		setupFramebuffer();
		updateUniforms();

		dither();

		m_changed = false;
	}

	gl::glClear(gl::GL_COLOR_BUFFER_BIT);


	m_screen->program()->setUniform("pos", m_inputHandler->pos());
	m_screen->program()->setUniform("zoom", m_inputHandler->zoom());
	m_screen->draw();
}

void Dithering::setOptionsChanged()
{
	m_changed = true;
}