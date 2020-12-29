using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph;

namespace Controller
{
    public partial class FormRssiViewer : Form
    {
        double[] x;
        double[] y;
        const int maxSize = 300;
        string targetName;
        bool isNodeSet;
        bool dataType;

        public FormRssiViewer()
        {
            InitializeComponent();
            isNodeSet = false;
        }

        public void SetNode(Node node)
        {
            if (isNodeSet == true)
                return;

            isNodeSet = true;
            lblNode.Text = "Node Name : " + node.Name;

            x = new double[maxSize];
            y = new double[maxSize];

            for (int i = 0; i < maxSize; i++)
            {
                y[i] = 0;
                x[i] = i;
            }

            PointPairList pointPairs = new PointPairList(x, y);
            LineItem lineItem = zedGraphControl1.GraphPane.AddCurve(node.Name, pointPairs, Color.Red);
            lineItem.Line.Width = 3.0f;

            zedGraphControl1.GraphPane.Title.Text = "Rssi vs Time";
            zedGraphControl1.GraphPane.XAxis.Title.Text = "Time";
            zedGraphControl1.GraphPane.YAxis.Title.Text = "Rssi";

            zedGraphControl1.AxisChange();
            zedGraphControl1.Invalidate();
            zedGraphControl1.Refresh();

            cmbTargets.Items.AddRange(node.TargetNames);

            node.OnTargetAdded += Node_onTargetAdded;
            node.OnTargetRemoved += Node_onTargetRemoved;
            node.OnUpdate += Node_onUpdate;

            this.FormClosing += new FormClosingEventHandler(delegate (Object sender, FormClosingEventArgs a)
            {
                node.OnUpdate -= Node_onUpdate;
                node.OnTargetAdded -= Node_onTargetAdded;
                node.OnTargetRemoved -= Node_onTargetRemoved;
            });

            cmbType.SelectedIndex = 0;
        }

        private void Node_onUpdate(Node node, RSSIInfo info, double distance)
        {
            if(info.targetName == this.targetName)
            {
                for (int i = 0; i < maxSize - 1; i++)
                {
                    y[i] = y[i + 1];
                }

                y[maxSize - 1] = dataType ? info.rssiValue : distance;

                this.Invoke(new Action(() => {
                    PointPairList pointPairs = new PointPairList(x, y);
                    zedGraphControl1.GraphPane.CurveList[0].Points = pointPairs;

                    zedGraphControl1.AxisChange();
                    zedGraphControl1.Invalidate();
                    zedGraphControl1.Refresh();
                }));
            }
        }

        private void Node_onTargetRemoved(Node node, string targetName)
        {
            this.Invoke(new Action(() => {
                cmbTargets.Items.Remove(targetName);
            }));
        }

        private void Node_onTargetAdded(Node node, string targetName)
        {
            this.Invoke(new Action(() => {
                cmbTargets.Items.Add(targetName);
            }));
        }

        private void FormRssiViewer_Load(object sender, EventArgs e)
        {

        }

        private void cmbTargets_SelectedIndexChanged(object sender, EventArgs e)
        {
            targetName = cmbTargets.Text;
        }

        private void cmbType_SelectedIndexChanged(object sender, EventArgs e)
        {
            dataType = cmbType.SelectedIndex == 0;

            zedGraphControl1.GraphPane.Title.Text = dataType ? "Rssi vs Time" : "Distance vs Time";
            zedGraphControl1.GraphPane.XAxis.Title.Text = "Time";
            zedGraphControl1.GraphPane.YAxis.Title.Text = dataType ? "Rssi" : "Distance";
        }
    }
}
