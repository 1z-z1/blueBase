# bluebase

To start enter...
```
./build.sh restore blueBase
```

Then once that is done enter...
```
./build.sh
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