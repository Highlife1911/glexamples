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

#include <globjects/Shader.h>
#include <globjects/Program.h>

#include <qimage.h>

using widgetzeug::make_unique;

GlrawExample::GlrawExample(gloperate::ResourceManager & resourceManager)
:   Painter(resourceManager)
,   m_targetFramebufferCapability(addCapability(new gloperate::TargetFramebufferCapability()))
,   m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
,   m_projectionCapability(addCapability(new gloperate::PerspectiveProjectionCapability(m_viewportCapability)))
,   m_cameraCapability(addCapability(new gloperate::CameraCapability()))
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
	/*
	
	//Load test file
	glraw::ConvertManager * manager = new glraw::ConvertManager();
	manager->setConverter(new glraw::Converter());
	manager->convert(img, glraw::AssetInformation());
	*/
	QImage img("C:/test.png");
	img = img.convertToFormat(QImage::Format_ARGB32);
	
	m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
	m_texture->image2D(0, gl::GL_RGBA, img.width(), img.height(), 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, img.bits());
	m_size = new glm::vec2(img.width(), img.height());
	
	m_fbo = new globjects::Framebuffer;
	m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_texture, 0);

	//Setup quad
	m_quad = new ScreenAlignedQuad();

	m_program_blur_horizontal = new globjects::Program;
	m_program_blur_horizontal->attach(
		globjects::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/glrawexample/default.vert"),
		globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/glrawexample/blur_horizontal.frag")
		);
	m_program_blur_horizontal->setUniform("size", m_size);

	m_program_blur_vertical = new globjects::Program;
	m_program_blur_vertical->attach(
		globjects::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/glrawexample/default.vert"),
		globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/glrawexample/blur_vertical.frag")
		);
	m_program_blur_vertical->setUniform("size", m_size);
	

    gl::glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

    setupProjection();

	m_fbo->unbind();
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

        m_viewportCapability->setChanged(false);
    }
	
	m_quad->m_vao->bind();
	m_program_blur_horizontal->use();

	m_fbo->bind();
	gl::glClear(gl::GL_COLOR_BUFFER_BIT);
	
	m_quad->draw();
	m_program_blur_horizontal->release();
	
	/*
	m_program_blur_vertical->use();
	m_quad->draw();
	m_program_blur_vertical->release();
	*/

	//std::array<int, 4> sourceRect = { { 0, 0, m_size->x, m_size->y } };
	std::array<int, 4> sourceRect = { { 0, 0, m_viewportCapability->width() / 4, m_viewportCapability->height() / 4 } };
	std::array<int, 4> destRect = { { 0, 0, m_viewportCapability->width(), m_viewportCapability->height() } };

	globjects::Framebuffer * targetFBO = m_targetFramebufferCapability->framebuffer() ? m_targetFramebufferCapability->framebuffer() : globjects::Framebuffer::defaultFBO();

	m_fbo->blit(gl::GL_COLOR_ATTACHMENT0, sourceRect, targetFBO, gl::GL_BACK_LEFT, destRect, gl::GL_COLOR_BUFFER_BIT, gl::GL_NEAREST);
	m_fbo->unbind();
}
