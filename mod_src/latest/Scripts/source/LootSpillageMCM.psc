ScriptName LootSpillageMCM Extends MCM_ConfigBase

Function UpdateLootSpillageSettings() global native

;--- Properties -----------------------------------------------------------
GlobalVariable Property DropAll Auto
GlobalVariable Property DropArmor Auto
GlobalVariable Property DropWeapons Auto
GlobalVariable Property DropOther Auto
GlobalVariable Property DropLimit Auto
GlobalVariable Property DropUnplayable Auto
GlobalVariable Property DropQuest Auto
GlobalVariable Property AllowItemKeywordExclusion Auto

GlobalVariable Property CreatureDrops Auto
GlobalVariable Property AllowActorKeywordExclusion Auto
GlobalVariable Property FilterByPlayerKills Auto
GlobalVariable Property FilterByFollowerKills Auto

GlobalVariable Property ApplyDelay Auto
GlobalVariable Property VfxDuration Auto
GlobalVariable Property VfxFallOff Auto

GlobalVariable Property BaseColor Auto
GlobalVariable Property WeaponColor Auto
GlobalVariable Property ArmorColor Auto
GlobalVariable Property ConsumableColor Auto
GlobalVariable Property ValuableColor Auto

;--- Private Variables ----------------------------------------------------
Bool migrated = False
String plugin = "LootSpillageMCM.esp"
Actor player

;--- Functions ------------------------------------------------------------

; Returns version of this script.
Int Function GetVersion()
    return 1 ;MCM Helper
EndFunction

Event OnVersionUpdate(int aVersion)
	parent.OnVersionUpdate(aVersion)
    MigrateToMCMHelper()
    VerboseMessage("OnVersionUpdate: MCM Updated For Version " + aVersion)
    RefreshMenu()
EndEvent

; Event called periodically if the active magic effect/alias/form is registered for update events. This event will not be sent if the game is in menu mode. 
Event OnUpdate()
    parent.OnUpdate()
    If !migrated
        MigrateToMCMHelper()
        migrated = True
        VerboseMessage("OnUpdate: Settings imported!")
    EndIf
EndEvent

; Called when game is reloaded.
Event OnGameReload()
    parent.OnGameReload()
    If !migrated
        MigrateToMCMHelper()
        migrated = True
        VerboseMessage("OnGameReload: Settings imported!")
    EndIf
    If GetModSettingBool("bLoadSettingsonReload:Maintenance")
        LoadSettings()
        VerboseMessage("OnGameReload: Settings autoloaded!")
    EndIf

    UpdateLootSpillageSettings()
EndEvent

; Called when this config menu is opened.
Event OnConfigOpen()
    parent.OnConfigOpen()
    If !migrated
        MigrateToMCMHelper()
        migrated = True
        VerboseMessage("OnConfigOpen: Settings imported!")
    EndIf
EndEvent

; Called when a new page is selected, including the initial empty page.
Event OnPageSelect(String a_page)
    parent.OnPageSelect(a_page)
    RefreshMenu()
EndEvent

; Called when this config menu is initialized.
Event OnConfigInit()
    parent.OnConfigInit()
    migrated = True
    LoadSettings()
EndEvent

; Called when setting changed to different value.
Event OnSettingChange(String a_ID)
    parent.OnSettingChange(a_ID)

    ; Drops
    If a_ID == "bDropAll:Drops"
        DropAll.SetValue(GetModSettingBool("bDropAll:Drops") as Float)
        RefreshMenu()
    ElseIf a_ID == "bDropArmor:Drops"
        DropArmor.SetValue(GetModSettingBool("bDropArmor:Drops") as Float)
    ElseIf a_ID == "bDropWeapons:Drops"
        DropWeapons.SetValue(GetModSettingBool("bDropWeapons:Drops") as Float)
    ElseIf a_ID == "bDropOther:Drops"
        DropOther.SetValue(GetModSettingBool("bDropOther:Drops") as Float)
    ElseIf a_ID == "iDropLimit:Drops"
        DropLimit.SetValue(GetModSettingInt("iDropLimit:Drops") as Float)
    ElseIf a_ID == "bDropUnplayable:Drops"
        DropUnplayable.SetValue(GetModSettingBool("bDropUnplayable:Drops") as Float)
    ElseIf a_ID == "bDropQuest:Drops"
        DropQuest.SetValue(GetModSettingBool("bDropQuest:Drops") as Float)
    ElseIf a_ID == "bAllowItemKeywordExclusion:Drops"
        AllowItemKeywordExclusion.SetValue(GetModSettingBool("bAllowItemKeywordExclusion:Drops") as Float)
    
    ; Actors
    ElseIf a_ID == "iFilterKills:Actors"
        Int filterKills = GetModSettingInt("iFilterKills:Actors")
        If filterKills == 0 ; No Filter
            FilterByPlayerKills.SetValue(0)
            FilterByFollowerKills.SetValue(0)
        ElseIf filterKills == 1 ; Player Kills
            FilterByPlayerKills.SetValue(1)
            FilterByFollowerKills.SetValue(0)
        ElseIf filterKills == 2 ; Player and Follower Kills
            FilterByPlayerKills.SetValue(1)
            FilterByFollowerKills.SetValue(1)
        EndIf
    ElseIf a_ID == "bCreatureDrops:Actors"
        CreatureDrops.SetValue(GetModSettingBool("bCreatureDrops:Actors") as Float)
    ElseIf a_ID == "bAllowActorKeywordExclusion:Actors"
        AllowActorKeywordExclusion.SetValue(GetModSettingBool("bAllowActorKeywordExclusion:Actors") as Float)
    
    ; Shaders
    ElseIf a_ID == "iVfxControl:Shaders"
        int vfxControl = GetModSettingInt("iVfxControl:Shaders")
        If vfxControl == 0 ; Disable
            SetModSettingInt("iShowVfxDuration:Shaders", 0)
            VfxDuration.SetValue(0.0)
        ElseIf vfxControl == 1 ; Fade After Duration
            SetModSettingInt("iShowVfxDuration:Shaders", 1)
            VfxDuration.SetValue(GetModSettingFloat("fVfxDuration:Shaders") as Float)
        ElseIf vfxControl == 2 ; Never Fade
            SetModSettingInt("iShowVfxDuration:Shaders", 0)
            VfxDuration.SetValue(-1.0)
        EndIf
        RefreshMenu()
    ElseIf a_ID == "iApplyDelay:Shaders"
        ApplyDelay.SetValue(GetModSettingInt("iApplyDelay:Shaders") as Float)
    ElseIf a_ID == "fVfxDuration:Shaders"
        SetModSettingInt("iVfxControl:Shaders", 1)
        VfxDuration.SetValue(GetModSettingFloat("fVfxDuration:Shaders") as Float)
        RefreshMenu()
    ElseIf a_ID == "fVfxFallOff:Shaders"
        VfxFallOff.SetValue(GetModSettingFloat("fVfxFallOff:Shaders") as Float)

    ; Shader Colors
    ElseIf a_ID == "iBaseColor:Shaders"
        ;Debug.MessageBox("Color Test - " + GetModSettingInt("iBaseColor:Shaders"))
        BaseColor.SetValue(GetModSettingInt("iBaseColor:Shaders") as Float)
        MiscUtil.PrintConsole("Loot Spillage: BaseColor ModSetting: " + GetModSettingInt("iBaseColor:Shaders"))
        MiscUtil.PrintConsole("Loot Spillage: BaseColor Global: " + BaseColor.GetValue() as Int)
    ElseIf a_ID == "iWeaponColor:Shaders"
        ;Debug.MessageBox("Color Test - " + GetModSettingInt("iWeaponColor:Shaders"))
        WeaponColor.SetValue(GetModSettingInt("iWeaponColor:Shaders") as Float)
    ElseIf a_ID == "iArmorColor:Shaders"
        ;Debug.MessageBox("Color Test - " + GetModSettingInt("iArmorColor:Shaders"))
        ArmorColor.SetValue(GetModSettingInt("iArmorColor:Shaders") as Float)
    ElseIf a_ID == "iConsumableColor:Shaders"
        ;Debug.MessageBox("Color Test - " + GetModSettingInt("iConsumableColor:Shaders"))
        ConsumableColor.SetValue(GetModSettingInt("iConsumableColor:Shaders") as Float)
    ElseIf a_ID == "iValuableColor:Shaders"
        ;Debug.MessageBox("Color Test - " + GetModSettingInt("iValuableColor:Shaders"))
        ValuableColor.SetValue(GetModSettingInt("iValuableColor:Shaders") as Float)
    EndIf

    UpdateLootSpillageSettings()
EndEvent

Function Default()
    SetModSettingBool("bDropAll:Drops", False)
    SetModSettingBool("bDropArmor:Drops", True)
    SetModSettingBool("bDropWeapons:Drops", True)
    SetModSettingBool("bDropOther:Drops", True)
    SetModSettingInt("iDropLimit:Drops", 0)
    SetModSettingBool("bDropUnplayable:Drops", False)
    SetModSettingBool("bDropQuest:Drops", False)
    SetModSettingBool("bAllowItemKeywordExclusion:Drops", True)

    SetModSettingInt("iFilterKills:Actors", 2)
    SetModSettingBool("bCreatureDrops:Actors", False)
    SetModSettingBool("bAllowActorKeywordExclusion:Actors", True)

    SetModSettingInt("iVfxControl:Shaders", 1)
    SetModSettingInt("iShowVfxDuration:Shaders", 1)
    SetModSettingInt("iApplyDelay:Shaders", 300)
    SetModSettingFloat("fVfxDuration:Shaders", 15.0)
    SetModSettingFloat("fVfxFallOff:Shaders", 3.0)

    SetModSettingInt("iBaseColor:Shaders", 0xffffff)
    SetModSettingInt("iWeaponColor:Shaders", 0xCC0033)
    SetModSettingInt("iArmorColor:Shaders", 0x4F2D7F)
    SetModSettingInt("iConsumableColor:Shaders", 0x008542)
    SetModSettingInt("iValuableColor:Shaders", 0xEAAB00)
    
    SetModSettingBool("bEnabled:Maintenance", True)
    SetModSettingInt("iLoadingDelay:Maintenance", 0)
    SetModSettingBool("bLoadSettingsonReload:Maintenance", False)
    SetModSettingBool("bVerbose:Maintenance", False)
    VerboseMessage("Settings reset!")
    Load()
EndFunction

Function Load()
    DropAll.SetValue(GetModSettingBool("bDropAll:Drops") as Float)
    DropArmor.SetValue(GetModSettingBool("bDropArmor:Drops") as Float)
    DropWeapons.SetValue(GetModSettingBool("bDropWeapons:Drops") as Float)
    DropOther.SetValue(GetModSettingBool("bDropOther:Drops") as Float)
    DropLimit.SetValue(GetModSettingInt("iDropLimit:Drops") as Float)
    DropUnplayable.SetValue(GetModSettingBool("bDropUnplayable:Drops") as Float)
    DropQuest.SetValue(GetModSettingBool("bDropQuest:Drops") as Float)
    AllowItemKeywordExclusion.SetValue(GetModSettingBool("bAllowItemKeywordExclusion:Drops") as Float)

    Int filterKills = GetModSettingInt("iFilterKills:Actors")
    If filterKills == 0 ; No Filter
        FilterByPlayerKills.SetValue(0)
        FilterByFollowerKills.SetValue(0)
    ElseIf filterKills == 1 ; Player Kills
        FilterByPlayerKills.SetValue(1)
        FilterByFollowerKills.SetValue(0)
    ElseIf filterKills == 2 ; Player and Follower Kills
        FilterByPlayerKills.SetValue(1)
        FilterByFollowerKills.SetValue(1)
    EndIf
    CreatureDrops.SetValue(GetModSettingBool("bCreatureDrops:Actors") as Float)
    AllowActorKeywordExclusion.SetValue(GetModSettingBool("bAllowActorKeywordExclusion:Actors") as Float)

    ApplyDelay.SetValue(GetModSettingInt("iApplyDelay:Shaders") as Float)
    int vfxControl = GetModSettingInt("iVfxControl:Shaders")
    If vfxControl == 0 ; Disable
        VfxDuration.SetValue(0.0)
    ElseIf vfxControl == 1 ; Fade After Duration
        VfxDuration.SetValue(GetModSettingFloat("fVfxDuration:Shaders") as Float)
    ElseIf vfxControl == 2 ; Never Fade
        VfxDuration.SetValue(-1.0)
    EndIf
    VfxFallOff.SetValue(GetModSettingFloat("fVfxFallOff:Shaders") as Float)

    BaseColor.SetValue(GetModSettingInt("iBaseColor:Shaders") as Float)
    WeaponColor.SetValue(GetModSettingInt("iWeaponColor:Shaders") as Float)
    ArmorColor.SetValue(GetModSettingInt("iArmorColor:Shaders") as Float)
    ConsumableColor.SetValue(GetModSettingInt("iConsumableColor:Shaders") as Float)
    ValuableColor.SetValue(GetModSettingInt("iValuableColor:Shaders") as Float)

    UpdateLootSpillageSettings()
    VerboseMessage("Settings applied!")

    ; Testing to see if this fixes the mod displaying incorrect colors after game reload
    MiscUtil.PrintConsole("Loot Spillage: BaseColor ModSetting: " + GetModSettingInt("iBaseColor:Shaders"))
    MiscUtil.PrintConsole("Loot Spillage: BaseColor Global: " + BaseColor.GetValue() as Int)
EndFunction

Function LoadSettings()
    If GetModSettingBool("bEnabled:Maintenance") == false
        return
    EndIf
    Utility.Wait(GetModSettingInt("iLoadingDelay:Maintenance"))
    Load()
    VerboseMessage("Settings autoloaded!")
EndFunction

Function MigrateToMCMHelper()
    ; Migrating to MCM Helper
    SetModSettingBool("bDropAll:Drops", DropAll.GetValue() as Bool)
    SetModSettingBool("bDropArmor:Drops", DropArmor.GetValue() as Bool)
    SetModSettingBool("bDropWeapons:Drops", DropWeapons.GetValue() as Bool)
    SetModSettingBool("bDropOther:Drops", DropOther.GetValue() as Bool)
    SetModSettingInt("iDropLimit:Drops", DropLimit.GetValue() as Int)
    SetModSettingBool("bDropUnplayable:Drops", DropUnplayable.GetValue() as Bool)
    SetModSettingBool("bDropQuest:Drops", DropQuest.GetValue() as Bool)
    SetModSettingBool("bAllowItemKeywordExclusion:Drops", AllowItemKeywordExclusion.GetValue() as Bool)

    Int FilterKillsValue = 0
    If FilterByPlayerKills.GetValue() as Bool
        FilterKillsValue = 1
        If FilterByFollowerKills.GetValue() as Bool
            FilterKillsValue = 2
        EndIf
    EndIf
    SetModSettingInt("iFilterKills:Actors", FilterKillsValue)
    SetModSettingBool("bCreatureDrops:Actors", CreatureDrops.GetValue() as Bool)
    SetModSettingBool("bAllowActorKeywordExclusion:Actors", AllowActorKeywordExclusion.GetValue() as Bool)

    SetModSettingInt("iVfxControl:Shaders", 1)
    SetModSettingInt("iShowVfxDuration:Shaders", 1)
    SetModSettingInt("iApplyDelay:Shaders", ApplyDelay.GetValue() as Int)
    SetModSettingFloat("fVfxDuration:Shaders", VfxDuration.GetValue())
    SetModSettingFloat("fVfxFallOff:Shaders", VfxFallOff.GetValue())

    SetModSettingInt("iBaseColor:Shaders", BaseColor.GetValue() as Int)
    SetModSettingInt("iWeaponColor:Shaders", WeaponColor.GetValue() as Int)
    SetModSettingInt("iArmorColor:Shaders", ArmorColor.GetValue() as Int)
    SetModSettingInt("iConsumableColor:Shaders", ConsumableColor.GetValue() as Int)
    SetModSettingInt("iValuableColor:Shaders", ValuableColor.GetValue() as Int)

    UpdateLootSpillageSettings()
EndFunction

Function VerboseMessage(String m)
    Debug.Trace("[Loot Spillage] " + m)
    If GetModSettingBool("bVerbose:Maintenance")
        Debug.Notification("[Loot Spillage] " + m)
    EndIf
EndFunction
