using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Config
{
    public partial class Main : Form
    {
        private int m_mouseDown = 0;
        private Point m_startPoint;
        private Point m_endPoint;
        private Pen m_pen;
        private List<Line> m_lines;
        public Main()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw | ControlStyles.AllPaintingInWmPaint, true);
            m_pen = new Pen(Color.Brown);
            m_lines = new List<Line>();
 
        }

        private void BtnOk_Click(object sender, EventArgs e)
        {
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
            m_lines.ForEach(line => graphics.DrawLine(m_pen, line.Start, line.End));
            if (m_mouseDown == 1)
            {
                graphics.DrawLine(m_pen, m_startPoint, m_endPoint);
            }
            myBuffer.Render(e.Graphics);
            graphics.Dispose();
            myBuffer.Dispose(); //释放资源
        }

        private void Main_MouseUp(object sender, MouseEventArgs e)
        {
            m_mouseDown = 0;
            //var x = m_endPoint.X;
            if (m_endPoint.Sub(m_startPoint).GetLength() > 200)
            {
                m_lines.Add(new Line(){Start = m_startPoint, End = m_endPoint});
            }
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
}
