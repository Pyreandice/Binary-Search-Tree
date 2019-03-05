#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <algorithm>
#include <ppltasks.h>

namespace nwacc
{
	template <typename T>
	class linked_list
	{
	private:
		struct node
		{
			T data;
			node *previous;
			node *next;

			node(
				const T &value = T {},
				node *previous = nullptr,
				node *next = nullptr
				) : data { value }, previous { previous }, next { next } {}

			node(T &&value, node *previous = nullptr, node *next = nullptr) 
				: data { std::move(value) }, previous { previous }, next { next } {}
		};
	public:
		class const_iterator
		{
		public:

			const_iterator() : current { nullptr } {}

			const T & operator*() const
			{
				return this->retrieve();
			}

			// prefix ++ operator
			const_iterator & operator++()
			{
				this->current = this->current->next;
				return *this;
			}
			// c++ REQUIRES! single anonymous int param
			// so the signatures of the methods are different. 
			// this is the postfix operator.
			const_iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			const_iterator & operator--()
			{
				this->current = this->current->previous;
				return *this;
			}

			const_iterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

			// Added to the code
			bool operator== (const const_iterator & rhs) const
			{
				return this->current == rhs.current;
			}

			bool operator!= (const const_iterator & rhs) const
			{
				return !(*this == rhs);
			}

		protected:
			node * current;

			T &retrieve() const
			{
				return this->current->data;
			}

			const_iterator(node *current) : current{ current } {}

			friend class linked_list<T>;
		};

		// This is the IS-A relationship. We now say
		// iterator IS-A const_iterator. iterator is a sub class of 
		// const_iterator. const_iterator is a super class to iterator.
		class iterator : public const_iterator
		{
		public:

			iterator() {}

			T & operator*()
			{
				return const_iterator::retrieve();
			}

			// Return the object stored at the current position.
			// For iterator, there is an accessor with a
			// const reference return type and a mutator with
			// a reference return type. The accessor is shown first.
			const T & operator* () const
			{
				return const_iterator::operator*();
			}

			iterator & operator++ ()
			{
				this->current = this->current->next;
				return *this;
			}

			iterator operator++ (int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			iterator & operator-- ()
			{
				this->current = this->current->previous;
				return *this;
			}

			iterator operator-- (int)
			{
				auto old = *this;
				--(*this);
				return old;
			}
		protected:
			// Protected constructor for iterator.
			// Expects the current position.
			iterator(node *current) : const_iterator{ current } { }

			friend class linked_list<T>;
		};

		linked_list()
		{
			this->init();
		}

		linked_list(const linked_list & rhs)
		{
			this->init();
			for (auto &value : rhs)
			{
				this->push_back(value);
			}
		}

		~linked_list()
		{
			this->clear();
			delete this->head;
			delete this->tail;
		}

		linked_list(linked_list && rhs)
			: my_size{ rhs.my_size }, head{ rhs.head }, tail{ rhs.tail }
		{
			rhs.my_size = 0;
			rhs.head = nullptr;
			rhs.tail = nullptr;
		}

		linked_list & operator=(linked_list && rhs)
		{
			std::swap(this->my_size, rhs.my_size);
			std::swap(this->head, rhs.head);
			std::swap(this->tail, rhs.tail);
			return *this;
		}

		linked_list & operator= (const linked_list & rhs)
		{
			auto copy = rhs;
			std::swap(*this, copy);
			return *this;
		}

		iterator begin()
		{
			return iterator(this->head->next);
		}

		const_iterator begin() const
		{
			return const_iterator(this->head->next);
		}

		iterator end()
		{
			return iterator(this->tail);
		}

		const_iterator end() const
		{
			return const_iterator(this->tail);
		}

		int size() const
		{
			return this->my_size;
		}

		bool is_empty() const
		{
			return this->size() == 0;
		}

		void clear()
		{
			while (!this->is_empty())
			{
				this->pop_front();
			}
		}

		T & front()
		{
			return *begin();
		}

		const T & front() const
		{
			return *begin();
		}

		T & back()
		{
			return *--end();
		}

		const T & back() const
		{
			return *--end();
		}

		void push_front(const T & value)
		{
			this->insert(this->begin(), value);
		}

		void push_back(const T & value)
		{
			this->insert(this->end(), value);
		}

		void push_front(T && value)
		{
			this->insert(this->begin(), std::move(value));
		}

		void push_back(T && value)
		{
			this->insert(this->end(), std::move(value));
		}

		void pop_front()
		{
			this->erase(this->begin());
		}

		void pop_back()
		{
			this->erase(--this->end());
		}


		// insert a value BEFORE iterator. 
		iterator insert(iterator current_iterator, const T & value)
		{
			auto *current = current_iterator.current;
			this->my_size++;
			return iterator(
				current->previous = current->previous->next =
				new node{ value, current->previous, current });
		}

		iterator insert(iterator current_iterator, T && value)
		{
			auto *current = current_iterator.current;
			this->my_size++;
			return iterator(
				current->previous = current->previous->next =
				new node{ std::move(value), current->previous, current });
		}

		// This will remove the value AT iterator. 
		iterator erase(iterator remove_iterator)
		{
			auto *current = remove_iterator.current;
			iterator value(current->previous);
			current->previous->next = current->next;
			current->next->previous = current->previous;
			delete current;
			this->my_size--;
			return value;
		}

	private:

		int my_size;
		node * head;
		node * tail;

		void init()
		{
			this->my_size = 0;
			this->head = new node;
			this->tail = new node;
			this->head->next = this->tail;
			this->tail->previous = this->head;
		}
	};

}

#endif