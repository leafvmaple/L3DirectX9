#pragma once

#ifdef LENGINE_EXPORTS
#define L3DENGINE_API extern "C" __declspec(dllexport)
#define L3DENGINE_CLASS __declspec(dllexport)
#else
#define L3DENGINE_API extern "C" __declspec(dllimport)
#define L3DENGINE_CLASS __declspec(dllimport)
#endif