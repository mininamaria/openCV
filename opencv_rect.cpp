#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
using namespace cv;
using namespace std;

//THIS CODE IS FOR PARSING CONFIG FILES
void clear_spaces(string& s) {

	string tmp;
	for (int i = 0; i < (int)s.size(); i++)
		if (!isspace(s[i]))
			tmp += s[i];
	s = tmp;

}

struct parameters {
	int num_x = 1;	// Кол-во знакомест по горизонтали             
	int num_y = 2;	// Кол-во знакомест по вертикали               
	double delta_x = 21.5; // Ширина одного знакоместа             
	double delta_y = 33.0; // Длина одного знакоместа                   
	int start_x = 28; // Откуда строим линиии по вертикали         
	int start_y = 23; // Откуда строим линии по горизонтали        
	vector<vector<bool>> positions;
};


bool parse_config(parameters& params, string filename) {
	map<string, double> kv;
	filename = filename.substr(0, filename.find(".")) + ".cfg";
	ifstream fin(filename.c_str());
	if (!fin.is_open()) {
		cout << "Can't open " << filename << endl;
		return false;
	}
	std::string line;
	while (getline(fin, line)) {
		cout << endl <<"$debug";
		clear_spaces(line);
		if (line[0] != '/' && !isdigit(line[0]) && line.size() > 1) {
			string key = line.substr(0, line.find("="));
			string value = line.substr(line.find("=") + 1);
			cout << '"' << key << "\" \"" << value << '"' << endl;
			kv[key] = stod(value);
		}
		else if (line[0] == '0' || line[0] == '1') {
			vector<bool> temp;
			for (auto s : line) {
				if (s == '0') temp.push_back(false);
				else if (s == '1') temp.push_back(true);
				else { 
					cout << "Parse config error. Waiting for 0 or 1. Got" << s << endl;
					return -1;
				}
				cout << temp.back();
			}
			params.positions.push_back(temp);
		}
	}
	fin.close();
	if (kv.count("delta_x"))
		params.delta_x = kv.at("delta_x");
	else
		return false;
	if (kv.count("delta_y"))
		params.delta_y = kv.at("delta_y");
	else
		return false;
	if (kv.count("start_x"))
		params.start_x = (int)kv.at("start_x");
	else
		return false;
	if (kv.count("start_y"))
		params.start_y = (int)kv.at("start_y");
	else
		return false;
	if (kv.count("num_x"))
		params.num_x = (int)kv.at("num_x");
	else
		return false;
	if (kv.count("num_y"))
		params.num_y = (int)kv.at("num_y");
	else
		return false;
	if ((int)params.positions.size() != params.num_y) {
		cout << params.positions.size() << " != " << params.num_y << endl;
		return false;
	}

	for (auto v : params.positions) {
		if ((int)v.size() != (int)params.num_x) {
			cout << v.size() << " != " << (int)params.num_x << endl;
			return false;
		}
	}
	return true;
}
// -----

int main(int argc, char** argv)
{
	parameters params;
	
	int counter = 0;
	const cv::Scalar Color(100, 0, 255);
	const String windowName = "The_Window";							// Name of the window
	namedWindow(windowName, cv::WINDOW_AUTOSIZE);					// Create a window
	cv::VideoCapture cap;											// Create an object for getting images
	Mat frame;														// Кадр
	if (argc > 1)
		cap.open(string(argv[1]));
	else {
		cout << "Usage: progname video.mpg" << endl;
		return -1;
	}
	if (!parse_config(params, string(argv[1]))) {
		cout << "Read config error" << endl;
		return -1;
	}
	for (int i = 0; i < params.num_y; i++) {
		for (int j = 0; j < params.num_x; j++) {
			//cout << params.positions.at(i).at(j);
			cout << params.positions[i][j];
		}
		cout << endl;
	}
	while (1) {
		cap >> frame;
		if (frame.empty()) break;											// The end of the movie
		
		
		/*
		for (int i = 0; i < params.num_y; i++) {
			for (int j = 0; j < params.num_x; j++) {
				cv::Point tl((int)(params.start_x + params.delta_x * j), (int)(params.start_y + params.delta_y * i));											// Top / Left
				cv::Point br((int)(params.start_x + params.delta_x * (j+1)), (int)(params.start_y + params.delta_y * (i+1)));									// Bottom / Right
				if (params.positions.at(i).at(j)) {
					cv::rectangle(frame, tl, br, Color, 1, 0);
				}
			}

		}
		
		*/
		
		// Capturing frames
		cv::imshow(windowName, frame);
		if (cv::waitKey(33) >= 0) break;	
		if (counter % 100 == 0) {
			const string filename = "capture" + to_string(counter/100) + ".jpg";
			imwrite(filename, frame);
			cout << frame.rows << " " << frame.cols << endl;
		}				
		counter++;
		
	}
	return 0;
}
