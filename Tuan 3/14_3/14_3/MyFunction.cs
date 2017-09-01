using System;
using System.Collections.Generic;
using Excel = Microsoft.Office.Interop.Excel;
using System.IO;
using static _14_3.Struct;
using System.Xml.Serialization;

namespace _14_3
{
    public static class MyFunction
    {
        public static int checkInput(string[] parameter)
        {
            if (parameter.Length != 6 && parameter.Length != 8)
            {
                printHowToUse();
                return 0; //wrong input
            }
            if(parameter[0].CompareTo("--task") != 0)
            {
                printHowToUse();
                return 0;
            }
            string task = parameter[1];
            if(task.CompareTo("train") == 0)
            {
                if(parameter.Length != 8)
                {
                    printHowToUse();
                    return 0;
                }
                if (parameter[2].CompareTo("--input") != 0)
                {
                    printHowToUse();
                    return 0;
                }
                if (parameter[4].CompareTo("--output") != 0)
                {
                    printHowToUse();
                    return 0;
                }
                if (parameter[6].CompareTo("--log") != 0)
                {
                    printHowToUse();
                    return 0;
                }
                if (File.Exists(parameter[3]) == false)
                {
                    Console.WriteLine("Can not found " + parameter[3]);
                    return 0;
                }
                return 1;
            }
            if(task.CompareTo("predict") == 0)
            {
                if (parameter.Length != 8)
                {
                    printHowToUse();
                    return 0;
                }
                if (parameter[2].CompareTo("--input") != 0)
                {
                    printHowToUse();
                    return 0;
                }
                if (parameter[4].CompareTo("--model") != 0)
                {
                    printHowToUse();
                    return 0;
                }
                if (parameter[6].CompareTo("--output") != 0)
                {
                    printHowToUse();
                    return 0;
                }
                if (File.Exists(parameter[3]) == false)
                {
                    Console.WriteLine("Can not found " + parameter[3]);
                    return 0;
                }
                if (File.Exists(parameter[5]) == false)
                {
                    Console.WriteLine("Can not found " + parameter[5]);
                    return 0;
                }
                return 2;
            }
            if(task.CompareTo("visualize") == 0)
            {
                if (parameter.Length != 6)
                {
                    printHowToUse();
                    return 0;
                }
                if (parameter[2].CompareTo("--model") != 0)
                {
                    printHowToUse();
                    return 0;
                }
                if (parameter[4].CompareTo("--output") != 0)
                {
                    printHowToUse();
                    return 0;
                }
                if (File.Exists(parameter[3]) == false)
                {
                    Console.WriteLine("Can not found " + parameter[3]);
                    return 0;
                }
                return 3;
            }
            printHowToUse();
            return 0;
        }
        private static void printHowToUse()
        {
            Console.WriteLine("*** Usage is one of the following:");
            Console.WriteLine("--task train --input <*.xlsx> --output <*.id3> --log <*.txt>");
            Console.WriteLine("--task predict --input <*.xlsx> --model <*.id3> --output <*.xlsx>");
            Console.WriteLine("--task visualize --model <*.id3> --output <*.jpg>");
            
        }
        public static void printTree(Node root)
        {
            Console.WriteLine("Property: [{0}], isLeaf: [{1}], ParentBranch: [{2}],  NumberOfChildren: [{3}]",
                root.PropertyName, root.isLeaf, root.ParentBranchName, root.Children.Count);
            foreach (Node child in root.Children)
            {
                printTree(child);
            }
        }
        public static List<Column> readExcel(string path)
        {
            List<Column> columnList = new List<Column>();
            //open Excel
            Excel.Application xlApp = new Excel.Application();
            Excel.Workbook xlWorkBook = xlApp.Workbooks.Open(path);
            Excel.Worksheet xlWorkSheet = xlWorkBook.Sheets[1]; //1-based
            Excel.Range range = xlWorkSheet.UsedRange;
            //đọc dữ liệu
            int rows = range.Rows.Count;
            int cols = range.Columns.Count;
            //đọc thuộc tính
            bool wrongFile = false;
            for (int c = 1; c <= cols; c++)
            {
                Column prop = new Column(range.Cells[1, c].Value.ToString());
                if (prop.Name == string.Empty)
                {
                    Console.WriteLine(string.Format("[Wrong Input File]: The property name of column {0} is empty, program has stopped working!", c));
                    columnList.Clear();
                    break;
                }
                for (int r = 2; r <= rows; r++)
                {
                    if (range.Cells[r, c].Value != null)
                    {
                        prop.Data.Add(range.Cells[r, c].Value.ToString());
                    }
                    else
                    {
                        Console.WriteLine(string.Format("[Wrong Input File]: The value of column {0}, row {1} is empty, program has stopped working!", c, r));
                        columnList.Clear();
                        wrongFile = true;
                        break;
                    }
                }
                if (wrongFile)
                    break;
                columnList.Add(prop);
            }
            //close excel
            xlApp.Visible = false;
            xlApp.DisplayAlerts = false;
            xlWorkBook.Close(false, System.Reflection.Missing.Value, System.Reflection.Missing.Value);
            xlApp.Quit();
            if (range != null) System.Runtime.InteropServices.Marshal.ReleaseComObject(range);
            if (xlWorkSheet != null) System.Runtime.InteropServices.Marshal.ReleaseComObject(xlWorkSheet);
            if (xlWorkBook != null) System.Runtime.InteropServices.Marshal.ReleaseComObject(xlWorkBook);
            if (xlApp != null) System.Runtime.InteropServices.Marshal.ReleaseComObject(xlApp);
            return columnList;
        }

        public static Node readId3(string path)
        {
            Stream stream = File.OpenRead(path);
            XmlSerializer xmlSer = new XmlSerializer(typeof(Node));
            Node root = (Node)xmlSer.Deserialize(stream);
            stream.Close();
            return root;
        } 

        public static void writeExcel(List<Column> columnList, string path)
        {
            //open excel
            Excel.Application xlApp = new Excel.Application();
            //tạo workbook
            Excel.Workbook xlWorkBook = xlApp.Workbooks.Add();
            //tạo sheet
            Excel.Worksheet xlWorkSheet = xlWorkBook.ActiveSheet;
            xlWorkSheet.Name = "Sheet1";
            //ghi dữ liệu
            for (int c = 0; c < columnList.Count; c++)
            {
                xlWorkSheet.Cells[1, c + 1].Value = columnList[c].Name;
                for (int r = 0; r < columnList[c].Data.Count; r++)
                {
                    xlWorkSheet.Cells[r + 2, c + 1].Value = columnList[c].Data[r];
                }
            }
            //save file
            xlWorkBook.SaveAs(path);
            //close excel
            xlApp.Visible = false;
            xlApp.DisplayAlerts = false;
            xlWorkBook.Close(false, System.Reflection.Missing.Value, System.Reflection.Missing.Value);
            xlApp.Quit();
            if (xlWorkSheet != null) System.Runtime.InteropServices.Marshal.ReleaseComObject(xlWorkSheet);
            if (xlWorkBook != null) System.Runtime.InteropServices.Marshal.ReleaseComObject(xlWorkBook);
            if (xlApp != null) System.Runtime.InteropServices.Marshal.ReleaseComObject(xlApp);
        }

        public static void writeTextFile(List<string> data, string path)
        {
            using (var writer = new StreamWriter(path))
            {
                foreach(string line in data)
                {
                    writer.WriteLine(line);
                }
            }
        }

        public static void writeTextFile(Node node, string path)
        {
            Stream stream = File.OpenWrite(path);
            XmlSerializer xmlSer = new XmlSerializer(typeof(Node));
            xmlSer.Serialize(stream, node);
            stream.Close();
        }

        public static void printColumnList(List<Column> columnList)
        {
            foreach (Column c in columnList)
            {
                Console.Write(string.Format("[{0}] ", c.Name));
            }
            Console.WriteLine();
            for (int r = 0; r < columnList[0].Data.Count; r++)
            {
                for (int c = 0; c < columnList.Count; c++)
                {
                    Console.Write(string.Format("[{0}] ", columnList[c].Data[r]));
                }
                Console.WriteLine();
            }

        }
        //----- train -----
        static List<string> LogRecord;
        public static void train(List<Column> OriginalInputData, string output, string log)
        {
            LogRecord = new List<string>();
            LogRecord.Add("Start");

            Console.WriteLine("Training...");
            Node root = new Node();
            root.ClassificationAttributeName = OriginalInputData[OriginalInputData.Count - 1].Name;
            selectAttribute(root, OriginalInputData);

            LogRecord.Add("End");
            Console.WriteLine("Saving log file...");
            writeTextFile(LogRecord, log);

            Console.WriteLine("Saving id3 file...");
            writeTextFile(root, output);
        }
        public static void selectAttribute(Node node, List<Column> data)
        {
            addLogRecord(data); ///

            //check if node is leaf
            List<Label> ClassList = detachData(null, data[data.Count - 1].Data);
            if (ClassList.Count == 1)
            {                
                node.isLeaf = true;
                node.PropertyName = ClassList[0].Name;
                LogRecord.Add("Leaf node found"); ///
                LogRecord.Add(string.Format("{0}: {1}", node.ParentBranchName, node.PropertyName)); ///
                return;
            }
            else
            {
                if (data.Count == 1)
                {
                    int maxCount = ClassList[0].Count;
                    int maxCountIndex = 0;
                    for (int i = 1; i < ClassList.Count; i++)
                    {
                        if (ClassList[i].Count > maxCount)
                        {
                            maxCount = ClassList[i].Count;
                            maxCountIndex = i;
                        }
                    }
                    node.isLeaf = true;
                    node.PropertyName = ClassList[maxCountIndex].Name;
                    LogRecord.Add("Leaf node found"); ///
                    LogRecord.Add(string.Format("{0}: {1}", node.ParentBranchName, node.PropertyName)); ///
                    return;
                }
            }

            //node is not leaf
            double maxInformationGain = 0;
            int maxIGColumnIndex = -1;
            double Info_D = calInfo_D(new List<string>(data[data.Count - 1].Data));
            for (int c = 0; c < data.Count - 1; c++)
            {
                double IG = Info_D - calInfo_A(new List<string>(data[data.Count - 1].Data), new List<string>(data[c].Data));
                LogRecord.Add(string.Format("Information Gain [{0}]: {1}", data[c].Name, IG)); ///
                if (IG > maxInformationGain)
                {
                    maxInformationGain = IG;
                    maxIGColumnIndex = c;
                }
            }
            node.isLeaf = false;
            node.PropertyName = data[maxIGColumnIndex].Name;
            LogRecord.Add(string.Format("Best attribute: [{0}]", node.PropertyName)); ///
            List<Label> branchList = detachData(null, new List<string>(data[maxIGColumnIndex].Data));
            foreach(Label branch in branchList)
            {
                LogRecord.Add(""); ///
                LogRecord.Add(string.Format("[{0}] : ({1})", node.PropertyName, branch.Name)); ///
                Node child = new Node();
                child.ParentBranchName = branch.Name;
                node.Children.Add(child);

                List<Column> subData = new List<Column>();
                for(int i = 0; i < data.Count; i++)
                {
                    subData.Add(new Column(data[i]));
                }

                for(int r = 0; r < subData[maxIGColumnIndex].Data.Count; r++)
                {
                    if(subData[maxIGColumnIndex].Data[r].CompareTo(branch.Name) != 0)
                    {
                        for(int c = 0; c < subData.Count; c++)
                        {
                            subData[c].Data.RemoveAt(r);
                        }
                        r--;
                        continue;
                    }
                }
                subData.RemoveAt(maxIGColumnIndex);
                selectAttribute(child, subData);
            }
        }

        public static double calInfo_D(List<string> Class)
        {
            double n = Class.Count;
            List<Label> labelList = detachData(null, Class);
            double IF = 0;
            foreach(Label lb in labelList)
            {
                IF += (lb.Count / n) * Math.Log(lb.Count / n, 2);
            }
            return -IF;
        }

        public static double calInfo_A(List<string> Class, List<string> Value)
        {
            double n = Class.Count;
            List<Label> labelList = detachData(Class, Value);
            double IF = 0;
            foreach(Label lb in labelList)
            {
                double n_2 = lb.Count;
                double temp = 0;
                foreach(Classification cl in lb.Class){
                    temp += (cl.Count / n_2) * Math.Log(cl.Count / n_2, 2);
                }
                IF -= (n_2 / n) * temp;
            }
            return IF;
        }

        public static List<Label> detachData(List<string> Class, List<string> Value)
        {
            List<Label> list = new List<Label>();
            for(int r = 0; r < Value.Count; r++)
            {
                string str = Value[r];
                bool existed_1 = false;
                for (int i = 0; i < list.Count; i++)
                {
                    if (list[i].Name.CompareTo(str) == 0)
                    {
                        list[i].Count++;
                        existed_1 = true;
                        if(Class != null)
                        {
                            bool existed_2 = false;
                            for(int j = 0; j < list[i].Class.Count; j++)
                            {
                                if(list[i].Class[j].Name.CompareTo(Class[r]) == 0)
                                {
                                    list[i].Class[j].Count++;
                                    existed_2 = true;
                                    break;
                                }
                            }
                            if(existed_2 == false)
                            {
                                Classification newClass = new Classification(Class[r], 1);
                                list[i].Class.Add(newClass);
                            }
                        }
                        break;
                    }
                }
                if (existed_1 == false)
                {
                    Label newLabel = new Label(str, 1);
                    if(Class != null)
                    {
                        newLabel.Class.Add(new Classification(Class[r], 1));
                    }
                    list.Add(newLabel);
                }
            }
            return list;
        }

        public static void addLogRecord(List<Column> list)
        {
            string header = "";
            for(int c = 0; c < list.Count; c++)
            {
                if(c == list.Count - 1)
                {
                    header += string.Format("[{0}]", list[c].Name);
                }
                else
                {
                    header += string.Format("[{0}] ", list[c].Name);
                }
            }
            LogRecord.Add(header);
            for(int r = 0; r < list[0].Data.Count; r++)
            {
                string value = "";
                for(int c = 0; c < list.Count; c++)
                {
                    if (c == list.Count - 1)
                    {
                        value += string.Format("({0})", list[c].Data[r]);
                    }
                    else
                    {
                        value += string.Format("({0}) ", list[c].Data[r]);
                    }
                }
                LogRecord.Add(value);
            }
        }

        //----- predict -----
        public static void predict(List<Column> OriginalInputData, string model, string output)
        {
            Console.WriteLine("Predicting...");
            Node root = readId3(model);
            Column Classification = new Column(root.ClassificationAttributeName);
            for(int r = 0; r < OriginalInputData[0].Data.Count; r++)
            {
                string[,] value = new string[2,OriginalInputData.Count];
                for(int c = 0; c < OriginalInputData.Count; c++)
                {
                    value[0, c] = OriginalInputData[c].Name;
                    value[1, c] = OriginalInputData[c].Data[r];
                }
                Classification.Data.Add(classify(root, value, 2, OriginalInputData.Count));
            }
            OriginalInputData.Add(Classification);
            Console.WriteLine("Saving excel file...");
            writeExcel(OriginalInputData, output);
        }
        
        public static string classify(Node node, string[,] value, int row, int column)
        {
            Node iNode = node;
            if (iNode.isLeaf)
                return iNode.PropertyName;

            bool found = false;
            for(int c = 0; c < column; c++)
            {
                if(value[0,c] == iNode.PropertyName)
                {
                    foreach(Node child in iNode.Children)
                    {
                        if(child.ParentBranchName == value[1, c])
                        {
                            found = true;
                            iNode = child;
                            break;
                        }
                    }
                    if (found)
                        break;
                }
            }
            return classify(iNode, value, row, column);
        }

        //----- visualize -----
        public static void visualize(string model, string output)
        {
            Node root = readId3(model);
            



        }
    }
}
