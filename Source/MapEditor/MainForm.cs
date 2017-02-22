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
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using DevExpress.XtraBars;

namespace MapEditor
{
	public partial class MainForm : DevExpress.XtraBars.Ribbon.RibbonForm
	{
		private DirtyFlags dirtyFlags;
		public System.Windows.Forms.Panel viewport;
		private MapProperties props;
		private Options options;
		private string _fileName;
		private string fileName
		{
			get { return _fileName; }
			set
			{
				_fileName = value;
				Text = _fileName;
			}
		}

		public MainForm(MapProperties props, Options options, DirtyFlags dirtyFlags)
		{
			InitializeComponent();

			viewport = panelView;

			fileName = "";
			this.props = props;
			this.options = options;
			this.dirtyFlags = dirtyFlags;

			TreeNode t = new TreeNode("root");
			sceneTreeView.Nodes.Add(t);
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
			if (String.Compare(suffix, ".xml", true) == 0)
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

		private void save(ProgressIndicator progressIndicator)
		{
			using (MemoryStream ms = new MemoryStream())
			{
				XmlWriterSettings settings = new XmlWriterSettings();
				settings.Indent = true;
				settings.Encoding = new UTF8Encoding(false);
				settings.NewLineChars = Environment.NewLine;
				using (XmlWriter xmlWriter = XmlWriter.Create(ms, settings))
				{
					xmlWriter.WriteStartElement("MapProperties");
					System.Xml.Serialization.XmlSerializer ser = new System.Xml.Serialization.XmlSerializer(typeof(SceneEntityProperties));
					foreach (KeyValuePair<String, SceneEntityProperties> kvp in props.entities)
					{
						ser.Serialize(xmlWriter, kvp.Value);
					}
					xmlWriter.WriteEndElement();
				}

				System.IO.StreamWriter fs = new System.IO.StreamWriter(fileName);
				string xml = Encoding.UTF8.GetString(ms.ToArray());
				fs.Write(xml);
				fs.Close();
			}

			progressIndicator.BeginInvoke(new Action(() =>
			{
				progressIndicator.Close();
			}));
		}

		private void load(ProgressIndicator progressIndicator)
		{
			dirtyFlags.clearView = true;
			props.copy(new MapProperties());
			XmlReaderSettings settings = new XmlReaderSettings();
			using (XmlReader xmlReader = XmlReader.Create(fileName, settings))
			{
				xmlReader.Read();
				xmlReader.ReadStartElement("MapProperties");
				while (xmlReader.NodeType != XmlNodeType.EndElement)
				{
					XmlSerializer xmlSerializer = new XmlSerializer(typeof(SceneEntityProperties));
					SceneEntityProperties _sep = (SceneEntityProperties)xmlSerializer.Deserialize(xmlReader);

					System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
					si.FileName = Application.StartupPath + "\\miniunz.exe";
					si.Arguments = String.Format("\"{0}\" -d \"{1}\" -o", _sep.entityFile, options.tempDir.Substring(0, options.tempDir.Length - 1));
					si.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
					System.Diagnostics.Process.Start(si).WaitForExit();

					System.Xml.Serialization.XmlSerializer ser = new System.Xml.Serialization.XmlSerializer(typeof(EntityProperties));
					System.IO.StreamReader fs = new System.IO.StreamReader(options.tempDir + "skeletalModel.xml");
					EntityProperties _ep = (EntityProperties)ser.Deserialize(fs);
					fs.Close();

					EntityProperties ep = new EntityProperties();
					ep = _ep;

					SceneEntityProperties sep = new SceneEntityProperties(ep);
					sep.entityFile = _sep.entityFile;
					sep.entityName = _sep.entityName;
					sep.position_x = _sep.position_x;
					sep.position_y = _sep.position_y;
					sep.position_z = _sep.position_z;
					sep.scale_x = _sep.scale_x;
					sep.scale_y = _sep.scale_y;
					sep.scale_z = _sep.scale_z;
					ep.mesh = Application.StartupPath + "\\res\\" + ep.mesh;
					ep.diffuseMap = Application.StartupPath + "\\res\\" + ep.diffuseMap;

					props.entities.Add(sep.entityName, sep);
					xmlReader.MoveToContent();
				}

				xmlReader.ReadEndElement();
			}

			progressIndicator.BeginInvoke(new Action(() =>
			{
				progressIndicator.Close();

				sceneTreeView.Nodes.Clear();
				TreeNode rootNode = new TreeNode("root");
				sceneTreeView.Nodes.Add(rootNode);
				foreach (KeyValuePair<String, SceneEntityProperties> kvp in props.entities)
				{
					TreeNode t = new TreeNode(kvp.Key);
					sceneTreeView.TopNode.Nodes.Add(t);
				}
				sceneTreeView.ExpandAll();
				entityPropertyGrid.SelectedObject = null;
			}));
		}

		private void barButtonItemSave_ItemClick(object sender, ItemClickEventArgs e)
		{
			if (fileName == "")
			{
				SaveFileDialog dialog = new SaveFileDialog();
				dialog.Filter = "|*.xml";
				dialog.ShowDialog();

				fileName = dialog.FileName;
			}

			if (fileName != "")
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
			dialog.Filter = "|*.xml";
			dialog.ShowDialog();

			if (dialog.FileName != "")
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
			dialog.Filter = "|*.xml";
			dialog.ShowDialog();
			if (dialog.FileName != "")
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
			dirtyFlags.clearView = true;
			fileName = "";
			props.copy(new MapProperties());
			entityPropertyGrid.SelectedObject = null;
			sceneTreeView.Nodes.Clear();
			TreeNode t = new TreeNode("root");
			sceneTreeView.Nodes.Add(t);
		}

		private String getFileName(String fullName, String ext)
		{
			String name = fullName.Substring(fullName.LastIndexOf('\\') + 1);
			int startSuffix = name.LastIndexOf(".");
			String suffix = name.Substring(startSuffix != -1 ? startSuffix : name.Length);
			if (String.Compare(suffix, ext, true) == 0)
				name = name.Substring(0, name.Length - suffix.Length);

			return name;
		}

		private void mainViewMenu_Add_Click(object sender, EventArgs e)
		{
			OpenFileDialog ofd = new OpenFileDialog();
			ofd.Filter = "实体文件(*.zip)|*.zip|所有文件|*.*";
			ofd.ValidateNames = true;
			ofd.CheckPathExists = true;
			ofd.CheckFileExists = true;
			if (ofd.ShowDialog() == DialogResult.OK)
			{
				string strFileName = ofd.FileName;

				System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
				si.FileName = Application.StartupPath + "\\miniunz.exe";
				si.Arguments = String.Format("\"{0}\" -d \"{1}\" -o", strFileName, options.tempDir.Substring(0, options.tempDir.Length - 1));
				si.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
				System.Diagnostics.Process.Start(si).WaitForExit();

				System.Xml.Serialization.XmlSerializer ser = new System.Xml.Serialization.XmlSerializer(typeof(EntityProperties));
				System.IO.StreamReader fs = new System.IO.StreamReader(options.tempDir + "skeletalModel.xml");
				EntityProperties p = (EntityProperties)ser.Deserialize(fs);
				fs.Close();

				p.mesh = Application.StartupPath + "\\res\\" + p.mesh;
				p.diffuseMap = Application.StartupPath + "\\res\\" + p.diffuseMap;

				SceneEntityProperties entityProps = new SceneEntityProperties(p);
				entityProps.entityFile = strFileName;
				entityProps.entityName = getFileName(strFileName, ".zip");
				if (props.entities.ContainsKey(entityProps.entityName))
					entityProps.entityName = entityProps.entityName + "_1";
				props.entities.Add(entityProps.entityName, entityProps);
				entityPropertyGrid.SelectedObject = entityProps;

				TreeNode t = new TreeNode(entityProps.entityName);
				sceneTreeView.TopNode.Nodes.Add(t);
				sceneTreeView.ExpandAll();
			}
		}

		private void sceneTreeView_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
		{
			if (props.entities.ContainsKey(e.Node.Text))
				entityPropertyGrid.SelectedObject = props.entities[e.Node.Text];
		}

		private void sceneTreeView_KeyDown(object sender, KeyEventArgs e)
		{
			if (sceneTreeView.SelectedNode != null && e.KeyCode == Keys.Delete)
			{
				String name = sceneTreeView.SelectedNode.Text;
				if (props.entities.ContainsKey(name))
				{
					props.entities[name].dirtyFlags.del = true;
					sceneTreeView.SelectedNode.Remove();
					entityPropertyGrid.SelectedObject = null;
				}
			}
		}
	}
}

