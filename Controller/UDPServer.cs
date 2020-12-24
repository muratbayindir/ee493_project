using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Controller
{
    public class UDPServer
    {
        private int listenPort;
        private Thread listenerThread;
        private bool work;

        public delegate void DataReceivedEventHandler(IPEndPoint endPoint, string message);
        public event DataReceivedEventHandler onDataReceived;

        private void StartListener()
        {
            UdpClient listener = new UdpClient(listenPort);
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);

            try
            {
                while (work)
                {
                    //Logger.WriteLine("Waiting for broadcast");
                    byte[] bytes = listener.Receive(ref groupEP);
                    string message = Encoding.ASCII.GetString(bytes, 0, bytes.Length);

                    //Logger.WriteLine($"Received broadcast from {groupEP} :");
                    //Logger.WriteLine($"{message}");

                    onDataReceived?.Invoke(groupEP, message);
                }
            }
            catch (SocketException e)
            {
                Logger.WriteLine(e.ToString(), LogType.Error);
            }
            finally
            {
                listener.Close();
            }
        }

        public void Start(int port)
        {
            listenPort = port;
            work = true;

            listenerThread = new Thread(new ThreadStart(StartListener));
            listenerThread.Start();
        }

        public void Stop()
        {
            if(listenerThread != null && listenerThread.IsAlive)
            {
                work = false;
                ClientTest();
                if (listenerThread.Join(100))
                    listenerThread.Abort();
            }
        }

        private void ClientTest()
        {
            Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            IPAddress broadcast = IPAddress.Parse("127.0.0.1");

            byte[] sendbuf = Encoding.ASCII.GetBytes("{\"hello\":\"world!\"}");
            IPEndPoint ep = new IPEndPoint(broadcast, listenPort);

            s.SendTo(sendbuf, ep);

            Logger.WriteLine("Message sent to the broadcast address");
        }
    }
}
