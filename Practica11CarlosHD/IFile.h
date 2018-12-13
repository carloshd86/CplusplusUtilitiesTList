#pragma once

namespace IFile
{
	typedef struct _fileId
	{
		void *content;
	} FileIdentifier;

	enum FileOpenMode
	{
		ReadOnly,
		ReadWrite,
	};

	FileIdentifier * OpenFile(const char * fileName, FileOpenMode openMode);
	int CloseFile(FileIdentifier * fileId);
	unsigned int ReadFile(char * outputBuffer, const FileIdentifier * fileId, unsigned int charactersToRead);
	unsigned int WriteFile(const char * inputBuffer, const FileIdentifier * fileId, unsigned int inputLength);

}