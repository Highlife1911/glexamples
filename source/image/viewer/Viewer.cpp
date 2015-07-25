#include "Viewer.h"

#include <glm/vec2.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/globjects.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/InputCapability.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>

#include "InputHandling.h"


Viewer::Viewer(gloperate::ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas)
	: Painter(resourceManager)
	, m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
	, m_inputCapability(addCapability(new gloperate::InputCapability()))
	, m_filter(canvas)
	, m_inputHandler(new InputHandling())
{
	addProperty<reflectionzeug::FilePath>("image", &m_options,
		&ViewerOptions::imagePath,
		&ViewerOptions::setImagePath);

	addProperty<bool>("pipeline", &m_options,
		&ViewerOptions::pipeline,
		&ViewerOptions::setPipeline);
}

void Viewer::onInitialize()
{
	globjects::init();

#ifdef __APPLE__
	Shader::clearGlobalReplacements();
	Shader::globalReplace("#version 140", "#version 150");

	globjects::debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

	m_inputCapability->addKeyboardHandler(m_inputHandler);

	if(!initializePipeline())
	{
		globjects::debug() << "Error while building the pipeline." << std::endl;
	}

	m_quad = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/viewer/screen.frag"));
	loadTexture();
	updateScreenSize();
}

bool Viewer::initializePipeline()
{
	return m_filter.addFilter(
	{
		{ "brightness", 
			{ { "amount", 0.1 } } 
		},
		{ "erosion", 
			{ { "size", 4 } } 
		},
		{ "radialblur", 
			{ { "x", 0.2 }, { "y", 0.3 }, { "blur", 0.15 } } 
		},
		{ "grayscale", 
			{} 
		}
	});
}

void Viewer::loadTexture()
{
	{
		globjects::ref_ptr<globjects::Texture> tex = m_resourceManager.load<globjects::Texture>(m_options.imagePath().toString());
		if(tex == nullptr)
		{
			globjects::debug() << "Error while loading '" << m_options.imagePath().toString() << "'." << std::endl;
			return;
		}

		if(m_options.pipeline())
		{
			m_texture = m_filter.process(tex);
		}
		else
		{
			m_texture = tex;
		}
	}

	{
		gl::GLint width = m_texture->getLevelParameter(0, gl::GL_TEXTURE_WIDTH);
		gl::GLint height = m_texture->getLevelParameter(0, gl::GL_TEXTURE_HEIGHT);

		float zoom_x = m_viewportCapability->width() / static_cast<float>(width);
		float zoom_y = m_viewportCapability->height() / static_cast<float>(height);

		static const int rounding = 10;
		int zoom = static_cast<int>(std::min(zoom_x, zoom_y) * rounding);
		m_inputHandler->setZoom(static_cast<float>(zoom) / rounding);
	}

	m_texture->setParameter(gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_BORDER);
	m_texture->setParameter(gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_BORDER);
	m_quad->setTexture(m_texture);
}

void Viewer::updateScreenSize()
{
	glm::vec2 size = { m_viewportCapability->width(), m_viewportCapability->height() };
	m_quad->program()->setUniform("size", size);
}

void Viewer::onPaint()
{
	if(m_options.changed())
	{
		loadTexture();
		m_options.setChanged(false);
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