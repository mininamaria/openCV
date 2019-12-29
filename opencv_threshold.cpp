
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " filename.jpg" << endl;
	}
	int fixed_threshold = 110;														// Наиболее оптимальное значение
	cv::namedWindow(argv[1], cv::WINDOW_AUTOSIZE);
	cv::Mat frame = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	cv::Mat dst;
	cv::createTrackbar("fix_threshold", argv[1], &fixed_threshold, 255, NULL, NULL);
	cv::createTrackbar("trackbar2", argv[1], 0, 100, NULL, NULL);
	cv::createTrackbar("trackbar3", argv[1], 0, 100, NULL, NULL);
	// Thresholds.
//
	
//	cv::adaptiveThreshold(Igray,Iat,255,adaptive_method,threshold_type,block_size,offset);


	

	while (true) {
		cv::threshold(frame, dst, double(fixed_threshold), 255, cv::THRESH_BINARY);
		cv::imshow(argv[1], dst);
		if (cv::waitKey(100) == 27) break;
	}
	cv::destroyWindow(argv[1]);

	return 0;
}