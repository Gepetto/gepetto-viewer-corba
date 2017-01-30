# Gepetto Viewer to Blender export

Script for importing in blender a motion viewed with gepetto viewer

* [Exporting the scene](#exporting-the-scene)
* [Recording frames](#recording-frames)
* [Loading the motion into blender](#loading-the-motion-into-blender)

## Exporting the scene

In python, run `gui.writeBlenderScript("blenderscript.py", ["node1", "node2"])`.

**Notice**: datas will be appended to `blenderscript.py` so it must be empty at the beginning. Otherwise, you will get an error when loading the file into blender.

## Recording frames

First, setup the export by calling `gui.setCaptureTransform (filename, [nodeName1, nodeName2, ...])` where `[nodeName1, nodeName2, ...]` refers to a node containing all the objects that will be recorded.

- Frame by frame record:
a frame can then be saved using `gui.captureTransform ()` to capture the current state.
- Capturing on refresh:
use `gui.captureTransformOnRefresh(True)` to start capturing and `gui.captureTransformOnRefresh(False)` to stop it.

**Notice**: datas will be appended to `filename` so it must be empty at the beginning. Otherwise, you will get an error when loading the file into blender.

**Notice**: The size of this file quickly increases. Have in mind that your movie will most likely be at 25 frames per second.

```python
gui.setCaptureTransform ("path.yaml", "hrp2")

# Frame by Frame
for i in range(0,100):
	   #update robot configuration
     ...
        gui.refresh ()
        gui.captureTransform ()


# Capture on refresh
gui.captureTransformOnRefresh(true)
for i in range(0,100):
	   #update robot configuration
     ...
        gui.refresh ()

gui.captureTransformOnRefresh(false)

```

## Loading the motion into blender

### Running blender

Blender is using python 3, so you may encounter some problems of you do not set `PYTHONPATH` environment properly.

Three options:

- ```blender --python <source-folder>/gepetto-viewer-corba/blender/gepettoimport.py```
- Install it as an addon (https://www.blender.org/manual/advanced/scripting/python/add_ons.html) - save the user preferences
- Manually load the plugin by running the script `blender/gepettoimport.py`
_Switch to the script view in blender (menu on the top the screen) the python script that you can find in the gepetto-viewer under `blender/gepettoimport.py` and press `Run script`_

Personnally, I'm using the following script to launch blender:
```bash
#!/bin/bash

export PYTHONPATH=/usr/lib/python3/dist-packages
blender-2.75a-linux-glibc211-i686/blender $@ --python "/local/jmirabel/devel/hpp/src/gepetto-viewer-corba/blender/gepettoimport.py"
```

### Load the motion

- Import the model file (generated at step 1):
  * Press `Space Bar` and search for "Import a URDF blender script".
  * You can do this as many times as you want.
- Import the motion file (generated at step 2):
  * Press `Space bar` and search for "Import a YAML Gepetto Viewer path file"
  * You can do this as many times as you want.
  * This can take a while.

Voilà! A set of frames should automatically be generated, resulting in an animation.

If some errors occur, the messages will appear in the console, or the terminal used to launch blender, depending on how the script was launched.
