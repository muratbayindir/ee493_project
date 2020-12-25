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
using System.Windows.Media.Media3D;

namespace Controller
{
    public partial class NodeViewer : UserControl
    {
        bool showRssi = false;
        FormRssiViewer rssiViewer;

        public NodeViewer(Node node)
        {
            InitializeComponent();

            node.OnUpdate += new Node.NodeUpdatedEventHandler(onNodeUpdate);

            node.OnLocationUpdated += new Node.LocationUpdatedEventHandler(delegate (Node n, Point3D loc)
            {
                this.Invoke(new Action(() =>
                {
                    lblLocation.Text = "(" + loc.X.ToString("N2") + "," + loc.Y.ToString("N2") 
                    + "," + loc.Z.ToString("N2") + ")";
                }));
            });
        }

        private void NodeViewer_Load(object sender, EventArgs e)
        {
        }

        void onNodeUpdate(Node node, RSSIInfo info)
        {            
            string tipText = "Name : " + node.Name + "\n"
                + "Rssi Value : " + info.rssiValue + "\n"
                + "Rssi Target : " + info.targetName + "\n"
                + "Last Updated : " + info.timeStamp;

            this.Invoke(new Action(() => {
                this.toolTip1.SetToolTip(lblNodeName, tipText);
                lblNodeName.Text = node.Name;

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
