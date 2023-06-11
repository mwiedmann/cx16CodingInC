# Sprites
Sprites are images that you can place anywhere on (or off) the screen. You use them for things like the player, enemies, missiles, and anything that moves and/or has an animation. They can be displayed in front of, between, or behind the 2 layers. This allows you to create some foreground/background effects and give your game visual depth. The CX16 also has some hardware support for detecting collisions between sprites.

The CX16 has support for 128 sprites and they have the following attributes:
- 4 bpp (16 color) or 8 bpp (256 colors) modes
- 8, 16, 32, or 64 pixel height and width
- 4 bit collision masks for configuring collision detection
- Z-Depth for setting where it displays around layers
- Vertical and Horizontal Flip bits
- 4 bit Palette Offset for color changes/cycling

Here are some of the VERA registers we will be dealing with. We will dive into sprite collisions and Interrupts later, so for now we just enable `bit 6 in 0x9F29 DC_VIDEO` to enable sprites.

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

## Sprite Attributes
Starting at VRAM address `0x1FC00` there are 128 entries `(each is 8 bytes)` for the 128 sprites. You are free to enable and use as many of them as you like, `BUT NOTE: Sprite 0 is used by the Kernal for the mouse pointer sprite if you enable  mouse support, so it is usually best to start at sprite 1.`

Each sprite has the following format:

<table>
    <tbody>
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
		<td align="center" colspan="8">Address (12:5)</td>
	</tr>
	<tr>
		<td>1</td>
		<td>Mode</td>
		<td align="center" colspan="3">-</td>
		<td align="center" colspan="4">Address (16:13)</td>
	</tr>
	<tr>
		<td>2</td>
		<td align="center" colspan="8">X (7:0)</td>
	</tr>
	<tr>
		<td>3</td>
		<td align="center" colspan="6">-</td>
		<td align="center" colspan="2">X (9:8)</td>
	</tr>
	<tr>
		<td>4</td>
		<td align="center" colspan="8">Y (7:0)</td>
	</tr>
	<tr>
		<td>5</td>
		<td align="center" colspan="6">-</td>
		<td align="center" colspan="2">Y (9:8)</td>
	</tr>
	<tr>
		<td>6</td>
		<td align="center" colspan="4">Collision mask</td>
		<td align="center" colspan="2">Z-depth</td>
		<td align="center">V-flip</td>
		<td align="center">H-flip</td>
	</tr>
	<tr>
		<td>7</td>
		<td align="center" colspan="2">Sprite height</td>
		<td align="center" colspan="2">Sprite width</td>
		<td align="center" colspan="4">Palette offset</td>
	</tr>
    </tbody>
</table>

Here are the tables for some of the values you will set:

#### Color Mode
| Mode | Description |
| ---- | ----------- |
| 0    | 4 bpp - 16 color      |
| 1    | 8 bpp - 256 color      |

#### Z-Depth (also enables sprite)
| Z-depth | Description                           |
| ------- | ------------------------------------- |
| 0       | Sprite disabled                       |
| 1       | Sprite between background and layer 0 |
| 2       | Sprite between layer 0 and layer 1    |
| 3       | Sprite in front of layer 1            |

#### Sprite Size
| Sprite width / height | Description |
| --------------------- | ----------- |
| 0                     | 8 pixels    |
| 1                     | 16 pixels   |
| 2                     | 32 pixels   |
| 3                     | 64 pixels   |

### Enabling a Sprite - Z-Depth
Once you have enabled sprites at the CX16 level `(bit 6 in 0x9F29 DC_VIDEO)`, you have to enable the individual sprites you want to use. You do this by setting the `Z-Depth (bits 3/2 in byte 6)` to a value other than 0. The sprite will then be enabled and will appear according to the Z-Depth chart above. 

### Color Mode and Sprite Size
Bit 7 in byte 1 controls the color mode of 4 or 8 bpp. In byte 7, bits 7/6 control the sprite height, and bits 5/4 control the sprite width. See the charts above for the values. Setting these is important because it controls how the image for the sprite will be interpreted and drawn to the screen. We discussed 4 and 8 bpp tiles and tiles of different widths/heights in the Tiles and Tile Sizes chapters. Sprite images work EXACTLY THE SAME WAY and use the same format.

### Flip Bits
Setting either of these bits to 1 will flip the image along the horizontal or vertical axis. This is very useful for sprites that need to go in both directions. For instance, if you have a player sprite, you want the player to face left when moving left, and face right when moving right. Rather than creating an entirely new sprite for each direction, you can use the H-Flip bit to reverse the image when moving in that direction.

### Sprite Image Address
Each sprite has a VRAM address that points to the image it should display. Its a 17 bit VRAM address spread across 2 bytes, but we only specify bits 16-5. We've run into this pattern several times already with TileBase and MapBase addresses. The lowest 5 bits (4-0) are defaulted to 0. This just means our images must start on a multiple of 32 bytes.

<table>
    <tbody>
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
		<td align="center" colspan="8">Address (12:5)</td>
	</tr>
	<tr>
		<td>1</td>
		<td>Mode</td>
		<td align="center" colspan="3">-</td>
		<td align="center" colspan="4">Address (16:13)</td>
	</tr>
    </tbody>
</table>

#### Setting a Sprite Address
Here is an example of setting the address on a sprite. Let's say the address was `0x1000` and you are setting it on Sprite 1:

```C
// This can be any VRAM Address (but must be a multiple of 32)
unsigned long spriteGraphicAddress = 0x1000;

// VRAM address for sprite 1 (this is fixed)
unsigned long sprite1Address = 0x1FC08;

// A sprite image would be created or loaded at spriteGraphicAddress

// Point to Sprite 1
VERA.address = sprite1Address;
VERA.address_hi = sprite1Address>>16;
// Set the Increment Mode, turn on bit 4
VERA.address_hi |= 0b10000;

// Write out 2 bytes of address info
// Shift the bits to get the parts we want
// Graphic address bits 12:5
VERA.data0 = spriteGraphicAddress>>5;
// Graphic address bits 16:13
VERA.data0 = spriteGraphicAddress>>13;
```

## Example
Look at `sprite.c` for an example of a 64x64 pixel, 256 color sprite moving around the screen. It creates a 64x64 pixel image for the sprite that uses all 256 colors in a pattern. Change it to anything you like. The sprite will move diagonally until it hits the edge of the screen then wraps back to the other side. `make sprite` and `make runsprite` to build/run it.

## Other Sprite Functionality
Many sprites in your games will not just be a fixed image, they will animate. They will also collide with other sprites, and need to be created/destroyed on the fly. We will dive into more advanced sprite usage in other chapters.

## cc65 Support for Sprites
This is one that you have to do on your own. I think because sprites are pretty different between many of the supported systems, and many of them don't have sprites at all, cc65 doesn't have a sprite library (at least I didn't see one). Hopefully you just saw from our code examples that its not too difficult to do. You can wrap a lot of that code up in reusable functions and make your own sprite library!

<!-- Extra styling info for some Markdown engines (e.g. VSCode) -->
<style>
table, th, td {
  border: 1px solid;
}
</style>