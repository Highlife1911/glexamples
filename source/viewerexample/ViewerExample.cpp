#include "ViewerExample.h"

#include <glm/vec2.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/globjects.h>

#include <gloperate/input/KeyboardInputHandler.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/InputCapability.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>

#include "InputHandling.h"

namespace
{
	const std::string DefaultImagePath = "data/ViewerExample/test.png";
}

ViewerExample::ViewerExample(gloperate::ResourceManager & resourceManager)
	: Painter(resourceManager)
	, m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
	, m_inputCapability(addCapability(new gloperate::InputCapability()))
	, m_inputHandler(new InputHandling())
	, m_imagePath(DefaultImagePath)
	, m_changed(false)
{
	addProperty<reflectionzeug::FilePath>("Image", this,
		&ViewerExample::imagePath,
		&ViewerExample::setImagePath);
}

void ViewerExample::onInitialize()
{
	globjects::init();

#ifdef __APPLE__
	Shader::clearGlobalReplacements();
	Shader::globalReplace("#version 140", "#version 150");

	globjects::debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

	m_inputCapability->addKeyboardHandler(m_inputHandler);

	m_quad = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/ViewerExample/screen.frag"));
	loadTexture();
	updateScreenSize();
}

void ViewerExample::loadTexture()
{
	m_texture = m_resourceManager.load<globjects::Texture>(imagePath().toString());
	if(m_texture == nullptr)
	{
		globjects::debug() << "Error while loading '" << imagePath().toString() << "'." << std::endl;
		return;
	}

	m_texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_BORDER);
	m_texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_BORDER);
	m_quad->setTexture(m_texture);
}

void ViewerExample::updateScreenSize()
{
	glm::vec2 size = { m_viewportCapability->width(), m_viewportCapability->height() };
	m_quad->program()->setUniform("size", size);
}

void ViewerExample::onPaint()
{
	if(m_changed)
	{
		loadTexture();
		m_changed = false;
	}

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

void ViewerExample::setImagePath(const reflectionzeug::FilePath & path)
{
	m_imagePath = path;
	m_changed = true;
}

reflectionzeug::FilePath ViewerExample::imagePath() const
{
	return m_imagePath;
}