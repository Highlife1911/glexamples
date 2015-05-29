#include "DitheringOptions.h"

#include "Dithering.h"

DitheringOptions::DitheringOptions(Dithering * owner)
 :	m_owner(owner)
 ,	m_chunkSize(32)
 ,	m_palette(DitheringPalette::Greyscale)
 ,	m_greyscalePalette(4)
{
	owner->addProperty<int>("chunk_size", this, 
		&DitheringOptions::chunkSize, 
		&DitheringOptions::setChunkSize)->setOptions({
			{ "minimum", 1 },
			{ "maximum", 1024 } });

	owner->addProperty<DitheringPalette>("Palette", this,
		&DitheringOptions::palette,
		&DitheringOptions::setPalette)->setStrings({
			{ DitheringPalette::Greyscale, "Greyscale" },
			{ DitheringPalette::Reduced, "16 colors" },
			{ DitheringPalette::Full, "256 colors" } });

	owner->addProperty<int>("grayscale_colors", this, 
		&DitheringOptions::greyscalePalette, 
		&DitheringOptions::setGreyscalePalette)->setOptions({
			{ "minimum", 2 },
			{ "maximum", 256 } });
}

void DitheringOptions::setChunkSize(int size)
{
	m_chunkSize = size;
	m_owner->setOptionsChanged();
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