#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

struct ThuocTinh {
	string ten;
	vector<string> data;
};

bool isDouble(string str);
double stringToDouble(string str);
string doubleToString(double dbl);
vector<ThuocTinh> removeEmptyRow(vector<ThuocTinh> propList);
vector<ThuocTinh> removeDuplicate(vector<ThuocTinh> propList);
vector<ThuocTinh> standardizingArea(vector<ThuocTinh> propList);

int main() {

	return 0;
}

//----------------------------------------------------
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

vector<ThuocTinh> removeEmptyRow(vector<ThuocTinh> propList) {
	vector<ThuocTinh> propList_2 = propList;
	for (int i = 0; i < propList_2[0].data.size(); i++) {
		//lấy ra dòng dữ liệu
		vector<string> currentRow;
		for (int j = 1; j < propList_2.size(); j++) {
			currentRow.push_back(propList_2[j].data[i]);
		}
		//kiểm tra xem dòng có rỗng hay không
		bool empty = true;
		for (int s = 0; s < currentRow.size(); s++) {
			if (currentRow[s] != "") {
				empty = false;
				break;
			}
		}
		//nếu là dòng rỗng thì xóa
		if (empty == true) {
			for (int p = 0; p < propList_2.size(); p++) {
				propList_2[p].data.erase(propList_2[p].data.begin() + i);
			}
			i--;
		}
	}
	return propList_2;
}

vector<ThuocTinh> removeDuplicate(vector<ThuocTinh> propList) {
	vector<ThuocTinh> propList_2 = propList;
	for (int i = 0; i < propList_2[0].data.size() - 1; i++) {
		//lấy ra dòng dữ liệu
		vector<string> currentRow;
		for (int j = 1; j < propList_2.size(); j++) {
			currentRow.push_back(propList_2[j].data[i]);
		}
		//duyệt so sánh với những dòng phía dưới
		for (int k = i + 1; k < propList_2[0].data.size(); k++) {
			vector<string> comparedRow;
			for (int j = 1; j < propList_2.size(); j++) {
				comparedRow.push_back(propList_2[j].data[k]);
			}
			//so sánh
			bool dup = true;
			for (int s = 0; s < currentRow.size(); s++) {
				if (currentRow[s] != comparedRow[s]) {
					dup = false;
					break;
				}
			}
			//nếu bị trùng thì xóa comparedRow
			if (dup == true) {
				for (int p = 0; p < propList_2.size(); p++) {
					propList_2[p].data.erase(propList_2[p].data.begin() + k);
				}
				k--;
			}
		}
	}
	return propList_2;
}

vector<ThuocTinh> standardizingArea(vector<ThuocTinh> propList) {
	vector<ThuocTinh> propList_2 = propList;
	for (int i = 0; i < propList_2.size(); i++) {
		//tìm thuộc tính có tên "area"
		if (propList_2[i].ten == "area") {
			//xét từng dòng của thuộc tính
			for (int j = 0; j < propList_2[i].data.size(); j++) {
				string value = propList_2[i].data[j];
				//nếu dòng đang xét không rỗng
				if (value != "") {
					int pos_mi = value.find("mi");
					//nếu trong dòng đang xét có đơn vị "mi", ta tách phần số ra
					if (pos_mi > 0) {
						string numberValue = value.substr(0, value.length() - 2);
						//kiểm tra xem có đúng là số và > 0 hay không
						if (isDouble(numberValue) == true) {
							double dbl = stringToDouble(numberValue);
							if (dbl > 0) {
								dbl *= 2.59; //1 mi2 bằng khoảng 2.59 km2
								propList_2[i].data[j] = doubleToString(dbl);
								propList_2[i].data[j] += "km";
							}
						}
					}
				}
			}
		}
	}
	return propList_2;
}