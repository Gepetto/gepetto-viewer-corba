# Gepetto Viewer to Blender export

Script for importing in blender a motion computed with gepetto viewer

* [Exporting the scene](#exporting-the-scene)
* [Recording frames](#recording-frames)
* [Loading the motion into blender](#loading-the-motion-into-blender)

## Exporting the scene

### Robot - moving objects
Robots have to be exported **before** applying any configuration. Right after having loaded all your models, do `gui.writeNodeFile (nodeName, filename)`

### Environment - static objects
Environment has to be exported **after** in place (having been placed correctly). Once they are at their good location, do
`gui.writeNodeFile (nodeName, filename)`

### File format
- Collada: seems OK
- OBJ: not tested

## Recording frames

- setup the export by calling `gui.setCaptureTransform (filename, nodeName)` where `nodeName` refers to a node containing all the objects that will be recorded.
- a frame can then be saved using `gui.captureTransform ()` to capture the current state.

**Notice**: datas will be appended to `filename` so it must be empty at the beginning.

```python
gui.setCaptureTransform ("path.yaml", "hrp2")

for i in range(0,100):
	   #update robot configuration
     ...
        gui.captureTransform ()
```

**TODO**: add a function `captureOnRefresh(bool)` that turns on/off an automatic capture when `refresh` is called.

## Loading the motion into blender

### Running blender

Two options:

- ```blender --python <source-folder>/gepetto-viewer-corba/blender/gepettoimport.py```
- Manually load the plugin by running the script `blender/gepettoimport.py`
_Switch to the script view in blender (menu on the top the screen) the python script that you can find in the gepetto-viewer under `blender/gepettoimport.py` and press `Run script`_

### Load the motion

- Load your object definition files of the robot and the environment.
- Press `Space bar` and start typing `gepetto` (the search result must be "Import a YAML Gepetto Viewer path file")
- Select the file containing the configurations and wait...

Voilà! A set of frames should automatically be generated, resulting in an animation.

If some errors occur, the messages will appear in the console, or the terminal used to launch blender, depending on how the script was launched.
