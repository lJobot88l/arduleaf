using System;
using System.IO.Ports;
namespace host
{
    class Program
    {
        static SerialPort SerialPort = new System.IO.Ports.SerialPort();
        static void Main(string[] args)
        {
			SerialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceived);
			SerialPort.PortName = "COM4";
			SerialPort.BaudRate = 9600;
			SerialPort.Parity = Parity.None;
			SerialPort.DataBits = 8;
			SerialPort.StopBits = StopBits.One;
			SerialPort.ReceivedBytesThreshold = 1;
            SerialPort.Open();
            while(true)
            {
                Console.WriteLine("Please enter command");
                string command = Console.ReadLine();
                switch(command)
                {
                    case "COL":
                        byte[] buffer = new byte[64];
                        buffer[0] = 0x00;
                        buffer[1] = 0x02;
                        buffer[2] = 0xff;
                        buffer[3] = 0xff;
                        buffer[4] = 0xff;
                        SerialPort.Write(buffer, 0, 64);
                        break;
                }
            }
        }
		private static void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
		{
            byte[] b = new byte[SerialPort.BytesToRead];
			SerialPort.Read(b, 0, b.Length);
            foreach(byte bb in b)
            {
                Console.Write(bb.ToString("X2") + " ");
            }
		}
    }
}
