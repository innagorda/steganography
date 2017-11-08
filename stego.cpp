// CannyStill.cpp

#include<opencv2/opencv.hpp>

#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows
#include <type_traits>
#include <bitset>

template <typename T>
T get_mask(size_t pos, size_t n)
{
	return ~(~static_cast<typename std::make_unsigned<T>::type>(0) << n) << pos;
}

cv::Mat save_img_dim(cv::Mat image, int h, int w) {
	//16*16 bit
	std::cout << h << " width: " << w << std::endl;

	int n = 0, j = 0, bit = 0;

	for (int i = 0; i < 16; i++) {
			std::cout << "before " << (int)(image.at<cv::Vec3b>(i, 0)[0]) << ' ' << (int)(image.at<cv::Vec3b>(i, 0)[1]) << std::endl;

			auto mask = get_mask<int>(i, 1);

			bit = (h & mask) >> i;

			std::cout << "mask = " << (int)mask	<< ", value = "  << bit << std::endl;

			image.at<cv::Vec3b>(i, j)[0] = image.at<cv::Vec3b>(i, j)[0] & get_mask<int>(1, 7) | bit;
			//image.at<cv::Vec3b>(i, j)[1] = 255;
			std::cout << "after " << (int)(image.at<cv::Vec3b>(i, 0)[0]) << ' ' << (int)(image.at<cv::Vec3b>(i, 0)[1]) << std::endl;
		}
	cv::Mat changed_image = image;
	return changed_image;
}

cv::Mat split_pixels(cv::Mat image) {

	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++) {
			image.at<cv::Vec3b>(i, j)[0] = 0;  //turn the pixel value @ (k,i) to yellow (0,255,255)
			image.at<cv::Vec3b>(i, j)[1] = 255;
			image.at<cv::Vec3b>(i, j)[2] = 255;
		}
	return image;
}

cv::Mat hide_image(cv::Mat original_image, cv::Mat hide_image) {
	cv::Mat changed_image=original_image;
	std::vector<cv::Mat> channels;
	cv::split(original_image, channels);

	for(int i = 0; i < original_image.rows; i++)
		for (int j = 0; j < original_image.cols; j++) {
			std::cout << (int)(channels[0].at<uchar>(i, j))  << " " << (int)channels[1].at<uchar>(i, j) << " " << (int)channels[2].at<uchar>(i, j) << std::endl;
			return changed_image;
		}
	return changed_image;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	cv::Mat imgOriginal;        // input image
	cv::Mat imgHidden;          // image which should be hidden in input image
	cv::Mat imgChanged;         // image contains hidden image
	
	

	imgOriginal = cv::imread("image.bmp");          // open image
	imgHidden = cv::imread("hi.jpg");          // open image

	if (imgOriginal.empty() || imgHidden.empty()) {                                  // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return(0);                                              // and exit program
	}

	//imgChanged = hide_image(imgOriginal, imgHidden);
	imgChanged = save_img_dim(imgOriginal, imgHidden.cols, imgHidden.rows);
									// declare windows
	//cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window
	cv::namedWindow("imgChanged", CV_WINDOW_AUTOSIZE);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
															// CV_WINDOW_AUTOSIZE is the default
	//cv::imshow("imgOriginal", imgOriginal);     // show windows
	//cv::imshow("imgChanged", imgChanged);

	cv::waitKey(0);                 // hold windows open until user presses a key

	return(0);
}
