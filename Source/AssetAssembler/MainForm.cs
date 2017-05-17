using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using DevExpress.XtraBars;

namespace EntityEditor
{
    public partial class MainForm : DevExpress.XtraBars.Ribbon.RibbonForm
    {
        public System.Windows.Forms.Panel viewport;
        private EntityProperties props;
        private Options options;
        private string _fileName;
        private string fileName
        {
            get
            {
                return _fileName;
            }

            set
            {
                _fileName = value;
                Text = _fileName;
            }
        }

        public MainForm(EntityProperties props, Options options)
        {
            InitializeComponent();

            viewport = panel1;
            this.props = props;
            this.options = options;
            fileName = "";

            propertyGrid1.SelectedObject = props;
        }

        public void refreshListBoxAnims()
        {
            listBoxAnims.DataSource = null;
            listBoxAnims.DataSource = props.anims;
            listBoxAnims.DisplayMember = "displayName";
            listBoxAnims.ValueMember = "fileName";
        }

        private void MainForm_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.All;
        }

        private void MainForm_DragDrop(object sender, DragEventArgs e)
        {
            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
            string file = files[0];
            string suffix = file.Substring(file.LastIndexOf('.'));

            if(String.Compare(suffix, ".PNM", true) == 0)
            {
                props.mesh = file;
                propertyGrid1.Refresh();
            }
            else if(String.Compare(suffix, ".TGA", true) == 0)
            {
                props.diffuseMap = file;
                propertyGrid1.Refresh();
            }
            else if(String.Compare(suffix, ".PNA", true) == 0)
            {
                Anim anim = new Anim(file);

                if (!props.anims.Contains(anim))
                    props.anims.Add(anim);

                propertyGrid1.Refresh();
            }
            else if (String.Compare(suffix, ".zip", true) == 0)
            {
                fileName = file;

                ProgressIndicator progressIndicator = new ProgressIndicator();

                System.Threading.Tasks.Task.Run(() =>
                {
                    load(progressIndicator);
                });

                progressIndicator.ShowDialog(this);
            }
        }

        private void listBoxAnims_SelectedValueChanged(object sender, EventArgs e)
        {
            options.curAnims.Clear();

            foreach(Object o in listBoxAnims.SelectedItems)
                options.curAnims.Add((Anim)o);
        }

        private void save(ProgressIndicator progressIndicator)
        {
            string entityFileName = options.tempDir + "skeletalModel.xml";
            string mesh = props.mesh;
            string diffuseMapTga = props.diffuseMap;
            string diffuseMapDxtPnt = props.getDiffuseMap_dxt_pnt();
            string diffuseMapEtcPnt = props.getDiffuseMap_etc_pnt();

            string diffuseMapEtcKtx = props.getDiffuseMap_etc_ktx();

            {
                System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
                si.FileName = Application.StartupPath + "\\PVRTexToolCLI.exe";
                si.Arguments = String.Format("-m -f ETC2_RGB -q etcfastperceptual -i \"{0}\" -o \"{1}\"", diffuseMapTga, diffuseMapEtcKtx);
                si.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                System.Diagnostics.Process.Start(si).WaitForExit();
            }

            {
                System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
                si.FileName = Application.StartupPath + "\\ktx2pnt.exe";
                si.Arguments = String.Format("\"{0}\" \"{1}\"", diffuseMapEtcKtx, diffuseMapEtcPnt);
                si.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                System.Diagnostics.Process.Start(si).WaitForExit();
            }

            {
                System.Xml.Serialization.XmlSerializer ser = new System.Xml.Serialization.XmlSerializer(typeof(EntityProperties));
                System.IO.StreamWriter fs = new System.IO.StreamWriter(entityFileName);
                ser.Serialize(fs, props.getPortable());
                fs.Close();
            }

            {
                System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
                si.FileName = Application.StartupPath + "\\minizip.exe";

                string args = String.Format("-9 -j \"{0}\" \"{1}\" \"{2}\" \"{3}\" \"{4}\" \"{5}\"", fileName, entityFileName, mesh, diffuseMapTga, diffuseMapDxtPnt, diffuseMapEtcPnt);

                foreach (Anim anim in props.anims)
                    args += " \"" + anim.fileName + "\"";

                si.Arguments = args;
                si.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                System.Diagnostics.Process.Start(si).WaitForExit();
            }

            progressIndicator.BeginInvoke(new Action(() =>
            {
                progressIndicator.Close();
            }));
        }

        private void load(ProgressIndicator progressIndicator)
        {
            {
                System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
                si.FileName = Application.StartupPath + "\\miniunz.exe";
                si.Arguments = String.Format("\"{0}\" -d \"{1}\" -o", fileName, options.tempDir.Substring(0, options.tempDir.Length - 1));
                si.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                System.Diagnostics.Process.Start(si).WaitForExit();
            }

            {
                System.Xml.Serialization.XmlSerializer ser = new System.Xml.Serialization.XmlSerializer(typeof(EntityProperties));
                System.IO.StreamReader fs = new System.IO.StreamReader(options.tempDir + "skeletalModel.xml");
                EntityProperties _props = (EntityProperties)ser.Deserialize(fs);
                fs.Close();
                props.copy(_props);
                options.curAnims.Clear();
            }

            progressIndicator.BeginInvoke(new Action(() =>
            {
                progressIndicator.Close();
            }));
        }

        private void barButtonItemSave_ItemClick(object sender, ItemClickEventArgs e)
        {
            if(fileName == "")
            {
                SaveFileDialog dialog = new SaveFileDialog();
                dialog.Filter = "|*.zip";
                dialog.ShowDialog();

                fileName = dialog.FileName;
            }

            if(fileName != "")
            {
                ProgressIndicator progressIndicator = new ProgressIndicator();

                System.Threading.Tasks.Task.Run(() =>
                {
                    save(progressIndicator);
                });

                progressIndicator.ShowDialog(this);
            }
        }

        private void barButtonItemSaveAs_ItemClick(object sender, ItemClickEventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Filter = "|*.zip";
            dialog.ShowDialog();

            if(dialog.FileName != "")
            {
                fileName = dialog.FileName;

                ProgressIndicator progressIndicator = new ProgressIndicator();

                System.Threading.Tasks.Task.Run(() =>
                {
                    save(progressIndicator);
                });

                progressIndicator.ShowDialog(this);
            }
        }

        private void barButtonItemOpen_ItemClick(object sender, ItemClickEventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "|*.zip";
            dialog.ShowDialog();

            if(dialog.FileName != "")
            {
                fileName = dialog.FileName;

                ProgressIndicator progressIndicator = new ProgressIndicator();

                System.Threading.Tasks.Task.Run(() =>
                {
                    load(progressIndicator);
                });

                progressIndicator.ShowDialog(this);
            }
        }

        private void barButtonItemNew_ItemClick(object sender, ItemClickEventArgs e)
        {
            fileName = "";
            props.copy(new EntityProperties());
        }
    }
}