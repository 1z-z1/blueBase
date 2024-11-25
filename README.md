## bgfx_project - blueBase
<p align="center">
<img src="https://github.com/1z-z1/blueBase/blob/main/readmepreviewpic.png" width="400" />
</p>

-------------------------------------
To build whole project enter...
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
and that should build 'bgfx_project'

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
