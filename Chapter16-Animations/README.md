# Animations
An animation is just a series of changing images, and that's all we need to do to animate our sprites. Once we have several frames of animation images loaded into VRAM, its as simple as changing the image address on a sprite to cycle through them at a fixed rate.

The CX16 doesn't have any special hardware or registers to support animations, so there isn't anything new to learn here. We just need to create a few frames of an animation and cycle through them. Let's dive right into an example using the code from the previous Sprites chapter. It will be the same program but we will have a few images for the sprite to cycle through so it animates as it moves around.

## Example
Look at `anim.c`. You'll see 16 animation frames (images) being created in VRAM. Each frames colors are slightly adjusted so the animation looks like a reverse waterfall. You can change the speed of the animation by adjusting:

```C
#define ANIMATION_SPEED 6 // Lower is faster
```

`make anim` to build, `make runanim` to run it.