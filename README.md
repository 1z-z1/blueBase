# bgfx_project - blueBase

To build whole project enter...
```
./build.sh
```
To run the project after this use
```
./bgfx_project
```
If you broke the project after the inital build enter...
```
./build.sh clean-project-files
```
then...
```
./build.sh
```
or if you want to do the build "manually" then enter...
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

lastly 
```
./bgfx_project
```
