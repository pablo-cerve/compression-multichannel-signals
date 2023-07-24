#ifndef __DYNARRAY_CPP
#define __DYNARRAY_CPP

#include "../../stdafx.h"
#include "DynArray.h"

template<class Entry>
DynArray<Entry>::DynArray(void)
{
	max_length = MAX_LENGTH;
	length = 0;
	values = new Entry[max_length];
}

template<class Entry>
DynArray<Entry>::DynArray(DynArray<Entry> &original)
{
	max_length = MAX_LENGTH;
	length = 0;
	values = new Entry[max_length];

	// Copy original values to current array
	for(int i = 0; i < original.size(); i++)
	{
		this->add(original.getAt(i));
	}
}

template<class Entry>
DynArray<Entry>::~DynArray(void)
{
	if (values != NULL)
	{
		delete[] values;
		values = NULL;
	}	
}

template<class Entry>
void DynArray<Entry>::add(Entry value)
{
	//expand when the array is full
	if (length == max_length - 1)
	{
		expandSize();
	}
	values[length++]= value;
}

template<class Entry>
void DynArray<Entry>::remove()
{
	assert(length > 0);
	length--;
}

template<class Entry>
void DynArray<Entry>::updateAt(int position, Entry newValue)	
{
	assert(position >= 0 && position < length);
	values[position]= newValue;
}

template<class Entry>
void DynArray<Entry>::replace(int position,Entry value)
{
	assert(position >= 0 && position < length);
	values[position]= value;
}

template<class Entry>
void DynArray<Entry>::clear()
{
	length = 0;
}

//reduce current array by new array with new size
template<class Entry>
void DynArray<Entry>::reduceSize(int newSize)
{
	Entry* temp_values = new Entry[newSize];
	length = newSize;

	//copy values
	for(int i = 0; i < newSize; i++)
	{
		temp_values[i] = values[i];
	}

	delete[] values;
	values = temp_values;
}


// double the size of current array
template<class Entry>
void DynArray<Entry>::expandSize()
{
	// set new size
	max_length = max_length * 2;
	Entry* temp_values = new Entry[max_length];

	//copy values
	for(int i = 0; i < length; i++)
	{
		temp_values[i] = values[i];
	}

	delete[] values;
	values = temp_values;
}

template<class Entry>
Entry DynArray<Entry>::getAt(int position)	
{
	assert(position < length && position >= 0);
	return values[position];
}

template<class Entry>
bool DynArray<Entry>::contains(Entry value)	
{
	for(int i = 0; i < length; i++)
	{
		if(values[i] == value)
		{
			return true;
		}
	}
	return false;
}

template<class Entry>
int DynArray<Entry>::size()	
{
	return length;
}

#endif 