#pragma once

#include <reflectionzeug/FilePath.h>

class ViewerOptions
{
public:
	ViewerOptions();
	~ViewerOptions() = default;

	void setImagePath(const reflectionzeug::FilePath & path);
	reflectionzeug::FilePath imagePath() const;

	void setPipeline(bool state);
	bool pipeline() const;

	void setChanged(bool state);
	bool changed() const;

protected:

	reflectionzeug::FilePath m_imagePath;
	bool m_pipeline;
	bool m_changed;
};