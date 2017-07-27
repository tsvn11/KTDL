#include <Windows.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "def.h"

vector<ThuocTinh> removeProperty(vector<ThuocTinh> propList, vector<string> removeList); //câu a
bool isDouble(string str);
double stringToDouble(string str);
string doubleToString(double dbl);
vector<ThuocTinh> MinMax(vector<ThuocTinh> propList); //câu b
vector<ThuocTinh> zScore(vector<ThuocTinh> propList); //câu c
vector<ThuocTinh> removeMissingInstance(vector<ThuocTinh> propList, vector<string> removeList); //câu f

// When passing char arrays as parameters they must be pointers
int main(int argc, char* argv[]) {
    if (argc < 11) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
		// Inform the user of how to use the program
		std::cout << "Usage is --in <infile> --out <outdir> --task <a|b|c|d|e|f> --proplist <{id,name}> --bin <number>\n";
		std::cout << "Task a: Xoa danh sach thuoc tinh\n";
		std::cout << "Task b: Chuan hoa min-max\n";
		std::cout << "Task c: Chuan hoa Z-scores\n";
		std::cout << "Task d: Chia gio theo do rong\n";
		std::cout << "Task e: Chia gio theo do sau\n";
		std::cout << "Task f: Xoa mau du lieu thieu\n";
        std::cin.get();
        exit(0);
    } else { // if we got enough parameters...
		std::string inFile, outFile, taskName, proplist, bin;
        std::cout << argv[0];
        for (int i = 1; i < argc; i++) { /* We will iterate over argv[] to get the parameters stored inside.
                                          * Note that we're starting on 1 because we don't need to know the 
                                          * path of the program, which is stored in argv[0] */
            if (i + 1 != argc) // Check that we haven't finished parsing already
				if (strcmp(argv[i], "--in") == 0) {
                    // We know the next argument *should* be the filename:
                    inFile = argv[i + 1];
                } else if (strcmp(argv[i], "--out") == 0) {
                    outFile = argv[i + 1];
                } else if (strcmp(argv[i], "--task") == 0) {
                    taskName = argv[i + 1];
				} else if (strcmp(argv[i], "--proplist") == 0) {
					proplist = argv[i + 1];
				} else if (strcmp(argv[i], "--bin") == 0) {
					bin = argv[i + 1];
            }
            std::cout << argv[i] << " ";
        }

        //... some more code
		
		/* Read input file */


		/* Execute task */
		if (taskName.compare("a") == 0)
		{
			std::cout << "Execute task a";
		}
		else if (taskName.compare("b") == 0)
		{
			std::cout << "Execute task b";
		}
		else if (taskName.compare("c") == 0)
		{
			std::cout << "Execute task c";
		}
		else if (taskName.compare("d") == 0)
		{
			std::cout << "Execute task d";
		}
		else if (taskName.compare("e") == 0)
		{
			std::cout << "Execute task e";
		}
		else if (taskName.compare("f") == 0)
		{
			std::cout << "Execute task f";
		}
		else
		{
			std::cout << "Not enough or invalid arguments, please try again.\n";
			Sleep(2000); 
			exit(0);
		}


		/* Write output file */


        std::cin.get();
        return 0;
    }
}

//----------------------------------------------------------------------
vector<ThuocTinh> removeProperty(vector<ThuocTinh> propList, vector<string> removeList) {
	vector<ThuocTinh> propList_2 = propList;
	for (int i = 0; i < removeList.size(); i++) {
		for (int j = 0; j < propList_2.size(); j++) {
			if (removeList[i] == propList_2[j].ten) {
				propList_2.erase(propList_2.begin() + j);
				break;
			}
		}
	}
	return propList_2;
}

//hàm kiểm tra xem string có parse sang double được hay không
bool isDouble(string str)
{
	const char* c = str.c_str();
	char* endptr = 0;
	strtod(c, &endptr);

	if (*endptr != '\0' || endptr == str)
		return false;
	return true;
}

//hàm parse string sang double
double stringToDouble(string str) {
	return atof(str.c_str());
}

//hàm parse double sang string; cần #include <sstream>
string doubleToString(double dbl) {
	std::ostringstream strs;
	strs << dbl;
	std::string str = strs.str();
	return str;
}

vector<ThuocTinh> MinMax(vector<ThuocTinh> propList) {
	vector<ThuocTinh> propList_2 = propList;
	for (int i = 0; i < propList_2.size(); i++) {
		double min = 2000000000; //số càng lớn càng tốt, đặt tạm 2 tỷ
		double max = 0;
		bool allRowAreDouble = true;
		//kiểm tra tất cả các dòng dữ liệu trong thuộc tính có hợp lệ hay không (là số và >= 0),
		//đồng thời tìm giá trị min, max trong thuộc tính (nếu có)
		//nếu tồn tại dòng không hợp lệ thì bỏ qua thuộc tính này
		for (int j = 0; j < propList_2[i].data.size(); j++) {
			if (isDouble(propList_2[i].data[j]) == false) {
				allRowAreDouble = false;
				break;
			}
			double value = stringToDouble(propList_2[i].data[j]);
			if (value < 0) {
				allRowAreDouble = false;
				break;
			}
			if (value > max)
				max = value;
			if (value < min)
				min = value;
		}
		//khi tất cả các dòng đều hợp lệ, ta có thể chuẩn hóa
		if (allRowAreDouble == true)
		{
			for (int j = 0; j < propList_2[i].data.size(); j++) {
				double value = stringToDouble(propList_2[i].data[j]);
				if (value == min) {
					propList_2[i].data[j] = doubleToString(0.0);
				}
				else {
					if (value == max) {
						propList_2[i].data[j] = doubleToString(1.0);
					}
					else {
						propList_2[i].data[j] = doubleToString((value - min) / (max - min));
					}
				}
			}
		}
	}
	return propList_2;
}

vector<ThuocTinh> zScore(vector<ThuocTinh> propList) {
	vector<ThuocTinh> propList_2 = propList;
	for (int i = 0; i < propList_2.size(); i++) {
		double sum = 0;
		bool allRowAreDouble = true;
		//kiểm tra tất cả các dòng dữ liệu trong thuộc tính có hợp lệ hay không (là số và >= 0),
		//đồng thời tìm giá trị tổng trong thuộc tính (nếu có)
		//nếu tồn tại dòng không hợp lệ thì bỏ qua thuộc tính này
		for (int j = 0; j < propList_2[i].data.size(); j++) {
			if (isDouble(propList_2[i].data[j]) == false) {
				allRowAreDouble = false;
				break;
			}
			double value = stringToDouble(propList_2[i].data[j]);
			if (value < 0) {
				allRowAreDouble = false;
				break;
			}
			sum += value;
		}

		//khi tất cả các dòng đều hợp lệ
		if (allRowAreDouble == true)
		{
			//tính giá trị trung bình(mean) và độ lệch chuẩn(standard deviation)
			//công thức: http://www.ieev.org/2009/11/standard-deviation-o-lech-chuan.html
			double mean = sum / propList_2[i].data.size();
			double sd = 0;
			for (int j = 0; j < propList_2[i].data.size(); j++) {
				double value = stringToDouble(propList_2[i].data[j]);
				sd += pow(value - mean, 2);
			}
			sd /= propList_2[i].data.size();
			sd = sqrt(sd);
			//chuẩn hóa z-score
			for (int j = 0; j < propList_2[i].data.size(); j++) {
				double value = stringToDouble(propList_2[i].data[j]);
				propList_2[i].data[j] = doubleToString((value - mean) / sd);
			}
		}
	}
	return propList_2;
}

vector<ThuocTinh> removeMissingInstance(vector<ThuocTinh> propList, vector<string> removeList) {
	vector<ThuocTinh> propList_2 = propList;
	//duyệt từng thuộc tính được yêu cầu kiểm tra
	for (int i = 0; i < removeList.size(); i++) {
		for (int j = 0; j < propList_2.size(); j++) {
			if (removeList[i] == propList_2[j].ten) {
				for (int k = 0; k < propList_2[j].data.size(); k++) {
					//khi phát hiện thuộc tính có dữ liệu bị thiếu
					if (propList_2[j].data[k] == "") {
						//xóa mẫu dữ liệu tại vị trí thiếu trên toàn bộ các thuộc tính
						for (int p = 0; p < propList_2.size(); p++) {
							propList_2[p].data.erase(propList_2[p].data.begin() + k);
						}
						k--;
					}
				}
			}
		}
	}
	return propList_2;
}