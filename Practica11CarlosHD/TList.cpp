#include "stdafx.h"
#include "TList.h"

#define listData static_cast<TListData *>(m_data)


struct TListElement
{
	const char * m_content;
	TListElement * m_nextElement;
};
/*********************/


struct TListData
{
	int m_size;
	TListElement * m_first;
	TListElement * m_last;
	TListElement * m_current;
};
/*********************/



// Default constructor
TList::TList()
{
	m_data = new TListData();
	listData->m_size = 0;
	listData->m_first = nullptr;
	listData->m_last = nullptr;
	listData->m_current = nullptr;
}
/*********************/


// Returns the number of elements
int TList::Size()
{
	return listData->m_size;
}
/*********************/


// Adds a string to the list
int TList::Push(const char * psz)
{
	if (psz) // TList cannot accept null as content of elements because Next returns null if the end has been reached.
	{
		//Common actions: creation of new element.
		TListElement * element = new TListElement();
		char * elementContent = nullptr;
		unsigned int strSize = strlen(psz) + 1;
		elementContent = new char[strSize];
		strcpy_s(elementContent, strSize, psz);

		element->m_content = elementContent;
		element->m_nextElement = nullptr;

		if (listData->m_first)
		{
			// This is not the first element. Last element must be adjusted.
			listData->m_last->m_nextElement = element;
		}
		else
		{
			// This is the first element
			listData->m_first = element;
			listData->m_current = element;
		}

		//Common actions: set new element as last and increase list size.
		listData->m_last = element;
		listData->m_size++;
	}

	return listData->m_size;
}
/*********************/


// Return the first element of the list
const char * TList::First()
{
	listData->m_current = listData->m_first;

	if (listData->m_first)
		return listData->m_first->m_content;
	else
		return nullptr;
}
/*********************/


// Returns the next element of the list
const char * TList::Next()
{
	if (listData->m_current)
	{
		// Set next as current and return its value if it's not null
		listData->m_current = listData->m_current->m_nextElement;
		if (listData->m_current)
			return listData->m_current->m_content;
		else
			return nullptr;
	}
	else
		return nullptr;
}
/*********************/


/** Returns the first element of the list and removes it from the list.
- User is responsible to release the memory of the returned buffer.

@return Buffer containing the string that was in the first place of the list.
*/
const char * TList::Pop()
{
	char * firstElement = nullptr;
	if (listData->m_first)
	{
		unsigned int strSize = strlen(listData->m_first->m_content) + 1;
		firstElement = new char[strSize];
		strcpy_s(firstElement, strSize, listData->m_first->m_content);

		// Set the new first element and remove the old one.
		TListElement * oldFirst = listData->m_first;

		if (listData->m_current == listData->m_first)
			listData->m_current = listData->m_first->m_nextElement;

		if (listData->m_last == listData->m_first)
			listData->m_last = listData->m_first->m_nextElement;

		listData->m_first = listData->m_first->m_nextElement;


		delete[]oldFirst->m_content;
		delete oldFirst;

		listData->m_size--;
	}
	return const_cast<const char *>(firstElement);
}
/*********************/


// Removes all elements from the list
void TList::Reset()
{
	while (listData->m_first)
	{
		TListElement * oldFirst = listData->m_first;
		listData->m_first = listData->m_first->m_nextElement;
		delete[]oldFirst->m_content;
		delete oldFirst;
		listData->m_size--;
	}

	listData->m_current = nullptr;
	listData->m_last = nullptr;
}
/*********************/


// Destructor
TList::~TList()
{
#ifdef _DEBUG
	printf("\n----- Destructor executing -----\n");
#endif
	Reset();
	delete m_data;

#ifdef _DEBUG
	printf("\n----- Destructor executed -----\n");
#endif
}
/*********************/
