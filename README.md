# Building Vurtigo Plugins using the Vurtigo SDK

## Overview
The Vurtigo SDK docker image allows developers to build and test plugins for Vurtigo 3.2.0

It is packaged into an Ubuntu 18.04 image with:
- Vurtigo 3.2.0
- Geometry Server 3.2.0
- Qt 4.8.7
- VTK 6.2
- ITK 5.0.1
- CTK
- OpenIGTLink

It includes:
- all necessary Vurtigo header files, under /usr/include/vurtigoIncludes
- an example plugin, with CMakeLists.txt file that can be used to build against installed Vurtigo libs
- developer tools (compiler, debugger, etc)

## Requirements
You will need:
- a Linux system with an X server
- docker
- appropriate graphics drivers

For testing the atypical workflow you will need:
- x11docker

The following will be helpful for a typical developer workflow, as described later in the document:
- git
- VS Code

See [installation page](http://vurtigo.ca#install) for more detailed instructions to install the requirements.

To install the sdk image, use:

`docker pull labonny/vurtigo-sdk:v320`

## Recommendations
When developing your own plugins, you will need the container to have access to code mounted from your host machine, and when you test your plugin with Vurtigo, you will also need the container to be able to access your X11 server.

The default user in the SDK image is "vdev": by default, this user will not have access to files on your host machine. We recommend using VS Code to build your own plugins as described below, which will mount your code locally and run the container with your host user permissions.

## Atypical workflow with sample CameraMotion plugin
This workflow will not typically be used for regular development but demonstrates how to build and run the sample plugin that is included with the SDK image without any additional tools beyond the ones already in use for the regular Vurtigo image.

If you prefer, you can skip this section and instead use VS Code to compile a different plugin from github as described in the "Typical workflow with the HelloWorld Plugin" section.

### Build the sample plugin in the SDK container

The example plugin is in /home/vdev/CameraMotion

Launch the container with a terminal using:
``docker run -it labonny/vurtigo-sdk:v320 /bin/bash``

This should log you in as the "vdev" user in "/home/vdev" in the container.

To build the sample plugin, navigate to the folder and make a build directory:
```
cd CameraMotion
mkdir build
```

- Configure using: `cmake -S . -Bbuild`
- Then build using: `make -C build`

### Save the changes to a new image
You can exit the container using the ``exit`` command from the terminal.

You can find the container name / ID using:
``docker ps --all``

There should be a container with the image name "labonny/vurtigo-sdk:v320" listed, whose status should show it was exited a few seconds ago.

Next, save the built plugin to a new image using the container's name / ID in place of "CONTAINER" below:
``docker commit --change='CMD ["/opt/vurt/start.sh"]' CONTAINER vurtigo-sdk:v320-mytest``

You can call your image something other than "vurtigo-sdk:v320-mytest" if you wish - then you must change the instruction below to run Vurtigo from your new image.

### Running Vurtigo
Vurtigo can be started in the same way as for the default vurtigo image, but with the name of the modified sdk image:

``x11docker -m --gpu --share /home/username/data --runtime=nvidia --network -- -p 1777:1777 -- vurtigo-sdk:v320-mytest``

This will launch Geometry Server and Vurtigo.

- you may not need the runtime option, depending on your graphics driver setup
- the share option can be edited to point to a folder on your host machine, or removed if you do not want to mount a folder to share between the host machine and container

#### Loading the plugin

The plugin can then be loaded in Vurtigo by choosing "Load Plugin File" in the Vurtigo Plugin Browser, and selecting "/home/vdev/CameraMotion/CameraMotionConfig.xml".

## Typical workflow with the HelloWorld plugin

This workflow uses a development container in [VS Code](https://code.visualstudio.com/), as demonstrated with the Hello World plugin. This has only been tested on Linux systems with an X Server. If you haven't used development containers before, you can learn more about them [here](https://code.visualstudio.com/docs/devcontainers/containers).

With this workflow, you will not need to launch Vurtigo using x11docker.

### VS Code setup
If you don't already have Visual Studio code installed:

`sudo snap install vscode --classic`

You can then launch VS Code with the command:

`code`

and install the **Dev Containers** [extension](https://code.visualstudio.com/docs/editor/extension-marketplace), by switching to the Extensions view (click the appropriate icon in the left sidebar or **Ctrl-Shift-X**) and using the search bar and install buttons.

You can instead install the extension from the command line, if you prefer:

`code --install-extension ms-vscode-remote.remote-containers`

### Hello World Plugin
This plugin is set up with default VS Code settings (devcontainer.json and launch.json files) that streamline the development process.

On your host machine, get the code from github:
`git clone https://github.com/WrightGroupSRI/HelloWorld.git`

If you still have VS Code open from the previous step, use the File menu to open the new HelloWorld folder, otherwise switch into the new directory and launch VS Code from there:
```
cd HelloWorld
code .
```
On opening this folder, you may see options pop up to "Reopen in Container" and "Configure" the project. Select "Reopen in Container".

If you don't see these options, open the Command Palette (**Ctrl-Shift-P**) then start typing "Dev Containers" and select "Dev Containers: Rebuild and Reopen in Container".

This will launch the Vurtigo SDK container, mounting the code into a "/workspaces" folder in this container. This may take a few seconds, and you will see the word "Dev Container" in the status bar. Your VS Code terminal and commands you run in this mode will run within the container.

#### Building the Plugin

 You should see a pop-up prompting you to "Configure the project Hello World": click "Yes". You will see some output from the configure step below the main window.

 If you don't see this, or hit "No" by accident: Type **Ctrl-Shift-P** to bring up the Command Palette, then start typing CMake. Select "CMake: Configure". You will have the option of selecting a compiler, and only one should be visible in the dropdown list: you can select that compiler or select "Unspecified" and cmake will select the compiler by default

Next, click the "Build" button at the bottom of the VS Code editor. Alternatively, type **Ctrl-Shift-P** to bring up the Command Palette, and select "CMake: Build" to build the plugin.

You should see the build progress below the main window: if it reaches 100% with an exit code of 0, it was successful.

#### Loading the Plugin
Launch Vurtigo by selecting "Run without Debugging" from the VS Code "Run" menu (or click the "Run" button in the status bar at the bottom of the VS Code editor). Click the "Load Plugin File" button in the Vurtigo Plugin Browser, and select /workspaces/HelloWorld/HelloWorldConfig.xml

You can now select the "Hello World" plugin from the Plugin Browser and interact with it: this plugin creates a label.

### Exiting the Dev Container
To exit the Dev Container, type **Ctrl-Shift-P** to bring up the Command Palette, then start typing Reopen. Select "Dev Containers: Reopen Folder Locally".

Changes you make to the plugin code while in the Dev Container should be reflected in your host files even after you have exited the container.

## Notes
### Git
- You can make git commits to the local repository of the sample plugin / your plugin from inside the Dev Container
- If you want to push and pull from a remote private git repository from inside the container, [share your git credentials with the container](https://code.visualstudio.com/remote/advancedcontainers/sharing-git-credentials)
### Debugging
- You can set breakpoints and use the debugger from the VS Code Run menu
### Shared and persistent files
- The default devcontainer.json file mounts the host home directory into /hosthome
  - you can remove or edit this line as desired (to say, mount a subdirectory of your home directory which contains DICOM files)
  - edit the devcontainer.json file to change other run-time arguments of the container as desired
- Vurtigo settings and the default Vurtigo dicom database will not persist across sessions unless specific directories are mounted from the host into the SDK container home directory: if you want persistence, you first need to create a ".config" folder and "Vurtigo" folder on the host and then mount these under ``/home/vdev`` on the container. The mounts can be done by modifying the "devcontainer.json" file to add bind mounts, similar to the ones used for home directory mounting and X11 socket sharing, using the "-v" flag in the runArgs section.
### Installing software into the SDK image
If you want to install additional software into the container, you can do this by extending the Dockerfile, making sure to switch user to root before installing what you need (if this is a system-wide install for the image), then switching back to the vdev user after. For example, to install the package that provides the *ping* networking tool, create this Dockerfile:
```
FROM labonny/vurtigo-sdk:v320
USER root
RUN apt install iputils-ping
USER vdev
```
You can then build a new vurtigo-sdk image from this Dockerfile, using:
``docker build -t vurtigo-sdk:v320-myVersion .``

Be sure to modify your devcontainer.json file to specify the new image version.
### Qt Designer
Plugin interfaces can be created using Qt Designer 4.8.7 to generate ".ui" files. Qt Designer can be launched from inside the Dev Container by typing "designer" in the Terminal.
