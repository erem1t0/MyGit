// Petrusevich.cpp

#pragma once
//
//#include <iostream>
//#include <fstream>
//#include <iterator>
//
//using namespace std;
//
//template <class T>
//class Element
//{//������� �������� ������
//private:
//	//��������� �� ���������� � ��������� �������
//	Element* next;
//	Element* prev;
//	//����������, �������� � ����
//	T field;
//public:
//	Element(T value = 0, Element<T>* next_ptr = NULL, Element<T>
//		* prev_ptr = NULL)
//	{
//		field = value;
//		next = next_ptr;
//		prev - prev_ptr;
//	}
//	//������ � ���� *next
//	virtual Element* getNext() { return next; }
//	virtual void setNext(Element* value) { next = value; }
//	//������ � ���� *prev
//	virtual Element* getPrevious() { return prev; }
//	virtual void setPrevious(Element* value) { prev = value; }
//	//������ � ���� � �������� ����������� field
//	virtual T getValue() { return field; }
//	virtual void setValue(T value) { field = value; }
//	template<class T> friend ostream& operator<< (ostream& ustream, Element<T>& obj);
//};
//
//
//template<class T>
//ostream& operator << (ostream& ustream, Element<T>& obj)
//{
//	ustream << obj.field;
//	return ustream;
//}
//
//
//template <class T>
//class LinkedListParent
//{
//protected:
//	//���������� ������� ������ � �����
//	Element<T>* head;
//	Element<T>* tail;
//	//��� �������� ������ ���������� ���������
//	int num;
//public:
//	virtual int Number() { return num; }
//	virtual Element<T>* getBegin() { return head; }
//	virtual Element<T>* getEnd() { return tail; }
//	LinkedListParent()
//	{
//		//����������� ��� ����������
//		cout << "\nParent constructor";
//		head = NULL;
//		num = 0;
//	}
//	//����� ����������� �������: ���� �� ����������� � ����� ������, �� ������ ����������� ����������
//		virtual Element<T>* push(const T& value) = 0;
//	//����� ����������� �������: ���� �� ����������� � ����� ������, �� ������ ����������� ��������
//		virtual Element<T>* pop() = 0;
//	virtual ~LinkedListParent()
//	{
//		Element<T>* curr = head;
//		while (curr != tail)
//		{
//			curr = curr->getNext();
//			delete curr->getPrevious();
//			--num;
//		}
//		delete curr;
//		//���������� - ������������ ������
//		cout << "\nParent destructor";
//	}
//	//��������� �������� �� ������� - ������ ��������������� ������ ����� �������� ?
//		virtual Element<T>*operator[](int i)
//	{
//		//����������
//		if (i<0 || i>num) return NULL;
//		int k = 0;
//		//���� i-� ������� - ������� � ������ � ����������� i ����� ������
//			Element<T>* cur = head;
//		for (k = 0; k < i; k++)
//		{
//			cur = cur->getNext();
//		}
//		return cur;
//	}
//	template<class T> friend ostream& operator<< (ostream&
//		ustream, LinkedListParent<T>& obj);
//	template<class T> friend istream& operator>> (istream&
//		ustream, LinkedListParent<T>& obj);
//};
//
//template<class T>
//ostream& operator << (ostream& ustream, LinkedListParent<T>& obj)
//{
//	if (typeid(ustream).name() == typeid(ofstream).name())
//	{
//		ustream << obj.num << "\n";
//		for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
//			ustream << current->getValue() << " ";
//		return ustream;
//	}
//	ustream << "\nLength: " << obj.num << "\n";
//	int i = 0;
//
//	for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext(), i++)
//		ustream << "arr[" << i << "] = " << current->getValue() << "\n";
//	return ustream;
//}
//
//template<class T>
//istream& operator >> (istream& ustream, LinkedListParent<T>& obj)
//{
//	//������ �� ����� � ������� ���������
//	int len;
//	ustream >> len;
//	//����� ���� �������� ������ ��� obj, ���������� obj.num = 0
//	double v = 0;
//	for (int i = 0; i < len; i++)
//	{
//		ustream >> v;
//		obj.push(v);
//	}
//	return ustream;
//}
//
//
//template<typename ValueType>
//class ListIterator : public std::iterator<std::input_iterator_tag, ValueType>
//{
//public:
//	ListIterator() 
//	{ 
//		ptr = nullptr;
//	}
//
//	ListIterator(Element<ValueType>* p) 
//	{
//		ptr = p; 
//	}
//
//	ListIterator(const ListIterator& it) 
//	{
//		ptr = it.ptr;
//	}
//	bool operator!=(ListIterator const& other) const
//	{
//		return ptr != other.ptr;
//	}
//	bool operator==(ListIterator const& other) const 
//	{
//		return ptr == other.ptr;
//	}
//
//	Element<ValueType>& operator*()
//	{
//		return *ptr;
//	}
//	ListIterator& operator++() 
//	{
//		ptr = ptr->getNext(); 
//		return *this;
//	}
//	ListIterator& operator++(int v) 
//	{
//		ptr = ptr->getNext();
//		return *this;
//	}
//	ListIterator& operator=(const ListIterator& it) 
//	{
//		ptr = it.ptr; return *this;
//	}
//	ListIterator& operator=(Element<ValueType>* p) 
//	{
//		ptr = p;
//		return *this;
//	}
//private:
//	Element<ValueType>* ptr;
//};
//
//template <class T>
//class IteratedLinkedList : public LinkedListParent<T>
//{
//public:
//
//	IteratedLinkedList() : LinkedListParent<T>() 
//	{
//		cout << "\nIteratedLinkedList constructor";
//	}
//	virtual ~IteratedLinkedList()
//	{
//		cout << "\nIteratedLinkedList destructor";
//	}
//
//	ListIterator<T> iterator;
//
//	ListIterator<T> begin()
//	{
//		ListIterator<T> it = LinkedListParent<T>::head; 
//		return it;
//	}
//
//	ListIterator<T> end() 
//	{
//		ListIterator<T> it = LinkedListParent<T>::tail; 
//		return it;
//	}
//};



#include <iostream>			// for std::cin, std::cout
#include <functional>		// for std::function
#include <list>				// for std::list
#include <string_view>		// for std::string_view
#include <string>			// for std::string
#include <algorithm>		// for std::find
#include <random>			// for std::mt19937

#include "Petrusevich.cpp"	// for LinkedList from Petrusevich


////////////////////////////////#1.1-1.2////////////////////////////

// All consonants
const std::string consonants = "BCDFGHJKLMNPQRSTVWXYZbcdfghjklmnpqrstvwxyz";

// Checking for a consonant
bool isConsonant(const char c)
{
	return std::find(consonants.begin(), consonants.end(), c) != consonants.end();
}

// Filter std::list<T> by predicate f
template<typename T>
std::list<T> filter(const std::list<T>& lst, std::function<bool(const char)>  f = isConsonant)
{
	if (!f) throw "Nullptr";
	std::list<T> ans;
	typename std::list<T>::const_iterator it = lst.begin();
	while (it != lst.end()) {
		if (f(*it))
			ans.push_front((*it)); ++it;
	}

	return ans;
}

// Function, that prints std::list<T>
template<typename T>
void print_list(const std::list<T>& lst)
{
	std::cout << "List: ";
	typename std::list<T>::const_iterator it = lst.begin();
	while (it != lst.end()) {
		std::cout << *it << ' '; ++it;
	}
	std::cout << '\n';
}

// Overloading of operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out, const std::list<T>& lst)
{
	print_list(lst);
	return out;
}

// Pop the element in random place of std::list<T>
template<typename T>
void pop(std::list<T>& lst, typename std::list<T>::const_iterator it)
{
	if (lst.size())
		lst.erase(it);
}

// Pop the specified element from the std::list<T>
template<typename T>
void pop(std::list<T>& lst, const T& data)
{
	typename std::list<T>::const_iterator it = lst.begin();
	while (it != lst.end() && (*it) != data) ++it;
	if (it != lst.end()) lst.erase(it);
}

// Push the specified element to the std::list<T>
template<typename T>
void push(std::list<T>& lst, const T& data)
{
	typename std::list<T>::const_iterator it = lst.begin();
	while (it != lst.end() && (*it) <= data) ++it;
	lst.insert(it, data);
}

// Below is the implementation of the class and its substitution in std::list<T>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// namespace for generating random numbers
namespace Random
{
	std::mt19937 mt{ std::random_device{}() };

	int get(const int min, const int max)
	{
		std::uniform_int_distribution<> die{ min, max };
		return die(mt);
	}
}

// Class to fill std::list<T>
class Patient
{
private:
	std::string first_name;
	std::string last_name;
	std::string birthday;
	std::string phone_number;
	std::string address;
	std::string card_number;
	std::string blood_type;

public:

	Patient(const std::string _first_name, const std::string _last_name, const std::string _birthday, const std::string _phone_number,
		const std::string _address, const std::string _card_number, const std::string _blood_type)
		: first_name(_first_name)
		, last_name(_last_name)
		, birthday(_birthday)
		, phone_number(_phone_number)
		, address(_address)
		, card_number(_card_number)
		, blood_type(_blood_type)
	{ }

	Patient() = default;

	Patient(const Patient& p)
		: first_name(p.first_name)
		, last_name(p.last_name)
		, birthday(p.birthday)
		, phone_number(p.phone_number)
		, address(p.address)
		, card_number(p.card_number)
		, blood_type(p.blood_type)
	{ }

	Patient& operator=(const Patient& p)
	{
		if (this == &p)
			return *this;

		first_name = p.first_name;
		last_name = p.last_name;
		birthday = p.birthday;
		phone_number = p.phone_number;
		address = p.address;
		card_number = p.card_number;
		blood_type = p.blood_type;

		return *this;
	}

	const std::string first_names[5]
	{
		"Alex", "Mark","Kate", "Andrew","Max"
	};

	const std::string last_names[5]
	{
		"Koch", "Chang","Fischer", "Chase","Preston"
	};

	std::string make_birthday(const int k1, const int k2, const int k3) const
	{
		std::string ans{ std::to_string(k1) };
		ans += '/'; ans += std::to_string(k2); ans += '/'; ans += std::to_string(k3);
		return ans;
	}

	std::string make_blood(const int k) const
	{
		switch (k)
		{
		case 1: return "I";
		case 2: return "II";
		case 3: return "III";
		case 4: return "IV";
		}
	}

	void fill()
	{
		int k1{ Random::get(0,4) }, k2{ Random::get(0,4) }, k3_1{ Random::get(1,28) }, k3_2{ Random::get(1,12) }, k3_3{ Random::get(1950,2023) },
			k4{ Random::get(10,99) }, k5{ Random::get(1,1000) }, k6{ Random::get(1,4) };

		first_name = first_names[k1];
		last_name = last_names[k2];
		birthday = make_birthday(k3_1, k3_2, k3_3);
		phone_number = "+101 (22) 343-35-" + std::to_string(k4);
		address = "Random street, number of house isn't mind)";
		card_number = std::to_string(k5);
		blood_type = make_blood(k6);
	}

	bool operator<(const Patient& a) const
	{
		return (card_number > a.card_number) || (card_number != a.card_number && blood_type > a.blood_type) ||
			(card_number != a.card_number && blood_type != a.blood_type && last_name < a.last_name) ||
			(card_number != a.card_number && blood_type != a.blood_type && last_name != a.last_name && first_name < a.first_name);
	}

	bool operator>(const Patient& a) const
	{
		return !(*this < a);
	}

	bool operator==(const Patient& a) const
	{
		return !(*this > a) && !(a > *this);
	}

	bool operator>=(const Patient& a) const
	{
		return !(*this < a);
	}

	bool operator<=(const Patient& a) const
	{
		return !(*this > a);
	}

	bool operator!=(const Patient& a) const
	{
		return !(*this == a);
	}

	void print() const
	{
		std::cout << "First name:\t" << first_name << "\nLast name:\t" << last_name << "\nBirthday:\t" << birthday << "\nPhone number:"
			"\t" << phone_number << "\nAddress:\t" << address << "\nCard number:\t" << card_number << "\nBlood type:\t" << blood_type << '\n';
	}

	friend std::ostream& operator<<(std::ostream& out, const Patient& a)
	{
		a.print();
		return out;
	}

	~Patient() = default;
};



///////////////////////////////#1.3-1.5///////////////////////////////////////////

template <typename T>
class Stack : public IteratedLinkedList<T>
{
public:

	virtual Element<T>* pop() override
	{
		// If List is empty
		if (!this->num)
			return nullptr;
		Element<T>* tmp;
		// If we pop last element of the list
		if (this->num == 1)
		{
			tmp = this->head;
			this->head = this->tail = nullptr;
		}
		else
		{
			tmp = this->tail;
			this->tail = this->tail->getPrevious();
			this->tail->setNext(nullptr);
			tmp->setPrevious(nullptr);
		}
		this->num--;
		return tmp;
	}

	virtual Element<T>* push(const T& data) override
	{
		Element<T>* newElement = new Element<T>(data);
		if (!newElement) throw "bad_alloc";
		// If list is empty
		if (!this->num)
		{
			this->head = newElement;
			this->tail = newElement;
		}
		else
		{
			this->tail->setNext(newElement);
			newElement->setPrevious(this->tail);
			this->tail = newElement;
		}
		this->num++;
		return newElement;
	}

	virtual ~Stack() = default;
};

template<typename T>
class D : public Stack<T>
{
public:

	virtual Element<T>* push(const T& data) override
	{
		Element<T>* newElement = new Element<T>(data);
		if (!newElement) throw "bad_alloc";
		// If list is empty
		if (!this->num)
		{
			this->head = newElement;
			this->tail = newElement;
		}
		else
		{
			Element<T>* current = this->tail;
			while (current != nullptr && current->getValue() < data) current = current->getPrev();

			// If we push to the begin
			if (current == nullptr)
			{
				this->tail->setNext(newElement);
				newElement->setPrevious(this->tail);
				this->tail = newElement;
			}
			// If we push to the end
			else if (current == this->tail)
			{
				newElement->setPrev(this->tail);
				this->tail->setNext(newElement);
				this->tail = newElement;
			}
			else
			{
				Element<T>* prev = current->getPrevious();

				newElement->setNext(current);
				current->setPrevious(newElement);
				newElement->setPrevious(prev);
				prev->setNext(newElement);
			}
		}
		this->num++;
		return newElement;
	}

	virtual ~D() = default;
};

// Universal filter
template<typename T>
D<T> universal_filter(const D<T>& lst, std::function<bool(T)> f)
{
	if (!f) throw "Nullptr";
	D<T> ans;
	ListIterator it = lst.begin();
	while (it != lst.end()) {
		if (f(*it))
			ans.push_front((*it)); ++it;
	}

	return ans;
}

int main()
{
	Stack<Patient> stack;
	Patient a1, b1, c1, d1; a1.fill(); b1.fill(); c1.fill(); d1.fill();
	stack.push(a1);
	stack.push(b1);
	stack.push(c1);
	stack.push(d1);

	cout << stack;

	Element<Patient>* poppedElement1 = stack.pop();
	Element<Patient>* poppedElement2 = stack.pop();

	cout << "Popped elements: ";
	if (poppedElement1 != nullptr)
		cout << poppedElement1->getValue() << " ";
	if (poppedElement2 != nullptr)
		cout << poppedElement2->getValue() << " ";
	cout << endl;

	D<int> list;
	list.push(9);
	list.push(2);
	list.push(7);
	list.push(5);

	cout << list;

	try
	{
		Patient a, b, c, d;
		a.fill(); b.fill(), c.fill(), d.fill();
		std::list<Patient> lst;
		push(lst, a); push(lst, b); push(lst, c); push(lst, d);
		std::list<Patient>::const_iterator it = lst.begin();
		pop(lst, it); it = lst.begin(); pop(lst, it); it = lst.begin(); pop(lst, it); it = lst.begin(); pop(lst, it);
		it = lst.begin(); pop(lst, a);
		std::cout << lst;
	}
	catch (std::string exc)
	{
		std::cout << "Find " << exc;
	}
	catch (...)
	{
		std::cout << "Unknown Error";
		std::abort();
	}

	return 0;
}