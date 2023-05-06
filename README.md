# Commander X16 Programming in C
The purpose of this repo is to discuss programming in C on the Commander X16. The main part of this will cover how the VERA chip works and how we interact with Video Memory. We will cover graphics modes, layers, tiles and tilemaps, and sprites. We will use banked memory, kernal functions, and C library functions created for the CX16 (some of which have their roots in the C64). As we go along we will take some of what we learn early on about interacting with the VERA chip and wrap that in functions. This will create a library of helper functions to make our programs a bit more understandable. As you build this library you will be able to follow along with all the chapters and work along side me. Each chapter will include the current state of the helper library in case you need to start fresh or want to jump in at a certain point. We will prioritize readability of our code over efficiency. This is about learning and not creating the most efficient CX16 code. Although the code here will be performant and usable in your real projects, there are always ways to optimize.

## Using These Tutorials
The best first step is to clone the repo, but this is optional. Start with Chapter01 to get everything installed. Chapter02 then will help you get things set up, write your first program, and make sure everything is configured and running. After that, you are off and running.


## VERA Registers
<style>
table, th, td {
  border: 1px solid;
}
</style>

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
		<td>$9F20</td>
		<td>ADDRx_L (x=ADDRSEL)</td>
		<td colspan="8" align="center">VRAM Address (7:0)</td>
	</tr>
	<tr>
		<td>$9F21</td>
		<td>ADDRx_M (x=ADDRSEL)</td>
		<td colspan="8" align="center">VRAM Address (15:8)</td>
	</tr>
	<tr>
		<td>$9F22</td>
		<td>ADDRx_H (x=ADDRSEL)</td>
		<td colspan="4" align="center">Address Increment</td>
		<td colspan="1" align="center">DECR</td>
		<td colspan="2" align="center">-</td>
		<td colspan="1" align="center">VRAM Address (16)</td>
	</tr>
	<tr>
		<td>$9F23</td>
		<td>DATA0</td>
		<td colspan="8" align="center">VRAM Data port 0</td>
	</tr>
	<tr>
		<td>$9F24</td>
		<td>DATA1</td>
		<td colspan="8" align="center">VRAM Data port 1</td>
	</tr>
	<tr>
		<td>$9F25</td>
		<td>CTRL</td>
		<td colspan="1" align="center">Reset</td>
		<td colspan="6" align="center">DCSEL</td>
		<td colspan="1" align="center">ADDRSEL</td>
	</tr>
	<tr>
		<td>$9F26</td>
		<td>IEN</td>
		<td colspan="1" align="center">IRQ line (8)</td>
		<td colspan="3" align="center">-</td>
		<td colspan="1" align="center">AFLOW</td>
		<td colspan="1" align="center">SPRCOL</td>
		<td colspan="1" align="center">LINE</td>
		<td colspan="1" align="center">VSYNC</td>
	</tr>
	<tr>
		<td>$9F27</td>
		<td>ISR</td>
		<td colspan="4" align="center">Sprite collisions</td>
		<td colspan="1" align="center">AFLOW</td>
		<td colspan="1" align="center">SPRCOL</td>
		<td colspan="1" align="center">LINE</td>
		<td colspan="1" align="center">VSYNC</td>
	</tr>
	<tr>
		<td>$9F28</td>
		<td>IRQLINE_L</td>
		<td colspan="8" align="center">IRQ line (7:0)</td>
	</tr>
	<tr>
		<td>$9F29</td>
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
		<td>$9F2A</td>
		<td>DC_HSCALE (DCSEL=0)</td>
		<td colspan="8" align="center">Active Display H-Scale</td>
	</tr>
	<tr>
		<td>$9F2B</td>
		<td>DC_VSCALE (DCSEL=0)</td>
		<td colspan="8" align="center">Active Display V-Scale</td>
	</tr>
	<tr>
		<td>$9F2C</td>
		<td>DC_BORDER (DCSEL=0)</td>
		<td colspan="8" align="center">Border Color</td>
	</tr>
	<tr>
		<td>$9F29</td>
		<td>DC_HSTART (DCSEL=1)</td>
		<td colspan="8" align="center">Active Display H-Start (9:2)</td>
	</tr>
	<tr>
		<td>$9F2A</td>
		<td>DC_HSTOP (DCSEL=1)</td>
		<td colspan="8" align="center">Active Display H-Stop (9:2)</td>
	</tr>
	<tr>
		<td>$9F2B</td>
		<td>DC_VSTART (DCSEL=1)</td>
		<td colspan="8" align="center">Active Display V-Start (8:1)</td>
	</tr>
	<tr>
		<td>$9F2C</td>
		<td>DC_VSTOP (DCSEL=1)</td>
		<td colspan="8" align="center">Active Display V-Stop (8:1)</td>
	</tr>
	<tr>
		<td>$9F29</td>
		<td>DC_VER0 (DCSEL=63)</td>
		<td colspan="8" align="center">Read only: the ASCII character "V"</td>
	</tr>
	<tr>
		<td>$9F2A</td>
		<td>DC_VER1 (DCSEL=63)</td>
		<td colspan="8" align="center">Read only: Major release</td>
	</tr>
	<tr>
		<td>$9F2B</td>
		<td>DC_VER2 (DCSEL=63)</td>
		<td colspan="8" align="center">Read only: Minor release</td>
	</tr>
	<tr>
		<td>$9F2C</td>
		<td>DC_VER3 (DCSEL=63)</td>
		<td colspan="8" align="center">Read only: Minor build number</td>
	</tr>
	<tr>
		<td>$9F2D</td>
		<td>L0_CONFIG</td>
		<td colspan="2" align="center">Map Height</td>
		<td colspan="2" align="center">Map Width</td>
		<td colspan="1" align="center">T256C</td>
		<td colspan="1" align="center">Bitmap Mode</td>
		<td colspan="2" align="center">Color Depth</td>
	</tr>
	<tr>
		<td>$9F2E</td>
		<td>L0_MAPBASE</td>
		<td colspan="8" align="center">Map Base Address (16:9)</td>
	</tr>
	<tr>
		<td>$9F2F</td>
		<td>L0_TILEBASE</td>
		<td colspan="6" align="center">Tile Base Address (16:11)</td>
		<td colspan="1" align="center">Tile Height</td>
		<td colspan="1" align="center">Tile Width</td>
	</tr>
	<tr>
		<td>$9F30</td>
		<td>L0_HSCROLL_L</td>
		<td colspan="8" align="center">H-Scroll (7:0)</td>
	</tr>
	<tr>
		<td>$9F31</td>
		<td>L0_HSCROLL_H</td>
		<td colspan="4" align="center">-</td>
		<td colspan="8" align="center">H-Scroll (11:8)</td>
	</tr>
	<tr>
		<td>$9F32</td>
		<td>L0_VSCROLL_L</td>
		<td colspan="8" align="center">V-Scroll (7:0)</td>
	</tr>
	<tr>
		<td>$9F33</td>
		<td>L0_VSCROLL_H</td>
		<td colspan="4" align="center">-</td>
		<td colspan="8" align="center">V-Scroll (11:8)</td>
	</tr>
	<tr>
		<td>$9F34</td>
		<td>L1_CONFIG</td>
		<td colspan="2" align="center">Map Height</td>
		<td colspan="2" align="center">Map Width</td>
		<td colspan="1" align="center">T256C</td>
		<td colspan="1" align="center">Bitmap Mode</td>
		<td colspan="2" align="center">Color Depth</td>
	</tr>
	<tr>
		<td>$9F35</td>
		<td>L1_MAPBASE</td>
		<td colspan="8" align="center">Map Base Address (16:9)</td>
	</tr>
	<tr>
		<td>$9F36</td>
		<td>L1_TILEBASE</td>
		<td colspan="6" align="center">Tile Base Address (16:11)</td>
		<td colspan="1" align="center">Tile Height</td>
		<td colspan="1" align="center">Tile Width</td>
	</tr>
	<tr>
		<td>$9F37</td>
		<td>L1_HSCROLL_L</td>
		<td colspan="8" align="center">H-Scroll (7:0)</td>
	</tr>
	<tr>
		<td>$9F38</td>
		<td>L1_HSCROLL_H</td>
		<td colspan="4" align="center">-</td>
		<td colspan="8" align="center">H-Scroll (11:8)</td>
	</tr>
	<tr>
		<td>$9F39</td>
		<td>L1_VSCROLL_L</td>
		<td colspan="8" align="center">V-Scroll (7:0)</td>
	</tr>
	<tr>
		<td>$9F3A</td>
		<td>L1_VSCROLL_H</td>
		<td colspan="4" align="center">-</td>
		<td colspan="8" align="center">V-Scroll (11:8)</td>
	</tr>
	<tr>
		<td>$9F3B</td>
		<td>AUDIO_CTRL</td>
		<td colspan="1" align="center">FIFO Full / FIFO Reset</td>
		<td colspan="1" align="center">FIFO_Empty<br>(read-only)</td>
		<td colspan="1" align="center">16-Bit</td>
		<td colspan="1" align="center">Stereo</td>
		<td colspan="4" align="center">PCM Volume</td>
	</tr>
	<tr>
		<td>$9F3C</td>
		<td>AUDIO_RATE</td>
		<td colspan="8" align="center">PCM Sample Rate</td>
	</tr>
	<tr>
		<td>$9F3D</td>
		<td>AUDIO_DATA</td>
		<td colspan="8" align="center">Audio FIFO data (write-only)</td>
	</tr>
	<tr>
		<td>$9F3E</td>
		<td>SPI_DATA</td>
		<td colspan="8" align="center">Data</td>
	</tr>
	<tr>
		<td>$9F3F</td>
		<td>SPI_CTRL</td>
		<td colspan="1" align="center">Busy</td>
		<td colspan="5" align="center">-</td>
		<td colspan="1" align="center">Slow clock</td>
		<td colspan="1" align="center">Select</td>
	</tr>
</tbody></table>