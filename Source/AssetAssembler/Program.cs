using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EntityEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            DirtyFlags dirtyFlags = new DirtyFlags();
            Options options = new Options(dirtyFlags);
            EntityProperties props = new EntityProperties(options, dirtyFlags);

            MainForm mainForm = new MainForm(props, options);
            mainForm.Show();

            EntityView entityView = new EntityView(mainForm.viewport.Handle, options);

            mainForm.FormClosed += (object sender, FormClosedEventArgs e) =>
            {
                entityView.Dispose();
            };

            options.verticalFov = 60.0f / 180.0f * 3.14159f;

            while(!mainForm.IsDisposed)
            {
			    System.Windows.Forms.Application.DoEvents();

                if (dirtyFlags.diffuseMap)
                {
                    string suffix = props.diffuseMap.Substring(props.diffuseMap.LastIndexOf('.') + 1);

                    if(String.Compare(suffix, "TGA", true) == 0)
                    {
                        ProgressIndicator progressIndicator = new ProgressIndicator();

                        System.Threading.Tasks.Task.Run(() =>
                        {
                            string src = props.diffuseMap;

                            string dxt_ktx = props.getDiffuseMap_dxt_ktx();
                            string dxt_pnt = props.getDiffuseMap_dxt_pnt();

                            {
                                System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
                                si.FileName = Application.StartupPath + "\\PVRTexToolCLI.exe";
                                si.Arguments = String.Format("-m -f BC1 -i \"{0}\" -o \"{1}\"", src, dxt_ktx);
                                si.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                                System.Diagnostics.Process.Start(si).WaitForExit();
                            }

                            {
                                System.Diagnostics.ProcessStartInfo si = new System.Diagnostics.ProcessStartInfo();
                                si.FileName = Application.StartupPath + "\\ktx2pnt.exe";
                                si.Arguments = String.Format("\"{0}\" \"{1}\"", dxt_ktx, dxt_pnt);
                                si.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
                                System.Diagnostics.Process.Start(si).WaitForExit();
                            }

                            progressIndicator.BeginInvoke(new Action(() =>
                            {
                                progressIndicator.Close();
                            }));
                        });

                        progressIndicator.ShowDialog(mainForm);
                    }
                }

                if (dirtyFlags.anims)
                    mainForm.refreshListBoxAnims();

                if (!entityView.isDisposed)
                    entityView.update(props, dirtyFlags);

                dirtyFlags.clear();
            }
        }
    }
}
