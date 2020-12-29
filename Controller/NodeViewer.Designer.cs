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
            this.btnInspect = new System.Windows.Forms.Button();
            this.lblLocation = new System.Windows.Forms.Label();
            this.pctObject = new System.Windows.Forms.PictureBox();
            this.pctBackground = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pctObject)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pctBackground)).BeginInit();
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
            // btnInspect
            // 
            this.btnInspect.Location = new System.Drawing.Point(214, 1);
            this.btnInspect.Name = "btnInspect";
            this.btnInspect.Size = new System.Drawing.Size(55, 23);
            this.btnInspect.TabIndex = 1;
            this.btnInspect.Text = "Inspect";
            this.btnInspect.UseVisualStyleBackColor = true;
            this.btnInspect.Click += new System.EventHandler(this.btnShowRssi_Click);
            // 
            // lblLocation
            // 
            this.lblLocation.AutoSize = true;
            this.lblLocation.Location = new System.Drawing.Point(87, 4);
            this.lblLocation.Name = "lblLocation";
            this.lblLocation.Size = new System.Drawing.Size(0, 13);
            this.lblLocation.TabIndex = 2;
            // 
            // pctObject
            // 
            this.pctObject.Location = new System.Drawing.Point(7, 39);
            this.pctObject.Name = "pctObject";
            this.pctObject.Size = new System.Drawing.Size(30, 30);
            this.pctObject.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pctObject.TabIndex = 3;
            this.pctObject.TabStop = false;
            // 
            // pctBackground
            // 
            this.pctBackground.Location = new System.Drawing.Point(150, 85);
            this.pctBackground.Name = "pctBackground";
            this.pctBackground.Size = new System.Drawing.Size(30, 30);
            this.pctBackground.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pctBackground.TabIndex = 4;
            this.pctBackground.TabStop = false;
            // 
            // NodeViewer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.Controls.Add(this.pctObject);
            this.Controls.Add(this.lblLocation);
            this.Controls.Add(this.btnInspect);
            this.Controls.Add(this.lblNodeName);
            this.Controls.Add(this.pctBackground);
            this.Name = "NodeViewer";
            this.Size = new System.Drawing.Size(274, 250);
            this.Load += new System.EventHandler(this.NodeViewer_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pctObject)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pctBackground)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Label lblNodeName;
        private System.Windows.Forms.Button btnInspect;
        private System.Windows.Forms.Label lblLocation;
        private System.Windows.Forms.PictureBox pctObject;
        private System.Windows.Forms.PictureBox pctBackground;
    }
}
