using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Controller
{
    public static class Logger
    {
        public static void Init()
        {

        }

        public static void WriteLine(string line, LogType logType = LogType.Info)
        {
            Debug.Write(logType.ToString() + " : ");
            Debug.WriteLine(line);
        }
    }

    public enum LogType
    {
        Info,
        Warning,
        Error
    }
}
