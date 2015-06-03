#include "DitheringOptions.h"

#include "Dithering.h"

#include <globjects/logging.h>

DitheringOptions::DitheringOptions(Dithering * owner)
 :	m_owner(owner)
 ,	m_chunkSize(16)
 ,	m_palette(DitheringPalette::Greyscale)
 ,	m_greyscalePalette(8)
 ,  m_imagePath("data/dithering/dithering.png")
 ,  m_formula(GreyscaleFormula::Modern)
{
	owner->addProperty<reflectionzeug::FilePath>("image", this,
		&DitheringOptions::imagePath,
		&DitheringOptions::setImagePath);

	owner->addProperty<int>("chunk_size", this,
		&DitheringOptions::chunkSize,
		&DitheringOptions::setChunkSize)->setOptions({
			{ "minimum", 1 },
			{ "maximum", 8192 } });

	owner->addProperty<DitheringPalette>("palette", this,
		&DitheringOptions::palette,
		&DitheringOptions::setPalette)->setStrings({
			{ DitheringPalette::Greyscale, "Greyscale" },
			{ DitheringPalette::Reduced, "16 colors" },
			{ DitheringPalette::Full, "256 colors" } });

	owner->addProperty<int>("grey_colors", this,
		&DitheringOptions::greyscalePalette,
		&DitheringOptions::setGreyscalePalette)->setOptions({
			{ "minimum", 2 },
			{ "maximum", 256 } });

	owner->addProperty<GreyscaleFormula>("grey_formula", this,
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

DitheringPalette DitheringOptions::palette() const
{
	return m_palette;
}

void DitheringOptions::setPalette(DitheringPalette p)
{
	m_palette = p;
	m_owner->setOptionsChanged();
}

int DitheringOptions::greyscalePalette() const
{
	return m_greyscalePalette;
}

void DitheringOptions::setGreyscalePalette(int bits)
{
	m_greyscalePalette = bits;
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
		return glm::vec3(0.299f, 0.587f, 0.114f);

	case GreyscaleFormula::Modern:
		return glm::vec3(0.2126f, 0.7152f, 0.0722f);

	default:
		return glm::vec3();
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