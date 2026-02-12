#include "forms.h"

namespace ambidb::ui {

	namespace {

		void
		MaybeSetItemWidth (float width) {
			if (width > 0.0f) {
				ImGui::SetNextItemWidth (width);
			}
		}

	}  // namespace

	bool
	DragFloatField (const char* label,
					float* value,
					float speed,
					float min,
					float max,
					const char* format,
					ImGuiSliderFlags flags,
					float width) {
		MaybeSetItemWidth (width);
		return ImGui::DragFloat (label, value, speed, min, max, format, flags);
	}

	bool
	SliderFloatField (const char* label, float* value, float min, float max, const char* format, ImGuiSliderFlags flags, float width) {
		MaybeSetItemWidth (width);
		return ImGui::SliderFloat (label, value, min, max, format, flags);
	}

	bool
	InputIntField (const char* label, int* value, int step, int stepFast, ImGuiInputTextFlags flags, float width) {
		MaybeSetItemWidth (width);
		return ImGui::InputInt (label, value, step, stepFast, flags);
	}

	bool
	InputFloatField (const char* label,
					 float* value,
					 float step,
					 float stepFast,
					 const char* format,
					 ImGuiInputTextFlags flags,
					 float width) {
		MaybeSetItemWidth (width);
		return ImGui::InputFloat (label, value, step, stepFast, format, flags);
	}

	bool
	InputTextField (const char* label, char* buffer, size_t bufferSize, ImGuiInputTextFlags flags, float width) {
		MaybeSetItemWidth (width);
		return ImGui::InputText (label, buffer, bufferSize, flags);
	}

	bool
	InputTextWithHintField (const char* label,
							const char* hint,
							char* buffer,
							size_t bufferSize,
							ImGuiInputTextFlags flags,
							float width) {
		MaybeSetItemWidth (width);
		return ImGui::InputTextWithHint (label, hint, buffer, bufferSize, flags);
	}

	void
	ProgressBarLabeled (float fraction, const char* overlay, float width) {
		const ImVec2 size = (width > 0.0f) ? ImVec2 (width, 0.0f) : ImVec2 (0.0f, 0.0f);
		ImGui::ProgressBar (fraction, size, overlay);
	}

}  // namespace ambidb::ui
