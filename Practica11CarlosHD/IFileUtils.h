#pragma once

#include "IFile.h"
#include "TList.h"

namespace IFileUtils
{
	unsigned int CountStringAppearances(const char * strToSearch, const IFile::FileIdentifier * fileId);
	int SumCommaSeparatedIntegers(const IFile::FileIdentifier * fileId);
	int GetCommaSeparatedIntegersList(const IFile::FileIdentifier * fileId, TList& out);
}