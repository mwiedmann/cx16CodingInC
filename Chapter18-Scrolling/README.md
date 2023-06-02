# Scrolling
Many games feature a moving or "scrolling" background (or even foreground). Platformers like Mario Bros., shoot-em-ups (SHUMPS) games like R-Type, Defender, or ground games like Contra, Commando, and Metal Slug, and sports games generally have the player able to move around areas larger than one screen by scrolling the field as the player moves. The CX16 helps you with this. Both Layers have "H-Scroll" and "V-Scroll" registers. We talked in the MapBase chapter about "hidden" tiles. These are tiles that are off screen. Well, with scrolling, they don't have to be off screen any longer. As you change the H/V-Scroll registers, the visible area of tiles will slide (scroll) left/right/up/down and show a new area of tiles as defined in your MapBase. So those previously "hidden" tiles will scroll into view. If you scroll beyond the end of your MapBase, VERA will simply wrap around and start displaying tiles from the other side. This effectively gives you infinite scrolling.

## H/V-Scroll Registers
Scrolling is handled by each layer. This allows you to have different scrolling speeds on each layer. You can achieve some nice depth effects this way by having the foreground layer scrolling more quickly than the background. Each layer has 2 registers each for the V/H-Scroll, that's 4 total registers per layer. There are 12 bits spread across 2 registers. This allows you to set a range of 0-4095 for each direction. This value is the number of pixels to scroll the layer in that direction.

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
</tbody></table>

## Sprite Adjustments
Depending on your game, you may need to adjust your sprite positions as you scroll a layer. Sprite x/y positions are their position on the screen. As you scroll your layers, your sprites will not be effected at all. They will continue to stay in their position on the screen. This may be what you want if your layer is just a scrolling background starfield for a space shooter game. As you adjust the starfield, you still want your player's ship to stay in the same position on the screen. If the layer is a soccer field though, as you scroll the soccer field you want the players to stay in that position on the field. This means you have to adjust the x/y position of the players as you scroll the field. So, it depends on if your sprite position are relative to the scrolling field or not.

## Example
In this pretty large example, we create an arena of sorts surrounded by walls. There are 32 sprites that start in random positions and move diagonally around the field and bounce off the walls. You can use the joystick/cursor keys to scroll around the field. You will see how VERA repeats things as it reaches the end of the MapBase size. There is a section of code you can uncomment to limit scrolling to within just the field. This is what you would do in a sports game like this.

You will see that we adjust the position of each sprite by the "scrollx" and "scrolly" values so that they stay on the correct position in the field. If we didn't do this, you would see the field scroll underneath the sprites and they would appear to be floating above it rather than staying in their correct field positions.

There are a bunch of `#define` statements at the top of `main.c` that control this example. Feel free to adjust them.
- `SPRITE_COUNT` - changes the number of sprites on the field. You'll see that it starts to slow down if you add too many. This is because we loop through each sprite and adjust it each game loop.
- `SCROLL_SPEED`, `BALL_SPEED` - changes the speed of the sprites and scrolling
- `XXX_COLOR` - there are several of these that control the colors

Some of the other WIDTH/HEIGHT defines are there as helpers and code changes would need to be made if you adjust them since there are some hardcoded values. So you won't be able to adjust some of those.

`make` to build the example and `make run` to run it.

<!-- Extra styling info for some Markdown engines (e.g. VSCode) -->
<style>
table, th, td {
  border: 1px solid;
}
</style>