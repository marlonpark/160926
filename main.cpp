#include <GLFW/glfw3.h>
#include <cstring>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <vector>
#include "math.h"

const int width = 360;
const int height = 250;

float* pixels = new float[width*height * 3];

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue);

class GeometricObject {
public:
	void (GeometricObject::*draw_surrounding_object_callback1)(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue) = nullptr;
	void (GeometricObject::*draw_surrounding_object_callback2)(const int& i0, const int& j0, const float& red, const float& green, const float& blue) = nullptr;
	void exec1() {
		if (draw_surrounding_object_callback1 != nullptr) {
			for (int i = 0; i < 2; i++) {
				(this->*draw_surrounding_object_callback1)(50 + 100 * (i * 2), 100, 100 + 100 * (i * 2), 150, 1.0f, 0.0f, 0.0f);
			}
			(this->*draw_surrounding_object_callback1)(25, 75, 125, 175, 1.0f, 0.0f, 0.0f);
		}
		if (draw_surrounding_object_callback2 != nullptr) {
			(this->*draw_surrounding_object_callback2)(275, 125, 1.0f, 0.0f, 0.0f);
		}
	}

	void drawBox(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue) {
		for (int i = i0; i <= i1; i++) {
			const int j = (j0 - j0)*(i - i0) / (i1 - i0) + j0;

			drawPixel(i, j, red, green, blue);
		}

		for (int i = i0; i <= i1; i++) {
			const int j = (j1 - j1)*(i - i0) / (i1 - i0) + j1;

			drawPixel(i, j, red, green, blue);
		}

		for (int j = j0; j <= j1; j++) {
			const int i = (i0 - i0)*(j - j0) / (j1 - j0) + i0;

			drawPixel(i, j, red, green, blue);
		}

		for (int j = j0; j <= j1; j++) {
			const int i = (i1 - i1)*(j - j0) / (j1 - j0) + i1;

			drawPixel(i, j, red, green, blue);
		}
	}

	void drawCircle(const int& i0, const int& j0, const float& red, const float& green, const float& blue) {
		int r = 50;
		for (int i = i0 - r; i < i0 + r; i++) {
			for (int j = j0 - r; j < j0 + r; j++) {
				int n = ((i - i0)*(i - i0) + (j - j0)*(j - j0) - r*r);
				int m = ((i - i0)*(i - i0) + (j - j0)*(j - j0) - (r - 1)*(r - 1));
				if (n < 0 && m>0) {
					drawPixel(i, j, red, green, blue);
				}
			}
		}
	}
};

void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue) {
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue) {
	for (int i = i0; i <= i1; i++) {
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;
		drawPixel(i, j, red, green, blue);
	}
}

void drawOnPixelBuffer() {
	std::fill_n(pixels, width*height * 3, 1.0f);	// white background

													//for (int i = 0; i<width*height; i++) {
													//	pixels[i * 3 + 0] = 1.0f; // red 
													//	pixels[i * 3 + 1] = 1.0f; // green
													//	pixels[i * 3 + 2] = 1.0f; // blue
													//}

	const int i = rand() % width, j = rand() % height;
	drawPixel(i, j, 0.0f, 0.0f, 0.0f);
}

int main(void) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

		drawOnPixelBuffer();

		GeometricObject my_box;

		my_box.draw_surrounding_object_callback1 = &GeometricObject::drawBox;

		my_box.exec1();

		GeometricObject my_circle;

		my_circle.draw_surrounding_object_callback2 = &GeometricObject::drawCircle;

		my_circle.exec1();

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array 

	return 0;
}