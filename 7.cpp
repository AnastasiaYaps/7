#include <iostream>
#include <string>
#include <exception>
#include<time.h>
#include <cstdlib>        

using namespace std;

template <class T>
class Stack;

class Game {
private:
	int id;
	string name;
	double cost;
public:
	Game() :id(0), name("Blank "), cost(0) { }
	Game(string t_name, double _cost, int id) :name(t_name), cost(_cost), id(id) { }
	~Game() { }
	bool operator< (const Game& co) { return id < co.id; }
	bool operator> (const Game& co) { return id > co.id; }
	bool operator== (const Game& co) { return id == co.id && name == co.name && cost == co.cost; }
	friend ostream& operator<< (ostream& out, const Game& c)
	{
		out << c.id << ' ' << c.name << ' ' << c.cost << '\n'; return out;
	}

};

class Exception
{
public:
	Exception(const string errorMessage = "Something wrong") : m_ErrorMessage(errorMessage) { }
	friend ostream& operator<< (ostream& out, const Exception& ex) { out << ex.m_ErrorMessage << '\n'; return out; }
private:
	string m_ErrorMessage;
};

template <class T>
struct Node {
	T value;
	Node* left, * right;
	Node() : value() { left = right = nullptr; }
};

template <class T>
class Iterator
{
public:
	friend class Stack<T>;
	Iterator(Node<T>* node = nullptr) : node(node) { }
	~Iterator() { }
	Iterator(const Iterator& it) { node = it.node; }
	Iterator& operator=(const Iterator& it) { node = it.node; return *this; }
	T& operator*() {
		if (node == nullptr)
			throw Exception("Empty iterator");
		return node->value;
	}
	T* operator->() { if (node == nullptr) throw Exception("Empty iterator"); return &node->value; }
	Iterator& operator++() { if (node == nullptr) throw Exception("Empty iterator"); node = node->right; return *this; }
	Iterator& operator--() { if (node == nullptr) throw Exception("Empty iterator"); node = node->left; return *this; }
	Iterator operator++(int unused) { if (node == nullptr) throw Exception("Empty iterator"); Iterator res = *this; node = node->right; return res; }
	Iterator operator--(int unused) { if (node == nullptr) throw Exception("Empty iterator"); Iterator res = *this; node = node->left; return res; }
	bool operator== (const Iterator& it) { return node == it.node; }
	bool operator!= (const Iterator& it) { return node != it.node; }
public:
	Node<T>* node;
};

template<class T>
class Stack
{
public:
	typedef Iterator<T> Iterator;
public:
	Stack() : head(nullptr) {}
	~Stack() {}
	void push(T value, bool unique = false)
	{
		if (unique) {
			Iterator it = find(value);
			if (it != null()) {
				throw Exception("duplicate value find");
			}
		}
		Node<T>* newNode = new Node<T>();
		newNode->value = value;
		newNode->right = nullptr;
		newNode->left = nullptr;
		if (head != nullptr) {
			head->right = newNode;
			newNode->left = head;
		}
		head = newNode;
	}
	void pop()
	{
		if (head == nullptr)
			throw Exception("Stuck is empty");
		Node<T>* temp = head;
		head = head->left;
		head->right = nullptr;
		delete temp;
	}
	Iterator erase(Iterator);
	Iterator find(T value) const;
	void show() const;
	void show(Iterator) const;
	Iterator begin() { return Iterator(head); }
	Iterator null() { return Iterator(); }
	void swap(Iterator, Iterator);
private:
	Node<T>* head;
};

template<class T>
typename Stack<T>::Iterator Stack<T>::erase(Stack<T>::Iterator it)
{
	if (head == nullptr)
		throw Exception("Stuck is empty");
	if (it == null())
		throw Exception("Invalid iterator");
	Node<T>* temp = it.node;
	if (it.node->left != nullptr) {
		it.node = it.node->left;
		it.node->right = temp->right;
		if (temp->right != nullptr)
			temp->right->left = it.node;
		delete temp;
		return it;
	}
	else {
		Node<T>* temp = it.node;
		it.node = it.node->right;
		it.node->left = nullptr;
		delete temp;
		return it;
	}
}

template<class T>
typename Stack<T>::Iterator Stack<T>::find(T value) const
{
	Node<T>* it = head;
	do
	{
		if (it->value == value)
			return Iterator(it);
		it = it->left;
	} while (it != nullptr);
	return Iterator();
}

template<class T>
void Stack<T>::show() const
{
	Node<T>* it = head;
	while (it->right != nullptr) it = it->right;
	do
	{
		cout << ' ' << it->value;
		it = it->left;
	} while (it != nullptr);
	cout << '\n';
}

template<class T>
void Stack<T>::show(Iterator it) const
{
	while (it.node->right != nullptr) it.node = it.node->right;
	do
	{
		cout << ' ' << it.node->value;
		it.node = it.node->left;
	} while (it.node != nullptr);
	cout << '\n';
}

template<class T>
void Stack<T>::swap(Stack<T>::Iterator it1, Stack<T>::Iterator it2) {
	Node<T>* temp = nullptr;

	if (it2.node->right) it2.node->right->left = it1.node;
	temp = it1.node->right;
	it1.node->right = it2.node->right;
	it2.node->right = temp;
	if (temp) temp->left = it2.node;
	if (it2.node->left) it2.node->left->right = it1.node;
	temp = it1.node->left;
	it1.node->left = it2.node->left;
	it2.node->left = temp;
	if (temp) temp->right = it2.node;

	if (this->head == it1.node) {
		this->head = it2.node;
	}
	if (this->head == it2.node) {
		this->head = it1.node;
	}
}

template<class T>
void fastSort(Iterator<T> begin, Iterator<T> end, Stack<T> stac)
{
	if (begin == Iterator<T>())
		throw Exception("Invalid iterator");
	if (end == Iterator<T>())
		throw Exception("Invalid iterator");
	if (begin == end) return;
	Iterator<T> bgn = begin;
	Iterator<T> nd = end;
	for (Iterator<T> i = begin; i != end; i--)
	{
		if (*i < *bgn)
		{
			stac.swap(i, bgn);
			begin = i;
			bgn = i;
		}
	}
	for (Iterator<T> i = end; i != begin; i++)
	{
		if (*i > * nd)
		{
			stac.swap(i, nd);
			end = i;
			nd = i;
		}
	}
	begin--;
	fastSort(begin, end, stac);
	begin++; end++;
	fastSort(begin, end, stac);
	return;
}

void my_terminate() {
	cerr << "terminate handler called\n";
	abort();
}
void my_unexpected_handler()
{
	cerr << "Unexpected exception\n";
	terminate();
}

int main() {
	srand(time(NULL));
	set_terminate(my_terminate);
	set_unexpected(my_unexpected_handler);
	Stack<Game> stack;
	try {
		stack.pop();
	}
	catch (Exception ex) {
		cerr << ex;
	}
	cout << "\nInserting\n";
	try {
		stack.push(Game("S", 213, 10));
		stack.push(Game("D", 454, 45));
		stack.push(Game("A", 645, 6));
		stack.push(Game("K", 9841, 7));
		stack.push(Game("M", 123, 9));
		stack.push(Game("X", 784, 4));
		stack.show();
	}
	catch (Exception ex) { cerr << ex; }
	try {
		stack.pop();
		stack.show();
		stack.show(stack.begin());
		try {
			cout << "\nE K\n";
			stack.erase(stack.find(Game("K", 9841, 7)));
		}
		catch (Exception ex) { cerr << ex; };
		stack.show();
		cout << "find"<<endl;
		try {
			auto  m= stack.find(Game("M", 123, 9));
			auto d= stack.find(Game("D", 454, 45));
			auto a = stack.find(Game("A", 645, 6));
			auto  s = stack.find(Game("S", 213, 9));
		//	stack.swap(dan, anz);
		}
		catch (Exception ex) { cerr << ex; };
		stack.show();
		try {
			Stack<Game>::Iterator bgn = stack.begin();
			Stack<Game>::Iterator end = stack.begin();
			while (end.node->left != nullptr) end--;
			fastSort(bgn, end, stack);
		}
		catch (Exception ex) { cerr << ex; };
		cout << "\nAfter Sorting\n";
		stack.show();
	}
	catch (Exception ex) { cerr << ex; }
	cout << "\nNew Temp\n";
	try {
		Stack<double> stac1;
		try {
			stac1.push(4.1);
			stac1.show();
			stac1.push(2.1);
			stac1.show();
			stac1.push(1.1);
			stac1.show();
			stac1.push(0);
			stac1.show();
			stac1.push(3);
			stac1.show();
			stac1.push(7);
			stac1.show();
		}
		catch (Exception ex) {
			cerr << ex;
		}
		try {
			Stack<double>::Iterator bgn = stac1.begin();
			Stack<double>::Iterator end = stac1.begin();
			while (end.node->left != nullptr) end--;
			fastSort(bgn, end, stac1);
		}
		catch (Exception ex) { cerr << ex; };
		cout << "\nAfter Sorting\n";
		stac1.show();
	}
	catch (Exception ex) { cerr << ex; };
	return 0;
}

