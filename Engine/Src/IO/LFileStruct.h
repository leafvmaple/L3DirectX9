#pragma once
#define defFILE_TAGLEN			8
#define defFILE_DESCLEN			64

#define MESH_SAVE_BONEBBOX  1
#define MAP_FILE_VERSION_PACKREGION 2

struct TFileHeader
{
	char szCommonTag[defFILE_TAGLEN];
	char szModuleTag[defFILE_TAGLEN];
	__int32 byVersion;
	char szDescription[defFILE_DESCLEN];
};
