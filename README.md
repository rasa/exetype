exetype 1.1 - Jul 19 2006
Copyright (c) 2005-2006 Ross Smith II (http://smithii.com) All Rights Reserved

Reports one of the following numbers representing the type of .EXE found:

IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
IMAGE_SUBSYSTEM_POSIX_CUI            7   // image runs in the Posix character subsystem.
IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8   // image is a native Win9x driver.
IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       9   // Image runs in the Windows CE subsystem.
IMAGE_SUBSYSTEM_EFI_APPLICATION      10  //
IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11   //
IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER   12  //
IMAGE_SUBSYSTEM_EFI_ROM              13
IMAGE_SUBSYSTEM_XBOX                 14

------------------------------------------------------------------------------

Usage: exetype [options] target.exe

Options (defaults in parentheses):
-q | --quiet                   Only display errors, set errorcode to exe type
-v | --version                 Show version and copyright and quit
-? | --help                    Show this help message and quit

Examples:

c:\>exetype exetype.exe

exetype.exe: Windows Console

c:\>exetype %windir%\notepad.exe
C:\WINDOWS\notepad.exe: Windows GUI

c:\>echo %errorlevel%
2

c:\>exetype -q exetype.exe

c:\>echo %errorlevel%
3

------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

$Id$
