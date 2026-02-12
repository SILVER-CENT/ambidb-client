#include "color_utils.h"

#include <algorithm>
#include <cmath>

namespace ambidb::ui {

	namespace {

		// The 6x6x6 color cube levels used by xterm-256.
		constexpr int kCubeLevels [6] = {0, 0x5f, 0x87, 0xaf, 0xd7, 0xff};

		int
		NearestCubeIndex (uint8_t value) {
			int best = 0;
			int bestDist = 256;
			for (int i = 0; i < 6; ++i) {
				const int d = std::abs (static_cast<int> (value) - kCubeLevels [i]);
				if (d < bestDist) {
					bestDist = d;
					best = i;
				}
			}
			return best;
		}

	}  // namespace

	uint8_t
	RgbToAnsi256 (uint8_t r, uint8_t g, uint8_t b) {
		// Greyscale shortcut: if all channels are equal (or very close)
		if (r == g && g == b) {
			if (r < 8) return 16;
			if (r > 248) return 231;
			return static_cast<uint8_t> (
				std::round ((static_cast<float> (r) - 8.0f) / 247.0f * 24.0f) + 232);
		}

		const int ri = NearestCubeIndex (r);
		const int gi = NearestCubeIndex (g);
		const int bi = NearestCubeIndex (b);
		return static_cast<uint8_t> (16 + 36 * ri + 6 * gi + bi);
	}

	uint8_t
	ColorToAnsi256 (const ImVec4& color) {
		const auto r = static_cast<uint8_t> (std::clamp (color.x, 0.0f, 1.0f) * 255.0f + 0.5f);
		const auto g = static_cast<uint8_t> (std::clamp (color.y, 0.0f, 1.0f) * 255.0f + 0.5f);
		const auto b = static_cast<uint8_t> (std::clamp (color.z, 0.0f, 1.0f) * 255.0f + 0.5f);
		return RgbToAnsi256 (r, g, b);
	}

	ImVec4
	Ansi256ToColor (uint8_t index, float alpha) {
		// Standard 16 colors (0-15): map to well-known values
		// We only care about 16-255 for theming; for 0-15 return approximations.
		if (index < 16) {
			// Basic ANSI colors — rough sRGB approximations
			static const uint8_t basic [16][3] = {
				{0x00, 0x00, 0x00},
				{0x80, 0x00, 0x00},
				{0x00, 0x80, 0x00},
				{0x80, 0x80, 0x00},
				{0x00, 0x00, 0x80},
				{0x80, 0x00, 0x80},
				{0x00, 0x80, 0x80},
				{0xc0, 0xc0, 0xc0},
				{0x80, 0x80, 0x80},
				{0xff, 0x00, 0x00},
				{0x00, 0xff, 0x00},
				{0xff, 0xff, 0x00},
				{0x00, 0x00, 0xff},
				{0xff, 0x00, 0xff},
				{0x00, 0xff, 0xff},
				{0xff, 0xff, 0xff},
			};
			return {static_cast<float> (basic [index][0]) / 255.0f,
					static_cast<float> (basic [index][1]) / 255.0f,
					static_cast<float> (basic [index][2]) / 255.0f,
					alpha};
		}

		// 6x6x6 color cube (16-231)
		if (index < 232) {
			const int idx = index - 16;
			const int ri = idx / 36;
			const int gi = (idx % 36) / 6;
			const int bi = idx % 6;
			return {static_cast<float> (kCubeLevels [ri]) / 255.0f,
					static_cast<float> (kCubeLevels [gi]) / 255.0f,
					static_cast<float> (kCubeLevels [bi]) / 255.0f,
					alpha};
		}

		// Greyscale ramp (232-255)
		const float grey = (8.0f + static_cast<float> (index - 232) * 10.0f) / 255.0f;
		return {grey, grey, grey, alpha};
	}

	ImVec4
	SnapToAnsi256 (const ImVec4& color) {
		const uint8_t idx = ColorToAnsi256 (color);
		return Ansi256ToColor (idx, color.w);
	}

	float
	Luminance (const ImVec4& color) {
		return 0.2126f * color.x + 0.7152f * color.y + 0.0722f * color.z;
	}

	float
	ContrastRatio (const ImVec4& a, const ImVec4& b) {
		const float la = Luminance (a) + 0.05f;
		const float lb = Luminance (b) + 0.05f;
		return (la > lb) ? (la / lb) : (lb / la);
	}

	bool
	HasSufficientContrast (const ImVec4& fg, const ImVec4& bg, float minRatio) {
		return ContrastRatio (fg, bg) >= minRatio;
	}

	ImVec4
	ScaleRGB (const ImVec4& color, float factor) {
		return {
			std::clamp (color.x * factor, 0.0f, 1.0f),
			std::clamp (color.y * factor, 0.0f, 1.0f),
			std::clamp (color.z * factor, 0.0f, 1.0f),
			color.w,
		};
	}

	ImVec4
	LerpColor (const ImVec4& a, const ImVec4& b, float t) {
		const float s = 1.0f - t;
		return {
			s * a.x + t * b.x,
			s * a.y + t * b.y,
			s * a.z + t * b.z,
			s * a.w + t * b.w,
		};
	}

}  // namespace ambidb::ui
