#pragma once

#include <cstddef>

#include "imgui.h"

namespace ambidb::ui {

	bool
	DragFloatField (const char* label,
					float* value,
					float speed = 1.0f,
					float min = 0.0f,
					float max = 0.0f,
					const char* format = "%.3f",
					ImGuiSliderFlags flags = 0,
					float width = 0.0f);

	bool
	SliderFloatField (const char* label,
					  float* value,
					  float min,
					  float max,
					  const char* format = "%.3f",
					  ImGuiSliderFlags flags = 0,
					  float width = 0.0f);

	bool
	InputIntField (const char* label,
				   int* value,
				   int step = 1,
				   int stepFast = 100,
				   ImGuiInputTextFlags flags = 0,
				   float width = 0.0f);

	bool
	InputFloatField (const char* label,
					 float* value,
					 float step = 0.0f,
					 float stepFast = 0.0f,
					 const char* format = "%.3f",
					 ImGuiInputTextFlags flags = 0,
					 float width = 0.0f);

	bool
	InputTextField (const char* label,
					char* buffer,
					size_t bufferSize,
					ImGuiInputTextFlags flags = 0,
					float width = 0.0f);

	bool
	InputTextWithHintField (const char* label,
							const char* hint,
							char* buffer,
							size_t bufferSize,
							ImGuiInputTextFlags flags = 0,
							float width = 0.0f);

	void
	ProgressBarLabeled (float fraction, const char* overlay = nullptr, float width = 0.0f);

}  // namespace ambidb::ui
