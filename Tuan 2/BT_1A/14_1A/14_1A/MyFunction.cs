using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace _14_1A
{
    public class MyFunction
    {
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
                            if (values[i].Contains("y") == true)
                                val = true;
                            data[i].value.Add(val);
                        }
                    }
                    row++;
                }
                return data;
            }
        }

        public static List<Row> convertToRowList(List<Column> col)
        {
            List<Row> data = new List<Row>();
            for(int r = 0; r < col[0].value.Count; r++)
            {
                Row row = new Row();
                for(int c = 0; c < col.Count; c++)
                {
                    if (col[c].value[r])
                    {
                        row.value.Add(col[c].name);
                    }
                }
                data.Add(row);
            }
            return data;
        }

        public static bool checkInput(string[] parameter)
        {
            if (parameter.Length != 5)
            {
                Console.WriteLine("Usage is <input> <output FI> <output AR> <minsup> <minconf>");
                return false;
            }

            if(System.IO.File.Exists(parameter[0]) == false)
            {
                Console.WriteLine("Can not found file " + parameter[0]);
                return false;
            }

            double minsup, minconf;

            if (double.TryParse(parameter[3], out minsup) == false)
            {
                Console.WriteLine("<minsup> must be a double variable");
                return false;
            }

            if (double.TryParse(parameter[4], out minconf) == false)
            {
                Console.WriteLine("<minconf> must be a double variable");
                return false;
            }

            return true;
        }

        public static void printColumnList(List<Column> data)
        {
            for(int i = 0; i < data.Count; i++)
            {
                Console.Write("[ " + data[i].name + " ]");
            }
            Console.WriteLine();
            for(int i = 0; i < data[0].value.Count; i++)
            {
                for (int j = 0; j < data.Count; j++)
                {
                    Console.Write("[ " + data[j].value[i] + " ]");
                }
                Console.WriteLine();
            }
        }

        public static void printRowList(List<Row> data)
        {
            for (int r = 0; r < data.Count; r++)
            {
                for (int c = 0; c < data[r].value.Count; c++)
                {
                    Console.Write("[ " + data[r].value[c] + " ]");
                }
                Console.WriteLine();
            }
        }

    }
}
