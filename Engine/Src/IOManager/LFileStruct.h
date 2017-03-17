#pragma once
#define defFILE_TAGLEN			8
#define defFILE_DESCLEN			64

#define MESH_SAVE_BONEBBOX  1

struct TFileHeader
{
	char szCommonTag[defFILE_TAGLEN];
	char szModuleTag[defFILE_TAGLEN];
	__int32 byVersion;
	char szDescription[defFILE_DESCLEN];
};