#include "stdafx.h"
#include <stdio.h>
#include "IFileUtils.h"
#include <assert.h>
#include <stdlib.h>
#include "TList.h"



const char COMMA_SEPARATOR = ',';

/********************/


unsigned int CountStringAppearancesInternal(const char * strToSearch, const char * buffer, unsigned int bufferSize);

void SumCharSeparatedIntegersInternal(int &iSum, const char * buffer, unsigned int bufferSize, const char separator);

/********************/


/**
Returns the number of appearances of a string inside a buffer.

@param strToSearch The string to search
@param buffer The buffer to search in
@param bufferSize Length of the buffer

@return The number of appearances of the string in the buffer
*/
unsigned int CountStringAppearancesInternal(const char * strToSearch, const char * buffer, unsigned int bufferSize)
{
	assert(strToSearch);
	assert(buffer);

	unsigned int iAppearances = 0;
	unsigned int strToSearchLength = strlen(strToSearch);

	// The string can only be contained in the container if its length is equal or less than the container's length
	if (strToSearchLength <= bufferSize)
	{
		const char * strToSearchStart = strToSearch;
		const char * strToSearchEnd = (strToSearch + strToSearchLength - 1);
		const char * bufferEnd = (buffer + bufferSize - 1);

		while (buffer++ <= bufferEnd)
		{
			if ((*buffer) == (*strToSearch)) // Char matches the string char
			{
				if (strToSearch == strToSearchEnd) // Reached the end of the string: appearance found
					iAppearances++;
				else // Check next char
					strToSearch++;
			}
			else // Char doesn't match: try again since the beginning of the string
				strToSearch = strToSearchStart;
		}
	}

	return iAppearances;
}


/**
Sums the comma separated integers inside the buffer plus the reference passed as parameter, which stores the final result in the end.

@param iSum Reference to the sum of all the integers
@param buffer The buffer which contains the integers
@param bufferSize Length of the buffer
*/
void SumCharSeparatedIntegersInternal(int &iSum, const char * buffer, unsigned int bufferSize, const char separator)
{
	assert(buffer);

	const char * bufferEnd = (buffer + bufferSize - 1);
	char * iTemp = nullptr;
	const char * pInt = buffer;
	unsigned int numberSize = 0;

	while (buffer <= bufferEnd)
	{
		if (pInt)
		{
			if (separator == (*buffer) || (buffer == bufferEnd)) // End of the int or end of the buffer
			{
				if (separator != (*buffer)
					&& ('-' != (*buffer)) && ((' ' == (*buffer)) || ((*buffer) >= '0' && (*buffer) <= '9')))
				{
					// If bufferEnd is valid int character and not the separator, this last character must be considered, incrementing the buffer pointer
					buffer++;
				}

				if ((buffer - pInt) && '-' != *(buffer - 1)) // Avoiding treating two consecutive commas or a single '-' as an integer
				{
					// End of the int
					numberSize = static_cast<unsigned int>(buffer - pInt) + 1;
					iTemp = new char[numberSize];

					for (unsigned int i = 0; i < (numberSize - 1); i++)
						iTemp[i] = *(pInt + i);

					iTemp[numberSize - 1] = '\0';
					iSum += atoi(iTemp);
					delete[]iTemp;
					iTemp = nullptr;
				}
				pInt = (buffer + 1);
			}
			else if (((pInt != buffer) && '-' == (*buffer))
				|| ('-' != (*buffer) && ((*buffer) < '0' || (*buffer) > '9'))) // Invalid int character
				pInt = nullptr;
		}
		else if (separator == (*buffer)) // Beginning a new int
			pInt = (buffer + 1);

		buffer++;
	}

}

/********************/


/**
Returns the number of appearances of a string inside a file.

@param strToSearch The string to search
@param fileId The identifier of the file to search in

@return The number of appearances of the string in the file
*/
unsigned int IFileUtils::CountStringAppearances(const char * strToSearch, const IFile::FileIdentifier * fileId)
{
	assert(strToSearch);
	assert(fileId);

	unsigned int iAppearances = 0;
	unsigned int bytesRead = 0;
	const unsigned int BUFFER_SIZE = 255;
	char * buffer = new char[BUFFER_SIZE];

	while (BUFFER_SIZE == (bytesRead = IFile::ReadFile(buffer, fileId, BUFFER_SIZE)))
	{
		iAppearances += CountStringAppearancesInternal(strToSearch, buffer, BUFFER_SIZE);
	}

	if (bytesRead)
	{
		// If some bytes were read, we check them too.
		iAppearances += CountStringAppearancesInternal(strToSearch, buffer, bytesRead);
	}

	delete[]buffer;

	return iAppearances;
}


/**
Returns the sum of a list of comma separated integers inside a file.

@param fileId The identifier of the file to do the operations

@return The sum of all the integers inside the file
*/
int IFileUtils::SumCommaSeparatedIntegers(const IFile::FileIdentifier * fileId)
{
	assert(fileId);

	int iSum = 0;

	unsigned int bytesRead = 0;
	const unsigned int BUFFER_SIZE = 255;
	char * buffer = new char[BUFFER_SIZE];

	while (BUFFER_SIZE == (bytesRead = IFile::ReadFile(buffer, fileId, BUFFER_SIZE)))
	{
		SumCharSeparatedIntegersInternal(iSum, buffer, BUFFER_SIZE, COMMA_SEPARATOR);
	}

	if (bytesRead)
	{
		// If some bytes were read, we check them too.
		SumCharSeparatedIntegersInternal(iSum, buffer, bytesRead, COMMA_SEPARATOR);
	}

	delete[]buffer;

	return iSum;
}


/**
Stores the tokens in a buffer separated by a delimiter string
*/
int StoreTokensInList(char * buffer, TList& list, const char * delimiter)
{
	int tokenCount = 0;
	char * token = nullptr;
	char * nextToken = nullptr;

	token = strtok_s(buffer, delimiter, &nextToken);
	while (token)
	{
		list.Push(token);
		token = strtok_s(NULL, delimiter, &nextToken);
		tokenCount++;
	}

	return tokenCount;
}

/**
Fills a list with the strings of the comma separated integers in a file.

@param fileId File containing the integers
@param out List where the integer strings will be stored. If it was already filled, it will be empty first

@return Number of integers set in the list.
*/
int IFileUtils::GetCommaSeparatedIntegersList(const IFile::FileIdentifier * fileId, TList& out)
{
	assert(fileId);

	int result = 0;
	unsigned int bytesRead = 0;
	char delimiter[2] = { COMMA_SEPARATOR, '\0' };
	const unsigned int BUFFER_SIZE = 255;
	char * buffer = new char[BUFFER_SIZE + 1];
	 
	// If the list to return already had elements, they are removed.
	if (out.Size())
		out.Reset();

	while (BUFFER_SIZE == (bytesRead = IFile::ReadFile(buffer, fileId, BUFFER_SIZE)))
	{
		// Buffer must end with '\0' to make strtok work properly
		buffer[BUFFER_SIZE] = '\0';
		result += StoreTokensInList(buffer, out, delimiter);
	}

	if (bytesRead)
	{
		// If some bytes were read, we check them too.
		// Buffer must end with '\0' to make strtok work properly
		buffer[bytesRead] = '\0';
		result += StoreTokensInList(buffer, out, delimiter);
	}

	delete[]buffer;
	
	return result;
}