#pragma once


class TList
{

public:
	TList();

	int Size();
	int Push(const char * psz);
	const char * First();
	const char * Next();
	const char * Pop();
	void Reset();

	~TList();

private:
	void *m_data;
};