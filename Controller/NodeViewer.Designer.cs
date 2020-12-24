namespace Controller
{
    partial class NodeViewer
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.lblNodeName = new System.Windows.Forms.Label();
            this.btnShowRssi = new System.Windows.Forms.Button();
            this.lblResult = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // toolTip1
            // 
            this.toolTip1.AutoPopDelay = 30000;
            this.toolTip1.InitialDelay = 500;
            this.toolTip1.ReshowDelay = 100;
            // 
            // lblNodeName
            // 
            this.lblNodeName.AutoSize = true;
            this.lblNodeName.Location = new System.Drawing.Point(4, 4);
            this.lblNodeName.Name = "lblNodeName";
            this.lblNodeName.Size = new System.Drawing.Size(33, 13);
            this.lblNodeName.TabIndex = 0;
            this.lblNodeName.Text = "Node";
            // 
            // btnShowRssi
            // 
            this.btnShowRssi.Location = new System.Drawing.Point(142, 1);
            this.btnShowRssi.Name = "btnShowRssi";
            this.btnShowRssi.Size = new System.Drawing.Size(41, 23);
            this.btnShowRssi.TabIndex = 1;
            this.btnShowRssi.Text = "Rssi";
            this.btnShowRssi.UseVisualStyleBackColor = true;
            this.btnShowRssi.Click += new System.EventHandler(this.btnShowRssi_Click);
            // 
            // lblResult
            // 
            this.lblResult.AutoSize = true;
            this.lblResult.Location = new System.Drawing.Point(91, 6);
            this.lblResult.Name = "lblResult";
            this.lblResult.Size = new System.Drawing.Size(35, 13);
            this.lblResult.TabIndex = 2;
            this.lblResult.Text = "label1";
            // 
            // NodeViewer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Controls.Add(this.lblResult);
            this.Controls.Add(this.btnShowRssi);
            this.Controls.Add(this.lblNodeName);
            this.Name = "NodeViewer";
            this.Size = new System.Drawing.Size(198, 25);
            this.Load += new System.EventHandler(this.NodeViewer_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label lblNodeName;
        private System.Windows.Forms.Button btnShowRssi;
        private System.Windows.Forms.Label lblResult;
    }
}
