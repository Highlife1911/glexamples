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
#include <glraw/AssetInformation.h>
#include <glraw/Converter.h>
#include <glraw/ConvertManager.h>


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
	QImage img = QImage("C:/test.png");

	//Load test file
	glraw::ConvertManager * manager = new glraw::ConvertManager();
	manager->setConverter(new glraw::Converter());
	manager->convert(img, glraw::AssetInformation());
	*/
	/*
	m_program = new globjects::Program;
	m_program->attach(
		globjects::Shader::fromFile(gl::GL_VERTEX_SHADER, "data/glrawexample/default.vert"),
		globjects::Shader::fromFile(gl::GL_FRAGMENT_SHADER, "data/glrawexample/blur_horizontal.frag")
	);*/

	//Setup quad
	

    //glClearColor(0.85f, 0.87f, 0.91f, 1.0f);

    setupProjection();
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

	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

	glEnable(gl::GL_DEPTH_TEST);

}
