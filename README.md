# exetype [![Flattr this][flatter_png]][flatter]

Display the type of a Windows EXE file.

## Usage

````
exetype [options] target.exe

Options (defaults in parentheses):
-q | --quiet                   Only display errors, set errorcode to exe type
-v | --version                 Show version and copyright and quit
-? | --help                    Show this help message and quit
````

## Examples

````batch
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

````
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
````

## Verify a Release

To verify a release, download the .zip, .sha256, and .asc files for the release 
(replacing exetype-1.3-win32.zip with the release you are verifying):

````
$ wget https://github.com/rasa/exetype/releases/download/v1.3/exetype-1.3-win32.zip{,.sha256,.asc}
````

Next, check that sha256sum reports "OK":
````
$ sha256sum -c exetype-1.3-win32.zip.sha256
exetype-1.3-win32.zip: OK
````

Lastly, check that GPG reports "Good signature":

````
$ gpg --keyserver hkps.pool.sks-keyservers.net --recv-key 0x105a5225b6ab4b22
$ gpg --verify exetype-1.3-win32.zip.asc exetype-1.3-win32.zip
gpg:                using RSA key 0xFF914F74B4BB6EF3
gpg: Good signature from "Ross Smith II <ross@smithii.com>" [ultimate]
...
````

## Contributing

To contribute to this project, please see [CONTRIBUTING.md](CONTRIBUTING.md).

## Bugs

To view existing bugs, or report a new bug, please see [issues](../../issues).

## Changelog

To view the version history for this project, please see [CHANGELOG.md](CHANGELOG.md).

## License

This project is [MIT licensed](LICENSE).

## Contact

This project was created and is maintained by [Ross Smith II][] [![endorse][endorse_png]][endorse]

Feedback, suggestions, and enhancements are welcome.

[Ross Smith II]: mailto:ross@smithii.com "ross@smithii.com"
[flatter]: https://flattr.com/submit/auto?user_id=rasa&url=https%3A%2F%2Fgithub.com%2Frasa%2Fexetype
[flatter_png]: http://button.flattr.com/flattr-badge-large.png "Flattr this"
[endorse]: https://coderwall.com/rasa
[endorse_png]: https://api.coderwall.com/rasa/endorsecount.png "endorse"

