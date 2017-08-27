using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using static _14_1B.Struct;

namespace _14_1B
{
    class MyFunction
    {
        public static bool checkInput(string[] parameter)
        {
            if (parameter.Length != 2)
            {
                Console.WriteLine("Usage is <input> <output>");
                return false;
            }

            if (System.IO.File.Exists(parameter[0]) == false)
            {
                Console.WriteLine("Can not found file " + parameter[0]);
                return false;
            }

            return true;
        }

        public static List<Row> readData(string path)
        {
            using (var reader = new StreamReader(path))
            {
                List<Row> data = new List<Row>();
                while (!reader.EndOfStream)
                {
                    Row r = new Row();
                    string line = reader.ReadLine();
                    string[] values = line.Split(',');
                    for (int i = 0; i < values.Length; i++)
                    {
                        r.value.Add(values[i].Trim());
                    }
                    data.Add(r);
                }
                return data;
            }
        }

        public static void removeFirstProperty(List<Row> data)
        {
            foreach(Row r in data)
            {
                r.value.RemoveAt(0);
            }
        }

        public static List<Column> convertToColumnList(List<Row> data)
        {
            //find all property
            List<Column> columnList = new List<Column>();
            foreach(Row r in data)
            {
                foreach(string value in r.value)
                {
                    bool existed = false;
                    foreach(Column c in columnList)
                    {
                        if(c.name.CompareTo(value) == 0)
                        {
                            existed = true;
                            break;
                        }
                    }
                    if(existed == false)
                    {
                        Column newCol = new Column(value);
                        columnList.Add(newCol);
                    }
                }
            }
            //convert
            foreach(Row r in data)
            {
                foreach(Column c in columnList)
                {
                    bool existed = false;
                    foreach(string value in r.value)
                    {
                        if(c.name.CompareTo(value) == 0)
                        {
                            existed = true;
                            break;
                        }
                    }
                    c.value.Add(existed);
                }
            }

            return columnList;
        }
        
        public static void saveToCSV(List<Column> list, string path)
        {
            using (var writer = new StreamWriter(path))
            {
                string p = "";
                //write property
                for(int i = 0; i < list.Count; i++)
                {
                    if(i == list.Count - 1)
                    {
                       p += list[i].name;
                    }
                    else
                    {
                        p += list[i].name + ", ";
                    }
                }
                writer.WriteLine(p);
                //write value
                for (int i = 0; i < list[0].value.Count; i++)
                {
                    string r = "";
                    for (int j = 0; j < list.Count; j++)
                    {
                        if (j == list.Count - 1)
                        {
                            if(list[j].value[i] == true)
                            {
                                r += "y";
                            }
                            else
                            {
                                r += "?";
                            }
                        }
                        else
                        {
                            if (list[j].value[i] == true)
                            {
                                r += "y, ";
                            }
                            else
                            {
                                r += "?, ";
                            }
                        }
                    }
                    writer.WriteLine(r);
                }
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

        public static void printColumnList(List<Column> data)
        {
            for (int i = 0; i < data.Count; i++)
            {
                Console.Write("[ " + data[i].name + " ]");
            }
            Console.WriteLine();
            for (int i = 0; i < data[0].value.Count; i++)
            {
                for (int j = 0; j < data.Count; j++)
                {
                    Console.Write("[ " + data[j].value[i] + " ]");
                }
                Console.WriteLine();
            }
        }
    }
}
