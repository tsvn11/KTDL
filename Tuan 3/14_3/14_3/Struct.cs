using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _14_3
{
    public static class Struct
    {
        public class Column
        {
            public Column()
            {
                Data = new List<string>();
            }
            public Column(string Name)
                :this()
            {
                this.Name = Name;
            }
            public Column(Column col)
            {
                Name = col.Name;
                Data = new List<string>(col.Data);
            }
            public string Name { get; set; }
            public List<string> Data { get; set; }
        }
        public class LabelEx
        {
            public LabelEx()
            {
                Class = new List<Classification>();
            }
            public LabelEx(string Name, int Count)
                :this()
            {
                this.Name = Name;
                this.Count = Count;
            }
            public string Name { get; set; }
            public int Count { get; set; }
            public List<Classification> Class { get; set; }
        }
        public class Classification
        {
            public Classification()
            {

            }
            public Classification(string Name, int Count)
            {
                this.Name = Name;
                this.Count = Count;
            }
            public string Name { get; set; }
            public int Count { get; set; }
        }

        public class Node
        {
            public Node()
            {
                Children = new List<Node>();
            }
            public Node(bool isLeaf, string PropertyName, string ParentBranchName)
                : this()
            {
                this.isLeaf = isLeaf;
                this.PropertyName = PropertyName;
                this.ParentBranchName = ParentBranchName;
            }
            public bool isLeaf { get; set; }
            public string PropertyName { get; set; }
            public string ParentBranchName { get; set; }
            public List<Node> Children { get; set; }
            public string ClassificationAttributeName { get; set; }
        }
    }
}
