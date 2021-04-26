using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph;
using System.IO.Ports;
using System.IO;
using System.Threading;

namespace GUI_HTDKN
{
    public partial class Form1 : Form
    {
        string data =string.Empty;
        string temp_data, humd_data;
        double temp_value, humd_value;
        double realtime = 0;
        string b = string.Empty;

        public Form1()
        {
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            comboBox1.DataSource = SerialPort.GetPortNames();
            GraphPane myPane = Graph.GraphPane;
            myPane.Title.Text = "Đáp ứng nhiệt độ theo thời gian";
            myPane.XAxis.Title.Text = "Thời gian (ms)";
            myPane.YAxis.Title.Text = "Nhiệt độ (độ C)";
            RollingPointPairList list = new RollingPointPairList(60000);
           // setpoint();
            //setpoint1();
            
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 50;
            myPane.XAxis.Scale.MinorStep = 1;
            myPane.XAxis.Scale.MajorStep = 10;
            myPane.YAxis.Scale.Min = 0;
            myPane.YAxis.Scale.Max = 50;
            myPane.AxisChange();
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void groupBox2_Enter(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            { }
            else
            {

                //  List.Add(realtime, humd_value);
                serialPort1.PortName = comboBox1.Text;
                serialPort1.Open();
                _Status_.Text = "Connected";
                ClearZedGraph();
                setpoint();
                setpoint1();
                serialPort1.Write(textBox_temp.Text);
                timer1.Start();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                timer1.Stop();
                serialPort1.Close();
                _Status_.Text = "Disconnect";
            }
            else
            { }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            
            string chuoi = "t" + textBox_temp.Text + "e" + textBox_error.Text; 
            serialPort1.Write(chuoi);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void zedGraphControl1_Load(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            ///data = "";
            data = serialPort1.ReadExisting();
            if (data.Length < 9) return;
        
           // if (data == "") return;
           
            temp_data = data.Substring(0,4); //string
            humd_data = data.Substring(5, 4); //string
            
            //data = "";
            //data = string.Empty;
            //b = null;
               
              //  for (int j = 1; j < data.Length - 1; j++)
                //{
                  //  if (Char.IsDigit(data[j])) //Get number from string
                    //    b += data[j];
               // }
           //     if (data[1] == 45) b = "-" + b;
            
             //   Display(b);
               // datas = Convert.ToDouble(b);
          
            temp_value = double.Parse(temp_data); //double
            humd_value = double.Parse(humd_data);
            Display_temp(temp_data);
            Display_humd(humd_data);
            temp_data = null;
            humd_data = null;
            
            //realtime += 0.5;
            //Draw();
            //textBox1.Text = temp_data;
        }
        private delegate void DlDisplay(string data);
        private void Display_temp(string data)
        {
            if (textBox1.InvokeRequired)
            {
                DlDisplay sd = new DlDisplay(Display_temp);
                textBox1.Invoke(sd, new object[] { data });
            }
            else
            {
                textBox1.Text = humd_data;
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                Draw();
                realtime += 0.5; //0.5s
                //   status = 0;
            }
        }

        private void Display_humd(string data)
        {
            if (textBox2.InvokeRequired)
            {
                DlDisplay sd = new DlDisplay(Display_humd);
                textBox1.Invoke(sd, new object[] { data });
            }
            else
            {
                textBox2.Text = temp_data;
            }
        }
        private
    void Draw()
        {

            if (Graph.GraphPane.CurveList.Count <= 0)
                return;

            LineItem curve = Graph.GraphPane.CurveList[0] as LineItem;

            if (curve == null)
                return;

            IPointListEdit list = curve.Points as IPointListEdit;

            if (list == null)
                return;

            list.Add(realtime, humd_value); // Thêm điểm t
            Scale xScale = Graph.GraphPane.XAxis.Scale;
            Scale yScale = Graph.GraphPane.YAxis.Scale;

            // Tự động Scale theo trục x
            if (realtime > xScale.Max - xScale.MajorStep)
            {
                xScale.Max = realtime + xScale.MajorStep;
                xScale.Min = xScale.Max - 30;
            }

            // Tự động Scale theo trục y
            if (temp_value > yScale.Max - yScale.MajorStep)
            {
                yScale.Max = temp_value + yScale.MajorStep;
            }
            else if (temp_value < yScale.Min + yScale.MajorStep)
            {
                yScale.Min = temp_value - yScale.MajorStep;
            }

            Graph.AxisChange();
            Graph.Invalidate();
            Graph.Refresh();
        }

        private void setpoint()
        {
            GraphPane myPane1 = Graph.GraphPane;
            PointPairList pointPairList = new PointPairList();
           // for (int i = 0; i < 10000; i += 5)
            //{
              //  PointPair pointPair = new PointPair(i, 38);
               // pointPairList.Add(pointPair);
            //}
            LineItem sp = myPane1.AddCurve("temperature", pointPairList, Color.Red, SymbolType.None);
            Graph.AxisChange();
        }

        private void setpoint1()
        {
            GraphPane myPane2 = Graph.GraphPane;
            PointPairList pointPairList = new PointPairList();
            for (int i = 0; i < 10000; i += 5)
            {
                PointPair pointPair = new PointPair(i, 38);
                pointPairList.Add(pointPair);
            }
            LineItem spe = myPane2.AddCurve("setpoint", pointPairList, Color.Blue, SymbolType.None);
            Graph.AxisChange();
        }

        void ClearZedGraph()
        {
            Graph.GraphPane.CurveList.Clear(); // Xóa đường
            Graph.GraphPane.GraphObjList.Clear(); // Xóa đối tượng
            realtime = -0.5;
            Graph.AxisChange();
            Graph.Invalidate();

            GraphPane myPane = Graph.GraphPane;
            myPane.Title.Text = "Đáp ứng nhiệt độ theo thời gian";
            myPane.XAxis.Title.Text = "Thời gian (ms)";
            myPane.YAxis.Title.Text = "Nhiệt độ (độ C)";

            RollingPointPairList list = new RollingPointPairList(60000);
            //LineItem curve = myPane.AddCurve("Value", list, Color.Red, SymbolType.None);

            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 30;
            myPane.XAxis.Scale.MinorStep = 1;
            myPane.XAxis.Scale.MajorStep = 5;
            myPane.YAxis.Scale.Min = 0;
            myPane.YAxis.Scale.Max = 50;

            Graph.AxisChange();
        }


    }
}