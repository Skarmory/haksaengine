#pragma once

#ifdef HAKSAENGINE_EXPORTS
#define HAKSAENGINE_API __declspec(dllexport)
#else
#define HAKSAENGINE_API __declspec(dllimport)
#endif