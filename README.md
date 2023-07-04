# Adobe Animate Test
This repository contains test native addons for Adobe Animate
You can read more about creating native addons [**here**](https://github.com/AdobeDocs/developers-animatesdk-docs/blob/master/C-Level_Extensibility/About_extensibility.md).


## Addons list

| Name | Description | Downloads |
| - | - | - |
| [**animate11**](https://github.com/DeMineArchiver/animate-test-addons/blob/master/animate11/README.md) <sup>new</sup> | <q>make Animate ***trully* immersive**!</q><br>This addon colors **all titlebars** in Adobe Animate the same as their windows | - [**0.1.0** <sup>latest</sup>](https://github.com/DeMineArchiver/animate-test-addons/releases/tag/animate11-v0.1.0 "animate11-v0.1.0")<br>- [**0.0.1**](https://github.com/DeMineArchiver/animate-test-addons/releases/tag/animate11-v0.0.1 "animate11-v0.0.1") |
| [**AltBlocker**](https://github.com/DeMineArchiver/animate-test-addons/blob/master/altblocker/README.md) <sup>new</sup> | <q>No more **alt**!</q><br>Prevents <kbd>⎇ Alt</kbd> from **blocking** keyboard input in **Adobe Animate** | - [**0.0.1** <sup>latest</sup>](https://github.com/DeMineArchiver/animate-test-addons/releases/tag/altblocker-v0.0.1 "altblocker-v0.0.1")
| [**Qt in Animate**](https://github.com/DeMineArchiver/animate-test-addons/blob/master/qt-in-animate/README.md) <sup>new</sup> | Proof of concept: use **Qt** in an **Animate Native Addon** | **Coming soon** |


## Installation
In this section there are common **installation instructions** for **most** of the addons above.
> **Before** installing an addon, check its respective page for **installation remarks**.

### 1. Download the addon
**Download** the file ending with `.dll` from the [**releases**](https:// "Releases") page.
### 2. Open the addons folder

The **addons installation** directory is
```
%LocalAppData%/Adobe/Animate <version>/<locale>/Configuration/External Libraries/
```

- `<version>` is your Animate version, e.g. `2023`
- `locale` is the language of your Animate, typically `en_US`

To quickly navigate to this directory, you can press <kbd><kbd>❖ Win</kbd>+<kbd>R</kbd></kbd> and copy the text there

### 3. Install the addon

**Copy** the file you **downloaded** into the **addons folder**.

### 4. Restart Animate

If Adobe Animate was **open** during installation, **restart** it.

## Uninstallation

### 1. Close Animate

If Adobe Animate is **running**, **close it** to **unlock** the addon files.

### 2. Uninstall the addon
Remove the addon's respective `.dll` file from the addons installation folder, i.e.
```
%LocalAppData%/Adobe/Animate <version>/<locale>/Configuration/External Libraries/
```
