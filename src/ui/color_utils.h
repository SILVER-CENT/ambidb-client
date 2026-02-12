#pragma once

#include "imgui.h"

#include <cstdint>

namespace ambidb::ui {

	// -------------------------------------------------------------------------
	// ANSI-256 color snapping
	// -------------------------------------------------------------------------
	// Mirrors the quantization logic in imtui-impl-text.cpp (rgbToAnsi256).
	// By pre-snapping our palette colors to values that survive the 6x6x6 color
	// cube (indices 16-231) or the greyscale ramp (232-255), we guarantee the
	// terminal renders the exact ANSI color we intended.

	/// Map an 8-bit R,G,B triple to the nearest ANSI-256 index.
	uint8_t
	RgbToAnsi256 (uint8_t r, uint8_t g, uint8_t b);

	/// Map an ImVec4 (0..1 floats, alpha ignored) to the nearest ANSI-256 index.
	uint8_t
	ColorToAnsi256 (const ImVec4& color);

	/// Return the canonical RGB for a given ANSI-256 index.
	/// This is the "representative center" of each cube cell / grey step,
	/// so round-tripping through the terminal yields the same index back.
	ImVec4
	Ansi256ToColor (uint8_t index, float alpha = 1.0f);

	/// Snap an ImVec4 color to its nearest ANSI-256 representative.
	/// Use this to build TUI-safe palettes: the RGB value you set in ImGui
	/// will land on the exact ANSI color you expect.
	ImVec4
	SnapToAnsi256 (const ImVec4& color);

	// -------------------------------------------------------------------------
	// Contrast helpers
	// -------------------------------------------------------------------------

	/// Approximate relative luminance (rec. 709).
	float
	Luminance (const ImVec4& color);

	/// WCAG-ish contrast ratio between two colors (1..21).
	float
	ContrastRatio (const ImVec4& a, const ImVec4& b);

	/// Returns true if the contrast ratio is >= minRatio (default 4.5 = AA).
	bool
	HasSufficientContrast (const ImVec4& fg, const ImVec4& bg, float minRatio = 4.5f);

	// -------------------------------------------------------------------------
	// Color manipulation
	// -------------------------------------------------------------------------

	/// Scale RGB channels by `factor` and clamp to [0,1]. Alpha unchanged.
	ImVec4
	ScaleRGB (const ImVec4& color, float factor);

	/// Linearly interpolate between two colors (including alpha).
	ImVec4
	LerpColor (const ImVec4& a, const ImVec4& b, float t);

	/// Build an ImVec4 from 0-255 integer components.
	inline ImVec4
	RGBA (int r, int g, int b, int a = 255) {
		return {
			static_cast<float> (r) / 255.0f,
			static_cast<float> (g) / 255.0f,
			static_cast<float> (b) / 255.0f,
			static_cast<float> (a) / 255.0f,
		};
	}

	/// Transparent black — lets the terminal's native background show through
	/// in TUI mode when imtui sees alpha == 0.
	inline ImVec4
	Transparent () {
		return {0.0f, 0.0f, 0.0f, 0.0f};
	}

}  // namespace ambidb::ui
