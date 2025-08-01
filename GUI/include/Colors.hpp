#pragma once

#include "imgui.h"

namespace Colors {
  
  constexpr ImU32 Green_U32 = IM_COL32(102,255,153,255);
  constexpr ImU32 Red_U32 = IM_COL32(255, 102, 102, 255);
  constexpr ImU32 White_U32 = IM_COL32(255,255,255,255);
  constexpr ImU32 Red_Light_U32 = IM_COL32(255, 64, 64, 90);
  constexpr ImU32 Green_Light_U32  = IM_COL32(51, 204, 102, 90);
  constexpr ImU32 Red_Bar_U32 = IM_COL32(255, 64, 64, 153);
  constexpr ImU32 Green_Bar_U32  = IM_COL32(51, 204, 102, 153);

  constexpr ImVec4 Background_V4  = ImVec4{40/255.f, 42/255.f, 54/255.f, 1.0f};
  constexpr ImVec4 Green_V4  = ImVec4{102/255.f, 1.f, 153/255.f, 1.f};
  constexpr ImVec4 Red_V4 = ImVec4{1.f, 102/255.f, 102/255.f, 1.f};
  constexpr ImVec4 Purple_V4 = ImVec4{0.7f, 0.6f, 0.8f, 0.7f};
  constexpr ImVec4 Blue_V4 = ImVec4{0.2f, 0.4f, 1.0f, 1.f};
  constexpr ImVec4 Yellow_V4 = ImVec4{1.0f, 0.85f, 0.25f, 1.0f};

  constexpr ImVec4 Green_Light_V4  = ImVec4{102/255.f, 1.f, 153/255.f, 0.5f};
  constexpr ImVec4 Red_Light_V4 = ImVec4{1.f, 102/255.f, 102/255.f, 0.5f};
};
