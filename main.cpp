#include <iostream>
#include <GLFW/glfw3.h>

int main(int argc, char *argv[]) {
  // Initialize the library
	if (!glfwInit()) {
    return -1;
	}

  glfwTerminate();

	std::cout << "Press any key to continue...";
	std::cin.ignore();

	return 0;
}
