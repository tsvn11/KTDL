using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _14_1A
{
    class Program
    {
        static void Main(string[] args)
        {
            if (MyFunction.checkInput(args) == false)
            {
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
                return;
            }

            string input = args[0];
            string outputFI = args[1];
            string outputAR = args[2];
            double minsup = double.Parse(args[3]);
            double minconf = double.Parse(args[4]);

            List<Column> InputData = MyFunction.readCSV(input);
            MyFunction.printColumnList(InputData);


            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
            return;
        }
    }
}
