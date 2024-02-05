![Loot Spillage - MCM Expansion](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/lsme_header.png)

Expands on Monitor144hz's amazing Loot Spillage mod by adding some new features and MCM configuration.

A **HUGE** thank you to Monitor144hz for all his initial work on Loot Spillage, and making the source code public allowing me to expand on it.

You're awesome Monitor!

![New Features](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/lsme_new_features_header.png)

*Loot Spillage NG - MCM Expansion* adds a few new features to the original Loot Spillage mod.

## MCM

In game Loot Spillage configuration. There is now an MCM menu that allows you to configure Loot Spillage settings when ever you want.
No more opening and closing the game to tweak ini settings.

![LootSpillage Splash](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/loot_spillage_splash.jpg)

## Keyword Exclusion

Items and Actors can now be excluded from dropping loot by applying the `DisableLootDrop` keyword to them. This keyword can be applied using [KID](https://www.nexusmods.com/skyrimspecialedition/mods/55728) for items and [SPID](https://www.nexusmods.com/skyrimspecialedition/mods/36869) for actors. Settings to toggle keyword exclusion for items and actors is available in the MCM.

I've included a few optional patches that use this feature. See the Optional Patches section for more details.

## Additional Shader Control

The MCM expansion adds additional control settings for item shaders.

- Completely disable item shading
- Fade after a set duration (default behavior)
- Never fade. Keep item shaders active until the item is picked up or the player leaves the cell.

![LootSpillage Shader Colors](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/loot_spillage_shader_settings.jpg)

## Shader Color Configuration

You can now configure the shader colors for each item type. The default colors are the same as the original Loot Spillage mod.

I plan to add 2 new shader colors for Unique and Legendary items. These colors will be assigned using keywords applied through [KID](https://www.nexusmods.com/skyrimspecialedition/mods/55728).

![LootSpillage Shader Colors](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/loot_spillage_shader_colors.jpg)

## Drop Limits

Limit the amount of items dropped per actor. If too many loot items are dropped all at once it can cause a CTD or large FPS drops depending on your setup. This should also help limit the number of item references introduced by Loot Spillage.

The default drop limit is set to 15 but can be adjusted in the MCM. Drop Limit is ignored If *DropAll* is set to true.

![LootSpillage Shader Colors](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/loot_spillage_drop_settings.jpg)

## Quest Item Drop Control

Quest Items can now be configured to be dropped or remain on the actors body. This feature is helpful in some cases where a quest item is required to be looted from a body to progress a quest like the Heads introduced by [Headhunter - Bounties Redone](https://www.nexusmods.com/skyrimspecialedition/mods/51847).

Quest Item drops are disabled by default.

## Actor Loot Drop Filters

In addition to the original `CreatureDrops` setting, you can now configure Loot Drops to only occur for actors killed by the player or a follower. This should help cut down on the number of item references introduced by Loot Spillage, specially in cases where a battle is happening in the same cell and actors are dying without the player being involved.

These filters are enabled by default, but can be toggled in the MCM.

![LootSpillage Shader Colors](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/loot_spillage_drop_filter.jpg)

![Bugs Squashed](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/lsme_bugs_squashed_header.png)

In addition to some of the new features, MCM Expansion also fixes up a few bugs.

- Fixed a bug where items with blank names would still go through the item drop process even though they are not valid.
- Unplayable items would still be dropped if the `DropAll` setting was enabled. The `DropAll` setting will now only override `DropWeapons`, `DropArmor`, and `DropOther`
- In some cases NPC inventory would include an item that is not a "droppable" FormType which would cause a CTD. To fix this, the logic for `DropOther` has been updated to white list supported FormTypes instead of dropping anything that's not weapons or armor.
- Added a few more checks for item reference validity after being removed from the actors inventory. In some cases changing cells would sometimes cause a CTD. This should fix that.
- Fixed a CTD caused by NPCs prematurely spilling their lootage before they were dead dead. The DeathEvent used to track actor deaths triggers twice, once when the actor is dying and once when they have finished dying. This will now only trigger once when the actor has finished dying so that spilling loot during certain death animations won't cause a CTD.
- New log messages have been added to help identify any additional issues with the item drop process. If you experience CTD related to LootSpillage.dll please send me your log file so I can investigate. I'll be looking at adding a *Debug Mode* setting to the MCM in the future to prevent log spam unless you need it.

![Optional Patches](https://raw.githubusercontent.com/raziell74/Skyrim-Loot-Spillage-NG-MCM-Expansion/main/mod_src/screenshots/lsme_optional_patches_header.png)

## Creature Exclusion Patch

This patch is for anyone having issues with creatures still dropping loot. The issue typically arises due to some mods like [SkyTEST - Realistic Animals and Predators SE](https://www.nexusmods.com/skyrimspecialedition/mods/1104) removing animals from the CreatureFaction. The CreatureFaction is what Loot Spillage uses to identify creatures for the 'CreatureDrops' setting. This patch uses [SPID](https://www.nexusmods.com/skyrimspecialedition/mods/36869) to apply the new `DisableLootDrop` keyword to all animal races. 

Requires [SPID](https://www.nexusmods.com/skyrimspecialedition/mods/36869) to work.

## Bandolier - Bags and Pouches Patch

Some of the models used in [Bandolier - Bags and Pouches](https://www.nexusmods.com/skyrimspecialedition/mods/3533) cause CTDs when shaders are applied. I'm not sure why, but this patch will prevent the bad items from dropping using the new `DisableLootDrop` keyword.

Requires [KID](https://www.nexusmods.com/skyrimspecialedition/mods/55728) to work.

## Underwear Exclusion Patch

This patch will prevent underwear from being dropped. Very helpful if you are using an underwear mod that dynamically adds underwear to NPCs like [NPC Underwear Distribution Engine](https://www.nexusmods.com/skyrimspecialedition/mods/94018) or [Equippable Underwear for NPCs](https://www.nexusmods.com/skyrimspecialedition/mods/45277). Dead actors will now longer be showing their bits until you manually take their no no cloth.

Requires [OCF](https://www.nexusmods.com/skyrimspecialedition/mods/81469) and [KID](https://www.nexusmods.com/skyrimspecialedition/mods/55728) to work.

***NOTE:*** If you don't already have [OCF](https://www.nexusmods.com/skyrimspecialedition/mods/81469) installed, a new game is recommended.

# Project Setup with CommonLibSSE NG

Because this uses [CommonLibSSE NG](https://github.com/CharmedBaryon/CommonLibSSE-NG), it supports Skyrim SE, AE, GOG, and VR. 

Hook IDs and offsets must still be found manually for each version.

# Requirements

- [Visual Studio 2022](https://visualstudio.microsoft.com/) (_the free Community edition_)
- [`vcpkg`](https://github.com/microsoft/vcpkg)
  - 1. Clone the repository using git OR [download it as a .zip](https://github.com/microsoft/vcpkg/archive/refs/heads/master.zip)
  - 2. Go into the `vcpkg` folder and double-click on `bootstrap-vcpkg.bat`
  - 3. Edit your system or user Environment Variables and add a new one:
    - Name: `VCPKG_ROOT`  
      Value: `C:\path\to\wherever\your\vcpkg\folder\is`

## Opening the project

Once you have Visual Studio 2022 installed, you can open this folder in basically any C++ editor, e.g. [VS Code](https://code.visualstudio.com/) or [CLion](https://www.jetbrains.com/clion/) or [Visual Studio](https://visualstudio.microsoft.com/)
- > _for VS Code, if you are not automatically prompted to install the [C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) and [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) extensions, please install those and then close VS Code and then open this project as a folder in VS Code_

You may need to click `OK` on a few windows, but the project should automatically run CMake!

It will _automatically_ download [CommonLibSSE NG](https://github.com/CharmedBaryon/CommonLibSSE-NG) and everything you need to get started making your new plugin!

# Project setup

By default, when this project compiles it will output a `.dll` for your SKSE plugin into the `build/` folder.

If you want to configure this project to output your plugin files
into your Skyrim Special Edition's "`Data`" folder:

- Set the `SKYRIM_FOLDER` environment variable to the path of your Skyrim installation  
  e.g. `C:\Program Files (x86)\Steam\steamapps\common\Skyrim Special Edition`

If you want to configure this project to output your plugin files
into your "`mods`" folder:  
(_for Mod Organizer 2 or Vortex_)

- Set the `SKYRIM_MODS_FOLDER` environment variable to the path of your mods folder:  
  e.g. `C:\Users\<user>\AppData\Local\ModOrganizer\Skyrim Special Edition\mods`  
  e.g. `C:\Users\<user>\AppData\Roaming\Vortex\skyrimse\mods`


