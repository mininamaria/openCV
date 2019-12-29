#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
using namespace cv;
using namespace std;

struct parameters {
	int num_x = 31;	// Кол-во знакомест по горизонтали             
	int num_y = 15;	// Кол-во знакомест по вертикали               
	double delta_x = 21.5; // Ширина одного знакоместа             
	double delta_y = 33.0; // Длина одного знакоместа                   
	int start_x = 28; // Откуда строим линиии по вертикали         
	int start_y = 23; // Откуда строим линии по горизонтали        
	vector<vector<bool>> positions;
};

void lb_mouse_callback(int event, int x, int y, int flags, void* param) {
		cout << x << ' ' << y << endl;
}

int main(int argc, char** argv)
{
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " filename.jpg" << endl;
		exit(1);
	}
	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	if (api->Init(NULL,"rus")) {
		cerr << "Could not initialize tesseract" << endl;
		exit(1);
	}
	cv::Mat dst;
	cv::setMouseCallback(argv[1], lb_mouse_callback, (void*)&dst);
	api->SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
	int fixed_threshold = 110;													// Наиболее оптимальное значение
	int block_size = 3;
	int offset = 0;
	int inv = 0;
	cv::namedWindow(argv[1], cv::WINDOW_AUTOSIZE);
	cv::Mat frame = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	cv::createTrackbar("fix_threshold", argv[1], &fixed_threshold, 255, NULL, NULL);
	cv::createTrackbar("block_size", argv[1], &block_size, 255, NULL, NULL);
	cv::createTrackbar("offset", argv[1], &offset, 255, NULL, NULL);
	cv::createTrackbar("inv", argv[1], &inv, 1, NULL, NULL);
	// Thresholds.
//
	
//	cv::adaptiveThreshold(Igray,Iat,255,adaptive_method,threshold_type,block_size,offset);


	

	while (true) {
		cv::threshold(frame, dst, double(fixed_threshold), 255, inv);
//		block_size += 1 - block_size % 2;
//		cv::adaptiveThreshold(frame, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, inv, block_size, offset);
		cv::imshow(argv[1], dst);
		int key = cv::waitKey(100);
		if (key == 27) break;
		else if (key == 32) {
			cv::Rect text1ROI(24, 26, 23, 27);
			api->SetVariable("user_defined_dpi", "96");
			api->TesseractRect(dst.data, 1, (int)dst.step1(), text1ROI.x, text1ROI.y, text1ROI.width, text1ROI.height);
			//api->SetImage((uchar*)sub.data, sub.size().width, sub.size().height, sub.channels(), (int)sub.step1());
			api->Recognize(0);
			char* outText;
			outText = api->GetUTF8Text();
			// convert multibyte UTF-8 to wide string UTF-16
			int length = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)outText, -1, NULL, 0);
			if (length > 0)
			{
				wchar_t* wide = new wchar_t[length];
				MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)outText, -1, wide, length);
				// convert it to ANSI
				char* ansi = new char[length];
				WideCharToMultiByte(CP_ACP, 0, wide, -1, ansi, length, NULL, NULL);
				//if (ansi[0] == 'Q')
				//	printf("%u - OCR output ANSI:\n%s", 1, ansi); // раньше фар работал в 1251, а тут смотрю 866
				WideCharToMultiByte(CP_OEMCP, 0, wide, -1, ansi, length, NULL, NULL);
				printf("%u - OCR output OEM:\n%s", 1, ansi);
				delete[] wide;
				delete[] ansi;
			}
			//printf("%u - OCR output UTF-8:\n%s", 1, outText);

			// Destroy used object and release memory
			delete[] outText;

		}
	}
	cv::destroyWindow(argv[1]);

	return 0;
}