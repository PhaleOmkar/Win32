using System;
using System.Drawing;
using System.Windows.Forms;

public class CSharpWindow : Form
{
    public static void Main()
    {
        Application.Run(new CSharpWindow());
    }

    public CSharpWindow()
    {
        Height = 800;
        Width = 600;
        ResizeRedraw = true;
    }

}