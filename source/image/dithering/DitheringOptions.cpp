#include "DitheringOptions.h"

#include "Dithering.h"

#include <globjects/logging.h>

DitheringOptions::DitheringOptions(Dithering * owner)
 :	m_owner(owner)
 ,	m_chunkSize(32)
 ,	m_greyscalePalette(12)
 ,  m_imagePath("data/viewer/test.png")
 ,  m_formula(GreyscaleFormula::Modern)
{
	owner->addProperty<reflectionzeug::FilePath>("image", this,
		&DitheringOptions::imagePath,
		&DitheringOptions::setImagePath);

	owner->addProperty<int>("chunk_size", this,
		&DitheringOptions::chunkSize,
		&DitheringOptions::setChunkSize)->setOptions({
			{ "minimum", 1 },
			{ "maximum", 512 } });

	owner->addProperty<int>("gray_colors", this,
		&DitheringOptions::greyscalePalette,
		&DitheringOptions::setGreyscalePalette)->setOptions({
			{ "minimum", 2 },
			{ "maximum", 256 } });

	owner->addProperty<GreyscaleFormula>("gray_formula", this,
		&DitheringOptions::formula,
		&DitheringOptions::setFormula)->setStrings({
			{ GreyscaleFormula::Classic, "Classic" },
			{ GreyscaleFormula::Modern, "Modern" } });
}

void DitheringOptions::setChunkSize(int size)
{
	if (size == m_chunkSize + 1)
	{
		size = m_chunkSize * 2;
	}
	else if (size == m_chunkSize - 1)
	{
		size = m_chunkSize / 2;
	}

	if (size < 8)
	{
		globjects::debug() << "The minimum valid chunk_size is 8. Entered: " << size;
	}
	else if (size > 8192)
	{
		globjects::debug() << "The maximum valid chunk_size is 8192. Entered: " << size;
	}
	else if (isPowerOfTwo(size))
	{
		m_chunkSize = size;
		m_owner->setOptionsChanged();
	}
	else
	{
		globjects::debug() << "chunk_size is not a power of 2. Entered: " << size;
	}
}

int DitheringOptions::chunkSize() const
{
	return m_chunkSize;
}

int DitheringOptions::greyscalePalette() const
{
	return m_greyscalePalette;
}

void DitheringOptions::setGreyscalePalette(int colors)
{
	m_greyscalePalette = colors;
	m_owner->setOptionsChanged();
}

GreyscaleFormula DitheringOptions::formula() const
{
	return m_formula;
}

glm::vec3 DitheringOptions::formulaData() const
{
	switch (m_formula)
	{
	case GreyscaleFormula::Classic:
		return { 0.299f, 0.587f, 0.114f };

	case GreyscaleFormula::Modern:
		return { 0.2126f, 0.7152f, 0.0722f };

	default:
		return {};
	}
}

void DitheringOptions::setFormula(GreyscaleFormula f)
{
	m_formula = f;
	m_owner->setOptionsChanged();
}

FilePath DitheringOptions::imagePath() const
{
	return FilePath( m_imagePath );
}

std::string DitheringOptions::imagePathString() const
{
	return m_imagePath;
}

void DitheringOptions::setImagePath(FilePath path)
{
	m_imagePath = path.string();
	m_owner->setOptionsChanged();
}

bool DitheringOptions::isPowerOfTwo(unsigned int number)
{ 
	return (number & (number - 1)) == 0;
}