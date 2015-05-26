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
	img = img.mirrored();
	img = img.convertToFormat(QImage::Format_RGB888);
	
	m_texture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
	m_texture->image2D(0, gl::GL_RGB, img.width(), img.height(), 0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, img.bits());

	auto m_tex = globjects::Texture::createDefault( gl::GL_TEXTURE_2D );
	m_tex->image2D( 0, gl::GL_RGB, img.width(), img.height(), 0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, nullptr );

	auto m_text = globjects::Texture::createDefault( gl::GL_TEXTURE_2D );
	m_text->image2D( 0, gl::GL_RGB, img.width(), img.height(), 0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, nullptr );

	m_size = new glm::vec2(img.width(), img.height());

	m_fbo = new globjects::Framebuffer;
	m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_tex, 0);
	m_fbo->unbind();

	m_fbo2 = new globjects::Framebuffer;
	m_fbo2->attachTexture( gl::GL_COLOR_ATTACHMENT0, m_text, 0 );
	m_fbo2->unbind();

	//Setup quad
	m_quad = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/glrawexample/blur_horizontal.frag"), m_texture);
	m_quad2 = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/glrawexample/blur_vertical.frag"), m_tex);

	m_quad->program()->setUniform( "size", *m_size);
	m_quad2->program()->setUniform("size", *m_size);


    gl::glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

    setupProjection();

}

#include <chrono>

void GlrawExample::onPaint()
{
	auto n = std::chrono::high_resolution_clock::now();
    if (m_viewportCapability->hasChanged())
    {
        gl::glViewport(
            m_viewportCapability->x(),
            m_viewportCapability->y(),
            m_viewportCapability->width(),
            m_viewportCapability->height());

        m_viewportCapability->setChanged(false);
    }
	
	gl::glClear(gl::GL_COLOR_BUFFER_BIT);

	m_fbo->bind();
	m_quad->draw();
	m_fbo->unbind();

	m_fbo2->bind();
	m_quad2->draw();
	
	std::array<int, 4> sourceRect = { { 0, 0, m_size->x, m_size->y } };
	std::array<int, 4> destRect = { { 0, 0, std::min( m_viewportCapability->width(), (int)m_size->x), std::min( m_viewportCapability->height(), (int) m_size->y ) } };

	globjects::Framebuffer * targetFBO = m_targetFramebufferCapability->framebuffer() ? m_targetFramebufferCapability->framebuffer() : globjects::Framebuffer::defaultFBO();

	m_fbo2->blit( gl::GL_COLOR_ATTACHMENT0, destRect, targetFBO, gl::GL_BACK_LEFT, sourceRect, gl::GL_COLOR_BUFFER_BIT, gl::GL_LINEAR );
	m_fbo2->unbind();

	//std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - n;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - n).count() << std::endl;
}
