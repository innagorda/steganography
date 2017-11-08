// CannyStill.cpp

#include<opencv2/opencv.hpp>
#include <iostream>
#include <conio.h>           // may have to modify this line if not using Windows
#include <type_traits>
#include <bitset>

template <typename T>
T get_mask(size_t pos, size_t n)
{
	return ~(~static_cast<typename std::make_unsigned<T>::type>(0) << n) << pos;
}

cv::Mat save_img_dim(cv::Mat image, int h, int w) {
	//write width and height (16*16 bit) of hide image into first 16 pixels. Use R and G channels.

	//std::cout << "height: " << h << " width: " << w << std::endl;

	int n = 0, j = 0, bit_h = 0, bit_w = 0;

	for (int i = 0; i < 16; i++) {
			//std::cout << "before " << (int)(image.at<cv::Vec3b>(i, 0)[0]) << ' ' << (int)(image.at<cv::Vec3b>(i, 0)[1]) << std::endl;

			auto mask = get_mask<int>(15-i, 1);

			bit_h = (h & mask) >> 15-i;
			bit_w = (w & mask) >> 15-i;

			image.at<cv::Vec3b>(0, i)[0] = image.at<cv::Vec3b>(0, i)[0] & get_mask<int>(1, 7) | bit_h;
			image.at<cv::Vec3b>(0, i)[1] = image.at<cv::Vec3b>(0, i)[1] & get_mask<int>(1, 7) | bit_w;
			//std::cout << "after " << (int)(image.at<cv::Vec3b>(i, 0)[0]) << ' ' << (int)(image.at<cv::Vec3b>(i, 0)[1]) << std::endl;
		}
	return image;
}



cv::Mat hide_image(cv::Mat image, cv::Mat hide_image) {
	
	int i_current = 0, j_current = 16;
	int curr_bit = 0;

	//if (image.rows * image.cols < hide_image.rows * hide_image.cols * 8 + 2)
	//	throw 1;

	image = save_img_dim(image, hide_image.cols, hide_image.rows);

	for(int i = 0; i < hide_image.rows; i++)
		for (int j = 0; j < hide_image.cols; j++)
			for (int bit = 0; bit < 8; bit++) {

				if (j_current < image.cols - 1) 
					j_current++;
				else {
					i_current++;
					j_current = 0;
				}

				//std::cout << (int)image.at<cv::Vec3b>(i_current, j_current)[0] << " " << (int)image.at<cv::Vec3b>(i_current, j_current)[1]
				//	<< " " << (int)image.at<cv::Vec3b>(i_current, j_current)[2] << std::endl;

				auto mask = get_mask<int>(8-bit, 1);

				for (int channel = 0; channel < 3; channel++) {

					curr_bit = (hide_image.at<cv::Vec3b>(i, j)[channel] & mask) >> 8-bit;
				//	std::cout << curr_bit;
					//std::cout << " i=" << i << " j=" << j << " i_current=" << i_current << " j_current=" << j_current << std::endl;
					image.at<cv::Vec3b>(i_current, j_current)[channel] = image.at<cv::Vec3b>(i_current, j_current)[channel] & get_mask<int>(1, 7) | curr_bit;

				}
				//std::cout << std::endl;


				//std::cout << (int)image.at<cv::Vec3b>(i_current, j_current)[0] << " " << (int)image.at<cv::Vec3b>(i_current, j_current)[1]
				//	<< " " << (int)image.at<cv::Vec3b>(i_current, j_current)[2] << std::endl;
			}
	return image;
}

void get_img_dim(cv::Mat image, int &h, int &w) {
	//write width and height (16*16 bit) of hide image into first 16 pixels. Use R and G channels.

	int n = 0,  bit_h = 0, bit_w = 0;

	for (int i = 0; i < 16; i++) {
		//std::cout << "before " << (int)(image.at<cv::Vec3b>(i, 0)[0]) << ' ' << (int)(image.at<cv::Vec3b>(i, 0)[1]) << std::endl;

		//auto mask = get_mask<int>(i, 1);

		bit_h = (image.at<cv::Vec3b>(0, i)[0] & 1);
		bit_w = (image.at<cv::Vec3b>(0, i)[1] & 1);

		h = (h << 1) | bit_h;
		w = (w << 1) | bit_w;
		//std::cout << "after " << (int)(image.at<cv::Vec3b>(i, 0)[0]) << ' ' << (int)(image.at<cv::Vec3b>(i, 0)[1]) << std::endl;
	}
	
}



cv::Mat get_image(cv::Mat image) {
	
	cv::Mat hide_image;
	int hide_image_cols = 0, hide_image_rows = 0;
	int i_current = 0, j_current = 16;
	int curr_bit = 0;

	get_img_dim(image, hide_image_cols, hide_image_rows);

	hide_image = cv::Mat(hide_image_rows, hide_image_cols, CV_8U);
	hide_image = cv::Mat::zeros(hide_image_rows, hide_image_cols, CV_8U);
	//std::cout << (hide_image.at<cv::Vec3b>(671, 298)) << std::endl;
	for (int i = 0; i < hide_image.rows; i++)
		for (int j = 0; j < hide_image.cols; j++)
			for (int bit = 0; bit < 8; bit++) {

				if (j_current < image.cols - 1)
					j_current++;
				else {
					i_current++;
					j_current = 0;
				}

				//std::cout << (int)image.at<cv::Vec3b>(i_current, j_current)[0] << " " << (int)image.at<cv::Vec3b>(i_current, j_current)[1]
				//	<< " " << (int)image.at<cv::Vec3b>(i_current, j_current)[2] << std::endl;
				std::cout << i << " cols " << j << std::endl;
				std::cout << (hide_image.at<cv::Vec3b>(i, j)) << std::endl;
				for (int channel = 0; channel < 3; channel++) {
					
					curr_bit = (image.at<cv::Vec3b>(i_current, j_current)[channel] & 1);
				
					hide_image.at<cv::Vec3b>(i, j)[channel] = (hide_image.at<cv::Vec3b>(i, j)[channel] << 1) | curr_bit;

				}
				//std::cout << std::endl;


				//std::cout << (int)image.at<cv::Vec3b>(i_current, j_current)[0] << " " << (int)image.at<cv::Vec3b>(i_current, j_current)[1]
				//	<< " " << (int)image.at<cv::Vec3b>(i_current, j_current)[2] << std::endl;
			}
	return hide_image;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	cv::Mat imgOriginal;        // input image
	cv::Mat imgHidden;          // image which should be hidden in input image
	cv::Mat imgChanged;         // image contains hidden image
	cv::Mat imgFound;
	

	imgOriginal = cv::imread("image.bmp");          // open image
	imgHidden = cv::imread("hi.jpg");          // open image

	if (imgOriginal.empty() || imgHidden.empty()) {                                  // if unable to open image
		std::cout << "error: image not read from file\n\n";     // show error message on command line
		_getch();                                               // may have to modify this line if not using Windows
		return(0);                                              // and exit program
	}

	//try {
		imgChanged = hide_image(imgOriginal, imgHidden);
	/*}
	catch (...) {
		printf_s("Caught exception from hide_image\n");
	}*/
	
		imgFound = get_image(imgChanged);
	cv::imwrite("image_with_hidden_image.bmp", imgChanged);
									// declare windows
	//cv::namedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);     // note: you can use CV_WINDOW_NORMAL which allows resizing the window
	//cv::namedWindow("imgChanged", CV_WINDOW_AUTOSIZE);        // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
															// CV_WINDOW_AUTOSIZE is the default
	
	cv::namedWindow("imgFound", CV_WINDOW_AUTOSIZE);														
	//cv::imshow("imgOriginal", imgOriginal);     // show windows
	//cv::imshow("imgChanged", imgChanged);
	cv::imshow("imgFound", imgFound);

	cv::waitKey(0);                 // hold windows open until user presses a key

	return(0);
}
