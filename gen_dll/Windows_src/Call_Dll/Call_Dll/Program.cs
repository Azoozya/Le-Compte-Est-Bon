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
        unsafe public static extern void Lceb(char* filename);

        unsafe static void call_Lceb(string arg)
        {
            char[] input = arg.ToCharArray();
           Lceb(input);
        }

        static void Main(string[] args)
        {
            call_Lceb(args[1]);
        }
    }
}
