#pragma once
#include <winnt.h>
#include <winerror.h>
#include <cstdio>

namespace L3D
{
	size_t GetPathDir(LPCWSTR wszFilePath, WCHAR* wszDir)
	{
		size_t uLength = wcslen(wszFilePath);
		for (; uLength > 0; uLength--)
		{
			if (wszFilePath[uLength] == '/')
			{
				wcsncpy_s(wszDir, FILENAME_MAX, wszFilePath, uLength + 1);
				return uLength + 1;
			}
		}
		return 0;
	}

	size_t GetFullPath(LPCWSTR wszFileName, WCHAR* wszPath)
	{
		size_t uLength = wcslen(wszFileName);
		for (size_t u = 0; u < uLength; u++)
		{
			if (wszFileName[u] == '.' || wszFileName[u] == '\\' || wszFileName[u] == '/')
				continue;
			wcscat_s(wszPath, FILENAME_MAX, &wszFileName[u]);
			return wcslen(wszPath);
		}
		return 0;
	}
}