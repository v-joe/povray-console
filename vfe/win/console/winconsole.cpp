//******************************************************************************
///
/// @file vfe/win/console/winconsole.cpp
///
/// This file contains a basic proof-of-concept POV implementation using VFE.
///
/// @author Christopher J. Cason
///
/// @copyright
/// @parblock
///
/// Persistence of Vision Ray Tracer ('POV-Ray') version 3.8.
/// Copyright 1991-2019 Persistence of Vision Raytracer Pty. Ltd.
///
/// POV-Ray is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Affero General Public License as
/// published by the Free Software Foundation, either version 3 of the
/// License, or (at your option) any later version.
///
/// POV-Ray is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Affero General Public License for more details.
///
/// You should have received a copy of the GNU Affero General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// ----------------------------------------------------------------------------
///
/// POV-Ray is based on the popular DKB raytracer version 2.12.
/// DKBTrace was originally written by David K. Buck.
/// DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
///
/// @endparblock
///
//******************************************************************************

#include "base/version_info.h"

#include "backend/povray.h"
#include "backend/control/benchmark.h"

#include "vfe.h"

#include <boost/algorithm/string.hpp>

#ifndef _CONSOLE
#error "You must define _CONSOLE in windows/povconfig/syspovconfig.h prior to building the console version, otherwise you will get link errors."
#endif

#define xstr(s) STR(s)
#define STR(s) #s

using namespace vfe;
using namespace vfePlatform;

namespace pov_frontend
{
    ////////////////////////////////
    // Called from the shellout code
    ////////////////////////////////
    bool MinimizeShellouts(void) { return false; } // TODO
    bool ShelloutsPermitted(void) { return false; } // TODO
}
// end of namespace pov_frontend

// Add ini file for parsing, if it exists
void AddIni(const char *path, vfeRenderOptions &opts) {
  DWORD dw = GetFileAttributes(path);

  if((dw & FILE_ATTRIBUTE_DIRECTORY) == 0 && dw != INVALID_FILE_ATTRIBUTES) {
    opts.AddINI (_strdup(path));
  }
}

// Determine localation of povray console executable and
// look for include and povray.ini in relative paths
// If in benchmark mode, don't parse povray.ini
#define pmax 32768
void AddSystemLibraryPathAndIni(vfeRenderOptions &opts, bool benchmark)
{
  // +14 to be sure to be able to append "etc\povray.ini" (or "include")
  char path[pmax+14];
  DWORD len,i,j;

  // Get path of povray console executable
  if (len = GetModuleFileNameA(NULL, path, pmax))
  {
    i = len - 1;
    for (; i >= 0 && path[i] != '\\'; i--);
    if (i > 0)
    {
      j = i - 1;
      for (; j >= 0 && path[j] != '\\'; j--);
      if (path[j] == '\\')
      {
        // If povray console executable is in bin* subdirectory,
        // check for ..\include as library path
        // otherwise check for include in same directory
        if (i - j >= 3
            && (path[j+1]=='b' || path[j+1]=='B')
            && (path[j + 2] == 'i' || path[j + 2] == 'I')
            && (path[j + 3] == 'n' || path[j + 3] == 'N')) i = j;
      }
    }
    else
    {
      // Path contains no backslash?
      // Don't know how to determine installation path in this case
      if (path[0] != '\\') return;
    }

    if(!benchmark)
    {
      strcpy(path + i + 1, "etc\\povray.ini");
      // If the determined path+\etc\povray.ini is a file, add it for parsing
      j = GetFileAttributes(path);
      if((j & FILE_ATTRIBUTE_DIRECTORY) == 0 && j != INVALID_FILE_ATTRIBUTES)
      {
        static std::string inipath(path);
        opts.AddINI (inipath);
      }
    }

    strcpy(path + i + 1, "include");
    // If the determined path+\include is a directory, add it to library search path
    j = GetFileAttributes(path);
    if((j & FILE_ATTRIBUTE_DIRECTORY) != 0 && j != INVALID_FILE_ATTRIBUTES)
    {
      static std::string incpath(path);
      opts.AddLibraryPath (incpath);
    }
  }
}

void PrintStatus (vfeSession *session)
{
  std::string str;
  vfeSession::MessageType type;
  static vfeSession::MessageType lastType = vfeSession::mUnclassified;

  while (session->GetNextCombinedMessage (type, str))
  {
    if (type != vfeSession::mGenericStatus)
    {
      if (lastType == vfeSession::mGenericStatus)
        fprintf (stderr, "\n") ;
      fprintf (stderr, "%s\n", str.c_str());
    }
    else
      fprintf (stderr, "%s\r", str.c_str());
    lastType = type;
  }
}

void PrepareBenchmark(vfeRenderOptions &opts, std::string pov, std::string ini)
{
  char path[pmax], tmp[pmax];

  if(!GetTempPathA(pmax, path))
  {
    path[0] = '.';
    path[1] = 0;
  }

  if(!GetTempFileNameA(path, "pov", 0, tmp)) {
    fprintf(stderr, "Cannot create temporary .pov source for benchmarking.\n");
    exit(1);
  }
  pov = std::string(tmp);
  ini = pov + ".ini";

  pov::Write_Benchmark_File(tmp, ini.c_str());
  opts.AddINI(ini);
  opts.SetSourceFile(pov);
}

void CleanBenchmark(std::string pov, std::string ini)
{
  remove(ini.c_str());
  remove(pov.c_str());
}

static void PrintVersion(void)
{
    // TODO -- GNU/Linux customs would be to print to stdout (among other differences).

    fprintf(stderr,
        "POV-Ray %s\n\n"
        "%s\n%s\n%s\n"
        "%s\n%s\n%s\n\n",
        POV_RAY_VERSION,
        DISTRIBUTION_MESSAGE_1, DISTRIBUTION_MESSAGE_2, DISTRIBUTION_MESSAGE_3,
        POV_RAY_COPYRIGHT, DISCLAIMER_MESSAGE_1, DISCLAIMER_MESSAGE_2
    );
    fprintf(stderr,
        "Built-in features:\n"
        "  I/O restrictions:          disabled\n"
        "  X Window display:          disabled\n"
        "  Supported image formats:   gif tga iff ppm pgm hdr png jpeg tiff openexr\n"
        "  Unsupported image formats: -\n\n"
    );
    fprintf(stderr,
        "Compilation settings:\n"
        "  Build architecture:  %s\n"
        "  Built/Optimized for: %s\n"
        "  Compiler:            %s\n",
        POVRAY_PLATFORM_NAME, POV_BUILD_INFO, COMPILER_NAME
    );
}

static void PrintGeneration(void)
{
    printf("%s\n", POV_RAY_GENERATION POV_RAY_BETA_SUFFIX);
}

static void PrintHelp(void)
{
    fprintf(stderr, "Platform specific command line options:\n\n"
        "  'general' options:\n\n"
        "    --version|-version|--V          display program version\n"
        "    --help|-help|-h|-?              display usage information\n"
        "    --generation                    display program generation (short version number)\n"
        "    --benchmark|-benchmark          run the standard POV-Ray benchmark\n\n");
}

void ErrorExit(vfeSession *session)
{
  fprintf (stderr, "%s\n", session->GetErrorString());
  session->Shutdown();
  delete session;
  exit (1);
}

// this is an example of a minimal console version of POV-Ray using the VFE
// (virtual front-end) library. it is NOT INTENDED TO BE A FULLY-FEATURED
// CONSOLE IMPLEMENTATION OF POV-RAY and is not officially supported. see
// the unix version for a example of a more comprehensive console build.
int main (int argc, char **argv)
{
  char              *si,*sh,*s;
  vfeWinSession     *session = new vfeWinSession() ;
  vfeStatusFlags    flags;
  vfeRenderOptions  opts;
  bool              dobenchmark = false;
  std::string       benchpov, benchini;

  /*
  fprintf(stderr,
          "This is an example of a minimal console build of POV-Ray under Windows.\n\n"
          "Persistence of Vision(tm) Ray Tracer Version " POV_RAY_VERSION_INFO ".\n"
          DISTRIBUTION_MESSAGE_1 "\n"
          DISTRIBUTION_MESSAGE_2 "\n"
          DISTRIBUTION_MESSAGE_3 "\n"
          POV_RAY_COPYRIGHT "\n"
          DISCLAIMER_MESSAGE_1 "\n"
          DISCLAIMER_MESSAGE_2 "\n\n");
  */

  if(argc > 1) {
    if (!(strcmp(argv[1], "--version")
       && strcmp(argv[1], "-version")
       && strcmp(argv[1], "-V"))) {
      PrintVersion();
      return 0;
    }
    else if (!strcmp(argv[1], "--generation")) {
      PrintGeneration();
      return 0;
    }
    else if (!(strcmp(argv[1], "--help")
            && strcmp(argv[1], "-help")
            && strcmp(argv[1], "-h")
            && strcmp(argv[1], "-?"))) {
      if (session->Initialize(nullptr, nullptr) == vfeNoError) {
        session->Shutdown();
        PrintStatus (session);
        delete session;
      }
      PrintHelp();
      return 0;
    }
  }
  for(int i = 0; i < argc; i++)
  {
    if(!(strcmp(argv[i], "--benchmark") && strcmp(argv[i], "-benchmark"))) {
      dobenchmark = true;
      break;
    }
  }

  if (session->Initialize(nullptr, nullptr) != vfeNoError)
    ErrorExit(session);

  if (!dobenchmark)
  {
    // Add ini from %POVINI%
    if ((si = std::getenv ("POVINI")) != nullptr)
      AddIni (si, opts);
    // Add ini from current directory
    AddIni ("povray.ini", opts);
    // Add user ini
    if ((sh = std::getenv ("USERPROFILE")) != nullptr)
      AddIni ((std::string(sh)+"\\.povray\\" xstr(POV_RAY_MAJOR_VERSION_INT) "."
              xstr(POV_RAY_MINOR_VERSION_INT) "\\povray.ini").c_str(), opts);

    if ((s = std::getenv ("POVINC")) != nullptr)
      opts.AddLibraryPath (s);
    while (*++argv)
      opts.AddCommand (*argv);

    AddSystemLibraryPathAndIni(opts, false);
  }
  else
  {
    // parse parameters for benchmarking
    while (*++argv)
    {
      std::string s = std::string(*argv);
      boost::to_lower(s);
      // only parse number of threads or library paths
      if (boost::starts_with(s, "+wt") || boost::starts_with(s, "-wt"))
      {
          s.erase(0, 3);
          int n = std::atoi(s.c_str());
          if (n)
              opts.SetThreadCount(n);
          else
              fprintf(stderr, "POV-Ray: ignoring malformed '%s' command-line option\n", *argv);
      }
      else if (boost::starts_with(s, "+l") || boost::starts_with(s, "-l"))
      {
          s.erase(0, 2);
          opts.AddLibraryPath(s);
      }
    }
    AddSystemLibraryPathAndIni(opts, true);

    int benchversion = pov::Get_Benchmark_Version();
    fprintf(stderr, "\
POV-Ray %s\n\n\
Entering the standard POV-Ray %s benchmark version %x.%02x.\n\n\
This built-in benchmark requires POV-Ray to be installed on your system\n\
before running it.  There will be neither display nor file output, and\n\
any additional command-line option except setting the number of render\n\
threads (+wtN for N threads) and library paths (+Lpath) will be ignored.\n\
To get an accurate benchmark result you might consider running POV-Ray\n\
with the Unix 'time' command (e.g. 'time povray -benchmark').\n\n\
The benchmark will run using %d render thread(s).\n\
Press <Enter> to continue or <Ctrl-C> to abort.\n\
",
        POV_RAY_VERSION_INFO,
        xstr(POV_RAY_MAJOR_VERSION_INT) "." xstr(POV_RAY_MINOR_VERSION_INT),
        benchversion / 256, benchversion % 256,
        opts.GetThreadCount()
    );
    fflush(stderr);
    getchar();
    PrepareBenchmark(opts, benchpov, benchini);
  }

  if (session->SetOptions(opts) != vfeNoError)
    ErrorExit(session);
  if (session->StartRender() != vfeNoError)
    ErrorExit(session);

  bool pauseWhenDone = session->GetBoolOption("Pause_When_Done", false);
  while (((flags = session->GetStatus(true, 1000)) & stRenderShutdown) == 0)
    PrintStatus (session) ;
  session->Shutdown() ;
  PrintStatus (session) ;
  delete session;

  if (dobenchmark) CleanBenchmark(benchpov, benchini);

  if (pauseWhenDone)
  {
    fprintf (stderr, "Press enter to continue ... ");
    fflush(stderr);
    getchar();
  }

  return 0;
}

