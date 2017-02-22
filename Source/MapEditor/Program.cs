using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor
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
			Options options = new Options();
			options.verticalFov = 60.0f / 180.0f * 3.14159f;
			MapProperties props = new MapProperties(options);
			MainForm mainForm = new MainForm(props, options, dirtyFlags);
			mainForm.Show();

			MapView mapView = new MapView(mainForm.viewport.Handle, options);
			mainForm.FormClosed += (object sender, FormClosedEventArgs e) =>
			{
				mapView.Dispose();
			};

			while (!mainForm.IsDisposed)
			{
				System.Windows.Forms.Application.DoEvents();

				if (dirtyFlags.clearView)
					mapView.clear();

				if (!mapView.isDisposed)
					mapView.update(props);

				dirtyFlags.clear();
			}
		}
	}
}

