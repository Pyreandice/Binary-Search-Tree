#ifndef TREE_H_
#define TREE_H_

#include <algorithm>
#include <iostream>
#include <string>

namespace nwacc
{
	template<typename T>
	class tree
	{
	private:
		struct node
		{
			T element;
			node *left;
			node *right;
			node *parent;

			node(const T &the_element, node *left_node, node *right_node, node *parent_node) :
				element{ the_element }, left{ left_node }, right{ right_node }, parent{ parent_node } {}

			node(T &&the_element, node *left_node, node *right_node, node parent_node) :
				element{ std::move(the_element) }, left{ left_node }, right{ right_node }, parent{ parent_node } {}
		};

	public:
		tree() : root { nullptr } {}

		tree(const tree &rhs) : root { nullptr }
		{
			this->root = this->clone(rhs.root);
		}

		tree(tree &&rhs) noexcept : root { rhs.root }
		{
			rhs.root = nullptr;
		}

		~tree()
		{
			this->empty(this->root);
		}

		/**
		 * Insert a value at the current node.
		 */
		void insert(const T &value)
		{
			this->insert(value, this->root, nullptr);
		}

		/**
		 * Remove the value at the current node.
		 */
		void remove(const T &value)
		{
			this->remove(value, this->root);
		}

		/**
		 * Determine if the reference value is contained within
		 * the current node and return true or false.
		 */
		bool contains(const T &value) const
		{
			return this->contains(value, this->root);
		}

		/**
		 * Determine whether or not the current node is empty
		 * or if it is not.
		 */
		bool is_empty() const
		{
			return this->root == nullptr;
		}

		/**
		 * Overload the print operator.
		 * If the current tree is empty, print "Empty Tree"
		 * Otherwise, print the current tree in no order.
		 */
		void print(std::ostream &out = std::cout)
		{
			if (this->is_empty())
			{
				out << "Empty Tree" << std::endl;
			}
			else
			{
				this->print(this->root, out);
			}
		}

		/**
		 * Overload the print_in_order operator
		 * If the current tree is empty, print "Empty Tree"
		 * Otherwise, start at the "first" value in the tree.
		 */
		void print_in_order(std::ostream &out = std::cout)
		{
			if (this->is_empty())
			{
				out << "Empty Tree" << std::endl;
			}
			else
			{
				this->print_in_order(this->root, out);
			}
		}

		/**
		 *Overload the print_post_order operator
		 * If the current tree is empty, print "Empty Tree"
		 * Otherwise, start at the last value in the tree.
		 */
		void print_post_order(std::ostream &out = std::cout)
		{
			if (this->is_empty())
			{
				out << "Empty Tree" << std::endl;
			}
			else
			{
				this->print_post_order(this->root, out);
			}
		}

		/**
		* Find the first node in the tree.
		* If the current value stored within the current node is a null pointer,
		* then return nullptr.
		* Else return the current value stored within the current node.
		*/
		node *begin(tree &value) const
		{
			if (value.root == nullptr)
			{
				return nullptr;
			}
			else
			{
				return find_min(value);
			}
		}

		class iterator
		{
		public:
			iterator() : current{ nullptr } {}

			/**
			 * Overload the pointer operator.
			 */
			T &operator*()
			{
				return this->retrieve();
			}

			/**
			 * Overload the ++ prefix operator.
			 */
			iterator &operator++()
			{
				this->current = this->current->right;
				return *this;
			}

			/**
			* This is the ++ postfix operator.
			*/
			iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			/**
			 * Overload the prefix -- operator.
			 */
			iterator &operator--()
			{
				this->current = this->current->left;
				return *this;
			}

			/**
			 * Overload the postfix -- operator.
			 */
			iterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

			/**
			* Return the comparison of the current node vs the rhs
			* of the current node are equal to each other.
			*/
			bool operator== (const iterator &rhs)
			{
				return this->current == rhs.current;
			}

			/**
			 * Return the comparison of the current node vs the rhs
			 * node is not equal to each other.
			 */
			bool operator!= (const iterator &rhs)
			{
				return !(*this == rhs);
			}

		private:
			node *current;

			T &retrieve()
			{
				return this->current->data;
			}

			iterator(node *current) : current{ current } {}

			friend class tree<T>;
		};

		class const_iterator
		{
		public:

			const_iterator() : current{ nullptr } {}

			/**
			 * Overload the pointer operator.
			 */
			const T &operator*() const
			{
				return this->retrieve();
			}

			// prefix ++ operator
			const_iterator &operator++()
			{
				this->current = this->current->right;
				return *this;
			}

			/**
			 * This is the postfix operator.
			 */
			const_iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			/**
			 * This is the prefix operator.
			 */
			const_iterator &operator--()
			{
				this->current = this->current->left;
				return *this;
			}

			/**
			 * This is the postfix operator.
			 */
			const_iterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

			/**
			 * Return the comparison of the current node vs the rhs
			 * of the current node are equal to each other.
			 */
			bool operator== (const const_iterator &rhs) const
			{
				return this->current == rhs.current;
			}

			/**
			 * Return the comparison of the current node vs the rhs
			 * node is not equal to each other.
			 */
			bool operator!= (const const_iterator &rhs) const
			{
				return !(*this == rhs);
			}

		protected:
			node *current;

			/**
			 * Retrieve the data that is stored in the current node.
			 */
			T &retrieve() const
			{
				return this->current->data;
			}

			const_iterator(node *current) : current{ current } {}

			friend class tree<T>;
		};

	private:

		node *root;

		/**
		 * Make a clone of the current node for manipulation and restructuring
		 * of the current tree set.
		 */
		node *clone(node *current) const
		{
			if (current == nullptr)
			{
				return nullptr;
			}
			else
			{
				return new node{ current->element, this->clone(current->left), this->clone(current->right), this->clone(current->parent) };
			}
		}

		/**
		 * Print the tree set in before it has been balanced.
		 * Also known as "Pre-Order".
		 */
		void print(node *current, std::ostream &out) const
		{
			if (current != nullptr)
			{
				// order is pre-order
				out << current->element << std::endl;
				this->print(current->left, out);
				this->print(current->right, out);
				//this->print(current->parent, out);
			}
		}

		void print_post_order(node *current, std::ostream &out) const
		{
			if (current != nullptr)
			{
				// order is post-order
				this->print_post_order(current->left, out);
				this->print_post_order(current->right, out);
				//this->print(current->parent, out);
				out << current->element << std::endl;
			}
		}

		void print_in_order(node *current, std::ostream &out) const
		{
			if (current != nullptr)
			{
				// order is in-order.
				this->print_in_order(current->left, out);
				out << current->element << std::endl;
				this->print_in_order(current->right, out);
				//this->print(current->parent, out);
			}
		}

		/**
		 * If the current node is empty, then delete the current node
		 * from the tree.
		 */
		void empty(node *&current)
		{
			if (current != nullptr)
			{
				this->empty(current->left);
				this->empty(current->right);
				delete current;
			}
			current = nullptr;
		}

		/**
		 * Determine if the reference value is currently contained within
		 * the current node and return either true or false.
		 */
		bool contains(const T &value, node *current) const
		{
			while (current != nullptr)
			{
				if (value < current->element)
				{
					current = current->left;
				}
				else if (current->element < value)
				{
					current = current->right;
				}
				else
				{
					return true;
				}
			}
			return false;
		}

		/**
		 * Insert a value at the current node. If the current node is equal to
		 * a null pointer. Then create a new node at the current location.
		 * If the current node is less than the right node, insert the value
		 * at the left node.
		 * If the current node is greater than the right node, insert the value
		 * at the right node.
		 * If a duplicate value is found. Do nothing.
		 */
		iterator insert(const T &value, node *&current, node *parent)
		{
			if (current == nullptr)
			{
				if (parent == nullptr)
				{
					current = new node{ value, nullptr, nullptr, nullptr };
					root = current;
					return iterator( current );
				}
				else
				{
					current = new node{ value, nullptr, nullptr, parent };
					return iterator( current );
				}
			}
			else if (value < current->element)
			{
				this->insert(value, current->left, current);
			}
			else if (current->element < value)
			{
				this->insert(value, current->right, current);
			}
			else
			{
				// we found a duplicate. do_nothing();
				return{};
			}
		}

		/**
		 * Remove the current node from the tree and replace it with a nullptr.
		 */
		void remove(const T &value, node *&current)
		{
			if (current == nullptr)
			{
				// we did not find the item to remove, we found nullptr. 
				return;
			} // else, we are still looking, do_nothing();

			if (value < current->element)
			{
				this->remove(value, current->left);
			}
			else if (current->element < value)
			{
				this->remove(value, current->right);
			}
			else if (current->left != nullptr && current->right != nullptr)
			{
				// we have two children!
				current->element = this->find_min(current->right)->element;
				this->remove(current->element, current->right);
			}
			else
			{
				// we have either one child or no children :(
				node *old_node = current;
				current = (current->left != nullptr) ? current->left : current->right;
				delete old_node;
			}
		}

		/**
		 * Find the left most node in the tree. This should represent the
		 * lowest value in the tree.
		 */
		node *find_min(node *current) const
		{
			if (current == nullptr)
			{
				return nullptr;
			} // else, current is not null, do_nothing();

			if (current->left == nullptr)
			{
				return current;
			} // else, we still have more children do_nothing();

			return this->find_min(current->left);
		}

		/**
		 * Finds the next node in the tree set.
		 * If the right child of the current node is equal to a null pointer,
		 * then return the value of the current node as the smallest value in the tree set.
		 * Else the current node is a child. Move up the tree.
		 */
		node *find_next_node(node *current)
		{
			if (current->right != nullptr)
			{
				return find_min(current->right);
			}
			else
			{
				while (current->parent != nullptr && current == current->parent->right)
				{
					current = current->parent;
				}
				return current->parent;
			}
		}
	};
}

#endif // TREE_H_
