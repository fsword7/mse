/*
 * list.h - list package
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#pragma once

template <typename T>
class List
{
public:
	List() { list.clear(); }
	~List() = default;

	inline void addNode(T *node) { list.push_back(node); }

	inline T *getNode(int index) { return index < list.size() ? list[index] : nullptr; }
	inline T *getFirst() { cidx = 0; return list[cidx++]; }
	inline T *getNext() { return cidx < list.size() ? list[cidx++] : nullptr; }

	inline void reset() { cidx = 0; }

	vector<T *> getList() { return list; }

private:
	vector<T *> list;
	int cidx = 0;
};
