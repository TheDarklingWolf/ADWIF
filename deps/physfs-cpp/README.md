PhysFS++ is a C++ wrapper for the excellent [PhysicsFS library][1] by Ryan C.
Gordon and others.

[1]: http://icculus.org/physfs

It is licensed under the zlib license - same as PhysicsFS (at the time of 
writing).

The wrapper simply wraps most functions in a PhysFS namespace, and gives them 
C++ signatures (`std::string` rather than `const char *`).

Additionally:
 - Functions that are related to byte order conversions are placed in the 
PhysFS::Util namespace.
 - Instead of replicating `PHYSFS_openRead`, `PHYSFS_openWrite`, and 
`PHYSFS_openAppend`, files are opened as streams using `PhysFS::ifstream` and 
`PhysFS::ofstream`.
 - ofstream's constructor takes a mode, which specifies either append or write.
 - Both ifstream and ofstream are standard streams, and only have a couple of
extra methods - `eof` and `length`, which call `PHYSFS_eof` and 
`PHYSFS_fileLength` respectively.
