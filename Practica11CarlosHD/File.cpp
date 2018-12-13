#include "stdafx.h"
#include "IFile.h"
#include <stdio.h>
#include <assert.h>


/************/
const unsigned int SIZEOF_CHAR = sizeof(char);
/************/


/**
Opens a file

@param fileName Name of the file
@param openMode The enum value of the open type

@return The new file identifier
*/
IFile::FileIdentifier * IFile::OpenFile(const char * fileName, IFile::FileOpenMode openMode)
{
	assert(fileName);

	IFile::FileIdentifier * fileId = new IFile::FileIdentifier;
	char stdioOpenMode[2] = "r";
	switch (openMode)
	{
	case IFile::FileOpenMode::ReadOnly:
		stdioOpenMode[0] = 'r';
		break;
	case IFile::FileOpenMode::ReadWrite:
		stdioOpenMode[0] = 'w';
		break;
	}
	FILE * fopenFile = fopen(fileName, stdioOpenMode);

	(*fileId).content = fopenFile;

	return fileId;
}


/**
Closes a file

@param fileId Identifier of a previously opened file

@return The result of the operation (0 if properly closed)
*/
int IFile::CloseFile(IFile::FileIdentifier * fileId)
{
	assert(fileId);
	assert(fileId->content);

	int result;

	result = fclose(static_cast<FILE *>(fileId->content));

	delete fileId;

	return result;
}


/**
Reads a number of characters in a file and stores them in an output buffer

@param outputBuffer Buffer where the read characters will be stored
@param fileId Identifier of the file to read frrom
@charactersToRead Number of characters to read

@return The number of characters read
*/
unsigned int IFile::ReadFile(char * outputBuffer, const IFile::FileIdentifier * fileId, unsigned int charactersToRead)
{
	assert(fileId);
	assert(fileId->content);
	assert(outputBuffer);

	return fread(outputBuffer, SIZEOF_CHAR, charactersToRead, static_cast<FILE *>(fileId->content));
}


/**
Writes the characters contained in an input buffer to the file passed as parameter.

@param inputBuffer which contains the characters to write
@param fileId Identifier of the file to write to
@param inputLength Length of the input buffer

@return The number of characters written
*/
unsigned int IFile::WriteFile(const char * inputBuffer, const IFile::FileIdentifier * fileId, unsigned int inputLength)
{
	assert(fileId);
	assert(fileId->content);
	assert(inputBuffer);

	return fwrite(inputBuffer, SIZEOF_CHAR, inputLength, static_cast<FILE *>(fileId->content));
}
