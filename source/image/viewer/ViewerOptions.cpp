#include "ViewerOptions.h"

namespace
{
	const std::string DefaultImagePath = "data/Viewer/test.png";
}

ViewerOptions::ViewerOptions()
	: m_imagePath(DefaultImagePath)
	, m_changed(false)
	, m_pipeline(false)
{
}

void ViewerOptions::setImagePath(const reflectionzeug::FilePath & path)
{
	m_imagePath = path;
	setChanged(true);
}

reflectionzeug::FilePath ViewerOptions::imagePath() const
{
	return m_imagePath;
}

void ViewerOptions::setPipeline(bool state)
{
	m_pipeline = state;
	setChanged(true);
}

bool ViewerOptions::pipeline() const
{
	return m_pipeline;
}

void ViewerOptions::setChanged(bool state)
{
	m_changed = true;
}

bool ViewerOptions::changed() const
{
	return m_changed;
}