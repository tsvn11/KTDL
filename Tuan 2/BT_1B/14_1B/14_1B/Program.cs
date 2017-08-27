using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using static _14_1B.Struct;

namespace _14_1B
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
            string output = args[1];

            Console.WriteLine("Reading input file...");
            List<Row> OriginalInputData = MyFunction.readData(input);
            Console.WriteLine("Removing first property...");
            MyFunction.removeFirstProperty(OriginalInputData);
            Console.WriteLine("Formatting...");
            List<Column> DataAfterConverted = MyFunction.convertToColumnList(OriginalInputData);
            Console.WriteLine("Saving to CSV file...");
            MyFunction.saveToCSV(DataAfterConverted, output);

            Console.WriteLine("[Done] Press any key to exit...");
            Console.ReadKey();
        }
    }
}
