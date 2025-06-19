using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace API_GUI
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void Slicing_Click(object sender, EventArgs e)
		{
			//Process slicing = Process.Start("E:\\Rateesh\\API\\IceSL.ahk");
			//slicing.WaitForExit();
			string path = "C:\\OptiLAM\\bin\\icesl_script.lua";
			string temp = Model.Text;
			for (int i = 0; i < temp.Length; i++)
			{
				//Console.WriteLine(i);
				if (temp[i] == '\\')
				{
					temp = temp.Insert(i, "\\");
					i++;
				}
			}
			File.WriteAllText(path, "emit(load(\'"+temp+"\'))");
			File.AppendAllText(path, "\nset_service(\'SVGSlicer\')");
			File.AppendAllText(path, "\nset_setting_value(\'slice_thickness_mm\', " + Thickness.Text + ")");
			File.AppendAllText(path, "\nrun_service(\'C:\\\\OptiLAM\\\\bin\\\\slices\')");

			Process slicing = Process.Start("C:\\OptiLAM\\bin\\Slicing_script.ahk");

			string temp = Model?.Text;
			if (string.IsNullOrEmpty(temp))
			{
    			MessageBox.Show("Please select an STL model file before slicing.");
   				return;
			}

			slicing.WaitForExit();

			path = "C:\\OptiLAM\\bin\\slices\\icesl_script_slices_SVG";
			while(!Directory.Exists(path))
			{ }
			Thread.Sleep(2000);
			int file_count = Directory.GetFiles(path, "*", SearchOption.TopDirectoryOnly).Length;
			label3.Text = "This file has " + file_count + " slices";
			path = "C:\\OptiLAM\\bin\\param\\Slices.txt";
			File.WriteAllText(path, file_count.ToString());
		}

		/*private void ListFiles_Click(object sender, EventArgs e)
		{
			Process list_files = Process.Start("E:\\Rateesh\\Nesting\\NestC++\\list_files.exe");
			list_files.WaitForExit();

			string path = "E:\\Rateesh\\Nesting\\NestC++\\Options.txt";
			string[] lines = File.ReadAllLines(path);

			Options.BeginUpdate();
			Options.Items.Clear();
			for (int i = 0; i < lines.Length; i++)
				Options.Items.Add(lines[i]);
			Options.EndUpdate();
		}

		private void Options_SelectedIndexChanged(object sender, EventArgs e)
		{
			string path = (string)Options.SelectedItem;
			path = path.Substring(1, path.Length - 2);
			int file_count = Directory.GetFiles(path, "*", SearchOption.TopDirectoryOnly).Length;
			label3.Text = "This file has " + file_count + " slices";
			path = "E:\\Rateesh\\Nesting\\NestC++\\Slices.txt";
			File.WriteAllText(path, file_count.ToString());
		}*/

		private void Nesting_Click(object sender, EventArgs e)
		{
			Process nesting = Process.Start("C:\\OptiLAM\\bin\\Nest_new.exe");
			nesting.WaitForExit();

			string path = "C:\\OptiLAM\\bin\\slices\\icesl_script_slices_SVG";
			if (Directory.Exists(path))
			{
				Directory.Delete(path, true);
			}

			Process output = Process.Start("C:\\OptiLAM\\bin\\Output folder.ahk");
		}

		private void Model_TextChanged(object sender, EventArgs e)
		{
			//string model = Model.Text;
			//string path = "E:\\Rateesh\\Nesting\\NestC++\\Model.txt";
			//File.WriteAllText(path, model);

		}

		private void Form1_Load(object sender, EventArgs e)
		{

		}

		private void Thickness_TextChanged(object sender, EventArgs e)
		{ 
			//string path = "E:\\Rateesh\\Nesting\\NestC++\\Thickness.txt";
			//File.WriteAllText(path, Thickness.Text);
		}

		private void Browse_Click(object sender, EventArgs e)
		{
			OpenFileDialog dialog1 = new OpenFileDialog();
			DialogResult result = dialog1.ShowDialog();
			if(result == DialogResult.OK)
			{
				Model.Text = dialog1.FileName;
			}
		}
	}
}
