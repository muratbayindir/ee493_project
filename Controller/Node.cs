using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace Controller
{
   public class Node
    {
        private List<RSSIInfo> rssiInfos;
        private List<string> targetNames;
        private int maxItemCount;
        private Point3D location;        

        public Point3D Location { get { return location; } }

        public delegate void NodeUpdatedEventHandler(Node node, RSSIInfo info);
        public event NodeUpdatedEventHandler OnUpdate;

        public delegate void TargetAddedEventHandler(Node node, string targetName);
        public event TargetAddedEventHandler OnTargetAdded;

        public delegate void TargetRemovedEventHandler(Node node, string targetName);
        public event TargetRemovedEventHandler OnTargetRemoved;

        public delegate void LocationUpdatedEventHandler(Node node, Point3D location);
        public event LocationUpdatedEventHandler OnLocationUpdated;

        private string name;

        Dictionary<string, Target> modems;

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
            modems = new Dictionary<string, Target>();

            foreach(Target target in FormMain.PredefinedTargets)
            {
                modems.Add(target.name, target);
            }
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

            if(modems.ContainsKey(info.targetName))
            {
                modems[info.targetName].rssi = info.rssiValue;
                OnLocationUpdated?.Invoke(this, UpdateLocation());
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

        private double RssiToMeter(double rssi)
        {
            return Math.Pow(10, (-69 - (rssi)) / (10 * 2));
        }

        private double RssiToMeter(string targetName, double rssi)
        {
            if(modems.TryGetValue(targetName, out Target target))
            {
                foreach(Fingerprint fingerprint in target.fingerprint)
                {
                    if(fingerprint.rssi >= rssi)
                    {
                        return fingerprint.distance;
                    }
                }
            }
            return 0.0;
        }

        private Point3D UpdateLocation()
        {
            double z1 = RssiToMeter(modems["SUPERONLINE_WiFi_4766"].rssi);
            double z2 = RssiToMeter(modems["esp2"].rssi);
            double z3 = RssiToMeter(modems["esp3"].rssi);

            z1 = z1 * z1;
            z2 = z2 * z2;
            z3 = z3 * z3;

            location.X = (z1 - z2 + 9) / 6; //for 1 meter square area 6->2, 9->1
            location.Y = (z1 - z3 + 9) / 6; //for 1 meter square area 6->2, 9->1
            location.Z = 0;

            return location;
        }
    }

    public class RSSIInfo
    {
        public long timeStamp;
        public string targetName;
        public double rssiValue;
    }

    public class Fingerprint
    {
        public double rssi { get; set; }
        public double distance { get; set; }
    }

    public class Target
    {
        public string name { get; set; }
        public List<double> location { get; set; }
        public double rssi { get; set; }
        public List<Fingerprint> fingerprint { get; set; }
    }

    public class Targets
    {
        public List<Target> targets { get; set; }
    }
}
