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
            float minsup = float.Parse(args[3]);
            float minconf = float.Parse(args[4]);

            List<Column> OriginalInputData = MyFunction.readCSV(input);
            List<Row> InputData = MyFunction.convertToRowList(OriginalInputData);

            FPGrowth fpGrowth = new FPGrowth();
            fpGrowth.CreateFPTreeAndGenerateFrequentItemsets(InputData, minsup);
            
            Console.WriteLine("\n[Done] Press any key to exit...");
            Console.ReadKey();
            return;
        }
    }
}
