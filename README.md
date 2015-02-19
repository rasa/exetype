# exetype 

Display the type of a Windows EXE file

## Usage

````
exetype [options] target.exe

Options (defaults in parentheses):
-q | --quiet                   Only display errors, set errorcode to exe type
-v | --version                 Show version and copyright and quit
-? | --help                    Show this help message and quit
````

## Examples

````
c:\>exetype exetype.exe

exetype.exe: Windows Console

c:\>exetype %windir%\notepad.exe
C:\WINDOWS\notepad.exe: Windows GUI

c:\>echo %errorlevel%
2

c:\>exetype -q exetype.exe

c:\>echo %errorlevel%
3
````

## EXE Types
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

## Contributing

To contribute to this project, please see [CONTRIBUTING.md](CONTRIBUTING.md).

## Bugs

To view existing bugs, or report a new bug, please see the [issues](/issues) page for this project.

## License

This project is [MIT licensed](LICENSE).

## Changelog

Please see [CHANGELOG.md](CHANGELOG.md) for the version history for this project.

## Contact

This project was originally developed by [Ross Smith II](mailto:ross@smithii.com).
Any enhancements and suggestions are welcome.
