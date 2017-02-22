namespace MapEditor
{
    partial class MainForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.ribbon = new DevExpress.XtraBars.Ribbon.RibbonControl();
			this.applicationMenu1 = new DevExpress.XtraBars.Ribbon.ApplicationMenu(this.components);
			this.barButtonItemNew = new DevExpress.XtraBars.BarButtonItem();
			this.barButtonItemOpen = new DevExpress.XtraBars.BarButtonItem();
			this.barButtonItemSave = new DevExpress.XtraBars.BarButtonItem();
			this.barButtonItemSaveAs = new DevExpress.XtraBars.BarButtonItem();
			this.barButtonItem1 = new DevExpress.XtraBars.BarButtonItem();
			this.barSubItem1 = new DevExpress.XtraBars.BarSubItem();
			this.ribbonStatusBar = new DevExpress.XtraBars.Ribbon.RibbonStatusBar();
			this.dockManager1 = new DevExpress.XtraBars.Docking.DockManager(this.components);
			this.dockPanel = new DevExpress.XtraBars.Docking.DockPanel();
			this.dockPanel2_Container = new DevExpress.XtraBars.Docking.ControlContainer();
			this.sceneTreeView = new System.Windows.Forms.TreeView();
			this.dockPanel1 = new DevExpress.XtraBars.Docking.DockPanel();
			this.dockPanel1_Container = new DevExpress.XtraBars.Docking.ControlContainer();
			this.entityPropertyGrid = new System.Windows.Forms.PropertyGrid();
			this.panelView = new System.Windows.Forms.Panel();
			this.mainViewMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.mainViewMenu_Add = new System.Windows.Forms.ToolStripMenuItem();
			this.defaultLookAndFeel1 = new DevExpress.LookAndFeel.DefaultLookAndFeel(this.components);
			((System.ComponentModel.ISupportInitialize)(this.ribbon)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.applicationMenu1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.dockManager1)).BeginInit();
			this.dockPanel.SuspendLayout();
			this.dockPanel2_Container.SuspendLayout();
			this.dockPanel1.SuspendLayout();
			this.dockPanel1_Container.SuspendLayout();
			this.mainViewMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// ribbon
			// 
			this.ribbon.ApplicationButtonDropDownControl = this.applicationMenu1;
			this.ribbon.ExpandCollapseItem.Id = 0;
			this.ribbon.Items.AddRange(new DevExpress.XtraBars.BarItem[] {
            this.ribbon.ExpandCollapseItem,
            this.barButtonItemNew,
            this.barButtonItemOpen,
            this.barButtonItemSave,
            this.barButtonItemSaveAs,
            this.barButtonItem1,
            this.barSubItem1});
			this.ribbon.Location = new System.Drawing.Point(0, 0);
			this.ribbon.MaxItemId = 9;
			this.ribbon.Name = "ribbon";
			this.ribbon.QuickToolbarItemLinks.Add(this.barButtonItem1);
			this.ribbon.RibbonStyle = DevExpress.XtraBars.Ribbon.RibbonControlStyle.Office2013;
			this.ribbon.Size = new System.Drawing.Size(1105, 50);
			this.ribbon.StatusBar = this.ribbonStatusBar;
			// 
			// applicationMenu1
			// 
			this.applicationMenu1.ItemLinks.Add(this.barButtonItemNew);
			this.applicationMenu1.ItemLinks.Add(this.barButtonItemOpen);
			this.applicationMenu1.ItemLinks.Add(this.barButtonItemSave);
			this.applicationMenu1.ItemLinks.Add(this.barButtonItemSaveAs);
			this.applicationMenu1.Name = "applicationMenu1";
			this.applicationMenu1.Ribbon = this.ribbon;
			// 
			// barButtonItemNew
			// 
			this.barButtonItemNew.Caption = "新建";
			this.barButtonItemNew.Id = 1;
			this.barButtonItemNew.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("barButtonItemNew.ImageOptions.Image")));
			this.barButtonItemNew.Name = "barButtonItemNew";
			this.barButtonItemNew.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.barButtonItemNew_ItemClick);
			// 
			// barButtonItemOpen
			// 
			this.barButtonItemOpen.Caption = "打开";
			this.barButtonItemOpen.Id = 2;
			this.barButtonItemOpen.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("barButtonItemOpen.ImageOptions.Image")));
			this.barButtonItemOpen.Name = "barButtonItemOpen";
			this.barButtonItemOpen.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.barButtonItemOpen_ItemClick);
			// 
			// barButtonItemSave
			// 
			this.barButtonItemSave.Caption = "保存";
			this.barButtonItemSave.Id = 3;
			this.barButtonItemSave.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("barButtonItemSave.ImageOptions.Image")));
			this.barButtonItemSave.ItemShortcut = new DevExpress.XtraBars.BarShortcut((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S));
			this.barButtonItemSave.Name = "barButtonItemSave";
			this.barButtonItemSave.ShowItemShortcut = DevExpress.Utils.DefaultBoolean.True;
			this.barButtonItemSave.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.barButtonItemSave_ItemClick);
			// 
			// barButtonItemSaveAs
			// 
			this.barButtonItemSaveAs.Caption = "另存为";
			this.barButtonItemSaveAs.Id = 4;
			this.barButtonItemSaveAs.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("barButtonItemSaveAs.ImageOptions.Image")));
			this.barButtonItemSaveAs.Name = "barButtonItemSaveAs";
			this.barButtonItemSaveAs.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.barButtonItemSaveAs_ItemClick);
			// 
			// barButtonItem1
			// 
			this.barButtonItem1.Caption = "另存为";
			this.barButtonItem1.Id = 5;
			this.barButtonItem1.ImageOptions.Image = ((System.Drawing.Image)(resources.GetObject("barButtonItem1.ImageOptions.Image")));
			this.barButtonItem1.Name = "barButtonItem1";
			this.barButtonItem1.ItemClick += new DevExpress.XtraBars.ItemClickEventHandler(this.barButtonItemSave_ItemClick);
			// 
			// barSubItem1
			// 
			this.barSubItem1.Caption = "barSubItem1";
			this.barSubItem1.Id = 6;
			this.barSubItem1.Name = "barSubItem1";
			// 
			// ribbonStatusBar
			// 
			this.ribbonStatusBar.Location = new System.Drawing.Point(0, 626);
			this.ribbonStatusBar.Name = "ribbonStatusBar";
			this.ribbonStatusBar.Ribbon = this.ribbon;
			this.ribbonStatusBar.Size = new System.Drawing.Size(1105, 31);
			// 
			// dockManager1
			// 
			this.dockManager1.Form = this;
			this.dockManager1.RootPanels.AddRange(new DevExpress.XtraBars.Docking.DockPanel[] {
            this.dockPanel,
            this.dockPanel1});
			this.dockManager1.TopZIndexControls.AddRange(new string[] {
            "DevExpress.XtraBars.BarDockControl",
            "DevExpress.XtraBars.StandaloneBarDockControl",
            "System.Windows.Forms.StatusBar",
            "System.Windows.Forms.MenuStrip",
            "System.Windows.Forms.StatusStrip",
            "DevExpress.XtraBars.Ribbon.RibbonStatusBar",
            "DevExpress.XtraBars.Ribbon.RibbonControl",
            "DevExpress.XtraBars.Navigation.OfficeNavigationBar",
            "DevExpress.XtraBars.Navigation.TileNavPane",
            "DevExpress.XtraBars.TabFormControl"});
			// 
			// dockPanel
			// 
			this.dockPanel.Controls.Add(this.dockPanel2_Container);
			this.dockPanel.Dock = DevExpress.XtraBars.Docking.DockingStyle.Left;
			this.dockPanel.ID = new System.Guid("9aab7eef-f75a-40e5-ae66-04e3989fa645");
			this.dockPanel.Location = new System.Drawing.Point(0, 50);
			this.dockPanel.Name = "dockPanel";
			this.dockPanel.OriginalSize = new System.Drawing.Size(195, 200);
			this.dockPanel.Size = new System.Drawing.Size(195, 576);
			this.dockPanel.Text = "场景";
			// 
			// dockPanel2_Container
			// 
			this.dockPanel2_Container.Controls.Add(this.sceneTreeView);
			this.dockPanel2_Container.Location = new System.Drawing.Point(4, 23);
			this.dockPanel2_Container.Name = "dockPanel2_Container";
			this.dockPanel2_Container.Size = new System.Drawing.Size(186, 549);
			this.dockPanel2_Container.TabIndex = 0;
			// 
			// sceneTreeView
			// 
			this.sceneTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.sceneTreeView.Location = new System.Drawing.Point(0, 0);
			this.sceneTreeView.Name = "sceneTreeView";
			this.sceneTreeView.Size = new System.Drawing.Size(186, 549);
			this.sceneTreeView.TabIndex = 0;
			this.sceneTreeView.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.sceneTreeView_NodeMouseDoubleClick);
			this.sceneTreeView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.sceneTreeView_KeyDown);
			// 
			// dockPanel1
			// 
			this.dockPanel1.Controls.Add(this.dockPanel1_Container);
			this.dockPanel1.Dock = DevExpress.XtraBars.Docking.DockingStyle.Right;
			this.dockPanel1.ID = new System.Guid("0a159aea-c922-45d0-bc8b-63bd6624f214");
			this.dockPanel1.Location = new System.Drawing.Point(905, 50);
			this.dockPanel1.Name = "dockPanel1";
			this.dockPanel1.OriginalSize = new System.Drawing.Size(200, 200);
			this.dockPanel1.Size = new System.Drawing.Size(200, 576);
			this.dockPanel1.Text = "属性";
			// 
			// dockPanel1_Container
			// 
			this.dockPanel1_Container.Controls.Add(this.entityPropertyGrid);
			this.dockPanel1_Container.Location = new System.Drawing.Point(5, 23);
			this.dockPanel1_Container.Name = "dockPanel1_Container";
			this.dockPanel1_Container.Size = new System.Drawing.Size(191, 549);
			this.dockPanel1_Container.TabIndex = 0;
			// 
			// entityPropertyGrid
			// 
			this.entityPropertyGrid.Dock = System.Windows.Forms.DockStyle.Fill;
			this.entityPropertyGrid.Location = new System.Drawing.Point(0, 0);
			this.entityPropertyGrid.Name = "entityPropertyGrid";
			this.entityPropertyGrid.Size = new System.Drawing.Size(191, 549);
			this.entityPropertyGrid.TabIndex = 0;
			// 
			// panelView
			// 
			this.panelView.ContextMenuStrip = this.mainViewMenu;
			this.panelView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panelView.Location = new System.Drawing.Point(195, 50);
			this.panelView.Name = "panelView";
			this.panelView.Size = new System.Drawing.Size(710, 576);
			this.panelView.TabIndex = 3;
			// 
			// mainViewMenu
			// 
			this.mainViewMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mainViewMenu_Add});
			this.mainViewMenu.Name = "mainViewMenu";
			this.mainViewMenu.Size = new System.Drawing.Size(101, 26);
			// 
			// mainViewMenu_Add
			// 
			this.mainViewMenu_Add.Name = "mainViewMenu_Add";
			this.mainViewMenu_Add.Size = new System.Drawing.Size(100, 22);
			this.mainViewMenu_Add.Text = "添加";
			this.mainViewMenu_Add.Click += new System.EventHandler(this.mainViewMenu_Add_Click);
			// 
			// MainForm
			// 
			this.AllowDrop = true;
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1105, 657);
			this.Controls.Add(this.panelView);
			this.Controls.Add(this.dockPanel1);
			this.Controls.Add(this.dockPanel);
			this.Controls.Add(this.ribbonStatusBar);
			this.Controls.Add(this.ribbon);
			this.Name = "MainForm";
			this.Ribbon = this.ribbon;
			this.StatusBar = this.ribbonStatusBar;
			this.Text = "MainForm";
			this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainForm_DragDrop);
			this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainForm_DragEnter);
			((System.ComponentModel.ISupportInitialize)(this.ribbon)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.applicationMenu1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.dockManager1)).EndInit();
			this.dockPanel.ResumeLayout(false);
			this.dockPanel2_Container.ResumeLayout(false);
			this.dockPanel1.ResumeLayout(false);
			this.dockPanel1_Container.ResumeLayout(false);
			this.mainViewMenu.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private DevExpress.XtraBars.Ribbon.RibbonControl ribbon;
        private DevExpress.XtraBars.Ribbon.RibbonStatusBar ribbonStatusBar;
        private DevExpress.XtraBars.Docking.DockManager dockManager1;
        private System.Windows.Forms.Panel panelView;
        private DevExpress.XtraBars.Docking.DockPanel dockPanel;
        private DevExpress.XtraBars.Docking.ControlContainer dockPanel2_Container;
        private DevExpress.XtraBars.Ribbon.ApplicationMenu applicationMenu1;
        private DevExpress.XtraBars.BarButtonItem barButtonItemNew;
        private DevExpress.XtraBars.BarButtonItem barButtonItemOpen;
        private DevExpress.XtraBars.BarButtonItem barButtonItemSave;
        private DevExpress.XtraBars.BarButtonItem barButtonItemSaveAs;
        private DevExpress.LookAndFeel.DefaultLookAndFeel defaultLookAndFeel1;
        private DevExpress.XtraBars.BarButtonItem barButtonItem1;
        private DevExpress.XtraBars.BarSubItem barSubItem1;
        private DevExpress.XtraBars.Docking.DockPanel dockPanel1;
        private DevExpress.XtraBars.Docking.ControlContainer dockPanel1_Container;
        private System.Windows.Forms.PropertyGrid entityPropertyGrid;
        private System.Windows.Forms.ContextMenuStrip mainViewMenu;
        private System.Windows.Forms.ToolStripMenuItem mainViewMenu_Add;
        private System.Windows.Forms.TreeView sceneTreeView;
    }
}