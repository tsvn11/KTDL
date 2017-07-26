#include <Windows.h>
#include <fstream>
#include <iostream>

#include "def.h"

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