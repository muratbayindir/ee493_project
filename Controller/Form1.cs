using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph;

namespace Controller
{
    public partial class FormMain : Form
    {
        public static UDPServer udpServer;
        Dictionary<string, Node> nodes;

        private static Targets targets_;

        public static Targets targets
        {
            get { return targets_; }
        }


        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            targets_ = (Targets)JsonConvert.DeserializeObject(File.ReadAllText("predefinedLocations.json"), typeof(Targets));

            nodes = new Dictionary<string, Node>();
            udpServer = new UDPServer();
            udpServer.Start(2828);
            udpServer.onDataReceived += Server_onDataReceived;
        }

        private void Server_onDataReceived(System.Net.IPEndPoint endPoint, string message)
        {
            Logger.WriteLine(endPoint.ToString() + " : " + message);


            /*
Info : 192.168.1.22:50075 : {
	"device_id":	"DEVICE_001",
	"timestamp":	7550,
	"payload":	{
		"type":	"rssi_info",
		"rssi_infos":	[{
				"rssi_value":	-38,
				"target_device":	"SUPERONLINE_WiFi_4766"
			}]
	}
}
             */


            try
            {
                dynamic root = JsonConvert.DeserializeObject(message);

                if (root.device_id != null)
                {
                    if (!nodes.ContainsKey(root.device_id.Value))
                    {
                        Node node = new Node(root.device_id.Value);
                        nodes.Add(root.device_id.Value, node);
                        NodeViewer nodeViewer = new NodeViewer(node);
                        this.Invoke(new Action(() => {
                            flpDevices.Controls.Add(nodeViewer);
                        }));
                    }

                    if (root.payload.type.Value == "rssi_info")
                    {
                        foreach(dynamic rssi_info in root.payload.rssi_infos)
                        {
                            RSSIInfo rssInfo = new RSSIInfo();

                            rssInfo.timeStamp = root.timestamp.Value;
                            rssInfo.targetName = rssi_info.target_name.Value;
                            rssInfo.rssiValue = rssi_info.value.Value;

                            nodes[root.device_id.Value].AddRssiInfo(rssInfo);
                        }
                    }
                }
            }
            catch(Exception ex)
            {
                Logger.WriteLine("Parse Error : " + ex.Message, LogType.Error);
            }
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            udpServer.Stop();
        }
    }
}
