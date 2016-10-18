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

                if (dirtyFlags.anims)
                    mainForm.refreshListBoxAnims();

                if (!entityView.isDisposed)
                    entityView.update(props, dirtyFlags);

                dirtyFlags.clear();
            }
        }
    }
}
