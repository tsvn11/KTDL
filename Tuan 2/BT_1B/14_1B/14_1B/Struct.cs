using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _14_1B
{
    public class Struct
    {
        public class Column
        {
            public Column()
            {
                name = "";
                value = new List<bool>();
            }
            public Column(string name)
            {
                this.name = name;
                value = new List<bool>();
            }

            public string name;
            public List<bool> value;
        }

        public class Row
        {
            public Row()
            {
                value = new List<string>();
            }

            public List<string> value;
        }
    }
}
