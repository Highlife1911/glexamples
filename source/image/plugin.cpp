#include <gloperate/plugin/plugin_api.h>

#include "viewer/Viewer.h"
#include "dithering/Dithering.h"

#include <glexamples-version.h>


namespace gloperate
{
	template <>
	Painter * PainterPlugin<Viewer>::createPainter(ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas) const
	{
		return new Viewer(resourceManager, canvas);
	}
}

GLOPERATE_PLUGIN_LIBRARY

	GLOPERATE_PLUGIN(Viewer
		, "Viewer"
		, "Loads various image formats."
		, GLEXAMPLES_AUTHOR_ORGANIZATION
		, "v1.0.0" )

	GLOPERATE_PLUGIN(Dithering
		, "Dithering"
		, "Shows gpu based dithering."
		, GLEXAMPLES_AUTHOR_ORGANIZATION
		, "v1.0.0")

GLOPERATE_PLUGIN_LIBRARY_END
