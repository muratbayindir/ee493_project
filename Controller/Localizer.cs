using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;

namespace Controller
{
    //public class Localizer
    //{
    //    LocalizerPackage package;

    //    public Localizer(string name)
    //    {
    //        package = new LocalizerPackage();
    //        package.targets = new List<Target>();

    //        foreach (Target target in FormMain.targets.targets)
    //        {
    //            package.targets.Add(target);
    //        }

    //        package.name = name;
    //    }

    //    public void UpdateTarget(RSSIInfo info)
    //    {
    //        for (int i = 0; i < package.targets.Count; ++i)
    //        {
    //            if (info.targetName.Equals(package.targets[i].name))
    //            {
    //                package.targets[i].rssi = info.rssiValue;
    //                return;
    //            }
    //        }
    //    }

    //    public Point3D Calculate()
    //    {
    //        NamedPipeClientStream pipeClient;
    //        StreamString streamPipe;

    //        pipeClient =
    //            new NamedPipeClientStream(".", "StronicsLocalizer",
    //                PipeDirection.InOut, PipeOptions.None,
    //                TokenImpersonationLevel.Impersonation);

    //        Logger.WriteLine("Connecting to server...\n");
    //        pipeClient.Connect();

    //        streamPipe = new StreamString(pipeClient);

    //        // {"name":"test", "targets":[{"name":"1","location":[1,2,3],"rssi":15},{}]}
    //        string strPackage = JsonConvert.SerializeObject(package);
    //        streamPipe.WriteString(strPackage);
    //        // {"location":[1, 2, 3]}
    //        string data = streamPipe.ReadString();

    //        pipeClient.Close();

    //        Location loc = (Location)JsonConvert.DeserializeObject(data, typeof(Location));
    //        return new Point3D(loc.location[0], loc.location[1], loc.location[2]);
    //    }
    //}

    //public class LocalizerPackage
    //{
    //    public string name { get; set; }
    //    public List<Target> targets { get; set; }
    //}

    //public class Location
    //{
    //    public List<double> location { get; set; }
    //}

    //// Defines the data protocol for reading and writing strings on our stream.
    //public class StreamString
    //{
    //    private Stream ioStream;
    //    private Encoding streamEncoding;

    //    public StreamString(Stream ioStream)
    //    {
    //        this.ioStream = ioStream;
    //        streamEncoding = Encoding.UTF8;
    //    }

    //    public string ReadString()
    //    {
    //        int len = 0;
    //        len += ioStream.ReadByte() << 0;
    //        len += ioStream.ReadByte() << 8;
    //        len += ioStream.ReadByte() << 16;
    //        len += ioStream.ReadByte() << 24;
    //        var inBuffer = new byte[len];
    //        ioStream.Read(inBuffer, 0, len);

    //        return streamEncoding.GetString(inBuffer);
    //    }

    //    public int WriteString(string outString)
    //    {
    //        byte[] outBuffer = streamEncoding.GetBytes(outString);
    //        int len = outBuffer.Length;
    //        byte[] lenBuffer = new byte[4];
    //        lenBuffer[0] = ((byte)((len >> 0) & 0xFF));
    //        lenBuffer[1] = ((byte)((len >> 8) & 0xFF));
    //        lenBuffer[2] = ((byte)((len >> 16) & 0xFF));
    //        lenBuffer[3] = ((byte)((len >> 24) & 0xFF));
    //        ioStream.Write(lenBuffer, 0, 4);
    //        ioStream.Write(outBuffer, 0, len);
    //        ioStream.Flush();

    //        return outBuffer.Length + 4;
    //    }
    //}
}
