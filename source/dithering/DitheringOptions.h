#pragma once
class Dithering;

enum class DitheringPalette
{
	Greyscale,
	Reduced,
	Full
};

class DitheringOptions
{
public:
	DitheringOptions(Dithering* owner);

	int chunkSize() const;
	void setChunkSize(int size);

	DitheringPalette palette() const;
	void setPalette(DitheringPalette p);

	int greyscalePalette() const;
	void setGreyscalePalette(int colors);

private:

	Dithering * const m_owner;

	int m_chunkSize;
	DitheringPalette m_palette;
	int m_greyscalePalette;
};