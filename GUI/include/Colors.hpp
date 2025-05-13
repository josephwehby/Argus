#pragma once

#include "imgui.h"

namespace Colors {
  constexpr ImU32 Green_U32 = IM_COL32(102,255,153,255);
  constexpr ImU32 Red_U32 = IM_COL32(255, 102, 102, 255);

  constexpr ImVec4 Background_V4  = ImVec4{40/255.f, 42/255.f, 54/255.f, 1.0f};
  constexpr ImVec4 Green_V4  = ImVec4{102/255.f, 1.f, 153/255.f, 1.f};
  constexpr ImVec4 Red_V4 = ImVec4{1.f, 102/255.f, 102/255.f, 1.f};
  constexpr ImVec4 Purple_V4 = ImVec4{0.7f, 0.6f, 0.8f, 0.7f};
  constexpr ImVec4 Blue_V4 = ImVec4{0.2f, 0.4f, 1.0f, 0.1f};
};
