// Practica11CarlosHD.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IFileUtils.h"


int main()
{
	TList list;
	int integersRead = 0;
	const char * strTemp = nullptr;

	IFile::FileIdentifier * file = IFile::OpenFile("integers.txt", IFile::FileOpenMode::ReadOnly);

	if (file)
	{
		integersRead = IFileUtils::GetCommaSeparatedIntegersList(file, list);

		if (integersRead)
		{
			if (list.Size())
			{
				// Iterating through elements (integers)
				printf("\n\nIntegers obtained:\n");
				strTemp = list.First();
				while (strTemp)
				{
					printf("\nNext integer: %s", strTemp);
					strTemp = list.Next();
				}
			}
			else
				printf("\n\nList is empty.");

		}
		else
			printf("\n\n0 integers obtained");
	}
	else
		printf("\n\nFile couldn't be opened");

	printf("\n\nPress Enter to quit...");
	getchar();

	return 0;
}

