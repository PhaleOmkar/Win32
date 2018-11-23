using System;
using System.Drawing;
using System.Windows.Forms;

public class CustomIconWindow : Form 
{
    public static void Main()
    {
        Application.Run(new CustomIconWindow());
    }

    public CustomIconWindow()
    {
        Icon = new Icon("earth.ico");
        Height = 800;
        Width = 600;
        ResizeRedraw = true;
        BackColor = Color.Black;
        this.KeyDown += new KeyEventHandler(MyKeyDown);
        this.MouseDown += new MouseEventHandler(MyMouseDown);
    }

    protected override void OnPaint(PaintEventArgs pea)
    {
        Graphics gfx = pea.Graphics;
        StringFormat strfmt = new StringFormat();
        strfmt.Alignment = StringAlignment.Center;
        strfmt.LineAlignment = StringAlignment.Center;
        gfx.DrawString("Custom Icon with Event Handler!",
            Font,
            new SolidBrush(System.Drawing.Color.Green),
            ClientRectangle,
            strfmt);
    }

    void MyKeyDown(Object sender, KeyEventArgs e)
    {
        MessageBox.Show("Key Pressed!");
    }

    void MyMouseDown(Object sender, MouseEventArgs e)
    {
        MessageBox.Show("Mouse Pressed!");
    }

}