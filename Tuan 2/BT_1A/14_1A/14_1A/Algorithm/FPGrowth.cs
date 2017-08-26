using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace _14_1A
{
    public class FPGrowth
    {
        FPTree fpTree;

        public FPGrowth()
        {
            fpTree = null;
        }
        public FPGrowth(FPTree tree)
            : this()
        {
            fpTree = tree;
        }
        public int GenerateFrequentItemSets()
        {
            List<Item> frequentItems = fpTree.FrequentItems;
            int totalFrequentItemSets = frequentItems.Count;
            foreach(Item anItem in frequentItems)
            {
                ItemSet anItemSet = new ItemSet();
                anItemSet.AddItem(anItem);
                totalFrequentItemSets += Mine(fpTree,anItemSet);
                Console.WriteLine(totalFrequentItemSets + " itemsets for "+ anItem.Symbol);
            }
            Console.WriteLine(totalFrequentItemSets);
            return totalFrequentItemSets;
        }

        private int Mine(FPTree fpTree, ItemSet anItemSet)
        {
            int minedItemSets = 0;
            FPTree projectedTree;
            projectedTree = fpTree.Project(anItemSet.GetLastItem());
            minedItemSets = projectedTree.FrequentItems.Count;
            foreach (Item anItem in projectedTree.FrequentItems)
            {
                ItemSet nextItemSet = anItemSet.Clone();
                nextItemSet.AddItem(anItem);
                minedItemSets += Mine(projectedTree,nextItemSet);
            }
            return minedItemSets;
        }
        public int CreateFPTreeAndGenerateFrequentItemsets(
            List<Row> inputData, float minSup)
        {
            var watch = System.Diagnostics.Stopwatch.StartNew();
            FPTree _fpTree = new FPTree(inputData, minSup);
            fpTree = _fpTree;
            _fpTree.Print(); //test
            int totalFrequentItemSets = GenerateFrequentItemSets();
            watch.Stop();
           
            // Output

            return totalFrequentItemSets;
        }

    }
}
