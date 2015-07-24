#pragma once

#include <glm/vec3.hpp>

#include <reflectionzeug/FilePath.h>

class Dithering;

using reflectionzeug::FilePath;

enum class DitheringMethod
{
	Nearest,
	FloydSteinberg,
	Jarvis,
	Stucki
};

enum class GreyscaleFormula
{
	Classic,
	Modern
};

class DitheringOptions
{
public:
	DitheringOptions(Dithering* owner);

	int chunkSize() const;
	void setChunkSize(int size);

	int greyscalePalette() const;
	void setGreyscalePalette(int colors);

	GreyscaleFormula formula() const;
	void setFormula(GreyscaleFormula f);
	glm::vec3 formulaData() const;

	FilePath imagePath() const;
	std::string imagePathString() const;
	void setImagePath(FilePath path);

private:

	static bool isPowerOfTwo(unsigned int number);

	Dithering * const m_owner;

	int m_chunkSize;
	int m_greyscalePalette;
	std::string m_imagePath;
	GreyscaleFormula m_formula;
};