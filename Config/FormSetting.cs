using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace Config
{
    public partial class FormSetting : Form
    {
        private int m_mouseDown = 0;
        private Point m_startPoint;
        private Point m_endPoint;
        private Pen m_pen;
        private List<split_area> splitAreas;
        public FormSetting()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw | ControlStyles.AllPaintingInWmPaint, true);
            m_pen = new Pen(Color.Red);
            if (File.Exists("config.json"))
            {
                ServiceApi.SetHook();
                Close();
            }
            string json = File.ReadAllText("config.json");
            splitAreas = JsonConvert.DeserializeObject<List<split_area>>(json);

        }

        private void BtnOk_Click(object sender, EventArgs e)
        {
            string json = JsonConvert.SerializeObject(splitAreas);
            File.WriteAllText("config.json", json);
            ServiceApi.UnHook();
            ServiceApi.SetHook();
            Close();
        }

        private void BtnCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void Main_MouseDown(object sender, MouseEventArgs e)
        {
            m_mouseDown = 1;
            m_startPoint = e.Location;
        }

        private void Main_MouseMove(object sender, MouseEventArgs e)
        {
            m_endPoint = e.Location;
            this.Invalidate();
        }

        private void Main_Paint(object sender, PaintEventArgs e)
        {
            Rectangle rect = e.ClipRectangle;
            BufferedGraphicsContext currentContext = BufferedGraphicsManager.Current;
            BufferedGraphics myBuffer = currentContext.Allocate(e.Graphics, e.ClipRectangle);
            Graphics graphics = myBuffer.Graphics;
            graphics.SmoothingMode = SmoothingMode.HighQuality;
            graphics.PixelOffsetMode = PixelOffsetMode.HighSpeed;
            graphics.Clear(this.BackColor);
            Font font = new Font("雅黑", 10);
            Pen pen = new Pen(Color.Chartreuse);
            pen.Width = 2;
            graphics.DrawLine(pen, m_endPoint.X - 100, m_endPoint.Y, m_endPoint.X + 100, m_endPoint.Y);
            graphics.DrawLine(pen, m_endPoint.X, m_endPoint.Y - 100, m_endPoint.X, m_endPoint.Y + 100);
            graphics.DrawString(m_endPoint.ToString(), font, new SolidBrush(Color.Red), m_endPoint);

            graphics.DrawString((m_endPoint.X * 1.0 / Width).ToString("F4"), font, new SolidBrush(Color.Red), (float) (m_endPoint.X * 0.5), m_endPoint.Y);
            graphics.DrawString((m_endPoint.Y *1.0  / Height).ToString("F4"), font, new SolidBrush(Color.Red), m_endPoint.X, (float)(m_endPoint.Y * 0.5));

            splitAreas.ForEach(area =>
                               {
                                   graphics.DrawRectangle(m_pen, area.rect.left, area.rect.top, area.rect.right - area.rect.left, area.rect.bottom - area.rect.top);
                               });
            if (m_mouseDown == 1)
            {
                graphics.DrawRectangle(m_pen, m_startPoint.X, m_startPoint.Y, m_endPoint.X - m_startPoint.X, m_endPoint.Y - m_startPoint.Y);

                graphics.DrawString(m_startPoint.ToString(), font, new SolidBrush(Color.Red),m_startPoint);

            }
            myBuffer.Render(e.Graphics);
            myBuffer.Dispose(); //释放资源
        }

        private void Main_MouseUp(object sender, MouseEventArgs e)
        {
            m_mouseDown = 0;
            //var x = m_endPoint.X;
            if (m_endPoint.Sub(m_startPoint).GetLength() > 200)
            {
                splitAreas.Add(new split_area(){rect = new RECT(){left = m_startPoint.X, top = m_startPoint.Y, bottom = m_endPoint.Y, right = m_endPoint.X}, level = 0});
            }
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                e.Cancel = true;
                WindowState = FormWindowState.Minimized;
                notifyIcon.Visible = true;
                Hide();
            }
        }

        private void notifyIcon_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (Visible)
            {
                WindowState = FormWindowState.Minimized;
                notifyIcon.Visible = true;
                Hide();
            }
            else
            {
                Visible = true;
                WindowState = FormWindowState.Maximized;
                Activate();
            }
        }

        private void MenuSetting_Click(object sender, EventArgs e)
        {
            if (Visible)
            {
                WindowState = FormWindowState.Minimized;
                notifyIcon.Visible = true;
                Hide();
            }
            else
            {
                Visible = true;
                WindowState = FormWindowState.Maximized;
                Activate();
            }
        }

        private void MenuExit_Click(object sender, EventArgs e)
        {
            ServiceApi.UnHook();
            Close();
        }
    }

    internal class Line
    {
        public Point Start { [DebuggerStepThrough] get; [DebuggerStepThrough] set; }

        public Point End { [DebuggerStepThrough] get; [DebuggerStepThrough] set; }
}

     static class PointExtend
    {
        public static Point Sub(this Point s1, Point s2)
        {
            int x = s2.X - s1.X;
            int y = s2.Y - s1.Y;
            return new Point(x, y);
        }

        public static Point Add(this Point s1, Point s2)
        {
            int x = s2.X + s1.X;
            int y = s2.Y + s1.Y;
            return new Point(x, y);
        }

        public static Point Mul(this Point s1, int d)
        {
            int x = s1.X * d;
            int y = s1.Y * d;
            return new Point(x, y);
        }

        public static double GetLength(this Point point)
        {
            return Math.Sqrt(point.X * point.X + point.Y * point.Y);
        }
    }


     struct split_area
     {
         //区域范围
        public RECT rect;
        //优先级
        public int level;
     };

    struct RECT
     {
         public int left;
         public int top;
         public int right;
         public int bottom;
     }
}
