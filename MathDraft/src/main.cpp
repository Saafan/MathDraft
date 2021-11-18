#include "ImageProcessing.h"
using namespace cv;


int main()
{
	srand(time(NULL));

	Mat image = imread("images/paint.jpg");

	Mat mat(HEIGHT, WIDTH, CV_8UC3);

	Render(mat);

	String windowName = "3D Image"; //Name of the window

	namedWindow(windowName); // Create a window

	imshow(windowName, mat); // Show our image inside the created window.

	waitKey(0); // Wait for any keystroke in the window

	destroyWindow(windowName); //destroy the created window

}