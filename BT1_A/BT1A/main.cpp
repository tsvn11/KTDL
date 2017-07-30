#include <Windows.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include "def.h"

#include "xlsxwriter.h"
#include "libxl.h"

using namespace libxl;

vector<string> splitProperty(string str);
vector<ThuocTinh> removeProperty(vector<ThuocTinh> propList, vector<string> removeList); //câu a
bool isDouble(string str);
inline bool isInteger(const std::string & s);
double stringToDouble(string str);
string doubleToString(double dbl);
bool checkBeforeBinning(ThuocTinh prop);
void rowSwap(vector<ThuocTinh> &propList, int rowA, int rowB);
vector<ThuocTinh> sortingAscending(vector<ThuocTinh> propList, int pos);
vector<ThuocTinh> MinMax(vector<ThuocTinh> propList); //câu b
vector<ThuocTinh> zScore(vector<ThuocTinh> propList); //câu c
vector<vector<ThuocTinh>> equalWidthBinning(vector<ThuocTinh> propList, vector<string> processedList, int numberOfBaskets); //câu d
vector<vector<ThuocTinh>> equalDepthBinning(vector<ThuocTinh> propList, vector<string> processedList, int depth); //câu e
vector<ThuocTinh> removeMissingInstance(vector<ThuocTinh> propList, vector<string> removeList); //câu f

bool readDataFromExcel(vector<ThuocTinh> &dsThuocTinh, std::string inputFile); 
bool writeDataToExcel(vector<ThuocTinh> dsThuocTinh, std::string outputFile);

void InitLibrary();
bool endOfRow(uint16_t row, uint16_t col);
bool cellEmpty(uint16_t row, uint16_t col);
const wchar_t* getDataCell(uint16_t row, uint16_t col);


std::string  WideToMB(const std::wstring& str, UINT codePage = CP_ACP);
std::wstring MBToWide(const std::string& str,  UINT codePage = CP_ACP);

std::string  WideToUTF8(const std::wstring& str) { return WideToMB(str, CP_UTF8); }
std::wstring UTF8ToWide(const std::string& str)  { return MBToWide(str, CP_UTF8); }

std::string ANSIToUTF8(const std::string& str, UINT codePage = CP_ACP) { return WideToUTF8(MBToWide(str, codePage)); }
std::string UTF8ToANSI(const std::string& str, UINT codePage = CP_ACP) { return WideToMB(UTF8ToWide(str), codePage); }

Book*			g_pBook = NULL;
Sheet*			g_pSheet = NULL;
unsigned int	g_nCountGetData = 0;
std::string		g_sInputFile = "";
std::string		g_sOutputFile = "";

// When passing char arrays as parameters they must be pointers
int main(int argc, char* argv[]) {

	//switch Windows console to UTF-8
	SetConsoleOutputCP(CP_UTF8);

	if (argc < 7) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
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
	}
	else { // if we got enough parameters...
		std::string inFile, outFile, taskName, proplist, bin;
		std::cout << argv[0];
		for (int i = 1; i < argc; i++) { /* We will iterate over argv[] to get the parameters stored inside.
										 * Note that we're starting on 1 because we don't need to know the
										 * path of the program, which is stored in argv[0] */
			if (i + 1 != argc) // Check that we haven't finished parsing already
				if (strcmp(argv[i], "--in") == 0) {
					// We know the next argument *should* be the filename:
					inFile = argv[i + 1];
				}
				else if (strcmp(argv[i], "--out") == 0) {
					outFile = argv[i + 1];
				}
				else if (strcmp(argv[i], "--task") == 0) {
					taskName = argv[i + 1];
				}
				else if (strcmp(argv[i], "--proplist") == 0) {
					proplist = argv[i + 1];
				}
				else if (strcmp(argv[i], "--bin") == 0) {
					bin = argv[i + 1];
				}
				std::cout << argv[i] << " ";
		}

		//chức năng nào cũng cần propList, propList rỗng chức năng sẽ không thể thực hiện
		//biến processedList sẽ lưu danh sách thuộc tính người dùng nhập vào
		size_t start = proplist.find('{');
		size_t end = proplist.find('}');
		if (start == string::npos || end == string::npos || proplist.find(' ') != string::npos) {
			cout << endl << "Cau truc proplist khong dung." << endl;
			cin.get();
			exit(0);
		}
		if (end - start == 1) {
			cout << "Proplist rong." << endl;
			cin.get();
			exit(0);
		}
		vector<string> processedList = splitProperty(proplist);
		int number = 0;
		//chức năng d, e cần number (số giỏ, độ sâu) để thực hiện
		//biến number sẽ lưu số nguyên người dùng nhập vào
		if (taskName.compare("d") == 0 || taskName.compare("e") == 0) {
			if (isInteger(bin) == false) {
				cout << "Gia tri sau --bin phai la mot so nguyen > 0." << endl;
				cin.get();
				exit(0);
			}
			number = (int)stringToDouble(bin);
			if (number <= 0) {
				cout << "Gia tri sau --bin phai la mot so nguyen > 0." << endl;
				cin.get();
				exit(0);
			}
		}


        //... some more code

		/* Read input file */
		vector<ThuocTinh> dsThuocTinh;

		g_sInputFile = inFile;
		g_sOutputFile = outFile;

		if (!readDataFromExcel(dsThuocTinh, inFile))
		{
			std::cout << "Read file error \n";
			std::cin.get();
			exit(0);
		}

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
		if (!writeDataToExcel(dsThuocTinh, outFile))
		{
			std::cout << "write file error \n";
			std::cin.get();
			exit(0);
		}


		std::cin.get();
		return 0;
	}
}

//----------------------------------------------------------------------
vector<string> splitProperty(string str) {
	string s = str.substr(1, str.length() - 2); //loại 2 dấu {}
	vector<string> result;
	while (true) {
		int comma = s.find(',');
		if (comma == string::npos) { //không có dấu ',' => chỉ còn 1 thuộc tính
			result.push_back(s);
			break;
		}
		else {
			string propName = s.substr(0, comma);
			result.push_back(propName);
			s = s.substr(propName.length() + 1, s.length() - propName.length());
		}
	}
	return result;
}

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

//hàm kiểm tra xem string có parse sang integer được hay không
inline bool isInteger(const std::string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
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
				cout << "[Message]: (" << propList_2[i].ten << ") is skipped, because row " << j + 1 << " is not number types" << endl;
				break;
			}
			double value = stringToDouble(propList_2[i].data[j]);
			if (value < 0) {
				allRowAreDouble = false;
				cout << "[Message]: (" << propList_2[i].ten << ") is skipped, because row " << j + 1 << " is < 0" << endl;
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
				cout << "[Message]: (" << propList_2[i].ten << ") is skipped, because row " << j + 1 << " is not number types" << endl;
				break;
			}
			double value = stringToDouble(propList_2[i].data[j]);
			if (value < 0) {
				allRowAreDouble = false;
				cout << "[Message]: (" << propList_2[i].ten << ") is skipped, because row " << j + 1 << " is < 0" << endl;
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

//--- cau d, e ---
//kiểm tra thuộc tính có hợp lệ hay không (không rỗng, là số) trước khi chia giỏ
bool checkBeforeBinning(ThuocTinh prop) {
	for (int i = 0; i < prop.data.size(); i++) {
		if (prop.data[i] == "") {
			cout << "[Message]: (" << prop.ten << ") is skipped, because row " << i + 1 << " is NULL" << endl;
			return false;
		}
		if (isDouble(prop.data[i]) == false) {
			cout << "[Message]: (" << prop.ten << ") is skipped, because row " << i + 1 << " is not number types" << endl;
			return false;
		}
	}
	return true;
}

//hàm hoán vị 2 dòng trên tập dữ liệu
void rowSwap(vector<ThuocTinh> &propList, int rowA, int rowB) {
	vector<string> tempRow;
	for (int i = 0; i < propList.size(); i++) {
		tempRow.push_back(propList[i].data[rowA]);
		propList[i].data[rowA] = propList[i].data[rowB];
		propList[i].data[rowB] = tempRow[i];
	}
}

//sắp xếp lại dữ liệu dựa theo vị trí thuộc tính được chỉ định pos
vector<ThuocTinh> sortingAscending(vector<ThuocTinh> propList, int pos) {
	vector<ThuocTinh> propList_2 = propList;
	for (int i = 0; i < propList_2[0].data.size() - 1; i++) {
		for (int j = i + 1; j < propList_2[0].data.size(); j++) {
			if (stringToDouble(propList_2[pos].data[i]) > stringToDouble(propList_2[pos].data[j])) {
				rowSwap(propList_2, i, j);
			}
		}
	}
	return propList_2;
}

//numberOfBaskets: số lượng giỏ muốn chia
//phương pháp khử nhiễu được sử dụng: giá trị trung bình giỏ
vector<vector<ThuocTinh>> equalWidthBinning(vector<ThuocTinh> propList, vector<string> processedList, int numberOfBaskets) {
	vector<vector<ThuocTinh>> result; //nhiều files, mỗi vector<ThuocTinh> xuất ra 1 file excel
									  //duyệt từng thuộc tính được yêu cầu chia giỏ
	for (int i = 0; i < processedList.size(); i++) {
		for (int j = 0; j < propList.size(); j++) {
			if (processedList[i] == propList[j].ten) {
				//kiểm tra thuộc tính có hợp lệ hay không (không rỗng, là số)
				if (checkBeforeBinning(propList[j]) == false)
					break;
				//sắp xếp lại dữ liệu 
				vector<ThuocTinh> propList_current = sortingAscending(propList, j);
				double minValue = stringToDouble(propList_current[j].data[0]);
				double maxValue = stringToDouble(propList_current[j].data[propList_current[0].data.size() - 1]);
				double width = (maxValue - minValue) / numberOfBaskets;
				//tính giá trị biên của các giỏ
				vector<double> boundaryValue;
				for (int b = 0; b <= numberOfBaskets; b++) {
					boundaryValue.push_back(minValue + (width*b));
				}
				//tính giá trị trung bình của từng giỏ
				vector<double> sumOfEachBasket;
				vector<int> coutOfEachBasket;
				for (int bas = 0; bas < numberOfBaskets; bas++) {
					sumOfEachBasket.push_back(0);
					coutOfEachBasket.push_back(0);
				}
				for (int line = 0; line < propList_current[0].data.size(); line++) {
					//duyệt ngược từ biên cao xuống thấp để xác định vị trí giỏ của giá trị
					for (int bas = numberOfBaskets - 1; bas >= 0; bas--) {
						double val = stringToDouble(propList_current[j].data[line]);
						if (val >= boundaryValue[bas]) {
							sumOfEachBasket[bas] += val;
							coutOfEachBasket[bas]++;
							break;
						}
					}
				}
				vector<double> meanOfEachBasket;
				for (int bas = 0; bas < numberOfBaskets; bas++) {
					if (coutOfEachBasket[bas] == 0) {
						meanOfEachBasket.push_back(0);
					}
					else {
						meanOfEachBasket.push_back(sumOfEachBasket[bas] / coutOfEachBasket[bas]);
					}
				}
				//sắp xếp các giá trị vào giỏ phù hợp và làm trơn bằng giá trị trung bình giỏ
				for (int line = 0; line < propList_current[0].data.size(); line++) {
					//xác định giỏ phù hợp
					for (int bas = 0; bas < numberOfBaskets; bas++) {
						int idx = 0;
						idx += coutOfEachBasket[bas];
						for (int beforeBas = 0; beforeBas < bas; beforeBas++) {
							idx += coutOfEachBasket[beforeBas];
						}
						if (line < idx) {
							propList_current[j].data[line] = doubleToString(meanOfEachBasket[bas]);
							break;
						}
					}
				}
				result.push_back(propList_current);
			}
		}
	}
	return result;
}

//depth: độ sâu giỏ muốn chia
//phương pháp khử nhiễu được sử dụng: giá trị trung bình giỏ
vector<vector<ThuocTinh>> equalDepthBinning(vector<ThuocTinh> propList, vector<string> processedList, int depth) {
	vector<vector<ThuocTinh>> result; //nhiều files, mỗi vector<ThuocTinh> xuất ra 1 file excel
									  //duyệt từng thuộc tính được yêu cầu chia giỏ
	for (int i = 0; i < processedList.size(); i++) {
		for (int j = 0; j < propList.size(); j++) {
			if (processedList[i] == propList[j].ten) {
				//kiểm tra thuộc tính có hợp lệ hay không (không rỗng, là số)
				if (checkBeforeBinning(propList[j]) == false)
					break;
				//sắp xếp lại dữ liệu 
				vector<ThuocTinh> propList_current = sortingAscending(propList, j);
				//tính số lượng giỏ
				int numberOfBaskets = propList_current[0].data.size() / depth;
				if (propList_current[0].data.size() % depth != 0)
					numberOfBaskets++;
				//tính giá trị trung bình của từng giỏ
				vector<double> sumOfEachBasket;
				vector<int> coutOfEachBasket;
				for (int bas = 0; bas < numberOfBaskets; bas++) {
					sumOfEachBasket.push_back(0);
					coutOfEachBasket.push_back(0);
				}
				//duyệt từ đầu đến cuối để xác định vị trí giỏ của giá trị
				for (int line = 0; line < propList_current[0].data.size(); line++) {
					int bas_pos = line / depth;
					sumOfEachBasket[bas_pos] += stringToDouble(propList_current[j].data[line]);
					coutOfEachBasket[bas_pos]++;
				}
				vector<double> meanOfEachBasket;
				for (int bas = 0; bas < numberOfBaskets; bas++) {
					meanOfEachBasket.push_back(sumOfEachBasket[bas] / coutOfEachBasket[bas]);
				}
				//sắp xếp các giá trị vào giỏ phù hợp và làm trơn bằng giá trị trung bình giỏ
				for (int line = 0; line < propList_current[0].data.size(); line++) {
					int bas_pos = line / depth;
					propList_current[j].data[line] = doubleToString(meanOfEachBasket[bas_pos]);
				}
				result.push_back(propList_current);
			}
		}
	}
	return result;
}

bool readDataFromExcel(vector<ThuocTinh> &dsThuocTinh, std::string inputFile)
{
	g_pBook = xlCreateXMLBook();

	if (g_pBook == NULL)
		return false;

	if (g_pBook->load(UTF8ToWide(inputFile).c_str()))
	{
		g_pSheet = g_pBook->getSheet(0);

		if (g_pSheet)
		{
			uint16_t col = 0;

			// Doc tat ca cac ten thuoc tinh,
			while (!cellEmpty(0 + 1, col))   // Ban TRIAL bo qua dong dau tien
			{
				// Lay ten thuoc tinh
				const wchar_t* buffer = getDataCell(0 + 1, col); // Ban TRIAL bo qua dong dau tien
				std::wstring wbuffer(buffer);

				ThuocTinh thuoctinh;
				thuoctinh.ten = WideToUTF8(wbuffer); // Luu ten thuoc tinh

				uint16_t row = 1 + 1;  // bo qua dong dau tien chua header // Ban TRIAL bo qua dong dau tien

				// Doc den dong cuoi cung
				while (!endOfRow(row, col))
				{
					// Lay du lieu
					const wchar_t* buffer2 = getDataCell(row, col);

					// Du lieu rong
					if (buffer2 == NULL)
						buffer2 = L"";

					std::wstring wbuffer2(buffer2);
					std::string dulieu(WideToUTF8(wbuffer2));

					// Luu du lieu
					thuoctinh.data.push_back(dulieu); 

					row++;
				}

				// Them thuoc tinh vao danh sach
				dsThuocTinh.push_back(thuoctinh);

				col++;
			}
		}
	}

	g_pBook->release();

	return true;
}

bool writeDataToExcel(vector<ThuocTinh> dsThuocTinh, std::string outputFile)
{
	lxw_workbook  *workbook  = workbook_new(outputFile.c_str());

	if (workbook == NULL)
		return false;

	lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

	if (worksheet == NULL)
		return false;

	// Duyet qua tat ca cac thuoc tinh
	for (uint16_t col = 0; col < dsThuocTinh.size(); col++)
	{
		// Dong dau tien la ten thuoc tinh
		worksheet_write_string(worksheet, 0, col, dsThuocTinh[col].ten.c_str(), NULL);

		// Ghi tat ca cac dong du lieu trong thuoc tinh 

		for (uint16_t row = 0; row < dsThuocTinh[col].data.size(); row++) 
		{
			// Ghi du lieu vao cell, bo qua dong header
			worksheet_write_string(worksheet, row + 1, col, dsThuocTinh[col].data[row].c_str(), NULL);
		}
	}

	workbook_close(workbook);

	return true;
}

std::string WideToMB(const std::wstring& str, UINT codePage /*= CP_ACP*/)
{
	std::string ret;
	if(str.length() > 0)
	{
		DWORD mbChars = WideCharToMultiByte(codePage, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
		_ASSERTE(mbChars > 0);
		if(mbChars > 0)
		{
			char* buf = new char[mbChars];
			_ASSERTE( buf != NULL );
			if( buf != NULL )
			{
				ZeroMemory(buf, mbChars);

				DWORD charsConverted = WideCharToMultiByte(codePage, 0, str.c_str(), -1, buf, mbChars, NULL, NULL);
				_ASSERTE( charsConverted > 0 );
				_ASSERTE( charsConverted <= mbChars );

				buf[mbChars - 1] = 0;
				ret = buf;

				delete[] buf;
			}
		}
	}
	return ret;
}

std::wstring MBToWide(const std::string& str, UINT codePage /*= CP_ACP*/)
{
	std::wstring ret;
	if(str.length() > 0)
	{
		DWORD wChars = MultiByteToWideChar(codePage, 0, str.c_str(), -1, NULL, 0);
		_ASSERTE(wChars > 0);
		if(wChars > 0)
		{
			wchar_t* buf = new wchar_t[wChars];
			_ASSERTE( buf != NULL );
			if( buf != NULL )
			{
				size_t bytesNeeded = sizeof(wchar_t)*wChars;
				ZeroMemory(buf, bytesNeeded);

				DWORD charsConverted = MultiByteToWideChar(codePage, 0, str.c_str(), -1, buf, wChars);
				_ASSERTE( charsConverted > 0 );
				_ASSERTE( charsConverted <= wChars );

				buf[wChars - 1] = 0;
				ret = buf;

				delete[] buf;
			}
		}
	}
	return ret;
}

// Kiem tra da den cuoi file chua
// Neu co 20 dong du lieu rong lien tiep => da doc het du lieu => dung lai
bool endOfRow(uint16_t row, uint16_t col)
{
	int count = 0;

	while(count < 20)
	{
		if (!cellEmpty(row, col))
			return false;

		row++;
		count++;
	}

	return true;
}

// Kiem tra cell rong 
bool cellEmpty(uint16_t row, uint16_t col)
{
	if (getDataCell(row, col))
	{
		return false;
	}

	return true;
}

/* Khoi tao lai library */
void InitLibrary()
{
	if (g_pBook)
	{
		g_pBook->release();
	}

	g_pBook = xlCreateXMLBook();

	if (g_pBook == NULL)
		return;

	if (g_pBook->load(UTF8ToWide(g_sInputFile).c_str()))
	{
		g_pSheet = g_pBook->getSheet(0);
	}
}

const wchar_t* getDataCell(uint16_t row, uint16_t col)
{
	g_nCountGetData++;

	// CHEAT: Lay data duoc 280 lan thi reset (300 lan la toi da)
	if (g_nCountGetData == 280)
	{
		InitLibrary();
		g_nCountGetData = 0;
	}

	if (row < 0 || col < 0)
		return NULL;

	return g_pSheet->readStr(row, col);
}
