using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using static _14_4.Struct;

namespace _14_4
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

            int k = int.Parse(args[5]);

            Console.WriteLine("Reading input file...");
            List<Column> inputData = MyFunction.readCSV(args[1]);
            

            MyFunction.printAllValue(inputData); //test

            Console.WriteLine("Done! Press any key to exit...");
            Console.ReadKey();
        }
    }
}
