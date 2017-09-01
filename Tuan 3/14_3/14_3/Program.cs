using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using static _14_3.Struct;

namespace _14_3
{
    class Program
    {
        static void Main(string[] args)
        {
            int checkResult = MyFunction.checkInput(args);

            //wrong input
            if (checkResult == 0)
            {
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
                return;
            }

            List<Column> OriginalInputData = null;
            if (checkResult == 1 || checkResult == 2)
            {
                Console.WriteLine("Reading excel file...");
                //read excel
                OriginalInputData = MyFunction.readExcel(args[3]);
                if (OriginalInputData.Count == 0)
                {
                    Console.WriteLine("Press any key to exit...");
                    Console.ReadKey();
                    return;
                }
            }

            //train
            if (checkResult == 1)
            {
                MyFunction.train(OriginalInputData, args[5], args[7]);
            }

            //predict
            if (checkResult == 2)
            {
                MyFunction.predict(OriginalInputData, args[5], args[7]);
            }

            //visualize
            if (checkResult == 3)
            {
                MyFunction.visualize(args[3], args[5]);
            }

            Console.WriteLine("\n[Done] Press any key to exit...");
            Console.ReadKey();
            return;
        }
    }
}
