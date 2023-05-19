# Layers
The CX16 has 2 layers for displaying tiles. When the CX16 starts, only `layer 1` is enabled. All of our examples so far have been using just this one layer. Well, we have an entire second layer to use, `layer 0`. Layer 0 has its own set of registers for programming it, but functions exactly the same way as Layer 1. The only difference is that when it is displayed on the screen, it is behind Layer 1.

## Transparency
Since layer 0 is behind layer 1, you might be wondering how you see layer 0. The answer is `transparency`. Color 0 is transparent and will instead show whatever is behind it. You can have individual pixels in a tile be transparent, or have an entire tile be transparent. Game will often start by using a bulk method to set an entire layer to transparent, then go and place the visible tiles.

## Enabling Layers
The `0x09F29 DC_VIDEO` register has bits 5/4 to turn layers on/off. Layer 1 is on when the CX16 starts, but you can use either layer or both. You can even turn them on/off at various points in your game.

```C
// Turn on BOTH layers
VERA.display.video |= 0b00110000;
```

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
</tbody>
</table>

## Memory Management
When using both layers, VRAM address start to get interesting. You now have 2 MapBase addresses (one for each layer). You want each layer to have its own area in VRAM so they can each display something different. If they shared a MapBase, they would just display the same thing. So you have to calculate how much memory your MapBase needs, then give each layer a MapBase address so they don't collide. Here is an example as seen in `main.c` in this chapter:

Let's say we have 16x16 pixel tiles, and we are using 8 bpp mode. We learned in the Tile Sizes chapter that our Tile W/H would be 64x32 (to accommodate the 40x30 tiles that would be on the screen). Each layer's MapBase would be `4k or 4,096 bytes`. How did we calculate that?

- 64x32 = 2,048 Tiles on (and off) screen
- 2,048 x 2 bytes per tile (see the MapBase chapter, each tile takes 2 bytes) = `4,096 bytes` needed for the MapBase

We also need some space for our Tiles. Let's say we have 24 tiles. This would need `6,144 bytes`. How did we calculate that?
- Tiles are 16x16 pixels and each pixel is 1 byte (because 8 bpp mode), so that's 16x16x1 = 256 bytes per tile
- We have 24 tiles so 24x256 = `6,144 bytes`

When we write our program and enable both layers, we need these 3 "chunks" of VRAM to not overlap. We also have to be careful of the address "boundaries" discussed in both the MapBase and Tile chapters. MapBase address must be a multiple of 512 bytes, and TileBase addresses must be a multiple of 2,048 bytes. This is due to the partial addresses you put into the registers. Addresses can also always start at 0.

Base on all of this, good addresses in this case could be:
- TileBase - 0x0
- Layer 0 MapBase - 0x1800 (6,144)
- Layer 1 MapBase - 0x2800 (10,240)

This respects the address boundaries for both and uses the minimum amount of VRAM. Sometimes you may not want them so close together if you are expecting to add more Tiles later. It may also make sense to have your TileBase LAST so you can add more tiles and not worry about crashing into your MapBases. The best plan is to use `#define` statements to control your addresses so you can easily change them later.

## Example Code
Take a look at `main.c` to see an example of the above scenarios.
- We enable both layers
- 16x16 pixel tiles
- 8 bpp mode
- On layer 0, we rotate through the 24 tiles displaying them in a pattern
- On layer 1, we alternate between 2 columns. One column of white tiles, and one column transparent. You will see the colorful tiles from layer 0 show through the transparent columns.

Change the value of `#define DRAW_LAYER_1` to `0` to have layer 1 be FULLY transparent and see all of layer 0 show through.

`make` to build the example and `make run` to run it.


<style>
table, th, td {
  border: 1px solid;
}
</style>