/*
 * list.h - list package
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#pragma once

template <typename node>
class List
{
public:
	List() { list.clear(); }
	~List() = default;

	inline node *getNode(int index) { return index < list.size() ? list[index] : nullptr; }
	inline node *getFirst() { cidx = 0; return list[cidx++]; }
	inline node *getNext() { return cidx < list.size() ? list[cidx++] : nullptr; }

	inline void reset() { cidx = 0; }

private:
	vector<node *> list;
	int cidx = 0;
};
