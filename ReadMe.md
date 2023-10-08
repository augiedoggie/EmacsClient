# EmacsClient

A small helper application for running Emacs on [Haiku](https://haiku-os.org).  This allows setting Emacs as the default handler for certain file types and opening them directly from Tracker or other apps.


## Building

- Using cmake.  This is the primary build system.
```
~> cd EmacsClient
~/EmacsClient> cmake .
~/EmacsClient> make
```

- Using jam.  You must have the `jamfile_engine` package installed.
```
~> cd EmacsClient
~/EmacsClient> jam
```
