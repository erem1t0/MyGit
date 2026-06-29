#include <iostream>		// For std::cout, std::cin
#include <vector>		// For std::vector
#include <cmath>		// For std::log2
#include <algorithm>	// For std::swap


// TreeNode
template<typename T>
class Node
{
private:

	Node<T>* parent;
	Node<T>* child;

	Node<T>* right;
	Node<T>* left;

	int degree; // count of childs
	bool mark; // *

	T data;

public:

	// Basic constructors
	Node<T>()
		: parent(nullptr)
		, child(nullptr)
		, right(nullptr)
		, left(nullptr)
		, degree(0)
		, mark(false)
		, data()
	{ }


	Node<T>(const T& data) : Node<T>()
	{
		Node::data = data;
	}


	Node<T>(Node<T>* n)
	{
		parent = n->getParent();
		child = n->getChild();
		left = n->getLeft();
		right = n->getRight();
		degree = n->getDegree();
		mark = n->getMark();
		data = n->getData();
	}

	// Operator =
	Node<T>* operator=(Node<T>* n)
	{
		parent = n->getParent();
		child = n->getChild();
		left = n->getLeft();
		right = n->getRight();
		degree = n->getDegree();
		mark = n->getMark();
		data = n->getData();

		return *this;
	}

	// Getters
	T getData()	const
	{
		return data;
	}


	Node<T>* getParent()	const
	{
		return parent;
	}


	Node<T>* getChild()	const
	{
		return child;
	}


	Node<T>* getRight()	const
	{
		return right;
	}


	Node<T>* getLeft()	const
	{
		return left;
	}


	bool getMark()	const
	{
		return mark;
	}


	int getDegree()	const
	{
		return degree;
	}

	// Setters
	void setData(const T& data)
	{
		Node::data = data;
	}


	void setParent(Node<T>* parent)
	{
		Node::parent = parent;
	}


	void setLeft(Node<T>* left)
	{
		Node::left = left;
	}


	void setRight(Node<T>* right)
	{
		Node::right = right;
	}


	void setChild(Node<T>* child)
	{
		Node::child = child;
	}


	void setMark(bool mark)
	{
		Node::mark = mark;
	}


	void setDegree(int degree)
	{
		Node::degree = degree;
	}

	// Remove Node from the list(index = 0 mean full-cut)
	inline void removeFromList(int index = 0)
	{
		this->getLeft()->setRight(this->getRight());
		this->getRight()->setLeft(this->getLeft());

		if (!index)
		{
			setLeft(nullptr);
			setRight(nullptr);
		}
	}

	// Insert x to the list near y
	friend void insert(Node<T>* x, Node<T>* y)
	{
		x->setLeft(y);
		x->setRight(y->getRight());

		y->getRight()->setLeft(x);
		y->setRight(x);
	}
};



template<typename T>
class FibHeap
{
protected:

	// Minimum of the heap
	Node<T>* min;

	// Count of nodes
	int count;

	// Update minimum
	inline void unite(Node<T>* node)
	{
		if (min && node->getData() < min->getData())
			min = node;
	}

	// Help-link func
	Node<T>* _link(Node<T>* x, Node<T>* y)
	{
		if (!x || !y) throw std::invalid_argument("Nullptr");

		if (x->getData() < y->getData())
			std::swap(x, y);

		return link(x, y);
	}

	// Merge 2 trees
	Node<T>* link(Node<T>* child, Node<T>* parent)
	{
		child->setParent(parent);
		child->removeFromList(1);

		if (!parent->getChild())
		{
			parent->setChild(child);
			child->setLeft(child);
			child->setRight(child);
		}
		else
			insert(parent->getChild(()), child);

		parent->setDegree(parent->getDegree() + 1);

		child->setMark(false);

		return parent;
	}

	// Get the Root (help when merge)
	Node<T>* getPtr()	const
	{
		return min;
	}

	// Get the max possible degree of Nodes
	inline int maxDegree()	const
	{
		return (int)std::log2(count) + 1;
	}

	// Cleaning-sevice
	void consolidate()
	{
		int maxDeg = maxDegree();
		std::vector<Node<T>*> degrees(maxDeg, nullptr);

		Node<T>* curr = min;
		std::vector<Node<T>*> nodes;

		do
		{
			nodes.push_back(curr);
			curr = curr->getRight();
		} while (curr != min);

		int degree;
		for (Node<T>* it : nodes)
		{
			degree = it->getDegree();
			while (degrees[degree])
			{
				Node<T>* other = degrees[degree];
				it = _link(it, other);
				degrees[degree] = nullptr;
				++degree;
			}

			degrees[degree] = it;
		}

		min = nullptr;

		for (Node<T>* it : degrees)
		{
			if (it)
			{
				if (min)
				{
					it->removeFromList(1);
					insert(min, it);
				}
				else min = it;
			}
		}
	}

	// Clear all without min
	void _delete_all(Node<T>* node)
	{
		if (node)
		{
			Node<T>* curr = node;
			do
			{
				Node<T>* y = curr;
				curr = curr->getRight();

				_delete_all(y->getChild());
				delete y;
			} while (curr != node);
		}
	}

	// Clear the heap
	void delete_all(Node<T>* node)
	{
		_delete_all(node);
		delete min; min = nullptr;
		count = 0;
	}

	// Cut the child
	void cut(Node<T>* child, Node<T>* parent)
	{
		if (child->getLeft() == child)
			parent->setChild(nullptr);
		else
		{
			if (parent->getChild() == child)
				parent->setChild(child->getLeft());

			child->removeFromList(1);
		}

		parent->setDegree(parent->getDegree() - 1);

		insert(min, child);

		child->setParent(nullptr);
		child->setMark(false);

	}

	// Check mark
	void cascadingCut(Node<T>* node)
	{
		Node<T>* x = node->getParent();
		if (x)
		{
			if (!node->getMark())
				node->setMark(true);
			else
			{
				cut(node, x);
				cascadingCut(x);
			}
		}
	}

	// Help print
	void _print(Node<T>* x, int depth)	const
	{
		if (!x) return;

		Node<T>* curr = x;
		do
		{
			std::cout << "Level " << depth << ": " << curr->getData() << '\n';

			_print(curr->getChild(), depth + 1);

			curr = curr->getRight();
		} while (curr != x);
	}


private:

	// I don't want this
	FibHeap<T>(const FibHeap<T>& f);


	FibHeap<T>& operator=(const FibHeap<T>& f) { }


public:

	// Basic constructors
	FibHeap<T>()
		: min(nullptr)
		, count(0)
	{ }

	// Get the count of elements
	int size() const
	{
		return count;
	}

	// Merge 2 heaps and make 2nd heap zero
	FibHeap<T>& merge(FibHeap<T>& heap)
	{
		Node<T>* xL = min->getLeft();
		Node<T>* yL = heap->getPtr()->getLeft();

		xL->setRight(heap->getPtr());
		heap->getPtr()->setLeft(xL);

		yL->setRight(min);
		min->setLeft(yL);

		unite(heap->getPtr());

		count += heap->count;
		heap->min = nullptr;
		heap->count = 0;

		return *this;
	}

	// Get the minimum
	T getMin()	const
	{
		if (!min) throw std::invalid_argument("Heap is empty");

		return min->getData();
	}

	// Get the ptr to min
	const Node<T>* getMinPtr()	const
	{
		if (!min) throw std::invalid_argument("Heap is empty");

		return min;
	}

	// Add tree rank 0 
	Node<T>* add(const T& data)
	{
		Node<T>* newNode = new Node<T>(data);
		return add(newNode);
	}

	// Add tree any rank
	Node<T>* add(Node<T>* node)
	{
		if (!min)
		{
			min = node;
			node->setLeft(node);
			node->setRight(node);
		}
		else
		{
			insert(min, node);
			unite(node);
		}
		++count;

		return node;
	}

	// Remove min from the heap
	T extractMin()
	{
		if (!min) throw std::invalid_argument("heap is empty");

		Node<T>* minNode = min;

		// If he has childs
		if (minNode->getChild())
		{
			Node<T>* child = minNode->getChild();
			do
			{
				Node<T>* next = child->getRight();

				insert(min, child);
				unite(child);

				child->setParent(nullptr);
				child = next;
			} while (child != minNode->getChild());
		}

		minNode->removeFromList(1);

		if (minNode == minNode->getRight())
			min = nullptr;
		else
		{
			min = minNode->getRight();
			consolidate();
		}

		int minValue = minNode->getData();
		delete minNode;

		--count;

		return minValue;
	}

	// Decrease
	void decreaseKey(Node<T>* node, const T& data)
	{
		if (data > node->getData()) return;

		node->setData(data);
		Node<T>* parent = node->getParent();

		if (parent && node->getData() < parent->getData())
		{
			cut(node, parent);
			cascadingCut(parent);
		}

		unite(node);
	}

	// Delete node
	void deleteNode(Node<T>* node)
	{
		decreaseKey(node, std::numeric_limits<T>::min());	// Have to has operator<
		extractMin();
	}

	// Is empty?
	bool empty() const
	{
		return (!min);
	}

	// Printing
	void print()	const
	{
		if (min)
		{
			std::cout << "Fibonacci Heap:\n";
			_print(min, 0);
		}
		else
			std::cout << "Fibonacci Heap is empty\n";
	}

	// Destructor
	~FibHeap<T>()
	{
		delete_all(min);
	}
};



int main()
{
	using namespace std;

	cout << "All is okay!\n";

	FibHeap<int> heap;
	cout << "add " << heap.add(10)->getData() << '\n';
	cout << "add " << heap.add(20)->getData() << '\n';
	cout << "add " << heap.add(30)->getData() << '\n' << '\n';

	heap.print();
	cout << '\n';

	int minVal = heap.extractMin();
	cout << "min is " << minVal << '\n';

	minVal = heap.extractMin();
	cout << "min is " << minVal << '\n';

	minVal = heap.extractMin();
	cout << "min is " << minVal << '\n';

	cout << "\nempty " << heap.empty() << '\n';

	Node<int>* n = new Node<int>(100);
	cout << "\nadd " << heap.add(n)->getData() << '\n';

	cout << "\nempty " << heap.empty() << '\n';

	cout << "\ndecrease 100 -> 90 \n";
	heap.decreaseKey(n, 90);

	cout << "\nmin is " << heap.getMin() << '\n';

	cout << "\ndelete 90\n";
	heap.deleteNode(n);

	cout << "\nempty " << heap.empty() << '\n';

	return 0;
}