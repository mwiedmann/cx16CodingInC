# Tile Size
So far we have been dealing with tiles that are 8x8 pixels (this is the default). The `TILEBASE` register lets us set a height and width for our tiles though.

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
		<td>0x9F36</td>
		<td>L1_TILEBASE</td>
		<td colspan="6" align="center">Tile Base Address (16:11)</td>
		<td colspan="1" align="center">Tile Height</td>
		<td colspan="1" align="center">Tile Width</td>
	</tr>
</tbody>
</table>

Both the width and the height can be set to either 8 or 16 pixels. For games, 8x8 pixel tiles may be a little too small, so you can set it to 16x16. You also may have a game where it is easier if your tiles are taller or wider, so you can switch to 8x16 or 16x8. Here are the values:

<table>
<thead>
	<tr>
		<th align="right">Value</th>
		<th>Tile width / height</th>
	</tr>
</thead>
<tbody>
	<tr>
		<td align="right">0</td>
		<td>8 pixels</td>
	</tr>
	<tr>
		<td align="right">1</td>
		<td>16 pixels</td>
	</tr>
</tbody>
</table>

## TileBase Memory Layout
In any height/width configuration, the tile bytes in memory go from the top-left of the tile, to the bottom-right. Meaning, with our standard TileBase VRAM address of `0x1F000`, the pixel(s) at the top-left start at `0x1F000`, then you work your way left-to-right and move down to the next row when you hit the end. This continues through the entire tile until you hit the byte(s) for the bottom-right pixel.

The tile examples we looked at in Chapters 6 and 9 were all loaded into memory this way. It doesn't matter what the tile height/width are.

## Map Height/Width
If you change the tile size on a layer, you may not need the Map Height/Width to be so large. The default when you start the emulator is 8x8 tiles with 128x64 Map width/height. This is because with 8x8 pixel tiles, you can display 80x60 tiles on a 640x480 screen. But, let's say we change our tile width/height to 16x8 pixel (wider) tiles. With 16 pixel width tiles, we can only show 40 of those on our 640 pixel width screen. We can lower the Map width value of 128 down to the next lowest value which is 64. It won't change what you see on the screen, but it reduces the number of "off screen" tiles we discussed in the chapter about MapBase. Basically it saves some VRAM.

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

<table>
<thead>
	<tr>
		<th align="right">Value</th>
		<th>Map width / height</th>
	</tr>
</thead>
<tbody>
	<tr>
		<td align="right">0</td>
		<td>32 tiles</td>
	</tr>
	<tr>
		<td align="right">1</td>
		<td>64 tiles</td>
	</tr>
	<tr>
		<td align="right">2</td>
		<td>128 tiles</td>
	</tr>
	<tr>
		<td align="right">3</td>
		<td>256 tiles</td>
	</tr>
</tbody>
</table>

## Example
Run the example to see a tile width of 16 pixels (wide tiles). We lower the Map Width down to 64. We create 2 tiles and fill the screen with them.

To build the example, run `make`

To run the example, run `make run`

You can also run it manually as usual.

<style>
table, th, td {
  border: 1px solid;
}
</style>