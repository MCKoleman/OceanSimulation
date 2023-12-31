# Ocean Simulation
This program attempts to create a realistic lighting simulation of an ocean.<br/>

![OceanSimulationSOS](https://github.com/MCKoleman/OceanSimulation/assets/20149932/2c3e166a-a3b2-4d9a-a46a-6b4f85990288)

![OceanSimulationSOSWireframe](https://github.com/MCKoleman/OceanSimulation/assets/20149932/378e511b-557d-4ef7-b810-c58bb0534c6a)

## Getting Started With The Simulator
To get the simulator to work, you need to use [CMake](https://cmake.org/cmake/help/latest/manual/cmake-gui.1.html) to generate the code.
![The file setup should look like this](./resources/images/Using-CMake-GUI.png)


Set up file directories as you see in the image above (source code is in the root and build it in `./bin`) then complete the following steps
1. Hit `Configure`, choose the version of Visual Studio to use, then wait for the "Configuring done" message to pop up in the terminal
2. Select `Generate` and use the default values if prompted
3. Click on `Open Project` and use the default values if prompted
4. In Visual Studio, run the debugger (CMake will have created this for you!)
5. You now should have the simulation running on your machine (after it loads using the terminal)!

When running the simulator, you can move the camera by holding the `ALT` key and either the `W`, `A`, `S`, and `D` keys to move forward, left, right, and down or the arrow keys to change the camera's pitch. You can also use `Alt` and the mouse for movement. `Alt` + Left click move the camera while `Alt` + right click pans on the XY or YZ plane depending where you are. Feel free to alter the values in the menu to see how it effects the simulation.

## Helpful Resources
To understand the equations we arrived to in creating this simulation, we used the following resource for background research in this project:
- The [NVIDIA GPUGems book](https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models)
- Acerola's two part series on developing a water shader in Unity
    - Part 1 [video](https://www.youtube.com/watch?v=PH9q0HNBjT4)
    - Part 2 [video](https://www.youtube.com/watch?v=yPfagLeUa7k)
    - His [repository](https://github.com/GarrettGunnell/Water)

And we used these pages for reference in implementing the Discrete Fourier and Fast Fourier transforms:
- [DFT](https://www.keithlantz.net/2011/10/ocean-simulation-part-one-using-the-discrete-fourier-transform/) 
- [FFT](https://www.keithlantz.net/2011/11/ocean-simulation-part-two-using-the-fast-fourier-transform/)

# Citations
We did not create our own skyboxes. You can find the skyboxes we used here:
- https://learnopengl.com/Advanced-OpenGL/Cubemaps
- https://opengameart.org/content/cloudy-skyboxes

# License
MIT License

Copyright (c) 2023 Kristian Kolehmainen and Robert Kilkenny

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
