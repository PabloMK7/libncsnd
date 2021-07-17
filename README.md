# libncsnd
Library that provides an interface for accessing the **CSND** audio service on the Nintendo 3DS. This gives a layer of abstraction for playing sound using such service, which useful in the case the main audio playback service (**DSP**) is not avaibale. For any uses other than audio playback, check the **CSND** implementation in *libctru*.

# Installation and Usage
1. Make sure you have [devkitpro](https://devkitpro.org/wiki/Getting_Started) installed and working.
2. Clone or download the repo and open a command prompt.
3. Run `make install` and confirm there aren't any errors.
4. In your project makefile, add the following to the `LIBDIRS` line (or similar): `$(DEVKITPRO)/libncsnd`
5. In your project makefile, add the following to the `LIBS` line (or similar): `-lncsnd` if the already listed libraries start with `-l` or just `ncsnd` if they don't.
6. Add `#include "ncsnd.h"` in your source files to use the library.

You can check all the available function calls in the documentation provided in [ncsnd.h](include/ncsnd.h).

# License

Check the [LICENSE](LICENSE.md) file.