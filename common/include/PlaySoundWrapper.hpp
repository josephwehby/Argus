#pragma once

#include <winsock2.h>
#include <windows.h>
#include <mmsystem.h>

inline void playWav(const char* file_name) {
  PlaySound(TEXT(file_name), NULL, SND_FILENAME | SND_ASYNC);
}
