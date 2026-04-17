# XEX MAP Symbol Importer (IDA)

Imports function names from a `.map` file into a loaded Xbox 360 XEX in IDA.

Originally created by [Xx jAmes t xX](https://github.com/XeAssert)

## Requirements

- IDA Pro
- [idaxex](https://github.com/emoose/idaxex) (IDA XEX loader plugin)

## Usage

1. Load your `.xex` in IDA using idaxex
2. Ensure the `.map` file:
   - has the same name as the `.xex`
   - is in the same folder

3. In IDA:
   - Go to **File -> Script file...**
   - Select `xenon_map_symbol_importer.idc`

## Notes

- Works for `.exe` basefiles
- Automatically rebases addresses using the `.map`'s preferred load address
