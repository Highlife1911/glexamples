#include <gloperate/plugin/plugin_api.h>

#include "ViewerExample.h"

#include <glexamples-version.h>


GLOPERATE_PLUGIN_LIBRARY

GLOPERATE_PLUGIN(ViewerExample
    , "ViewerExample"
    , "Example that load various image formats."
    , GLEXAMPLES_AUTHOR_ORGANIZATION
    , "v1.0.0" )

GLOPERATE_PLUGIN_LIBRARY_END
