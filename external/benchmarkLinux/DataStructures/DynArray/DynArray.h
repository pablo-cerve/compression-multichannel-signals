#ifndef __DYNARRAY_H
#define __DYNARRAY_H

#define MAX_LENGTH 300; // PABLO: I changed it from 100 because I allow windows which have as much as 200 elements

template<class Entry>
class DynArray
{
protected:
	int length;
	Entry* values;
	int max_length;
	void expandSize(); //expand when the array is full
	
public:
	DynArray(void);
	DynArray(DynArray<Entry> &original);
	~DynArray(void);

	void add(Entry value);
	void remove();
	void updateAt(int position, Entry newValue);	
	void replace(int position, Entry value);	
	void clear();
	void reduceSize(int newSize);

	Entry getAt(int position);	
	bool contains(Entry value);	
	int size();
};

#endif