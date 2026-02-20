using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

namespace bettercpp.sentinel
{
    class SentinelCore
    {
        static List<string> criticalServices = new List<string> { 
            "wpcmonsvc", "svchost", "lsass", "services", "wininit", 
            "csrss", "smss", "winlogon", "taskhostw", "spoolsv" 
        };
        static string quarantineDir = @"C:\Tools\Jenny\Quarantine";
        static string logFile = "JennySentinel_Master.log";
        static List<string> detectedThreats = new List<string>();

        static void Main(string[] args)
        {
            if (!Directory.Exists(quarantineDir)) Directory.CreateDirectory(quarantineDir);

            if (args.Length > 0 && args[0].ToLower() == "--restore")
            {
                RestoreQuarantine();
                return;
            }
            
            string path = args.Length > 0 ? args[0] : Environment.CurrentDirectory;
            Console.WriteLine("\n[SENTINEL CORE] High-Efficiency Scan Initiated: " + path);
            
            try {
                ScanDirectoryRecursively(path);
                Console.WriteLine("\n" + new string('-', 50));
                Console.WriteLine("[+] Scan Finished. Total Suspicious Files: " + detectedThreats.Count);
                
                if (detectedThreats.Count > 0)
                {
                    Console.ForegroundColor = ConsoleColor.Yellow;
                    Console.Write("\n[?] Move detected files to secure quarantine? (-y / -n): ");
                    string choice = Console.ReadLine().ToLower();
                    Console.ResetColor();

                    if (choice == "-y")
                    {
                        foreach (string file in detectedThreats) QuarantineFile(file);
                        Console.WriteLine("\n[+] Isolation complete. System is secure.");
                    }
                }
            }
            catch (Exception e) { Console.WriteLine("Fatal Error: " + e.Message); }

            Console.WriteLine("\n[PRESS ENTER TO EXIT]");
            Console.ReadLine();
        }

        static void RestoreQuarantine()
        {
            Console.WriteLine("\n[SENTINEL RESTORE] Accessing Quarantine Vault...");
            string[] mapFiles = Directory.GetFiles(quarantineDir, "*.map");

            if (mapFiles.Length == 0) {
                Console.WriteLine("[*] No restore data found.");
                return;
            }

            foreach (string map in mapFiles)
            {
                try {
                    string originalPath = File.ReadAllText(map).Trim();
                    string lockedFile = map.Replace(".map", ".jny_locked");

                    if (File.Exists(lockedFile))
                    {
                        string targetDir = Path.GetDirectoryName(originalPath);
                        if (!Directory.Exists(targetDir)) Directory.CreateDirectory(targetDir);

                        File.Move(lockedFile, originalPath);
                        File.Delete(map);
                        Console.WriteLine("[+] SUCCESS: Restored " + Path.GetFileName(originalPath));
                    }
                }
                catch (Exception e) { Console.WriteLine("[X] Restore Error: " + e.Message); }
            }
            Console.WriteLine("\n[!] Restore process finished.");
        }

        static void ScanDirectoryRecursively(string root)
        {
            string folderName = Path.GetFileName(root).ToLower();
            if (folderName == "system32" || folderName == "winsxs" || folderName == "servicing" || folderName == "syswow64") return;

            try {
                foreach (string file in Directory.GetFiles(root, "*.exe")) {
                    if (AnalyzeFile(file)) detectedThreats.Add(file);
                }
                foreach (string dir in Directory.GetDirectories(root)) {
                    ScanDirectoryRecursively(dir);
                }
            } catch { }
        }

        static bool AnalyzeFile(string fullPath)
        {
            string fileName = Path.GetFileNameWithoutExtension(fullPath).ToLower();
            foreach (string service in criticalServices) {
                if (fileName != service && IsSimilar(fileName, service)) {
                    Report(fullPath, "Potential Mimicking of " + service);
                    return true;
                }
            }
            return false;
        }

        static bool IsSimilar(string s1, string s2) {
            if (Math.Abs(s1.Length - s2.Length) > 1) return false;
            int diffs = 0;
            int minLen = Math.Min(s1.Length, s2.Length);
            for (int i = 0; i < minLen; i++) if (s1[i] != s2[i]) diffs++;
            return diffs > 0 && diffs <= 2; 
        }

        static void QuarantineFile(string sourcePath)
        {
            try {
                string fileName = Path.GetFileName(sourcePath);
                string destPath = Path.Combine(quarantineDir, fileName + ".jny_locked");
                string mapPath = Path.Combine(quarantineDir, fileName + ".map");

                if (File.Exists(sourcePath)) {
                    File.WriteAllText(mapPath, sourcePath);
                    File.Move(sourcePath, destPath);
                    Console.WriteLine("[!] Isolated: " + fileName);
                }
            } catch { }
        }

        static void Report(string path, string reason) {
            string msg = "[!!!!] FLAG: " + reason + " | AT: { " + path + " }";
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(msg);
            Console.ResetColor();
            File.AppendAllText(logFile, "[" + DateTime.Now.ToString() + "] " + msg + Environment.NewLine);
        }
    }
}
