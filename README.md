# LivkuDumper

A Runtime Dumper For Android Il2Cpp Games

# PLEASE READ
Don't create an issue saying "It didn't get every assembly" or "It didn't dump everything"
Some games rename their symbols such as il2cpp_domain_get_assemblies
This dumper has an automatic work-around to this, however it needs to be configured properly
If you really want it to work everytime, stop being lazy and find the renamed symbol yourself before complaining to me

# Features
Thread Check Detection

Easily Configurable

# Loading The Library
## With Modloader
Just put the file in the nativemods folder

## Without Modloader
1. Decompile the apk
2. Put the libLivkuDumper.so in the libs/arm64-v8a folder
3. Open smali\com\unity3d\player\UnityPlayerActivity.smali
4. In OnCreate add this
```
const-string v0, "LivkuDumper"

invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```
It should look something like this

![image](https://github.com/user-attachments/assets/9a98c40a-5712-4184-a40e-b4d2dec291aa)

5. Recompile the apk
6. Run the game and the dump.cs file will be generated in Android/data/packageName/dump.cs

# Config
If you want, change the CallCountType variable in the Config namespace

Change the callsBeforeLoad variable in the Globals namespace
