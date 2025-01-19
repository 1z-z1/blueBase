## bgfx_project - blueBase
<p align="center">
<img src="https://github.com/1z-z1/blueBase/blob/main/readmepreviewpic.png" width="400" />
</p>

-------------------------------------
Hey everyone, this is a bgfx base build that I used to get familar with the BGFX build system in a way that made sense to me. I hope in the future to share more things I have been working on related to this project but for now this is WYSIWYG.

-------------------------------------
To build the whole project enter...
```
./build.sh
```
To run the program after this use
```
./bgfx_project
```
-------------------------------------
If you broke the project after the inital build enter...
```
./build.sh clean-project-files
```
then...
```
./build.sh
```
-------------------------------------
If you want to do the build "manually" then enter...
```
./build.sh clean-project-files
```
```
./build.sh restore blueBase
```
After that enter...
```
./3rdparty/genie/bin/linux/genie gmake
```
You can enter...
```
./compile_shaders 
```
but it doesn't do too much in this version.

Then you enter...
```
make config=release64 
```
or
```
make config=debug64
```
and one those should build 'bgfx_project'

Then...
```
./bgfx_project
```
To run the program.

-------------------------------------
Lastly if you would like to save a project you are working on you can do...
```
./build.sh backup [PROJECT NAME]
```
to save it in the bak directory like blueBase.

Note that if you do this you should be using the manual method of building.
The build script can be modified to not restore blueBase and launch it everytime.
I added that for first time ease of use.
