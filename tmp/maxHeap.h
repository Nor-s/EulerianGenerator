#ifndef MAXHEAP_H_
#define MAXHEAP_H_

#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <iterator>

namespace nlib
{
	template <class T1, class T2>
	class maxHeap
	{
	private:
		std::vector<std::pair<T1, T2> > heap;
		std::map<T2, int> keyPos;
	public:
		maxHeap()
		{
			heap = std::vector<std::pair<T1, T2> >();
			keyPos = std::map<T2, int>();
		}
		void swapPos(int a, int b);
		void push(std::pair<T1, T2> item);
		void pop();
		std::pair<T1, T2> top();
		bool update(std::pair<T1, T2> item);
		int size();
		void erase(T2 key);
		int find(T2 key);
		bool empty();
		int adjustDown(int idx);
		int adjustUp(int idx);
	};
}
namespace nlib
{
	template <class T1, class T2>
	void maxHeap<T1, T2>::swapPos(int a, int b)
	{
		int aPos = keyPos[heap[a].second];
		int bPos = keyPos[heap[b].second];
		keyPos[heap[a].second] = bPos;
		keyPos[heap[b].second] = aPos;
		std::pair<T1, T2> tmp = heap[a];
		heap[a] = heap[b];
		heap[b] = tmp;
	}
	template <class T1, class T2>
	int maxHeap<T1, T2>::adjustDown(int parent)
	{
		int size = this->size();
		while (true) {
			if (parent * 2 + 1 >= size) break;
			int next = parent;

			if (heap[parent].first < heap[parent * 2 + 1].first)
				next = parent * 2 + 1;
			if (parent * 2 + 2 < size && heap[next].first < heap[parent * 2 + 2].first)
				next = parent * 2 + 2;
			if (next == parent) break;
			swapPos(parent, next);
			parent = next;
		}
		return parent;
	}
	template <class T1, class T2>
	int maxHeap<T1, T2>::adjustUp(int child)
	{
		while (heap[(child - 1) / 2].first < heap[child].first)
		{
			swapPos((child - 1) / 2, child);
			child = (child - 1) / 2;
		}
		return child;
	}
	template <class T1, class T2>
	void maxHeap<T1, T2>::push(std::pair<T1, T2> item)
	{
		if (keyPos.count(item.second) == 0)
		{
			heap.push_back(item);
			keyPos[item.second] = size() - 1;
			if (size() != 1)
				adjustUp(size() - 1);
		}
	}

	template <class T1, class T2>
	void maxHeap<T1, T2>::pop()
	{
	//	std::cout << " size: " << size() << "\n";
		if (size() == 0)
		{
			std::cout << "empty\n";
			return;
		}
		swapPos(0, size() - 1);
		
		keyPos.erase(heap.back().second);
		heap.pop_back();
		if (size() == 0)
			keyPos.clear();

	//	std::cout<<" pos: "<<adjustDown(0);
		//std::cout << " adjust: last: " << keyPos[last.second] << " " << last.second << "\n";
	//	std::cout << " size: "<< size() << "\n";
	}
	template <class T1, class T2>
	void maxHeap<T1, T2>::erase(T2 key)
	{
		int pos = find(key);
		swapPos(pos, size() - 1);
		heap.pop_back();
		adjustUp(adjustDown(pos));
	}
	template <class T1, class T2>
	std::pair<T1, T2> maxHeap<T1, T2>::top()
	{
		if (size() == 0)
		{
			std::cout << "empty\n";
		}
		return heap[0];
	}
	template <class T1, class T2>
	bool maxHeap<T1, T2>::update(std::pair<T1, T2> item)
	{
		int pos = find(item.second);
		if (pos == -1) return false;
	//	std::cout <<"----update: "<< pos <<" size: "<<size()<<" item:  "<<item.second<<"\n";
		heap[pos] = item;
		adjustUp(adjustDown(pos));
		return true;
	}
	template <class T1, class T2>
	int maxHeap<T1, T2>::size()
	{
		return (int)heap.size();
	}
	template <class T1, class T2>
	int maxHeap<T1, T2>::find(T2 key)
	{
		if (keyPos.count(key) == 0|| size()==0)
			return -1;
		return keyPos[key];
	}
	template <class T1, class T2>
	bool maxHeap<T1, T2>::empty()
	{
		return size() == 0;
	}

}
#endif