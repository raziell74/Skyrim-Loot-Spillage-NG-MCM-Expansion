# Loot Spillage NG - MCM Expansion - Change Log

## Version 1.0

- New Features
    - MCM in game setting configuration
    - New setting 'AllowItemKeywordExclusion' prevents items with the keyword 'DisableLootDrop' from being dropped. I recommend using (KID)[https://www.nexusmods.com/skyrimspecialedition/mods/55728] to apply the keyword to any items you don't want to drop.
    - New setting 'AllowActorKeywordExclusion' prevents actors from dropping any loot if they have the 'DisableLootDrop' keyword. I recommend using (SPID)[https://www.nexusmods.com/skyrimspecialedition/mods/36869] to apply the keyword to any actors you don't want to drop loot.
    - Loot will now have a slightly randomized height offset when dropping. Better simulates a "Popcorn" effect when dropping loot.
    - Shader colors can now be configured in the MCM
    - New shader application controls: Disabled, Fade after duration, and Never Fade
    - Drop limits. You can now configure a limit to the number of items dropped per actor.
    - Drop Quest Items. Quest Items can now be configured to be dropped. This is disabled by default.
    - Actor Loot Drop filters. You can now configure Loot Spillage to drop loot only for actors killed by the player or a follower. This should help cut down on the number of item references introduced by Loot Spillage.
- Bugs Squashed
    - Updated the shader application delay to correctly use the ApplyDelay configuration setting
    - Shaders were not being applied if the dropped item's 3D model was not loaded by the time the shader delay finished. It will now attempt multiple times to apply the shader until the 3D model is loaded. Fixes for 99% of cases where the shader was not being applied.
    - Fixed a bug where items with blank names would still go through the item drop process even though they are not valid.
    - Unplayable items would still be dropped if the DropAll setting was enabled. The DropAll setting will now only override DropWeapons, DropArmor, and DropOther
    - Rewrote the DropOther logic to use a FormType whitelist instead of dropping anything that's not weapons or armor. This should prevent any CTDs caused by having invalid form types in the inventory, which shouldn't happen but this is God Howard's world... we just live in it.
    - The item handle from actor->RemoveItem handle now has additional validation to ensure we're not attempting to drop a null item handle
    - Added a debug message to the log right before an item is dropped to help identify any additional issues with the item drop process. If you experience CTD related to LootSpillage.dll please send me your log file so I can investigate.
    - Fixed a CTD caused by NPCs prematurely spilling their lootage before they were dead. The DeathEvent used to track actor deaths triggers twice, once when the actor is dying and once when they have finished dying. This will now only trigger once when the actor has finished dying so that spilling loot during certain death animations won't cause a CTD. 
- Optional Patches
    - Prevent underwear loot drops. Very helpful if you are using an underwear mod that dynamically adds underwear to NPCs. Aura's (Object Categorization Framework)[https://www.nexusmods.com/skyrimspecialedition/mods/81469] and (KID)[https://www.nexusmods.com/skyrimspecialedition/mods/55728] are required for this patch to work. ***NOTE:*** *If you don't already have (OCF)[https://www.nexusmods.com/skyrimspecialedition/mods/81469] installed, a new game is recommended.*
    - Animal Exclusion Patch. This patch is for anyone having issues with creatures still dropping loot. The issue typically arises due to some mods like 'SkyTEST Creatures' removing animals from the CreatureFaction. The CreatureFaction is what Loot Spillage uses to identify creatures for the 'CreatureDrops' setting. This patch uses SPID to apply the new 'DisableLootDrop' keyword to all animals. This patch requires (SPID)[https://www.nexusmods.com/skyrimspecialedition/mods/36869] to work.
    - Bandolier - Bags and Pouches Patch. Some of the models used in Bandolier - Bags and Pouches cause CTDs when shaders are applied. I'm not sure why, but this patch will prevent the items from this mod from dropping. This patch requires (KID)[https://www.nexusmods.com/skyrimspecialedition/mods/55728] to work. 

## Plans

- [-] Item reference cleanup ***In Progress***
    - [ ] Keep track of dropped item references - and the actor reference they dropped from
        - There is now a formlist which contains all the uncleaned loot references.
    - Trigger cleanup on cell change
        - Add duration? It might be a good idea not to immediately move the item in case the player is moving between cells quickly. Then after a set duration, run the clean up process on them.
    - Clean Up Method: The dropped items will be moved to a container in a custom cell
        - Dropped loot will be moved to a container in a custom cell.
        - Containers allow you to set a reset time. So the clean up of items can be easily done using the games built in reset system.
        - Quest items and Unique items will have their own FormList and under go the normal clean up process.
            - In case of an emergency, the MCM will have a button to access these items.
            - When the button is used it will move any items in that quest/unique formlist to a container that will not reset and open it for the player to retrieve their items.
            - There will be a script on the container that will not allow the player to store items in it. To prevent abuse of the system.
    - Add manual clean up button to MCM
- [ ] Add additional Debug logs that can be toggled on and off in the Maintenance section of the MCM
- [ ] Push updates to new Fork
- [ ] Additional drop settings for each "Other" form type
    - [ ] Books
    - [ ] Scrolls
    - [ ] Ingredients
    - [ ] Potions
    - [ ] Food
    - [ ] Keys
    - [ ] Soul Gems
    - [ ] Misc
- [ ] New unique shader colors. Keywords will be used to apply the new shader colors, and will be applied using KID.
  - Rare - Default Blue
  - Legendary - Default Orange
  - Unique - Default Teal

## Brainstorming

**Shader Light Columns** Look into using the "Addon Models" Effect Shader property to determine if it's possible to have some kind of "light column" mesh apply to the item when it's dropped. This would be a nice visual indicator that the item is being dropped. Currently even with the shader applied, it can be difficult to see the items if there's tall grass or other clutter around. 
*Update* Turns out Addon Models are part of the particle system for Effect Shaders and they do not work unless attached to an "Actor" reference. This means they cannot be used for this purpose.

**Loot Drop conditions** Some have requested to have loot only drop when actor is killed by a power attack. I'm not sure this is worth the effort to implement, but it's something to consider. Another condition that would be easier to implement would be to only drop loot from "Boss" or Unique actors.