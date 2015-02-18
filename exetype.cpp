/*

$Id$

Copyright (c) 2005-2006 Ross Smith II (http://smithii.com). All rights reserved.

This program is free software; you can redistribute it and/or modify it
under the terms of version 2 of the GNU General Public License
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

*/

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif

//#define _WIN32_WINNT 0x501

#include <cstdio>

#include <windows.h>
#include <winnt.h>
//#include <atlbase.h> // required for Stackwalker

//#include <stdio.h>
#include <tchar.h>

//#include <iostream>
#include <stdlib.h>	/* exit() */

#include <imagehlp.h>

#include "Stackwalker.h"
#include "Optimize.h"
#include "debug.h"
#include "getopt.h"
#include "SystemMessage.h"
#include "version.h"

#ifndef IMAGE_SIZEOF_NT_OPTIONAL_HEADER
#ifdef _WIN64
#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER     240
#else
#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER     224
#endif
#endif

#define APPNAME			VER_INTERNAL_NAME
#define APPVERSION		VER_STRING2
#define APPCOPYRIGHT	VER_LEGAL_COPYRIGHT

struct _opt {
	int		quiet;
};

typedef struct _opt opt_t;

static opt_t opt = {
	0	// quiet
};

static char *short_options = "qv?";

static struct option long_options[] = {
	{"quiet",					no_argument, 0, 'q'},
	{"version",					no_argument, 0, 'v'},
	{"help",					no_argument, 0, '?'},
	{NULL, 0, 0, 0}
};

void usage() {
	printf(
		"Usage: " APPNAME " [options] target.exe\n"
		"\n"
		"Options (defaults in parentheses):\n"

//        1         2         3         4         5         6         7         8
//2345678901234567890123456789012345678901234567890123456789012345678901234567890
"-q | --quiet                   Only display errors, set errorcode to exe type\n"
"-v | --version                 Show version and copyright and quit\n"
"-? | --help                    Show this help message and quit\n"
);
}

static bool ReadFileBytes(const HANDLE hFile, LPVOID lpBuffer, const DWORD dwSize) {
	DWORD dwBytes = 0;

	if (!ReadFile (hFile, lpBuffer, dwSize, &dwBytes, NULL)) {
//		fprintf(stderr, _T("Failed to read file!\n"));
		return false;
	}

	if (dwSize != dwBytes) {
//		fprintf(stderr, _T("Wrong number of bytes read, expected\n"));
		return false;
	}

	return true;
}

static int get_ver_info(const TCHAR *filename, TCHAR **type) {
	HANDLE hImage;
	DWORD dwCoffHeaderOffset;
	DWORD dwNewOffset;
	DWORD dwMoreDosHeader[16];
	ULONG ulNTSignature;
	static TCHAR buf[128];

	IMAGE_DOS_HEADER dos_header;
	IMAGE_FILE_HEADER file_header;
	IMAGE_OPTIONAL_HEADER optional_header;

	*type = _T("");

	hImage = CreateFile(
		filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hImage == INVALID_HANDLE_VALUE) {
		*type = _T("Cannot open file");
		return -1;
	}

	// Read MS-Dos image header.

	if (!ReadFileBytes (hImage, &dos_header, sizeof(IMAGE_DOS_HEADER))) {
		*type = _T("Cannot read file");
		return -2;
	}

	if (dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
		*type = _T("Unknown file type");
		return -3;
	}

	// Read more MS-Dos header.

	if (!ReadFileBytes (hImage, dwMoreDosHeader, sizeof (dwMoreDosHeader))) {
		*type = _T("Cannot read file");
		return -2;
	}

	// Move the file pointer to get the actual COFF header.

	dwNewOffset = SetFilePointer(hImage, dos_header.e_lfanew, NULL, FILE_BEGIN);
	dwCoffHeaderOffset = dwNewOffset + sizeof (ULONG);

	if (dwCoffHeaderOffset == 0xFFFFFFFF) {
		*type = _T("Cannot seek file");
		return -4;
	}

	// Read NT signature of the file.

	if (!ReadFileBytes (hImage, &ulNTSignature, sizeof (ULONG))) {
		*type = _T("Cannot read NT signature");
		return -2;
	}

	if (ulNTSignature != IMAGE_NT_SIGNATURE) {
		*type = _T("Missing NT signature");
		return -5;
	}

	if (!ReadFileBytes (hImage, &file_header, IMAGE_SIZEOF_FILE_HEADER)) {
		*type = _T("Cannot read header");
		return -2;
	}

	if (file_header.Machine != IMAGE_FILE_MACHINE_I386) {
		sprintf(buf, _T("Unknown machine type: %04x"), file_header.Machine);
		*type = buf;
		return -6;
	}

	// Read the optional header of file.

	if (!ReadFileBytes (hImage, &optional_header, IMAGE_SIZEOF_NT_OPTIONAL_HEADER)) {
		*type = _T("Cannot read optional header");
		return -2;
	}

	switch (optional_header.Subsystem) {
		case IMAGE_SUBSYSTEM_UNKNOWN:
			*type = _T("Unknown");
			break;
		case IMAGE_SUBSYSTEM_NATIVE:
			*type = _T("Native");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI: // 2
			*type = _T("Windows GUI");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI: // 3
			*type = _T("Windows Console");
			break;
		case IMAGE_SUBSYSTEM_OS2_CUI:
			*type = _T("OS/2 Console");
			break;
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			*type = _T("Posix Console");
			break;
		case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:
			*type = _T("Native Win9x");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
			*type = _T("Windows CE GUI");
			break;
		case IMAGE_SUBSYSTEM_EFI_APPLICATION:
			*type = _T("EFI Application");
			break;
		case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
			*type = _T("EFI Boot Service Driver");
			break;
		case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
			*type = _T("EFI Runtime Driver");
			break;
		case IMAGE_SUBSYSTEM_EFI_ROM:
			*type = _T("EFI ROM");
			break;
		case IMAGE_SUBSYSTEM_XBOX:
			*type = _T("XBOX");
			break;

/*
#define IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
#define IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
#define IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
#define IMAGE_SUBSYSTEM_POSIX_CUI            7   // image runs in the Posix character subsystem.
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8   // image is a native Win9x driver.
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       9   // Image runs in the Windows CE subsystem.
#define IMAGE_SUBSYSTEM_EFI_APPLICATION      10  //
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11   //
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER   12  //
#define IMAGE_SUBSYSTEM_EFI_ROM              13
#define IMAGE_SUBSYSTEM_XBOX                 14
*/

		default:
			sprintf(buf, "Unknown subsystem type %d", optional_header.Subsystem);
			*type = buf;
	}

	return optional_header.Subsystem;
}

int main(int argc, char **argv) {
	opterr = 0;

	while (true) {
		int option_index = 0;

		if (optind < argc && argv[optind] && argv[optind][0] == '/')
			argv[optind][0] = '-';

		int c = getopt_long(argc, argv, short_options, long_options, &option_index);

		if (c == -1)
			break;

        switch (c) {
            case 'q':
				opt.quiet = 1;
                break;
            case 'v': // --version
				printf(
					APPNAME " " APPVERSION " - " __DATE__ "\n"
					APPCOPYRIGHT "\n");
				exit(0);
                break;
			case '?': // --help
				usage();
				exit(0);
			default:
				usage();
				exit(1);
        }
    }

	int rv = 0;
	if (optind >= argc) {
		usage();
		exit(1);
	}

	TCHAR *type;

	type = _T("");
	while (optind < argc) {
		rv = get_ver_info(argv[optind], &type);
		if (!opt.quiet || rv < 0)
			fprintf(rv ? stderr : stdout, "%s: %s\n", argv[optind], type);
		++optind;
	}

//	_getch();

	return rv;
}
