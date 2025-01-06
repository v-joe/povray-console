This is an unoffical fork with full command line, ini file, and benchmarking support for Microsoft Windows of

[POV-Ray](http://www.povray.org/) - The Persistence of Vision Raytracer
=======================================================================

- This fork of POV-Ray strives for supporting the same command line arguments in the Microsoft Windows console version as in the unix version.
- To support flexible installation locations, relative paths for includes and ini files are used. If the POV-Ray Windows console executable is located in a <tt>bin</tt> directory, the presence of <tt>..\include</tt> is checked as a system-wide library path. If the directory name doesn't contain <tt>bin</tt>, <tt>include</tt> is searched in the same path as the executable.
- Similarly, a system-wide config file is searched in relative paths to the executable in <tt>..\etc\povray.ini</tt> or <tt>etc\povray.ini</tt>, respectively.
- In line with the unix version, the environment variable <tt>%POVINI%</tt>, the current directory, and the user directory <tt>%USERPROFILE%\\.povray\3.8</tt> are searched in this order for <tt>povray.ini</tt> files and read ahead of the system-wide <tt>etc\povray.ini</tt>.
- <tt>--benchmark</tt> triggers the rendition of the standard POV-Ray benchmark, where <tt>+wt<i>num</i></tt> controls the number <tt><i>num</i></tt> of threads.

<hr>
Further information about POV-Ray below:
<hr>

[![Quick Tests](https://github.com/POV-Ray/povray/actions/workflows/test_build_quick.yml/badge.svg)](https://github.com/POV-Ray/povray/actions/workflows/test_build_quick.yml)
[![Code Analysis](https://github.com/POV-Ray/povray/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/POV-Ray/povray/actions/workflows/codeql-analysis.yml)
[![Maintenance Status](https://img.shields.io/maintenance/yes/2024.svg)](README.md "Last edited 2024-11-03")

- [Development Status (November 2024)](#development-status-november-2024)
- [License](#license)
- [Forums](#forums)
- [Bug Reports](#bug-reports)
- [Official Binaries](#official-binaries)
- [Building POV-Ray](#building-pov-ray)
- [IDE Versions](#ide-versions)
- [3D Modeller](#3d-modeller)
- [Documentation](#documentation)
- [Contacting Us](#contacting-us)

Development Status (November 2024)
--------------------------------------
After a long break in releases of 3.8 beta test candidates, development is resuming, though not at the
pace of past releases. That said, we hope to have a new 3.8 beta ready before the end of the year and
a final release in Q1 2025.

One impediment we have yet to overcome is code signing for the Windows platform. Currently we have
no means of doing so as our previous key has expired. The difficulty is both cost and the need for
flexibility in verifying us as an organization. Despite the fact that we have a company registered
in Australia (Persistence of Vision Raytracer Pty. Ltd.) and it is visible on the ASIC website, it
is our experience that the verification process gets hung up by the fact the company doesn't sell
anything, have a street address, or have a phone number.

While we do not generally sign beta releases, it is our policy that official releases for the Windows
platform are expected to be signed.

License
--------------------------------------

As of version v3.7, the source for POV-Ray is licensed under the AGPL3. The documentation is under the
Creative Commons Attribution-Noncommercial-ShareAlike 2.5 license, and support files such
as SDL includes, macros, sample scenes and so forth are under the Creative Commons Attribution-ShareAlike
3.0 Unported License (see each file header for the specific one).

Forums
--------------------------------------

Discussion regarding POV-Ray is traditionally done via our forums at https://news.povray.org/.
These are also available via NNTP at news://news.povray.org/ for those preferring that.

Please note that the POV-Ray developers do not monitor all forums regularly. The ones we
tend to check most frequently are povray.general, povray.windows and povray.unix.

Bug Reports
--------------------------------------

It's generally a good idea to mention a bug in the forums prior to lodging a formal
report; this can save you some time if it's a non-bug or a solution is known. You
should also first check the [known issues](https://github.com/POV-Ray/povray/issues)
to see if it has been reported already.

If you're sure something is a bug then please do lodge a bug report on the GitHub issues tracker.

Official Binaries
--------------------------------------

At this point in time, the only platform for which the project distributes pre-built
'official' (i.e. supported) binaries is Microsoft Windows. These may be
obtained via https://www.povray.org/download/..

Official Windows binaries of selected development versions are made available at
https://github.com/POV-Ray/povray/releases on a semi-irregular basis.

Building POV-Ray
--------------------------------------

At this point in time we generally recommend building from the latest version of the
[`latest-stable` branch](https://github.com/POV-Ray/povray/tree/latest-stable). Alternatively,
you may want to opt for a recent [tagged version](https://github.com/POV-Ray/povray/tags)
to test-drive features that have been added since the latest stable release.

_Please do not build directly from the master branch_ (or any other non-stable branch
for that matter), as versions from that branch may report ambiguous version numbers,
making it difficult to obtain version-specific support or report bugs in a useful manner.

POV-Ray should compile on any POSIX-compliant system with the required tools (please see
[unix/README.md](unix/README.md) for build instructions),
on Microsoft Windows systems that have Visual Studio 2015 Update 1 or later installed (targeting
XP or later, both 32 and 64-bit - be sure to see [windows/README.md](windows/README.md),
otherwise your build _will not work_), and also on Mac systems (console mode only, using
an appropriately-modified version of the unix build - not currently provided by us).

If you are using an operating system with a package or ports system such as
Ubuntu or FreeBSD, you may like to check whether or not POV-Ray is available
via that route.

IDE versions
--------------------------------------

Currently the only version of POV-Ray with an IDE as such is the Windows build.
We do want to change that, though. With the release of POV-Ray v3.7 we have added
a clear split between the backend (renderer) and frontend (UI or console), along
with a C++ layer which abstracts this into a fairly easily-understood set of 
classes (VFE, aka 'Virtual Front End').

We will offer support to those wishing to use this interface layer to integrate
POV-Ray into an open-source cross-platform IDE. We would also be interested in
hearing suggestions as to what we could base such an IDE on, should we go ahead
to integrate it ourselves.

Putting it another way: we consider getting a cross-platform IDE a high priority.

3D Modeller
-------------------------------------

POV-Ray does not currently have its own 3d modelling application (at least, not one
in a usable state). We do own the rights to the Moray modeller, which was formerly
commercial, but it needs a little work to get it working with v3.7 or later. It is also
Windows only (due to its use of MFC). Nevertheless we will be adding the source
to the repository at a future date.

Authors of open-source modellers with a compatible licence wishing to directly
integrate POV-Ray are welcome to contact us for support in doing so.

Documentation
--------------------------------------

When built and installed via the means provided in the source tree, all versions
of POV-Ray come with documentation. For the Unix build, this is in the form of a
manpage giving basic usage, and full HTML-based documentation. For the Windows
version, there is a HtmlHelp (.CHM) file provided.

The official location for the online documentation is https://www.povray.org/documentation/.
Further information, as well as online documentation for the current development
version, can be found at https://wiki.povray.org.

Contacting Us
--------------------------------------

We prefer that you contact us via the forums mentioned at the head of this document.
If the matter is one that requires direct email contact (and this generally will NOT
include tech support requests, though exceptions are made for package maintainers)
you may use the address listed at the bottom of https://www.povray.org/povlegal.html.

