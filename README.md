# PineBuds Open Source SDK
SDK from the Pine64 Wiki with Ralim's Docker build added (but it doesnt work, read on)

### Notable changes from the default SDK:
- Fixed pairing
- Custom EQ
  - I made the EQ based on a hearing test from Peace Equalizer APO. It should make the response curve much closer to flat, but is relative to my ears

### Notes:
- The SDK does not seem to work with new compilers. Use [GCC Version 9-2019-q4-major from ARM](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads/9-2019-q4-major)
  - The prebuilt open_source.bin in the out directory of the SDK archive worked, but my builds didn't. The only apparent difference was the compiler. Using the same version as the prebuilt made my build work, though I couldn't tell you why.
- HW IIR can't be used at the same time as ANC :(

### How to build
1. Download the version of GCC specified above
2. Extract the GCC archive
3. Clone this repo
4. `export CROSS_COMPILE=/<path to your extracted gcc directory>/bin/arm-none-eabi-`
5. `make T=open_source DEBUG=1`

Use the Windows programming tool from the Pine64 Wiki to flash, following the manual.

