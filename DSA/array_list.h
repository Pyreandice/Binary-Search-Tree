#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <algorithm>
#include <stdexcept>

namespace nwacc
{
	template <typename T>
	class array_list
	{
	public:

		explicit array_list(int capacity = kDefaultCapacity) :
		my_size { 0 }, capacity { capacity >= kDefaultCapacity ? capacity : kDefaultCapacity }
		{
			this->data = new T[this->capacity];
		}

		// copy constructor. 
		array_list(const array_list &rhs) :
		my_size { rhs.my_size }, capacity { rhs.capacity }, data { nullptr }
		{
			this->data = new T[this->capacity];
			for (auto index = 0; index < this->my_size; index++)
			{
				this->data[index] = rhs.data[index];
			}
		}

		array_list(array_list &&rhs) :
		my_size { rhs.my_size }, capacity { rhs.capacity }, data { rhs.data }
		{
			rhs.data = nullptr;
			rhs.my_size = 0;
			rhs.capacity = 0;
		}

		~array_list()
		{
			delete[] this->data;
		}

		void push_back(T &&value)
		{
			if (this->size() == this->capacity)
			{
				this->reserve();
			} // else, my_size is within range, do_nothing();
			this->data[this->my_size++] = std::move(value);
		}

		void push_back(const T &value)
		{
			if (this->size() == this->capacity)
			{
				this->reserve();
			} // else my_size is within range, do_nothing();

			this->data[this->my_size++] = value;
		}

		bool is_empty() const
		{
			return this->size() == 0;
		}

		/**
		 * Summary
		 * @return what does this return. 
		 */
		int size() const
		{
			return this->my_size;
		}

		int get_capacity() const
		{
			return this->capacity;
		}

		array_list &operator=(const array_list &rhs)
		{
			auto copy = rhs;
			std::swap(*this, copy);
			return *this;
		}

		array_list &operator=(array_list &&rhs)
		{
			std::swap(this->my_size, rhs.my_size);
			std::swap(this->capacity, rhs.capacity);
			std::swap(this->data, rhs.data);
			return *this;
		}

		void pop_back()
		{
			if (this->is_empty())
			{
				throw std::out_of_range("No elements in array list");
			} // else, we are not empty do_nothing();

			this->my_size--;
		}

		const T & back() const
		{
			if (this->is_empty())
			{
				throw std::out_of_range("No elements in the array list");
			} // else, we are not empty, do_nothing();

			return this->data[this->my_size - 1];
		}

	private:
		static const int kDefaultCapacity = 16;

		/*
		* what is this attribute
		*/
		int my_size;
		int capacity;
		T * data;

		void reserve()
		{
			if (this->size() < this->capacity)
			{
				return;
			} // else, we need to change the capacity do_nothing();

			// NOTE - Magic numbers here are fine! 
			auto new_capacity = (this->capacity * 3) / 2 + 1;
			T * new_array = new T[new_capacity];
			for (auto index = 0; index < this->size(); index++)
			{
				new_array[index] = std::move(this->data[index]);
			}

			this->capacity = new_capacity;
			std::swap(this->data, new_array);
			delete[] new_array;
		}
	};
}

#endif