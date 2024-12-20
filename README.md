LegaC Launcher
===============

A Minecraft launcher made with GTK and Curl in pure C.

The main goal is to provide the old looking style of the Minecraft launcher,
while maintaining new features that modern launchers have.

Usage
-----

To install it is really simple. Clone, enter, make, run, rage.
```shell
git clone https://gitlab.com/SimplyCEO/LegaC-Launcher.git
cd LegaC-Launcher
make BUILD_TYPE=Release
bin/legac_launcher
```
Or just open `legac_launcher` located inside `bin` folder.

To avoid overwritting your `CFLAGS` use `make BUILD_TYPE=Custom`.

Installation
------------

Copy the contents of the `assets` folder to the local launcher folder:
```shell
mkdir -p "${HOME}/.local/share/legac_launcher"
cp -r ./assets "${HOME}/.local/share/legac_launcher"
```

<details><summary>Local installation</summary>

To install only for the local user, the `PATH` environment variable need to be set to the specific path
which will be assumed it is under the `HOME` directory.

Exporting the `PATH` environment variable is easy, just need a few steps:
```shell
export PATH="${HOME}/.local/bin:${PATH}"
cp bin/legac_launcher "${HOME}/.local/bin"
```

Additionaly, the desktop file for Linux devices can be accessed through the menu and/or desktop:
```shell
cp assets/icon.png "${HOME}/.icons/legac_launcher_icon.png"
cp LegaC_Launcher.desktop "${HOME}/.local/share/applications"
cp LegaC_Launcher.desktop "${HOME}/Desktop"
```

Remember to add the binary path to the desktop file:
```desktop
Path=/home/username/.local/bin
Exec=./legac_launcher
```

</details>

<details><summary>System-wide installation</summary>

To install system-wide, the `/usr/local/bin` directory can be used:
```shell
cp bin/legac_launcher "/usr/.local/bin"
```

Additionaly, the desktop file for Linux devices can be accessed through the menu and/or desktop:
```shell
cp assets/icon.png "/usr/share/icons/legac_launcher_icon.png"
cp LegaC_Launcher.desktop "/usr/share/applications"
```

At the moment it is not recommended to install on system. Just run locally until the stable version comes out.

</details>

TODO
----

There are nothing much dynamic at this point, since the launcher was tested
for a specific version.

Why? Because it took me almost an entire week to understand how all of this works.

The initial code given here works as intended, but not how it should be... there
WILL be more work to do.

This is the first open source Minecraft launcher available in C.
And hopefully, at the end of the development version, occur a stable in ANSI C.

Gallery
-------

<p align="left">
  <img src="./.media/screenshots/SCR_2024-12-14_22-15-01.jpeg" width="384" title="LegaC Launcher v0.0.4">
</p>

