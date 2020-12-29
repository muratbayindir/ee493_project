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
        Point3D currentLoc;
        private double rectX = 10;
        private double rectY = 30;
        private double rectW = 215;
        private double rectH = 215;
        private double objW = 25;
        private double objH = 25;
        private double maxW = 1.0;
        private double maxH = 1.0;
        string backgroundImageFile = "background.png";
        string objectImageFile = "object.png";

        public NodeViewer(Node node)
        {
            InitializeComponent();

            currentLoc = new Point3D(0, 0, 0);

            pctObject.BackColor = Color.Transparent;
            pctObject.Size = new Size((int)objW, (int)objH);
            pctObject.SizeMode = PictureBoxSizeMode.StretchImage;
            pctObject.Image = Image.FromFile(objectImageFile);

            pctBackground.BackColor = Color.Transparent;
            pctBackground.Location = new Point((int)rectX, (int)rectY);
            pctBackground.Size = new Size((int)rectW, (int)rectH);
            pctBackground.SizeMode = PictureBoxSizeMode.StretchImage;
            pctBackground.Image = Image.FromFile(backgroundImageFile);

            node.OnUpdate += new Node.NodeUpdatedEventHandler(onNodeUpdate);

            node.OnLocationUpdated += Node_OnLocationUpdated;

            this.Paint += (o, e) => {
                Graphics g = e.Graphics;
                double penWidth = 1.5;
                using (Pen selPen = new Pen(Color.Blue, (float)penWidth))
                {
                    g.DrawRectangle(selPen, (float)(rectX - penWidth), (float)(rectY - penWidth)
                        , (float)(rectW + 2 * penWidth), (float)(rectH + 2 * penWidth));
                }
            };
        }

        private void Node_OnLocationUpdated(Node node, Point3D location)
        {
            currentLoc = location;

            this.Invoke(new Action(() =>
            {
                DrawNode();
                lblLocation.Text = "(" + currentLoc.X.ToString("N2") + "," + currentLoc.Y.ToString("N2")
                + "," + currentLoc.Z.ToString("N2") + ")";
            }));
        }

        private void NodeViewer_Load(object sender, EventArgs e)
        {
        }

        void onNodeUpdate(Node node, RSSIInfo info, double distance)
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

        public void DrawNode()
        {
            // test purposes
            //currentLoc.X = DateTime.Now.Second / 60.0;
            //currentLoc.Y = DateTime.Now.Millisecond / 1000.0;

            double x = currentLoc.X / maxW * rectW + rectX - objW / 2.0;
            double y = rectH - currentLoc.Y / maxH * rectH + rectY - objH / 2.0;
            pctObject.Location = new Point((int)x, (int)y);
        }
    }
}
