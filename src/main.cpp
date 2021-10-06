#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <limits.h>

using namespace cv;
using namespace std;

#define result_name  "result"

void display_input(string l_dir_, string r_dir_) {
	fprintf(stdout, "input left image : %s\n", l_dir_);
	fprintf(stdout, "input right image : %s\n", r_dir_);
}

bool read_img(string l_dir, string r_dir, Mat& left_img, Mat& right_img) {
	Mat l_img_ = imread(l_dir);
	Mat r_img_ = imread(r_dir);
	if (l_img_.empty() || r_img_.empty()) { fprintf(stdout, "assert(l_img_.empty() || r_img_.empty())"); return false; }

	l_img_.copyTo(left_img);
	r_img_.copyTo(right_img);
	
	if (left_img.size().width != right_img.size().width || left_img.size().height != right_img.size().height) {
		fprintf(stdout, "warning :: left size != right size\n");
		fprintf(stdout, "resizing...\n");
		int img_w = min(left_img.size().width, left_img.size().width);
		int img_h = min(right_img.size().height, right_img.size().height);
		resize(left_img, left_img, Size(img_w, img_h), 0, 0, INTER_CUBIC);
		resize(right_img, right_img, Size(img_w, img_h), 0, 0, INTER_CUBIC);

	//	return false;
	}

	return true;
}

bool write_img(string out_name, Mat& c_img_) {
	/*
	int out_last_pos = in_name.size();
	string format_str = in_name.substr(out_last_pos-4, out_last_pos);
	string img_name = result_name + format_str;
	*/

	fprintf(stdout,  "output image : %s\n", out_name);
	
	imwrite(out_name, c_img_);

	return true;
}

inline int clipping(int el) {
	return el < UCHAR_MAX ? el > 0 ? el : 0 : UCHAR_MAX;
}

bool anaglyph_maker(Mat& left_img, Mat& right_img, Mat& combined_img) {

	int img_w = left_img.size().width;
	int img_h = left_img.size().height;
	int img_area = left_img.size().area();

	Mat c_img(img_h, img_w, CV_8UC3);

	uchar* l_data = left_img.data;
	uchar* r_data = right_img.data;

	
	uchar* c_data = c_img.data;

	int idx = 0;


	unsigned int r_alpha, l_alpha;
	unsigned int alpha;
	unsigned int r, g, b;

	for (int i = 0; i < img_area; i++) {
		c_data[i * 3] = r_data[i * 3];
		c_data[i * 3 + 1] = r_data[i * 3 + 1];
		c_data[i * 3 + 2] = l_data[i * 3 + 2];
	}

	c_img.copyTo(combined_img);
	return true;
}


//usage example : ./dataset/Piano_im0.png ./dataset/Piano_im1.png result.png
void main(int argc, char* argv[]) {

	string left_file = argv[1];
	string right_file = argv[2];
	string output_file = argv[3];
	
	display_input(left_file, right_file);

	Mat left_img, right_img, combined_img;
	if (!read_img(left_file, right_file, left_img, right_img) || !anaglyph_maker(left_img, right_img, combined_img)) { fprintf(stdout, "error\n"); return; }

	write_img(output_file, combined_img);
	fprintf(stdout, "finish\n");
	return;
}
