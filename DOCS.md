# CX16 Documentation
There is great documentation for the CX16 and we will reference it often in these tutorials. This section will serve as a catch-all for important links so you can easily reference them later.

## The VERA Programmer's Reference
Believe it or not, just about everything you need to know about graphics programming on the CX16 is contained in one document. Bookmark this, always have it open in a browser tab, print it, and wallpaper your room with it. You will refer to this document A LOT and learn to love it!

[The VERA Programmer's Reference](https://github.com/X16Community/x16-docs/blob/master/VERA%20Programmer's%20Reference.md)

## Emulator, CX16, and VERA Docs
The emulator downloads include a "docs" folder that should have the latest documentation for that release, or you can also access the latest docs through github:

[Online docs](https://github.com/X16Community/x16-docs)

## cc65 Docs and Function References
The C compiler we will use, cc65, has thorough documentation about the compiler and available libraries and their functions. This includes the all important `cx16.h`:
- [Compiler info](https://cc65.github.io/doc/cc65.html)
- [CX16 spec.ific info](https://cc65.github.io/doc/cx16.html)
- [Basic Function reference](https://cc65.github.io/doc/funcref.html)
- [Header Files (Good reference)](https://github.com/cc65/cc65/tree/master/include)
- [cx16.h - IMPORTANT](https://github.com/cc65/cc65/blob/master/include/cx16.h)

## VERA Registers
This is available online but I'm displaying parts of it in the various chapters so this is just here for copy/paste. It will be removed in the end in favor of a link to the latest documentation.

<table>
	<tbody>
    <tr>
		<th>Addr</th>
		<th>Name</th>
		<th>Bit&nbsp;7</th>
		<th>Bit&nbsp;6</th>
		<th>Bit&nbsp;5 </th>
		<th>Bit&nbsp;4</th>
		<th>Bit&nbsp;3 </th>
		<th>Bit&nbsp;2</th>
		<th>Bit&nbsp;1 </th>
		<th>Bit&nbsp;0</th>
	</tr>
	<tr>
		<td>0x9F20</td>
		<td>ADDRx_L (x=ADDRSEL)</td>
		<td colspan="8" align="center">VRAM Address (7:0)</td>
	</tr>
	<tr>
		<td>0x9F21</td>
		<td>ADDRx_M (x=ADDRSEL)</td>
		<td colspan="8" align="center">VRAM Address (15:8)</td>
	</tr>
	<tr>
		<td>0x9F22</td>
		<td>ADDRx_H (x=ADDRSEL)</td>
		<td colspan="4" align="center">Address Increment</td>
		<td colspan="1" align="center">DECR</td>
		<td colspan="2" align="center">-</td>
		<td colspan="1" align="center">VRAM Address (16)</td>
	</tr>
	<tr>
		<td>0x9F23</td>
		<td>DATA0</td>
		<td colspan="8" align="center">VRAM Data port 0</td>
	</tr>
	<tr>
		<td>0x9F24</td>
		<td>DATA1</td>
		<td colspan="8" align="center">VRAM Data port 1</td>
	</tr>
	<tr>
		<td>0x9F25</td>
		<td>CTRL</td>
		<td colspan="1" align="center">Reset</td>
		<td colspan="6" align="center">DCSEL</td>
		<td colspan="1" align="center">ADDRSEL</td>
	</tr>
	<tr>
		<td>0x9F26</td>
		<td>IEN</td>
		<td colspan="1" align="center">IRQ line (8)</td>
		<td colspan="3" align="center">-</td>
		<td colspan="1" align="center">AFLOW</td>
		<td colspan="1" align="center">SPRCOL</td>
		<td colspan="1" align="center">LINE</td>
		<td colspan="1" align="center">VSYNC</td>
	</tr>
	<tr>
		<td>0x9F27</td>
		<td>ISR</td>
		<td colspan="4" align="center">Sprite collisions</td>
		<td colspan="1" align="center">AFLOW</td>
		<td colspan="1" align="center">SPRCOL</td>
		<td colspan="1" align="center">LINE</td>
		<td colspan="1" align="center">VSYNC</td>
	</tr>
	<tr>
		<td>0x9F28</td>
		<td>IRQLINE_L</td>
		<td colspan="8" align="center">IRQ line (7:0)</td>
	</tr>
	<tr>
		<td>0x9F29</td>
		<td>DC_VIDEO (DCSEL=0)</td>
		<td colspan="1" align="center">Current Field</td>
		<td colspan="1" align="center">Sprites Enable</td>
		<td colspan="1" align="center">Layer1 Enable</td>
		<td colspan="1" align="center">Layer0 Enable</td>
		<td colspan="1" align="center">NTSC/RGB: 240P</td>
		<td colspan="1" align="center">NTSC: Chroma Disable / RGB: HV Sync </td>
		<td colspan="2" align="center">Output Mode</td>
	</tr>
	<tr>
		<td>0x9F2A</td>
		<td>DC_HSCALE (DCSEL=0)</td>
		<td colspan="8" align="center">Active Display H-Scale</td>
	</tr>
	<tr>
		<td>0x9F2B</td>
		<td>DC_VSCALE (DCSEL=0)</td>
		<td colspan="8" align="center">Active Display V-Scale</td>
	</tr>
	<tr>
		<td>0x9F2C</td>
		<td>DC_BORDER (DCSEL=0)</td>
		<td colspan="8" align="center">Border Color</td>
	</tr>
	<tr>
		<td>0x9F29</td>
		<td>DC_HSTART (DCSEL=1)</td>
		<td colspan="8" align="center">Active Display H-Start (9:2)</td>
	</tr>
	<tr>
		<td>0x9F2A</td>
		<td>DC_HSTOP (DCSEL=1)</td>
		<td colspan="8" align="center">Active Display H-Stop (9:2)</td>
	</tr>
	<tr>
		<td>0x9F2B</td>
		<td>DC_VSTART (DCSEL=1)</td>
		<td colspan="8" align="center">Active Display V-Start (8:1)</td>
	</tr>
	<tr>
		<td>0x9F2C</td>
		<td>DC_VSTOP (DCSEL=1)</td>
		<td colspan="8" align="center">Active Display V-Stop (8:1)</td>
	</tr>
	<tr>
		<td>0x9F29</td>
		<td>DC_VER0 (DCSEL=63)</td>
		<td colspan="8" align="center">Read only: the ASCII character "V"</td>
	</tr>
	<tr>
		<td>0x9F2A</td>
		<td>DC_VER1 (DCSEL=63)</td>
		<td colspan="8" align="center">Read only: Major release</td>
	</tr>
	<tr>
		<td>0x9F2B</td>
		<td>DC_VER2 (DCSEL=63)</td>
		<td colspan="8" align="center">Read only: Minor release</td>
	</tr>
	<tr>
		<td>0x9F2C</td>
		<td>DC_VER3 (DCSEL=63)</td>
		<td colspan="8" align="center">Read only: Minor build number</td>
	</tr>
	<tr>
		<td>0x9F2D</td>
		<td>L0_CONFIG</td>
		<td colspan="2" align="center">Map Height</td>
		<td colspan="2" align="center">Map Width</td>
		<td colspan="1" align="center">T256C</td>
		<td colspan="1" align="center">Bitmap Mode</td>
		<td colspan="2" align="center">Color Depth</td>
	</tr>
	<tr>
		<td>0x9F2E</td>
		<td>L0_MAPBASE</td>
		<td colspan="8" align="center">Map Base Address (16:9)</td>
	</tr>
	<tr>
		<td>0x9F2F</td>
		<td>L0_TILEBASE</td>
		<td colspan="6" align="center">Tile Base Address (16:11)</td>
		<td colspan="1" align="center">Tile Height</td>
		<td colspan="1" align="center">Tile Width</td>
	</tr>
	<tr>
		<td>0x9F30</td>
		<td>L0_HSCROLL_L</td>
		<td colspan="8" align="center">H-Scroll (7:0)</td>
	</tr>
	<tr>
		<td>0x9F31</td>
		<td>L0_HSCROLL_H</td>
		<td colspan="4" align="center">-</td>
		<td colspan="8" align="center">H-Scroll (11:8)</td>
	</tr>
	<tr>
		<td>0x9F32</td>
		<td>L0_VSCROLL_L</td>
		<td colspan="8" align="center">V-Scroll (7:0)</td>
	</tr>
	<tr>
		<td>0x9F33</td>
		<td>L0_VSCROLL_H</td>
		<td colspan="4" align="center">-</td>
		<td colspan="8" align="center">V-Scroll (11:8)</td>
	</tr>
	<tr>
		<td>0x9F34</td>
		<td>L1_CONFIG</td>
		<td colspan="2" align="center">Map Height</td>
		<td colspan="2" align="center">Map Width</td>
		<td colspan="1" align="center">T256C</td>
		<td colspan="1" align="center">Bitmap Mode</td>
		<td colspan="2" align="center">Color Depth</td>
	</tr>
	<tr>
		<td>0x9F35</td>
		<td>L1_MAPBASE</td>
		<td colspan="8" align="center">Map Base Address (16:9)</td>
	</tr>
	<tr>
		<td>0x9F36</td>
		<td>L1_TILEBASE</td>
		<td colspan="6" align="center">Tile Base Address (16:11)</td>
		<td colspan="1" align="center">Tile Height</td>
		<td colspan="1" align="center">Tile Width</td>
	</tr>
	<tr>
		<td>0x9F37</td>
		<td>L1_HSCROLL_L</td>
		<td colspan="8" align="center">H-Scroll (7:0)</td>
	</tr>
	<tr>
		<td>0x9F38</td>
		<td>L1_HSCROLL_H</td>
		<td colspan="4" align="center">-</td>
		<td colspan="8" align="center">H-Scroll (11:8)</td>
	</tr>
	<tr>
		<td>0x9F39</td>
		<td>L1_VSCROLL_L</td>
		<td colspan="8" align="center">V-Scroll (7:0)</td>
	</tr>
	<tr>
		<td>0x9F3A</td>
		<td>L1_VSCROLL_H</td>
		<td colspan="4" align="center">-</td>
		<td colspan="8" align="center">V-Scroll (11:8)</td>
	</tr>
	<tr>
		<td>0x9F3B</td>
		<td>AUDIO_CTRL</td>
		<td colspan="1" align="center">FIFO Full / FIFO Reset</td>
		<td colspan="1" align="center">FIFO_Empty<br>(read-only)</td>
		<td colspan="1" align="center">16-Bit</td>
		<td colspan="1" align="center">Stereo</td>
		<td colspan="4" align="center">PCM Volume</td>
	</tr>
	<tr>
		<td>0x9F3C</td>
		<td>AUDIO_RATE</td>
		<td colspan="8" align="center">PCM Sample Rate</td>
	</tr>
	<tr>
		<td>0x9F3D</td>
		<td>AUDIO_DATA</td>
		<td colspan="8" align="center">Audio FIFO data (write-only)</td>
	</tr>
	<tr>
		<td>0x9F3E</td>
		<td>SPI_DATA</td>
		<td colspan="8" align="center">Data</td>
	</tr>
	<tr>
		<td>0x9F3F</td>
		<td>SPI_CTRL</td>
		<td colspan="1" align="center">Busy</td>
		<td colspan="5" align="center">-</td>
		<td colspan="1" align="center">Slow clock</td>
		<td colspan="1" align="center">Select</td>
	</tr>
</tbody></table>

<!-- Extra styling info for some Markdown engines (e.g. VSCode) -->
<style>
table, th, td {
  border: 1px solid;
}
</style>
