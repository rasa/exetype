// Copyright (c) 2005-2016 Ross Smith II. See Mit LICENSE in /LICENSE

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif

#define _WIN32_WINNT 0x501

#include <cstdio>

#include <windows.h>
#include <winnt.h>
//#include <atlbase.h> // required for Stackwalker

//#include <stdio.h>
#include <tchar.h>

//#include <iostream>
#include <stdlib.h>	/* exit() */

#include <imagehlp.h>

#include "getopt.h"
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
	int		type;
	int		cpu;
	int		ignore;
	int		quiet;
};

typedef struct _opt opt_t;

static opt_t opt = {
	1,	// type
	1,	// cpu
	0,	// ignore
	0	// quiet
};

static char *short_options = "tciqv?";

static struct option long_options[] = {
	{"type",					no_argument, 0, 't'},
	{"cpu",						no_argument, 0, 'c'},
	{"ignore",					no_argument, 0, 'i'},
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
"-t | --type                    Only display exe type\n"
"-c | --cpu                     Only display cpu type\n"
"-i | --ignore                  Report and ignore errors\n"
"-q | --quiet                   Only display errors, set errorcode to exe|cpu type\n"
"-v | --version                 Show version and copyright and quit\n"
"-? | --help                    Show this help message and quit\n"
);
}

struct _result {
	unsigned int retval;
	WORD	machine_id;
	WORD	subsystem_id;
	TCHAR	*machine;
	TCHAR	*subsystem;
	TCHAR	*output;
};

typedef struct _result result_t;

static result_t result = {
	0,
	0,
	0,
	_T(""),
	_T(""),
	_T("")
};

enum e_errnum {
	ERROR_NO_ERROR,
	ERROR_CANNOT_OPEN_FILE,
	ERROR_CANNOT_READ_FILE,
	ERROR_UNKNOWN_FILE_TYPE,
	ERROR_CANNOT_READ_FILE2,
	ERROR_CANNOT_SEEK_FILE,
	ERROR_CANNOT_READ_NT_SIGNATURE,
	ERROR_MISSING_NT_SIGNATURE,
	ERROR_CANNOT_READ_HEADER,
	ERROR_CANNOT_READ_OPTIONAL_HEADER,
	ERROR_UNKNOWN_CPU_TYPE,
	ERROR_UNKNOWN_SUBSYSTEM_TYPE
};

typedef enum e_errnum errnum_t;

struct _err {
	errnum_t	num;
	TCHAR	*str;
};

typedef struct _err error_t;

static error_t errors[] = {
	{ERROR_NO_ERROR, _T("No error")},
	{ERROR_CANNOT_OPEN_FILE, _T("Cannot open file")},
	{ERROR_CANNOT_READ_FILE, _T("Cannot read file")},
	{ERROR_UNKNOWN_FILE_TYPE, _T("Missing DOS signature (MZ)")},
	{ERROR_CANNOT_READ_FILE2, _T("Cannot read file")},
	{ERROR_CANNOT_SEEK_FILE, _T("Cannot seek file")},
	{ERROR_CANNOT_READ_NT_SIGNATURE, _T("Cannot read NT signature")},
	{ERROR_MISSING_NT_SIGNATURE, _T("Missing NT signature")},
	{ERROR_CANNOT_READ_HEADER, _T("Cannot read header")},
	{ERROR_CANNOT_READ_OPTIONAL_HEADER, _T("Cannot read optional header")},
	{ERROR_UNKNOWN_CPU_TYPE, _T("Unknown cpu type")},
	{ERROR_UNKNOWN_SUBSYSTEM_TYPE, _T("Unknown subsystem type")}
};

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

static errnum_t get_machine(WORD machine, TCHAR **result) {
	static TCHAR buf[128];

	switch (machine) {
		case IMAGE_FILE_MACHINE_I386:
			*result = _T("x86 (32-bit)");
			break;
		case IMAGE_FILE_MACHINE_IA64:
			*result = _T("Intel Itanium (64-bit)");
			break;
		case IMAGE_FILE_MACHINE_AMD64:
			*result = _T("x64 (64-bit)");
			break;

    	case 0x13: // IMAGE_FILE_MACHINE_AM33      = 0x13,
    		*result = _T("AM33");
    		break;

    	case 0x1C0: // IMAGE_FILE_MACHINE_ARM       = 0x1C0,
    		*result = _T("ARM");
    		break;

    	case 0x1C4: // IMAGE_FILE_MACHINE_ARMNT     = 0x1C4,
    		*result = _T("ARMNT");
    		break;

    	case 0xAA64: // IMAGE_FILE_MACHINE_ARM64     = 0xAA64,
    		*result = _T("ARM64");
    		break;

    	case 0xEBC: // IMAGE_FILE_MACHINE_EBC       = 0xEBC,
    		*result = _T("EBC");
    		break;

    	case 0x9041: // IMAGE_FILE_MACHINE_M32R      = 0x9041,
    		*result = _T("M32R");
    		break;

    	case 0x266: // IMAGE_FILE_MACHINE_MIPS16    = 0x266,
    		*result = _T("MIPS16");
    		break;

    	case 0x366: // IMAGE_FILE_MACHINE_MIPSFPU   = 0x366,
    		*result = _T("MIPSFPU");
    		break;

    	case 0x466: // IMAGE_FILE_MACHINE_MIPSFPU16 = 0x466,
    		*result = _T("MIPSFPU16");
    		break;

    	case 0x1F0: // IMAGE_FILE_MACHINE_POWERPC   = 0x1F0,
    		*result = _T("POWERPC");
    		break;

    	case 0x1F1: // IMAGE_FILE_MACHINE_POWERPCFP = 0x1F1,
    		*result = _T("POWERPCFP");
    		break;

    	case 0x166: // IMAGE_FILE_MACHINE_R4000     = 0x166,
    		*result = _T("R4000");
    		break;

    	case 0x1A2: // IMAGE_FILE_MACHINE_SH3       = 0x1A2,
    		*result = _T("SH3");
    		break;

    	case 0x1A3: // IMAGE_FILE_MACHINE_SH3DSP    = 0x1A3,
    		*result = _T("SH3DSP");
    		break;

    	case 0x1A6: // IMAGE_FILE_MACHINE_SH4       = 0x1A6,
    		*result = _T("SH4");
    		break;

    	case 0x1A8: // IMAGE_FILE_MACHINE_SH5       = 0x1A8,
    		*result = _T("SH5");
    		break;

    	case 0x1C2: // IMAGE_FILE_MACHINE_THUMB     = 0x1C2,
    		*result = _T("THUMB");
    		break;

    	case 0x16: // IMAGE_FILE_MACHINE_WCEMIPSV2 = 0x169
    		*result = _T("WCEMIPSV2");
    		break;

   		default:
			sprintf(buf, _T("Unknown cpu type: %04x"), machine);
			*result = buf;
			return ERROR_UNKNOWN_CPU_TYPE;
	}

	return ERROR_NO_ERROR;
}

/*
See https://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx
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
#define IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION 16
*/

static errnum_t get_subsystem(WORD subsystem, TCHAR **result) {
	static TCHAR buf[128];

	switch (subsystem) {
		case IMAGE_SUBSYSTEM_UNKNOWN:
			*result = _T("Unknown");
			break;
		case IMAGE_SUBSYSTEM_NATIVE:
			*result = _T("Native");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_GUI: // 2
			*result = _T("Windows GUI");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CUI: // 3
			*result = _T("Windows Console");
			break;
		case IMAGE_SUBSYSTEM_OS2_CUI:
			*result = _T("OS/2 Console");
			break;
		case IMAGE_SUBSYSTEM_POSIX_CUI:
			*result = _T("Posix Console");
			break;
		case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:
			*result = _T("Native Win9x");
			break;
		case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
			*result = _T("Windows CE GUI");
			break;
		case IMAGE_SUBSYSTEM_EFI_APPLICATION:
			*result = _T("EFI Application");
			break;
		case IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER:
			*result = _T("EFI Boot Service Driver");
			break;
		case IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER:
			*result = _T("EFI Runtime Driver");
			break;
		case IMAGE_SUBSYSTEM_EFI_ROM:
			*result = _T("EFI ROM");
			break;
		case IMAGE_SUBSYSTEM_XBOX:
			*result = _T("XBOX");
			break;
#ifdef IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION
		case IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION:
			*result = _T("Boot Application");
			break;
#endif
		default:
			sprintf(buf, _T("Unknown subsystem type %d"), subsystem);
			*result = buf;
			return ERROR_UNKNOWN_SUBSYSTEM_TYPE;
	}

	return ERROR_NO_ERROR;
}

static int get_ver_info(const TCHAR *filename) {
	HANDLE hImage;
	DWORD dwCoffHeaderOffset;
	DWORD dwNewOffset;
	DWORD dwMoreDosHeader[16];
	ULONG ulNTSignature;
	errnum_t rv;

	IMAGE_DOS_HEADER dos_header;
	IMAGE_FILE_HEADER file_header;
	IMAGE_OPTIONAL_HEADER optional_header;

	result.retval = 0;
	result.machine_id = 0;
	result.subsystem_id = 0;
	result.machine = _T("");
	result.subsystem = _T("");
	result.output = _T("");

	hImage = CreateFile(
		filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hImage == INVALID_HANDLE_VALUE) {
		return ERROR_CANNOT_OPEN_FILE;
	}

	// Read MS-Dos image header.

	if (!ReadFileBytes (hImage, &dos_header, sizeof(IMAGE_DOS_HEADER))) {
		return ERROR_CANNOT_READ_FILE;
	}

	if (dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
		return ERROR_UNKNOWN_FILE_TYPE;
	}

	// Read more MS-Dos header.

	if (!ReadFileBytes (hImage, dwMoreDosHeader, sizeof (dwMoreDosHeader))) {
		return ERROR_CANNOT_READ_FILE2;
	}

	// Move the file pointer to get the actual COFF header.

	dwNewOffset = SetFilePointer(hImage, dos_header.e_lfanew, NULL, FILE_BEGIN);
	dwCoffHeaderOffset = dwNewOffset + sizeof (ULONG);

	if (dwCoffHeaderOffset == 0xFFFFFFFF) {
		return ERROR_CANNOT_SEEK_FILE;
	}

	// Read NT signature of the file.

	if (!ReadFileBytes (hImage, &ulNTSignature, sizeof (ULONG))) {
		return ERROR_CANNOT_READ_NT_SIGNATURE;
	}

	if (ulNTSignature != IMAGE_NT_SIGNATURE) {
		return ERROR_MISSING_NT_SIGNATURE;
	}

	if (!ReadFileBytes (hImage, &file_header, IMAGE_SIZEOF_FILE_HEADER)) {
		return ERROR_CANNOT_READ_HEADER;
	}

	result.machine_id = file_header.Machine;

	rv = get_machine(file_header.Machine, &result.machine);
	if (rv == ERROR_UNKNOWN_CPU_TYPE) {
		result.output = result.machine;
		return rv;
	}

	// Read the optional header of file.

	if (!ReadFileBytes (hImage, &optional_header, IMAGE_SIZEOF_NT_OPTIONAL_HEADER)) {
		return ERROR_CANNOT_READ_OPTIONAL_HEADER;
	}

	result.subsystem_id = optional_header.Subsystem;

	rv = get_subsystem(optional_header.Subsystem, &result.subsystem);
	if (rv == ERROR_UNKNOWN_SUBSYSTEM_TYPE) {
		result.output = result.subsystem;
		return rv;
	}

	return ERROR_NO_ERROR;
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
            case 't':
				opt.type = 1;
				opt.cpu = 0;
                break;
            case 'c':
				opt.cpu = 1;
				opt.type = 0;
                break;
            case 'i':
				opt.ignore = 1;
                break;
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
			case ':':
				fprintf(stderr, "Option -%c requires an operand\n", optopt);
				// fallthrough
			default:
				usage();
				exit(-1);
        }
    }

	int rv = 0;
	if (optind >= argc) {
		usage();
		exit(-1);
	}

	while (optind < argc) {
		char *arg = argv[optind];
		++optind;

		rv = get_ver_info(arg);
		if (rv != ERROR_NO_ERROR) {
			if (strlen(result.output) == 0) {
				result.output = errors[rv].str;
			}
			fprintf(stderr, "%s: %s\n", arg, result.output);
			if (opt.ignore) {
				continue;
			}
			break;
		}
		if (opt.quiet) {
			rv = result.subsystem_id;
			if (opt.cpu) {
				rv = result.machine_id;
			}
			continue;
		}
		if (opt.type) {
			if (opt.cpu) {
				fprintf(stdout, "%s: %s: %s\n", arg, result.subsystem, result.machine);
			} else {
				fprintf(stdout, "%s: %s\n", arg, result.subsystem);
			}
		} else {
			fprintf(stdout, "%s: %s\n", arg, result.machine);
		}
	}

	return (int) rv;
}
