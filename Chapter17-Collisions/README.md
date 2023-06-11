# Sprite Collisions
Detecting sprite collisions is part of just about every game. You have to be careful though because it can be CPU intensive to check if any of your dozens of sprites are colliding every game loop. Fortunately, the CX16 gives us some help. It has hardware level sprite collision detection. It doesn't do ALL the work for you, but it gets you most of the way there.

## Collision Detection
We saw these registers briefly in the Sprites chapter, but let's look at how to use them for sprite collisions.

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
</tbody>
</table>

### Enable Collisions
To enable sprite collision detection, enable (set to 1) `bit 2 on 0x9F26 IEN (Interrupt Enable)`.

## IRQ (Interrupt Request)
Sprite collisions make use of an IRQ or Interrupt Request. This is a low level processor event that is a bit beyond the scope of this document, but we have to understand a few things. An IRQ is a way for the processor to be "interrupted". When it happens, the processor will suddenly start executing code associated with the IRQ. When it is done, it will return back to where it was interrupted. It's usually used for hardware events but the 6502 can handle software events like a (BRK - Break).

The code for wiring up an IRQ handler requires some assembly language. I'm going to wave the white flag on this one and just use the `set_irq` function that cc65 provides for us in `6502.h`. You can see the source assembly for it in [interrupt.s](https://github.com/cc65/cc65/blob/652949f183783198e398fa40d839c5fcc1f9c156/libsrc/common/interrupt.s) if you are interested.

`set_irq` allows us to run a C function whenever the IRQ is fired. This is our clue that a collision has occurred. The only thing we need to do in this function is check `bit 2 in 0x9F27 ISR (Interrupt Source)` to see if it was fired because of a collision. If it was, we clear the interrupt by `setting that same bit = 1`, and returning a value of 1 (IRQ_HANDLED). Otherwise, we just return a value of 0 (IRQ_NOT_HANDLED), and the interrupt will be handled by some other code.

> NOTE: Because the C function you create will be executed as an interrupt, you also have to provide some memory for it to use as a "stack". You are interrupting the normal "flow" of your code and you can't use the normal stack, so you will see that we create a small array to go along with the function.

After the IRQ handler has run, the `0x9F27 ISR "Sprite collisions" bits 7-4` will hold details about the collision. We can then take action against the collision. Here is the basic code:

```C
#include "wait.h"
#include <6502.h>
#include <cx16.h>

// Your function needs its own stack
// It has to be large enough to hold any local variables
// and any other functions it calls.
// It is best to keep your handler small. This should be enough.
#define IRQ_HANDLER_STACK_SIZE 8
unsigned char irqHandlerStack[IRQ_HANDLER_STACK_SIZE];

unsigned char irqHandler() {
    // See if there is a collision
    if (VERA.irq_flags & 0b100) {
        // Clear the collision IRQ by writing 1 to the SPRCOL (bit 2) in the ISR
        VERA.irq_flags = 0b100; 
        return IRQ_HANDLED;
    }

    // This was fired for another reason (VSYNC probably)
    return IRQ_NOT_HANDLED;
}

void main() {
    // ...

    // Setup the IRQ handler for sprite collisions
    set_irq(&irqHandler, irqHandlerStack, IRQ_HANDLER_STACK_SIZE);

    while (1) {
        // Move your sprites ...

        wait();

        // Check the collision bits to see if there were any collisions
        if (VERA.irq_flags & 0b11110000) {
            // At a minimum, move or disable the sprite(s)
            // or the IRQ will keep firing.
        }
    }
}
```

## Collision Masks
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
		<td>6</td>
		<td align="center" colspan="4">Collision mask</td>
		<td align="center" colspan="2">Z-depth</td>
		<td align="center">V-flip</td>
		<td align="center">H-flip</td>
	</tr>
    </tbody>
</table>

Each sprite has `4 bits on byte 6` called `Collision Mask`. By setting these bits, you can control which sprites will collide with which other sprites. When the hardware sees 2 sprites colliding, it will do a `| (OR)` between the Collision Mask on each of the sprites. If any of the same bits are set to 1, then the result will be a non-zero value. The hardware will consider this a collision you care about and fire your IRQ handler. So, if you want 2 sprites to collide, set at least 1 of the same bits in their Collision Masks to be `on (1)`. Here is an example:

<table>
    <tbody>
    <tr>
        <th>Sprite</th>
        <th>Collision Mask</th>
    </tr>
    <tr>
        <td>A</td>
        <td>0001</td>
    </tr>
    <tr>
        <td>B</td>
        <td>0010</td>
    </tr>
    <tr>
        <td>C</td>
        <td>0110</td>
    </tr>
    <tr>
        <td>D</td>
        <td>1100</td>
    </tr>
    <tr>
        <td>E</td>
        <td>1111</td>
    </tr>
    </tbody>
</table>

### Collision Matrix
When these sprites collide, here are the results of the collision. No, means it would not be detected as a collision.

<table>
    <tbody>
        <tr>
            <th></th><th>A 0001</th><th>B 0010</th><th>C 0110</th><th>D 1100</th><th>E 1111</th>
        </tr>
        <tr>
            <td>A</td>
            <td>0001</td>
            <td>NO</td>
            <td>NO</td>
            <td>NO</td>
            <td>0001</td>
        </tr>
        <tr>
            <td>B</td>
            <td></td>
            <td>0010</td>
            <td>0010</td>
            <td>NO</td>
            <td>0010</td>
        </tr>
        <tr>
            <td>C</td>
            <td></td>
            <td></td>
            <td>0110</td>
            <td>0100</td>
            <td>0110</td>
        </tr>
        <tr>
            <td>D</td>
            <td></td>
            <td></td>
            <td></td>
            <td>1100</td>
            <td>1100</td>
        </tr>
        <tr>
            <td>E</td>
            <td></td>
            <td></td>
            <td></td>
            <td></td>
            <td>1111</td>
        </tr>
    </tbody>
</table>

## Example
Take a look at `collision.c` for an example of 2 sprites repeatedly colliding. `make collision` and `make runcollision` to build/run. It uses the IRQ code from above, and some of the sprite routines have been moved into their own functions so we can reuse them for each sprite. This is the kind of thing you will do in your games. Eventually you want to make some nice libraries that you can reuse across your other games.

<!-- Extra styling info for some Markdown engines (e.g. VSCode) -->
<style>
table, th, td {
  border: 1px solid;
}
</style>
