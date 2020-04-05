# Pulsar
A recreation of the Teenage Engineering Tombola OP-1 sequencer

### What Am I looking at?
It's kind of like a random sequencer (although, barring floating point errors, I guess all of the mechanics operate within a deterministic system - so its not really random. Given the exact same initial / temporal conditions you could recreate anything here).

There is this rotating (where it came from or why I'm not sure), collapsing polygon which holds little bouncing balls. Clicking in the shape or pressing a key on an input Midi device spawns a ball (either where you clicked or randomly with a note on midi input event) and when these balls collide with eachother or the sides of the polygonal prison they scream out the original midi note that birthed them or something random if they were born out of the quantum-fluctuation that occurs after a mouseclick. If there's someone listening on a midi output device it might just convert and echo and remit the wail. 

Using the box2d engine for physics this cosmic ballet plays out endlessly and if you feel particularly benevolent you can very slightly open up the polygon and let some of those balls escape and live the life they've always wanted & deserved.

### Compiling / Building / Running

At the moment the only way to get this running is to compile it yourself (and only tested on macOS).
[JUCE](https://juce.com/) is the audio framework used. I've included it as a submodule, so getting everything you need should be as simple as:

```
git clone git@github.com:matthewh806/Pulsar.git
cd Pulsar
git submodule update --init --recursive
```

Then open the projucer and click to compile the Xcode project and build!

### TODO
I've mostly tried to stick quite closely to the original. But have a few ideas for some neat additions:
* A free play mode which generates random notes (maybe within a certain key) at random time intervals with all other properties randomized too. Like an audiosaver?
* Ability to modulate things like gravity, rotation speed, polygon separation

