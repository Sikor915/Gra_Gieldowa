#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <iterator>
#include <cstddef>
#include "Stock.h"

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) 
{
    os << p.second << " " << p.first;
    return os;
}

template <typename T>
bool operator==(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) 
{
	return lhs.get() == rhs.get();
}
template <typename T>
bool operator<(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) 
{
	return lhs.get() < rhs.get();
}

//template <typename T>
//bool operator<(const T& lhs, const T& rhs)
//{
//	return lhs < rhs;
//}

bool operator>(const Stock<std::string>& lhs, const Stock<std::string>& rhs)
{
    return lhs > rhs;
}

template <typename T>
class Node 
{
public:
    T data;
    std::shared_ptr<Node<T>> next;
    std::weak_ptr<Node<T>> prev;

    Node(const T& val) : data(val), next(nullptr), prev() {}
    Node(Node<T>* ptr): data(ptr->data), next(ptr->next), prev(ptr->prev) {}
    Node(T&& val) noexcept : data(std::move(val)), next(nullptr), prev() {}
    Node(const Node& other) : data(other.data), next(other.next), prev(other.prev) {}
	Node(Node&& other) noexcept : data(std::move(other.data)), next(std::move(other.next)), prev(std::move(other.prev)) {}

    bool operator>(const Node& other) const
    {
        return data > other.data;
    }

    bool operator<(const Node& other) const
	{
		return data < other.data;
	}

	bool operator==(const Node& other) const 
	{
		return data == other.data;
	}

	bool operator!=(const Node& other) const 
	{
		return !(*this == other);
	}

	Node& operator=(const Node& other) 
	{
		if (this == &other) 
		{
			return *this;
		}

		data = other.data;
		next = other.next;
		prev = other.prev;

		return *this;
	}

	Node& operator=(Node&& other) noexcept 
	{
		if (this == &other) 
		{
			return *this;
		}

		data = std::move(other.data);
		next = std::move(other.next);
		prev = std::move(other.prev);

		return *this;
	}

	~Node() = default;
};


template <typename T>
class DoublyLinkedList 
{
    std::shared_ptr<Node<T>> head;
    std::shared_ptr<Node<T>> tail;

public:
    
    struct Iterator : public std::bidirectional_iterator_tag
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;
    private:
        std::shared_ptr<Node<T>> m_PTR;
    public:
        Iterator(std::shared_ptr<Node<T>> ptr) : m_PTR(ptr) {}
        Iterator(const Iterator& other): m_PTR(other.m_PTR) {}
        Iterator(Iterator&& other) noexcept: m_PTR(std::move(other.m_PTR)) {}
        Iterator(Node<T>* node) : m_PTR(std::make_shared<Node<T>>(node)) {}
        ~Iterator() = default;


        const reference operator*() { return m_PTR->data; }
        reference operator*() const { return m_PTR->get(); }
        pointer operator->() { return *m_PTR; }

        /*Iterator& operator++() { m_PTR++; return *this; }
        Iterator& operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { m_PTR--; return *this; }
        Iterator& operator--(int) { Iterator tmp = *this; --(*this); return tmp; }
        Iterator& operator+=(const difference_type other) noexcept { m_PTR += other; return *this; }
        Iterator operator+(const difference_type other) noexcept { return Iterator(m_PTR + other); }
        Iterator& operator-=(const difference_type other) noexcept { m_PTR -= other; return *this; }
        Iterator operator-(const difference_type other) noexcept { return Iterator(m_PTR - other); }
        difference_type operator-(const Iterator& other) const noexcept { return m_PTR - other.m_PTR; }
        reference& operator[](difference_type index) const { return *(m_PTR + index); }
        
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_PTR == b.m_PTR; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_PTR != b.m_PTR; };
        bool operator< (const Iterator& other) { return m_PTR < other.m_PTR; }
        bool operator> (const Iterator& other) { return m_PTR > other.m_PTR; }
        bool operator<= (const Iterator& other) { return !(m_PTR > other.m_PTR); }
        bool operator>= (const Iterator& other) { return !(m_PTR < other.m_PTR); }*/

        Iterator& operator++() { m_PTR = m_PTR->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { m_PTR = m_PTR->prev.lock(); return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }
        Iterator operator-(const difference_type other) noexcept { return Iterator(m_PTR - other); }
        Iterator operator-(const Iterator& other) noexcept { return Iterator(m_PTR - other.m_PTR); }
        Iterator operator+(const difference_type other) noexcept { return Iterator(m_PTR + other); }

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_PTR == b.m_PTR; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_PTR != b.m_PTR; };
        bool operator< (const Iterator& other) { return m_PTR < other.m_PTR; }
        bool operator> (const Iterator& other) { return m_PTR > other.m_PTR; }
        bool operator<= (const Iterator& other) { return !(m_PTR > other.m_PTR); }
        bool operator>= (const Iterator& other) { return !(m_PTR < other.m_PTR); }


        std::shared_ptr<Node<T>> getPtr() const { return m_PTR; }
        const std::shared_ptr<Node<T>>& getConstPtr() const { return m_PTR; }
    };

    DoublyLinkedList() : head(nullptr), tail(nullptr) {}
    DoublyLinkedList(const DoublyLinkedList& other): head(nullptr), tail(nullptr)
	{
		std::shared_ptr<Node<T>> current = other.head;
		while (current) 
		{
			pushBack(current->data);
			current = current->next;
		}
	}
    DoublyLinkedList(DoublyLinkedList&& other) noexcept : head(std::move(other.head)), tail(std::move(other.tail)) {}

    void sort();
    void pushBack(const T& value);
    void Insert(const std::pair<Stock<std::string>, int>& data);
    void popBack();
    void clear();
    void erase(const Stock<std::string>& key);
    void display() const;


    std::shared_ptr<Node<std::pair<Stock<std::string>, int>>> Find(const Stock<std::string>& key) const;
    std::shared_ptr<Node<std::pair<Stock<std::string>, int>>> Find(const std::string& key) const;
    std::shared_ptr<Node<std::pair<Stock<std::string>, int>>> BeginList() const;
    std::shared_ptr<Node<std::pair<Stock<std::string>, int>>> EndList() const;

    Iterator begin() { return Iterator(head.get()); }
    Iterator end() { return Iterator(nullptr); }

    std::shared_ptr<Node<T>> getHead() const 
    { 
        return head; 
    }
    std::shared_ptr<Node<T>> getTail() const 
	{ 
		return tail; 
	}

    DoublyLinkedList& operator=(const DoublyLinkedList& other) 
	{
		if (this == &other) 
		{
			return *this;
		}

		while (head) 
		{
			popBack();
		}

		std::shared_ptr<Node<T>> current = other.head;
		while (current) 
		{
			pushBack(current->data);
			current = current->next;
		}

		return *this;
	}

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        
        while (head)
        {
            popBack();
        }
        std::shared_ptr<Node<T>> current = other.head;
        while (current)
		{
			pushBack(current->data);
			current = current->next;
		}
        
        while (other.head)
        {
            other.popBack();
        }

        return *this;
    }

    std::pair<Stock<std::string>, int>& operator[](std::size_t index) 
    {
        auto current = head;
        for (std::size_t i = 0; i < index && current; ++i) 
        {
            current = current->next;
        }

        if (!current) 
        {
            throw std::out_of_range("Index out of bounds");
        }

        return current->data;
    }

    bool operator==(const DoublyLinkedList& other) const
    {
        if (*head == *other.head and *tail == *other.tail)
        {
            return true;
        }
        return false;
    }

    ~DoublyLinkedList() 
	{
		while (head) 
		{
			popBack();
		}
	}
};

template<typename T>
inline void DoublyLinkedList<T>::sort()
{
    /*std::shared_ptr<Node<T>> prev;
    std::shared_ptr<Node<T>> curr;
    for(curr = head; curr != nullptr; curr = curr->next)
	{
		for(prev = curr->next; prev != nullptr; prev = prev->next)
		{
			if(curr->data > prev->data)
			{
				std::swap(curr->data, prev->data);
			}
		}
	}*/
    //std::sort(begin, end);
    
}

template<typename T>
inline void DoublyLinkedList<T>::pushBack(const T& value)
{
    auto newNode = std::make_shared<Node<T>>(value);
    if (!head)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
}

template<typename T>
inline void DoublyLinkedList<T>::Insert(const std::pair<Stock<std::string>, int>& data)
{
    auto newNode = std::make_shared<Node<std::pair<Stock<std::string>, int>>>(data);
    if (!head)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
}

template<typename T>
inline void DoublyLinkedList<T>::popBack()
{
    if (!tail)
    {
        std::cerr << "List is empty. Cannot pop back." << std::endl;
        return;
    }

    if (tail->prev.lock())
    {
        tail = tail->prev.lock();
        tail->next = nullptr;
    }
    else
    {
        head = nullptr;
        tail = nullptr;
    }
}

template<typename T>
inline void DoublyLinkedList<T>::clear()
{
    while (head)
    {
        popBack();
    }
}

template<typename T>
inline void DoublyLinkedList<T>::erase(const Stock<std::string>& key)
{
    auto current = Find(key);
    if (!current)
    {
        std::cerr << "Element not found. Cannot erase." << std::endl;
        return;
    }

    if (current->prev.lock())
    {
        current->prev.lock()->next = current->next;
    }
    else
    {
        head = current->next;
    }

    if (current->next)
    {
        current->next->prev = current->prev;
    }
    else
    {
        tail = current->prev.lock();
    }
}

template<typename T>
inline void DoublyLinkedList<T>::display() const
{
    std::shared_ptr<Node<T>> current = head;
    while (current)
    {
        std::cout << current->data << std::endl;
        current = current->next;
    }
    std::cout << std::endl;
}

template<typename T>
inline std::shared_ptr<Node<std::pair<Stock<std::string>, int>>> DoublyLinkedList<T>::Find(const Stock<std::string>& key) const
{
    auto current = head;
    while (current && !(current->data.first == key))
    {
        current = current->next;
    }

    return current;
}

template<typename T>
inline std::shared_ptr<Node<std::pair<Stock<std::string>, int>>> DoublyLinkedList<T>::Find(const std::string& key) const
{
    auto current = head;
	while (current && !(current->data.first.getIndex() == key))
	{
		current = current->next;
	}

	return current;
}

template<typename T>
inline std::shared_ptr<Node<std::pair<Stock<std::string>, int>>> DoublyLinkedList<T>::BeginList() const
{
    return head;
}

template<typename T>
inline std::shared_ptr<Node<std::pair<Stock<std::string>, int>>> DoublyLinkedList<T>::EndList() const
{
    return nullptr;
}

