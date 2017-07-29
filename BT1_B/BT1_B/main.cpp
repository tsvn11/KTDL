#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

struct ThuocTinh {
	string ten;
	vector<string> data;
};

bool fileExists(string fileName);
bool isDouble(string str);
double stringToDouble(string str);
string doubleToString(double dbl);
void printData(vector<ThuocTinh> propList);
vector<ThuocTinh> loadTextFile(string path);
vector<ThuocTinh> removeEmptyRow(vector<ThuocTinh> propList);
vector<ThuocTinh> removeDuplicate(vector<ThuocTinh> propList);
vector<ThuocTinh> standardizingArea(vector<ThuocTinh> propList);
void writeDataToExcel(vector<ThuocTinh> dsThuocTinh, std::string outputFile);

int main(int argc, char* argv[]) {
	if (argc != 5) {
		std::cout << "Usage is --in <infile> --out <outfile>";
		std::cin.get();
		exit(0);
	}

	std::string inFile, outFile;
	for (int i = 1; i < argc; i++) {
		if (i + 1 != argc) // Check that we haven't finished parsing already
			if (strcmp(argv[i], "--in") == 0) {
				// We know the next argument *should* be the filename:
				inFile = argv[i + 1];
			}
			else if (strcmp(argv[i], "--out") == 0) {
				outFile = argv[i + 1];
			}
	}
	if (fileExists(inFile) == false) {
		std::cout << inFile << " not found" << endl;
		std::cin.get();
		exit(0);
	}
	if (fileExists(outFile) == true) {
		std::cout << outFile << " already exists" << endl;
		std::cin.get();
		exit(0);
	}

	cout << "Please wait..." << endl;
	vector<ThuocTinh> ds = loadTextFile(inFile);
	vector<ThuocTinh> ds_2 = removeEmptyRow(ds);
	vector<ThuocTinh> ds_3 = removeDuplicate(ds_2);
	vector<ThuocTinh> ds_4 = standardizingArea(ds_3);
	writeDataToExcel(ds_4, outFile + ".xls");
	//hiện tại chưa tìm được thư viện hỗ trợ ghi excel nên lưu tạm bằng xls
	//lưu bằng xlsx thì sẽ mở không được
	cout << "Done!" << endl;
	system("pause");
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

void printData(vector<ThuocTinh> propList) {
	for (int i = 0; i < propList.size(); i++) {
		cout << "[" << propList[i].ten << "][" << propList[i].data.size() << "]:" << endl;
		for (int j = 0; j < propList[i].data.size(); j++) {
			cout << propList[i].data[j] << ", ";
		}
		cout << endl;
	}
}

vector<ThuocTinh> loadTextFile(string path) {
	vector<ThuocTinh> ds;
	ThuocTinh tt_country, tt_name, tt_longName, tt_foundingDate, tt_population, tt_capital, tt_largestCity, tt_area;
	tt_country.ten = "country";
	tt_name.ten = "name";
	tt_longName.ten = "longName";
	tt_foundingDate.ten = "foundingDate";
	tt_population.ten = "population";
	tt_capital.ten = "capital";
	tt_largestCity.ten = "largestCity";
	tt_area.ten = "area";
	ds.push_back(tt_country);
	ds.push_back(tt_name);
	ds.push_back(tt_longName);
	ds.push_back(tt_foundingDate);
	ds.push_back(tt_population);
	ds.push_back(tt_capital);
	ds.push_back(tt_largestCity);
	ds.push_back(tt_area);

	ifstream file(path);
	string str;
	int country = -1;
	while (getline(file, str))
	{
		int equalSymbolPosition = str.find('=');
		string propName = str.substr(0, equalSymbolPosition);
		string propValue = str.substr(equalSymbolPosition + 1);
		if (propName == "country") {
			country++;
			if (country > 0) {
				ds[0].data.push_back("");
				ds[1].data.push_back("");
				ds[2].data.push_back("");
				ds[3].data.push_back("");
				ds[4].data.push_back("");
				ds[5].data.push_back("");
				ds[6].data.push_back("");
				ds[7].data.push_back("");
			}
		}
		if (country > 0) {
			if (propName == "country")
				ds[0].data[country - 1] = propValue;
			if (propName == "name")
				ds[1].data[country - 1] = propValue;
			if (propName == "longName")
				ds[2].data[country - 1] = propValue;
			if (propName == "foundingDate")
				ds[3].data[country - 1] = propValue;
			if (propName == "population")
				ds[4].data[country - 1] = propValue;
			if (propName == "capital")
				ds[5].data[country - 1] = propValue;
			if (propName == "largestCity")
				ds[6].data[country - 1] = propValue;
			if (propName == "area")
				ds[7].data[country - 1] = propValue;
		}
	}
	return ds;
}

bool fileExists(string fileName) {
	if (std::ifstream(fileName))
		return true;
	return false;
}

void writeDataToExcel(vector<ThuocTinh> dsThuocTinh, std::string outputFile)
{
	ofstream newFile(outputFile);
	for (int i = 0; i < dsThuocTinh.size(); i++) {
		newFile << dsThuocTinh[i].ten << "\t";
	}
	newFile << "\n";
	for (int i = 0; i < dsThuocTinh[0].data.size(); i++) {
		for (int j = 0; j < dsThuocTinh.size(); j++) {
			newFile << dsThuocTinh[j].data[i] << "\t";
		}
		newFile << "\n";
	}
	newFile.close();
}