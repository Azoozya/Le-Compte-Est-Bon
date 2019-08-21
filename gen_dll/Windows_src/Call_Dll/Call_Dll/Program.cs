using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;     // DLL support


namespace Call_Dll
{
    class Program
    {
        [DllImport("Lceb.dll")]
        public static extern void Lceb();

        static void Main(string[] args)
        {
            Lceb();
        }
    }
}
