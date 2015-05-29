#include "Dithering.h"

//#include <glm/gtc/constants.hpp>

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

#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>

#include <widgetzeug/make_unique.hpp>

#include <reflectionzeug/PropertyGroup.h>

using widgetzeug::make_unique;


Dithering::Dithering( gloperate::ResourceManager & resourceManager )
:   Painter(resourceManager)
,   m_targetFramebufferCapability(addCapability(new gloperate::TargetFramebufferCapability()))
,   m_viewportCapability(addCapability(new gloperate::ViewportCapability()))
,	m_chunkSize(32)
{
	addProperty<int>( "chunk_size", this, &Dithering::chunkSize, &Dithering::setChunkSize );
}

Dithering::~Dithering() = default;

void Dithering::setChunkSize( int size )
{
	m_chunkSize = size;
}

int Dithering::chunkSize() const
{
	return m_chunkSize;
}

void Dithering::onInitialize()
{
    globjects::init();

#ifdef __APPLE__
    Shader::clearGlobalReplacements();
    Shader::globalReplace("#version 140", "#version 150");

    debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

	m_texture = m_loader.load( "dithering.png", std::function<void(int,int)>() );

	m_textureSize.x = m_texture->getLevelParameter( 0, gl::GL_TEXTURE_WIDTH );
	m_textureSize.y = m_texture->getLevelParameter( 0, gl::GL_TEXTURE_HEIGHT );
	
	assert(m_textureSize.x % m_chunkSize == 0);
	assert(m_textureSize.y % m_chunkSize == 0);

	m_texture->bindImageTexture(0, 0, gl::GL_FALSE, 0, gl::GL_READ_ONLY, gl::GL_RGBA8);

	m_comptex = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
	m_comptex->image2D( 0, gl::GL_RGB, m_textureSize.x / m_chunkSize, m_textureSize.y / m_chunkSize, 0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, nullptr );

	//auto m_text = globjects::Texture::createDefault( gl::GL_TEXTURE_2D );
	//m_text->image2D( 0, gl::GL_RGB, img.width(), img.height(), 0, gl::GL_RGB, gl::GL_UNSIGNED_BYTE, nullptr );

	m_fbo = new globjects::Framebuffer;
	m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_comptex, 0);
	//m_fbo->unbind();

	//m_fbo2 = new globjects::Framebuffer;
	//m_fbo2->attachTexture( gl::GL_COLOR_ATTACHMENT0, m_text, 0 );
	//m_fbo2->unbind();

	//Setup quad
	m_quad = new gloperate::ScreenAlignedQuad(globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/dithering/dither.frag"), m_texture);
	m_quad2 = new gloperate::ScreenAlignedQuad(m_texture);


	m_quad->program()->setUniform("chunk_size", m_chunkSize);
	m_quad->program()->setUniform("img_size", m_textureSize);


    gl::glClearColor(0.85f, 0.87f, 0.91f, 1.0f);
}

//#include <chrono>

void Dithering::onPaint()
{
	//auto n = std::chrono::high_resolution_clock::now();
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

	//m_fbo2->bind();
	m_quad2->draw();
	//m_fbo2->unbind();

	//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - n).count() << std::endl;
}
