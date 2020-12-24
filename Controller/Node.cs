using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Controller
{
   public class Node
    {
        private List<RSSIInfo> rssiInfos;
        private List<string> targetNames;
        private int maxItemCount;

        public delegate void NodeUpdatedEventHandler(Node node, RSSIInfo info);
        public event NodeUpdatedEventHandler OnUpdate;

        public delegate void TargetAddedEventHandler(Node node, string targetName);
        public event TargetAddedEventHandler OnTargetAdded;

        public delegate void TargetRemovedEventHandler(Node node, string targetName);
        public event TargetRemovedEventHandler OnTargetRemoved;

        private string name;

        public string Name
        {
            get { return name; }
        }

        public string[] TargetNames => targetNames.ToArray();

        public Node(string name, int maxItemCount = 20)
        {
            this.name = name;
            rssiInfos = new List<RSSIInfo>();
            targetNames = new List<string>();
            this.maxItemCount = maxItemCount;
        }

        public void AddRssiInfo(RSSIInfo info)
        {
            rssiInfos.Add(info);

            if (rssiInfos.Count > maxItemCount)
            {
                rssiInfos.RemoveAt(0);
            }

            if (!targetNames.Contains(info.targetName))
            {
                targetNames.Add(info.targetName);
                OnTargetAdded?.Invoke(this, info.targetName);
            }

            OnUpdate?.Invoke(this, info);
        }

        public RSSIInfo GetLastRssiInfo()
        {
            if (rssiInfos.Count > 0)
            {
                return rssiInfos[rssiInfos.Count - 1];
            }

            return new RSSIInfo();
        }

        public RSSIInfo GetLastRssiInfoFromTarget(string targetName)
        {
            int i = rssiInfos.Count;

            while(--i >= 0)
            {
                if(rssiInfos[i].targetName == targetName)
                {
                    return rssiInfos[i];
                }
            }

            if(targetNames.Contains(targetName))
            {
                targetNames.Remove(targetName);
                OnTargetRemoved?.Invoke(this, targetName);
            }

            return new RSSIInfo();
        }


    }

    public class RSSIInfo
    {
        public long timeStamp;
        public string targetName;
        public double rssiValue;
    }
}
