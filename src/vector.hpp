#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <algorithm>
#include <utility>

namespace sjtu
{

/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		T* ptr;
		const vector* vec;

	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			iterator tmp(*this);
			tmp.ptr += n;
			return tmp;
		}
		iterator operator-(const int &n) const
		{
			iterator tmp(*this);
			tmp.ptr -= n;
			return tmp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (vec != rhs.vec) throw invalid_iterator();
			return ptr - rhs.ptr;
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp(*this);
			ptr++;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			ptr++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp(*this);
			ptr--;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			ptr--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}

		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}

		bool operator<(const iterator &rhs) const {
			return ptr < rhs.ptr;
		}

		bool operator>(const iterator &rhs) const {
			return ptr > rhs.ptr;
		}

		// Constructor
		iterator(T* p, const vector* v) : ptr(p), vec(v) {}
		friend class vector;
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::output_iterator_tag;

	private:
		const T* ptr;
		const vector* vec;

	public:
		const_iterator operator+(const int &n) const
		{
			const_iterator tmp(*this);
			tmp.ptr += n;
			return tmp;
		}
		const_iterator operator-(const int &n) const
		{
			const_iterator tmp(*this);
			tmp.ptr -= n;
			return tmp;
		}
		int operator-(const const_iterator &rhs) const
		{
			if (vec != rhs.vec) throw invalid_iterator();
			return ptr - rhs.ptr;
		}
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator tmp(*this);
			ptr++;
			return tmp;
		}
		const_iterator& operator++() {
			ptr++;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator tmp(*this);
			ptr--;
			return tmp;
		}
		const_iterator& operator--() {
			ptr--;
			return *this;
		}
		const T& operator*() const {
			return *ptr;
		}
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}

		const_iterator(const T* p, const vector* v) : ptr(p), vec(v) {}
		const_iterator(const iterator& it) : ptr(it.ptr), vec(it.vec) {}
		friend class vector;
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data(nullptr), sz(0), cap(0) {}
	vector(const vector &other) : data(nullptr), sz(0), cap(0) {
		if (other.sz > 0) {
			data = static_cast<T*>(::operator new(other.cap * sizeof(T)));
			for (size_t i = 0; i < other.sz; ++i) {
				new (data + i) T(other.data[i]);
			}
			sz = other.sz;
			cap = other.cap;
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		if (data) {
			for (size_t i = 0; i < sz; ++i) {
				data[i].~T();
			}
			::operator delete(data);
		}
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this != &other) {
			vector tmp(other);
			std::swap(data, tmp.data);
			std::swap(sz, tmp.sz);
			std::swap(cap, tmp.cap);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= sz) throw index_out_of_bound();
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= sz) throw index_out_of_bound();
		return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos >= sz) throw index_out_of_bound();
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos >= sz) throw index_out_of_bound();
		return data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (sz == 0) throw container_is_empty();
		return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (sz == 0) throw container_is_empty();
		return data[sz - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(data, this);
	}
	const_iterator begin() const {
		return const_iterator(data, this);
	}
	const_iterator cbegin() const {
		return const_iterator(data, this);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(data + sz, this);
	}
	const_iterator end() const {
		return const_iterator(data + sz, this);
	}
	const_iterator cend() const {
		return const_iterator(data + sz, this);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return sz == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return sz;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < sz; ++i) {
			data[i].~T();
		}
		sz = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		size_t index = pos.ptr - data;
		if (index > sz) throw invalid_iterator();

		shift_right(index);
		data[index] = value;
		return iterator(data + index, this);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > sz) throw index_out_of_bound();

		shift_right(ind);
		data[ind] = value;
		return iterator(data + ind, this);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		size_t index = pos - begin();
		if (index >= sz) throw invalid_iterator();

		for (size_t i = index; i < sz - 1; ++i) {
			data[i] = std::move(data[i + 1]);
		}
		sz--;
		data[sz].~T();
		return iterator(data + index, this);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= sz) throw index_out_of_bound();

		for (size_t i = ind; i < sz - 1; ++i) {
			data[i] = std::move(data[i + 1]);
		}
		sz--;
		data[sz].~T();
		return iterator(data + ind, this);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (sz >= cap) {
			reallocate(cap == 0 ? INITIAL_CAPACITY : cap * 2);
		}
		new (data + sz) T(value);
		sz++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (sz == 0) throw container_is_empty();
		sz--;
		data[sz].~T();
	}

private:
	T* data;
	size_t sz;
	size_t cap;

	static const size_t INITIAL_CAPACITY = 16;

	void reallocate(size_t new_cap) {
		if (new_cap < sz) new_cap = sz;
		if (new_cap < INITIAL_CAPACITY) new_cap = INITIAL_CAPACITY;

		T* new_data = static_cast<T*>(::operator new(new_cap * sizeof(T)));

		for (size_t i = 0; i < sz; ++i) {
			new (new_data + i) T(std::move(data[i]));
			data[i].~T();
		}

		::operator delete(data);
		data = new_data;
		cap = new_cap;
	}

	void shift_right(size_t pos) {
		if (sz >= cap) {
			reallocate(cap == 0 ? INITIAL_CAPACITY : cap * 2);
		}
		new (data + sz) T();
		for (size_t i = sz; i > pos; --i) {
			data[i] = std::move(data[i - 1]);
		}
		sz++;
	}
};


}

#endif