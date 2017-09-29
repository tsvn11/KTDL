using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _14_4
{
    public static class Struct
    {
        public class Column
        {
            public Column()
            {
                Data = new List<bool>();
            }
            public Column(string Name)
                : this()
            {
                this.Name = Name;
            }
            public Column(Column col)
            {
                Name = col.Name;
                Data = new List<bool>(col.Data);
            }
            public string Name { get; set; }
            public List<bool> Data { get; set; }
        }
    }
}
