using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph;

namespace Controller
{
    public partial class NodeViewer : UserControl
    {
        bool showRssi = false;
        FormRssiViewer rssiViewer;
        Localizer localizer;

        public NodeViewer(Node node)
        {
            InitializeComponent();

            localizer = new Localizer();

            node.OnUpdate += new Node.NodeUpdatedEventHandler(onNodeUpdate);
        }

        private void NodeViewer_Load(object sender, EventArgs e)
        {
        }

        void onNodeUpdate(Node node, RSSIInfo info)
        {
            LocalizerPackage package = new LocalizerPackage();
            package.name = "test";
            package.targets = new List<Target>();
            Target target = new Target();
            target.location = new List<double>();
            target.location.Add(0.0);
            target.location.Add(1.0);
            target.location.Add(2.0);
            target.name = info.targetName;
            target.rssi = info.rssiValue;
            package.targets.Add(target);
            string result = localizer.UpdateTargets(package);

            //if(info.targetName == "SUPERONLINE_WiFi_4766")
            //{
            //}
            string tipText = "Name : " + node.Name + "\n"
                + "Rssi Value : " + info.rssiValue + "\n"
                + "Rssi Target : " + info.targetName + "\n"
                + "Last Updated : " + info.timeStamp;

            this.Invoke(new Action(() => {
                this.toolTip1.SetToolTip(lblNodeName, tipText);
                lblNodeName.Text = node.Name;
                lblResult.Text = result;

                if (showRssi)
                {
                    rssiViewer.SetNode(node);
                    showRssi = false;
                }
            }));
        }

        private void btnShowRssi_Click(object sender, EventArgs e)
        {
            rssiViewer = new FormRssiViewer();
            showRssi = true;
            rssiViewer.Show();
        }
    }
}
