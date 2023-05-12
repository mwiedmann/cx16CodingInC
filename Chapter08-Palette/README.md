# Palette
The CX16 has a palette of 256 colors. Every pixel displayed on the screen must be one of these 256 colors. Each palette color has an index associated with it from 0-255. Your tiles and sprites will not have color information like RGB values. Instead, they will have palette indexes that point to one of palette colors. We saw that in 1bpp mode, the foreground and background colors each had a 4 bit index. This only lets you reference the first 16 colors of the palette. If you turn on the `T256C` bit, then you can only set a foreground color but you have access to all 256 colors. In 2/4/8 bpp modes, you also have access to all 256 colors (we will look at these in later chapters). The point is, you are always using palette colors.

## The Standard Palette
When you start the emulator, it has a standard palette already loaded for you with a wide variety of colors. It is likely that you can create all of your tiles and sprites using the standard palette.

- Color indexes 0-15 contain a palette somewhat similar to the C64 color palette.
- Color indexes 16-31 contain a grayscale ramp.
- Color indexes 32-255 contain various hues, saturation levels, brightness levels.

Here is an image of the standard palette from the CX16 documentation:

![CX16 Standard Palette](https://github.com/X16Community/x16-docs/blob/master/cx16palette.png?raw=true)

## Palette Location
In previous chapters, we saw that when dealing with TileBase and MapBase, we are able to use some registers to specify the address in VRAM that stores that data. The emulator starts with some default values, but we can actually change the location of TileBase/MapBase data to be anywhere in VRAM. The palette is a bit different. It has a FIXED VRAM address that you CANNOT CHANGE and also CANNOT READ. The palette is stored in a special set of memory and is mapped to a specific area of VRAM that is WRITE-ONLY. This means you are free to write to/change the palette colors, but you can't read them back. This is fine as you generally don't care what the current palette colors are and just want to set them to your specific values. Where is the palette located in VRAM?

According to VERA documentation: `Palette: 0x1FA00 - 0x1FBFF`

This is exactly 512 bytes. This makes sense because each palette color has 2 bytes of information. 2 * 256 = 512 bytes.

>Note on READING Palette memory. It won't cause an error if you try to read a palette VRAM address, you just won't get what is actually stored in that palette entry initially and may get random data. The VERA docs have more info about this, but in general I would avoid doing this.

## Palette Entries
Each 2 byte palette color has 4 bits each for the RGB info. This leaves 4 unused bits. There are 256 (2 byte) entries starting at VRAM address `0x1FA00`. Here is the format from the VERA documentation:

<table>
	<tr>
		<th>Offset</th>
		<th>Bit&nbsp;7</th>
		<th>Bit&nbsp;6</th>
		<th>Bit&nbsp;5</th>
		<th>Bit&nbsp;4</th>
		<th>Bit&nbsp;3</th>
		<th>Bit&nbsp;2</th>
		<th>Bit&nbsp;1</th>
		<th>Bit&nbsp;0</th>
	</tr>
	<tr>
		<td>0</td>
		<td align="center" colspan="4">Green</td>
		<td align="center" colspan="4">Blue</td>
	</tr>
	<tr>
		<td>1</td>
		<td align="center" colspan="4">-</td>
		<td align="center" colspan="4">Red</td>
	</tr>
</table>

With 12 bits for a color, that gives us 4096 possible colors to choose from. Each RGB value is 4 bits (16 possible values) to specify the weight of that value. This is basically the same thing as standard RGB colors you use with HTML/CSS, but instead of 0-255/0x00-0xFF for each part, you just have 0-15/0x0-0xF. A value of 15 would be equal to 255, a value of 8 would be around 128 (the middle), and 0 is 0. So you can express the same range of colors but with less precision. I think you will find that its hard to tell the difference honestly.

## Changing the Palette
We know that our current 1bpp mode is only using the first 16 colors. By using the VRAM writing technique we've learned, we can change these bytes (colors) to whatever we want. Let's change them and see what happens. Look at and run `main.c` to see how to change the first 16 colors to shades of blue. You'll see that every color in the emulator is now a shade of blue. Go ahead and try changing it to shades of red or other colors.

<!-- Extra styling info for some Markdown engines (e.g. VSCode) -->
<style>
table, th, td {
  border: 1px solid;
}
</style>