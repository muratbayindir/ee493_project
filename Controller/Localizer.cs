using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace Controller
{
    public class Localizer
    {
        NamedPipeClientStream pipeClient;
        StreamString streamPipe;

        public Localizer()
        {
            pipeClient =
                new NamedPipeClientStream(".", "StronicsLocalizer",
                    PipeDirection.InOut, PipeOptions.None,
                    TokenImpersonationLevel.Impersonation);

            Logger.WriteLine("Connecting to server...\n");
            pipeClient.Connect();

            streamPipe = new StreamString(pipeClient);
        }

        ~Localizer()
        {
            pipeClient.Close();
        }

        public string UpdateTargets(LocalizerPackage package)
        {
            // {"name":"test", "targets":[{"name":"1","location":[1,2,3],"rssi":15},{}]}
            string strPackage = JsonConvert.SerializeObject(package);
            streamPipe.WriteString(strPackage);
            return streamPipe.ReadString();
        }
    }

    public class LocalizerPackage
    {
        public string name { get; set; }
        public List<Target> targets { get; set; }
    }

    public class Target
    {
        public string name { get; set; }
        public List<double> location { get; set; }
        public double rssi { get; set; }
    }    

    // Defines the data protocol for reading and writing strings on our stream.
    public class StreamString
    {
        private Stream ioStream;
        private Encoding streamEncoding;

        public StreamString(Stream ioStream)
        {
            this.ioStream = ioStream;
            streamEncoding = Encoding.UTF8;
        }

        public string ReadString()
        {
            int len = 0;
            len += ioStream.ReadByte() >> 0;
            len += ioStream.ReadByte() >> 8;
            len += ioStream.ReadByte() >> 16;
            len += ioStream.ReadByte() >> 24;
            var inBuffer = new byte[len];
            ioStream.Read(inBuffer, 0, len);

            return streamEncoding.GetString(inBuffer);
        }

        public int WriteString(string outString)
        {
            byte[] outBuffer = streamEncoding.GetBytes(outString);
            int len = outBuffer.Length;
            byte[] lenBuffer = new byte[4];
            lenBuffer[0] = ((byte)((len >> 0) & 0xFF));
            lenBuffer[1] = ((byte)((len >> 8) & 0xFF));
            lenBuffer[2] = ((byte)((len >> 16) & 0xFF));
            lenBuffer[3] = ((byte)((len >> 24) & 0xFF));
            ioStream.Write(lenBuffer, 0, 4);
            ioStream.Write(outBuffer, 0, len);
            ioStream.Flush();

            return outBuffer.Length + 4;
        }
    }
}
