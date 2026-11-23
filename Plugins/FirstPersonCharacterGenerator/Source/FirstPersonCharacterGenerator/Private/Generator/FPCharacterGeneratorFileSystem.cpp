// Copyright (c) 2026 Douglas Mendes Carmona All Rights Reserved.

#include "Generator/FPCharacterGeneratorFileSystem.h"

bool UFPCharacterGeneratorFileSystem::SaveFile(const FString& FullFileName, const FString& FileContent)
{
	const FString Directory = FPaths::GetPath(FullFileName);

	IPlatformFile& PlatformFile =
		FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.CreateDirectoryTree(*Directory))
	{
		return false;
	}

	return FFileHelper::SaveStringToFile(FileContent, *FullFileName);
}
