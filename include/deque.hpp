#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu { 

template<class T>
class deque {
	struct node{
		node* prev;
		node* next;
		T* object;

		node(): prev(nullptr), next(nullptr), object(nullptr) {};
		node(node* _prev, node* _next, const T &obj):
				prev(_prev), next(_next), object(new T(obj)) {};
		~node() {
			if (object) {
				delete object;
			}
		}
	};

	size_t curr_size;
	node* dummy_head;
	node* dummy_tail;
public:
	class const_iterator;
	class iterator {
		friend class const_iterator;
		friend class deque;
	private:
		const deque* dq;
		node* ptr;
	public:
		iterator() = default;
		iterator(const deque* q, node* p):dq(q), ptr(p) {
		};
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, the behaviour is **undefined**.
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			if (n < 0) {
				return operator-(-n);
			}
			node* ret = ptr;
			for (int i = 0; i < n && ret->next; ++i) {
				ret = ret->next;
			}
			return iterator(dq, ret);
		}
		iterator operator-(const int &n) const {
			if (n < 0) {
				return operator+(-n);
			}
			node* ret = ptr;
			for (int i = 0; i < n && ret->prev; ++i) {
				ret = ret->prev;
			}
			return iterator(dq, ret);
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			if(dq != rhs.dq) {
				throw invalid_iterator();
			}
			node* lptr = ptr;
			node* rptr = rhs.ptr;
			while(lptr && rptr) {
				lptr = lptr->prev;
				rptr = rptr->prev;
			}
            int sign = 1;
            if (!lptr) {
                sign = -1;
                lptr = rptr;
            }

			int ret = 0;
			while(lptr) {
				++ret;
				lptr = lptr->prev;
			}
			return sign * ret;
		}
		iterator operator+=(const int &n) {
			if (n < 0) {
				return operator-=(-n);
			}
			for (int i = 0; i < n && ptr->next; ++i) {
				ptr = ptr->next;
			}
            return *this;
		}
		iterator operator-=(const int &n) {
			if (n < 0) {
				return operator+=(-n);
			}
			for (int i = 0; i < n && ptr->prev; ++i) {
				ptr = ptr->prev;
			}
            return *this;
		}

		iterator operator++(int) {
			iterator ret(dq, ptr);
			if(ptr->next) {
				ptr = ptr->next;
			}
			return ret;
		}
		iterator& operator++() {
			if(ptr->next) {
				ptr = ptr->next;
			}
			return *this;
		}
		iterator operator--(int) {
			iterator ret(dq, ptr);
			if(ptr->prev) {
				ptr = ptr->prev;
			}
			return ret;
		}
		iterator& operator--() {
			if(ptr->prev) {
				ptr = ptr->prev;
			}
			return *this;
		}

		T& operator*() const {
			if (!ptr->object) {
				throw invalid_iterator();
			}
			return *ptr->object;
		}
		T* operator->() const noexcept {
			if (!ptr->object) {
				throw invalid_iterator();
			}
			return ptr->object;
		}

		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return rhs == *this;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return rhs != *this;
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			const deque* dq;
			node* ptr;
		public:
            const_iterator() = default;
			const_iterator(const deque* q, node* p): dq(q), ptr(p){};
			const_iterator(const const_iterator &other) {
				dq = other.dq;
				ptr = other.ptr;
			}
			const_iterator(const iterator &other) {
				dq = other.dq;
				ptr = other.ptr;
			}

			const_iterator operator+(const int &n) const {
				if (n < 0) {
					return operator-(-n);
				}
				node* ret = ptr;

				for (int i = 0; i < n && ret->next; ++i) {
					ret = ret->next;
				}
				return iterator(dq, ret);
			}

			const_iterator operator-(const int &n) const {
				if (n < 0) {
					return operator+(-n);
				}
				node* ret = ptr;

				for (int i = 0; i < n && ret->prev; ++i) {
					ret = ret->prev;
				}
				return iterator(dq, ret);
			}

			int operator-(const const_iterator& rhs) {
				if(dq != rhs.dq) {
					throw invalid_iterator();
				}
				node* lptr = ptr;
				node* rptr = rhs.ptr;
				while(lptr && rptr) {
					lptr = lptr->prev;
					rptr = rptr->prev;
				}
                int sign = 1;
                if (!lptr) {
                    sign = -1;
                    lptr = rptr;
                }

				int ret = 0;
				while(lptr) {
					++ret;
					lptr = lptr->prev;
				}
				return sign * ret;
			}

			const_iterator operator+=(const int &n) {
				if (n < 0) {
					return operator-=(-n);
				}
				for (int i = 0; i < n && ptr->next; ++i) {
					ptr = ptr->next;
				}
				return *this;
			}

			const_iterator operator-=(const int &n) {
				if (n < 0) {
					return operator+(-n);
				}
				for (int i = 0; i < n && ptr->prev; ++i) {
					ptr = ptr->prev;
				}

				return *this;
			}

			const_iterator operator++(int) {
				const_iterator ret(dq, ptr);
				if(ptr->next) {
					ptr = ptr->next;
				}
				return ret;
			}
			const_iterator& operator++() {
				if(ptr->next) {
					ptr = ptr->next;
				}
				return *this;
			}
			const_iterator operator--(int) {
				const_iterator ret(dq, ptr);
				if(ptr->prev) {
					ptr = ptr->prev;
				}
				return ret;
			}
			const_iterator& operator--() {
				if(ptr->prev) {
					ptr = ptr->prev;
				}
				return *this;
			}

			const T& operator*() const {
				if (!ptr->object) {
					throw invalid_iterator();
				}
				return *ptr->object;
			}

			const T* operator->() const noexcept {
				if (!ptr->object) {
					throw invalid_iterator();
				}
				return ptr->object;
			}

			bool operator==(const const_iterator &rhs) const{
				return ptr == rhs.ptr;
			}

			bool operator==(const iterator &rhs) const {
				return ptr == rhs.ptr;
			}

			bool operator!=(const const_iterator &rhs) const{
				return ptr != rhs.ptr;
			}

			bool operator!=(const iterator &rhs) const{
				return ptr != rhs.ptr;
			}
	};

	deque() {
		curr_size = 0;
		dummy_head = new node();
		dummy_tail = new node();
		dummy_head->next = dummy_tail;
		dummy_tail->prev = dummy_head;
	}

	deque(const deque &other) {
		dummy_head = new node();
		dummy_tail = new node();
		dummy_head->next = dummy_tail;
		dummy_tail->prev = dummy_head;
		curr_size = 0;

		for (const_iterator cit = other.cbegin(); cit != other.cend(); ++cit) {
			push_back(*cit);
		}
	}

	~deque() {
		clear();
		delete dummy_head;
		delete dummy_tail;
	}

	deque &operator=(const deque &other) {
		if (&other == this) {
			return *this;
		}

		clear();
		for (const_iterator cit = other.cbegin(); cit != other.cend(); ++cit) {
			push_back(*cit);
		}

		return *this;
	}
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
		if (pos < 0 || curr_size <= pos) {
			throw index_out_of_bound();
		}
		iterator it = begin() +  pos;
		return *it;
	}
	const T & at(const size_t &pos) const {
		if (pos < 0 || curr_size <= pos) {
			throw index_out_of_bound();
		}
		const_iterator cit = cbegin() +  pos;
		return *cit;
	}
	T & operator[](const size_t &pos) {
		return at(pos);
	}
	const T & operator[](const size_t &pos) const {
		return at(pos);
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
		if (!curr_size) {
			throw container_is_empty();
		}

		return *cbegin();
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
		if (!curr_size) {
			throw container_is_empty();
		}

		return *(--cend());
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(this, dummy_head->next);
	}
	const_iterator cbegin() const {
		return const_iterator(this, dummy_head->next);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(this, dummy_tail);
	}
	const_iterator cend() const {
		return const_iterator(this, dummy_tail);
	}
	/**
	 * checks whether the container is empty.
	 */
	bool empty() const {
		return curr_size == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return curr_size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		curr_size = 0;

		node* tmp;
		for (node* curr = dummy_head->next; curr != dummy_tail; curr = tmp) {
			tmp = curr->next;
			delete curr;
		}
		dummy_head->next = dummy_tail;
		dummy_tail->prev = dummy_head;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) {
		if (pos.dq != this || pos.ptr == dummy_head) {
			throw invalid_iterator();
		}
		node* next = pos.ptr;
		node* prev = next->prev;
		node* tmp = new node(prev, next, value);
		prev->next = tmp;
		next->prev = tmp;

		++curr_size;
		return iterator(this, tmp);
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
		if (!curr_size || pos.dq != this || pos.ptr == dummy_tail) {
			throw invalid_iterator();
		}

		node* prev = pos.ptr->prev;
		node* next = pos.ptr->next;
		prev->next = next;
		next->prev = prev;

		--curr_size;
		delete(pos.ptr);

		return iterator(this, next);
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
		node* next = dummy_tail;
		node* prev = next->prev;
		node* tmp = new node(prev, next, value);
		next->prev = tmp;
		prev->next = tmp;

		++curr_size;
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
		if (!curr_size) {
			throw container_is_empty();
		}

		node* back = dummy_tail->prev;
		back->prev->next = dummy_tail;
		dummy_tail->prev = back->prev;

		--curr_size;
		delete back;
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
		node* prev = dummy_head;
		node* next = prev->next;
		node* tmp = new node(prev, next, value);
		prev->next = tmp;
		next->prev = tmp;

		++curr_size;
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
		if (!curr_size) {
			throw container_is_empty();
		}

		node* front = dummy_head->next;
		front->next->prev = dummy_head;
		dummy_head->next = front->next;

		delete front;
		--curr_size;
	}
};

}

#endif