# Other Color Depth Modes
We've been talking about Color Depth mode 0 so far. This is 1 bpp (bit per pixel). This is the default mode when you start the emulator. This uses the minimum amount of video memory because each tile in the TileBase only uses 8 bytes. The down side though, is that you have limited color options. Depending on the `L1_CONFIG - T256C` bit, you an either have a 16 color foreground/background, or a 256 color foreground only. This doesn't let you set colors for individual pixels. The color effects the entire tile. This mode is fine for text based programs, and perhaps some games that have limited requirements for colors on your tiles, but most games are going to want better pixel level color control.

<table>
<thead>
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
</thead>
<tbody>
	<tr>
		<td>0x9F34</td>
		<td>L1_CONFIG</td>
		<td colspan="2" align="center">Map Height</td>
		<td colspan="2" align="center">Map Width</td>
		<td colspan="1" align="center">T256C</td>
		<td colspan="1" align="center">Bitmap Mode</td>
		<td colspan="2" align="center">Color Depth</td>
	</tr>
</tbody>
</table>

## Color Depth Modes 2/4/8 bpp
We saw in a previous chapter that the `0x9F34 (L1_CONFIG) Color Depth bits (1-0)` has 4 modes that control the number of `bpp` for each tile pixel. With Color Depths 1-3, we have 2, 4, and 8 bpp.

<table>
<thead>
	<tr>
		<th align="right">Color Depth</th>
		<th>Description</th>
	</tr>
</thead>
<tbody>
	<tr>
		<td align="right">0</td>
		<td>1 bpp</td>
	</tr>
	<tr>
		<td align="right">1</td>
		<td>2 bpp</td>
	</tr>
	<tr>
		<td align="right">2</td>
		<td>4 bpp</td>
	</tr>
	<tr>
		<td align="right">3</td>
		<td>8 bpp</td>
	</tr>
</tbody>
</table>

In Color Depth 0 (1 bpp), a pixel was either on or off, now with multiple bpp we instead provide a Palette Index value. We learned about using palette colors in the MapBase chapter when we set colors for our 1 bpp tiles. With 1 bpp, the color information was encoded in the MapBase where you set the color for the entire tile. The TileBase tiles had no color information. In our new 2/4/8 bpp modes, its the opposite. The TileBase tiles WILL have color information for each pixel.

### Mode 1 - 2 bpp
To change the Color Depth to 2 bpp, OR the current value with `1`. This will set `bit 0` and enable 2 bpp:
```C
VERA.layer1.config |= 0b1;
```

If you run this, everything in the emulator will turn to black and green and look garbled. This makes sense because the tiles we have loaded are 1 bpp tiles, and suddenly VERA is reading them as 2 bpp tiles which has a totally different encoding (hence the garbling). We need to create some 2 bpp tiles and use those.

## Tiles in Mode 1 - 2 bpp
With 2 bpp, we have 4 values per pixel (2 bits = 4 possible values). These values (0-3) are palette indexes. This means we can use the first 4 colors of the palette. 

>Note that palette index 0 is `transparent`. This allows tiles (from layer 0) and sprites behind that pixel to show through.

2 bpp is kind of weird though because a byte has 8 bits. This means each byte has 4 pixels worth of information in it. So you end up setting pairs of bits inside these bytes for individual pixels. An example may help. Here is a 4 color smiley face tile:

![2 bpp Tile](2bpp-tile.jpg)

You can see how each 2 bit pair controls 1 pixel. Tiles in this mode use 16 bytes. This is double what the 1 bpp mode used but you have more control over the colors.

## Tiles in Mode 2 - 4 bpp
With 4 bpp, we have 16 values per pixel (4 bits = 16 possible values). These values (0-15) are palette indexes for the first 16 colors of the palette. Color 0 is still transparent. Each byte in this mode covers 2 pixels (4 bits each). Each tile uses 32 bytes. Here is an example tile in 4 bpp mode:

![4 bpp Tile](4bpp-tile.jpg)

## MapBase for 2/4/8 bpp
The MapBase now still has a tile index, but instead of color information, it has some additional bits to control tile flipping and palette adjustments.

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
		<td align="center" colspan="8">Tile index (7:0)</td>
	</tr>
	<tr>
		<td>1</td>
		<td align="center" colspan="4">Palette offset</td>
		<td align="center">V-flip</td>
		<td align="center">H-flip</td>
		<td align="center" colspan="2">Tile index (9:8)</td>
	</tr>
</table>

<style>
table, th, td {
  border: 1px solid;
}
</style>