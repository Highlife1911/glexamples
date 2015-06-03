#include <gloperate/plugin/plugin_api.h>

#include "GlrawExample.h"

#include <glexamples-version.h>

namespace gloperate
{
	template <>
	Painter * PainterPlugin<GlrawExample>::createPainter(ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas) const
	{
		return new GlrawExample(resourceManager, canvas);
	}
}

GLOPERATE_PLUGIN_LIBRARY

    GLOPERATE_PLUGIN(GlrawExample
    , "GlrawExample"
    , "Example that shows the capabilities of glraw."
    , GLEXAMPLES_AUTHOR_ORGANIZATION
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
