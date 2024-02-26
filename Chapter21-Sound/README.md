# Sound
As stated in the CX16 documentation, it has three sound generators
- Yamaha YM2151: 8 channels, FM synthesis
- VERA PSG: 16 channels, 4 waveforms
- VERA PCM: 48 kHz, 16 bit, stereo

I recommend using the "ZSMKit" library from MooingLemur:

https://github.com/mooinglemur/zsmkit

It's made to use in Assembly but you can use it in C with `asm` calls.

You need some special entries in your .cfg file for cc65 to know when the ZSMkit library is in memory. See `cx16-zsm-bank.cfg` for an example. This config file includes support for outputting Banked RAM code and ZSMKit. To use it, you would add `-C cx16-zsm-bank.cfg` to your cl65 compiling/linking command.

I've also included a prebuilt `zsmkit.lib` for you. You need to include this in the list of files being compiled by cl65. Alternatively, you can build your own from the zsmkit repo. You may want to do this if MooingLemur makes any updates.
