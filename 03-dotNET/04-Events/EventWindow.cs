using System;
using System.Drawing;
using System.Windows.Forms;

public class EventWindow : Form 
{
    public static void Main()
    {
        Application.Run(new EventWindow());
    }

    public EventWindow()
    {
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
        gfx.DrawString("Press any key or mouse button!",
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