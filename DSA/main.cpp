// Author: Grayson Beam
// Creates a tree and populates it with random values.
// Prints the tree in order, pre order, and post order.

#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include "tree.h"

int main()
{
	nwacc::tree<int> bst;
	for (auto counter = 0; counter <= 49; counter++)
	{
		bst.insert(rand() % 100);
	}

	std::cout << "Print pre-order:" << std::endl;
	bst.print();
	std::cout << "Print in-order:" << std::endl;
	bst.print_in_order();
	std::cout << "Print post-order:" << std::endl;
	bst.print_post_order();

	return 0;
}