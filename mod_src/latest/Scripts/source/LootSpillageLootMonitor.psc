ScriptName LootSpillageLootMonitor extends ActiveMagicEffect

Actor property PlayerRef auto
FormList Property SpilledLootList Auto

;--- Events ------------------------------------------------------------

Event OnItemAdded(Form akBaseItem, int aiItemCount, ObjectReference akItemReference, ObjectReference akSourceContainer)
    if !akSourceContainer
        Debug.Trace("I picked up " + aiItemCount + "x " + akBaseItem + " from the world")
        MiscUtil.PrintConsole("I picked up " + aiItemCount + "x " + akBaseItem + " from the world")
    elseif akSourceContainer == Game.GetPlayer()
        Debug.Trace("The player gave me " + aiItemCount + "x " + akBaseItem)
        MiscUtil.PrintConsole("The player gave me " + aiItemCount + "x " + akBaseItem)
    else
        Debug.Trace("I got " + aiItemCount + "x " + akBaseItem + " from another container")
        MiscUtil.PrintConsole("I got " + aiItemCount + "x " + akBaseItem + " from another container")
    endIf
    
    ; Check if this is a reference to spilled loot
    If !akSourceContainer
        If akItemReference && SpilledLootList.HasForm(akItemReference)
            Debug.Trace("Reference is spilled loot - Removing from tracking list")
            MiscUtil.PrintConsole("Loot Spillage: Reference is spilled loot - Removing from tracking list")
            SpilledLootList.RemoveAddedForm(akItemReference)
        ElseIf akBaseItem && SpilledLootList.HasForm(akBaseItem) ; If the item is not a refr, check for the base form within the list
            Debug.Trace("Base form is spilled loot - Removing from tracking list")
            MiscUtil.PrintConsole("Loot Spillage: Base form is spilled loot - Removing from tracking list")
            SpilledLootList.RemoveAddedForm(akBaseItem)
        EndIf
    EndIf
EndEvent
