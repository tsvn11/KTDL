using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using static _14_4.Struct;

namespace _14_4
{
    class MyFunction
    {
        public static bool checkInput(string[] parameter)
        {
            if (parameter.Length != 6)
            {
                printHowToUse();
                return false; //wrong input
            }
            if (parameter[0].CompareTo("--input") != 0 || parameter[2].CompareTo("--output") != 0 || parameter[4].CompareTo("--k") != 0)
            {
                printHowToUse();
                return false;
            }
            if (File.Exists(parameter[1]) == false)
            {
                Console.WriteLine("Can not found " + parameter[3]);
                return false;
            }
            int k;
            if(int.TryParse(parameter[5], out k) == false)
            {
                Console.WriteLine("k value must be an integer");
                return false;
            }
            return true;
        }

        private static void printHowToUse()
        {
            Console.WriteLine("* Usage is: --input <*.csv> --output <*.txt> --k <NumberOfGroups>");
        }

        public static List<Column> readCSV(string path)
        {
            using (var reader = new StreamReader(path))
            {
                List<Column> data = new List<Column>();
                int row = 1;
                while (!reader.EndOfStream)
                {
                    string line = reader.ReadLine();
                    string[] values = line.Split(',');
                    if (row == 1)
                    {
                        for (int i = 0; i < values.Length; i++)
                        {
                            Column col = new Column(values[i].Trim());
                            data.Add(col);
                        }
                    }
                    else
                    {
                        for (int i = 0; i < values.Length; i++)
                        {
                            bool val = false;
                            if (values[i].Contains("1"))
                                val = true;
                            data[i].Data.Add(val);
                        }
                    }
                    row++;
                }
                return data;
            }
        }

        //tính giá trị của một mẫu dữ liệu
        public static double calValue(List<bool> values)
        {
            float count = 0;
            //đếm số lượng khu vực xuất hiện của loài/tất cả các khu vực
            for(int i = 0; i < values.Count; i++)
            {
                if (values[i])
                {
                    count += 1;
                }
            }
            return count / values.Count;
        }

        //test - in giá trị của tất cả mẫu dữ liệu
        public static void printAllValue(List<Column> data)
        {
            for(int r = 0; r < data[0].Data.Count; r++)
            {
                List<bool> rowData = new List<bool>();
                for(int c = 0; c < data.Count; c++)
                {
                    rowData.Add(data[c].Data[r]);
                }
                Console.WriteLine(string.Format("[{0}]: {1}", r, calValue(rowData)));
            }
        }
    }
}
