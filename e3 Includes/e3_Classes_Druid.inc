|----------------------------------------------------------------------------|
|- e3_Classes_Druid.inc v3.0 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Contains Druid specific functions related to,							-|
|-		Setup and Ini entry information										-|
|-		MainLoop, and AssistLoop hooks										-|
|-		Over-all functionality / flow										-|
|----------------------------------------------------------------------------|



Sub DRU_Setup
/if (${Debug}) /echo |- DRU_Setup ==>
	
	/call IniToVar "${Character_Ini},Druid,Evac Spell" Evac_Spell string outer
	
/if (${Debug}) /echo <== DRU_Setup -|
/return



Sub DRU_Background_Events

/return



SUB DRU_MacroSettings
/if (${Debug}) /echo |- DRU_MacroSettings ==>

	| Adv Settings
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#1" background 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#2" basics 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#3" casting 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#4" begForBuffs 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#5" buffCheck 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#6" buy 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#7" heal 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#8" assist 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#9" class 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#10" corpseSummon 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#11" loot 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#12" sell 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#13" smoothMoves 1
	/call WriteToIni "${advSettings_Ini},Druid Included Setups,Include#14" Wait4Rez 1
	/delay 15

	/call WriteToIni "${advSettings_Ini},DRU Functions,DRU Function#1" "check_Heals" 1
	/call WriteToIni "${advSettings_Ini},DRU Functions,DRU Function#2" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},DRU Functions,DRU Function#3" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},DRU Functions,DRU Function#4" "check_Debuffs" 1
	/call WriteToIni "${advSettings_Ini},DRU Functions,DRU Function#5" "check_DoTs" 1
	/call WriteToIni "${advSettings_Ini},DRU Functions,DRU Function#6" "check_Rain" 1
	/call WriteToIni "${advSettings_Ini},DRU Functions,DRU Function#7" "check_Nukes" 1
	/call WriteToIni "${advSettings_Ini},DRU Functions,DRU Function#8" "check_Buffs" 1

/if (${Debug}) /echo <== DRU_MacroSettings -|
/RETURN



SUB DRU_CharacterSettings
/if (${Debug}) /echo |- DRU_CharacterSettings ==>

	| Bot Ini
	/call WriteToIni "${Character_Ini},Druid,Evac Spell"

/if (${Debug}) /echo <== DRU_CharacterSettings -|
/RETURN



Sub DRU_Aliases
/if (${Debug}) /echo |- DRU_Aliases ==>

/if (${Debug}) /echo <== DRU_Aliases -|
/return

| --------------------------------------------------------------------------------------|
| -- Causes Druids/Wizards to port to the destination requested                        -|
| --------------------------------------------------------------------------------------|

#event portDestination "#1# tell your party, 'Port #2#'"
#event portDestination "#1# tells the group, 'Port #2#'"
#event portDestination "[#1#(msg)] Port to #2#"

SUB EVENT_portDestination(line, portRequester, shortDestination)
	
	/declare e3xUtilitiesDebug bool local FALSE

	| ---------------- Pause to wait for whatever is happening to end
	/delay 10s

	| ---------------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) /echo 200: shortDestination is ${shortDestination} in EVENT_portDestination

	| ---------------- Memorize and cast appropriate port spell
	/declare fullDestination string local
	/varset fullDestination Circle of ${shortDestination}

	| ---------------- Over-ride fullDestination if the destination is skyfire and Class.ShortName = DRU
	/if (${shortDestination.Equal[skyfire]} || ${shortDestination.Equal[sf]}) /varset fullDestination Wind of the North

	| ---------------- Over-ride fullDestination if the destination is ej and Class.ShortName = DRU
	/if (${shortDestination.Equal[ej]}) /varset fullDestination Wind of the South

	| ---------------- Over-ride fullDestination if the destination is dl and Class.ShortName = DRU
	/if (${shortDestination.Equal[dl]}) /varset fullDestination Circle of the Combines

	| ---------------- Over-ride fullDestination if the destination is sro and Class.ShortName = DRU
	/if (${shortDestination.Equal[sro]}) /varset fullDestination Circle of Ro

	| ---------------- Over-ride fullDestination if the destination is nexus and Class.ShortName = DRU
	/if (${shortDestination.Equal[nexus]}) /varset fullDestination Circle of the Nexus

			| ---------------- Over-ride fullDestination if the destination is tox and Class.ShortName = DRU
	/if (${shortDestination.Equal[tox]}) /varset fullDestination Circle of Toxxulia

	| ---------------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) /echo 210: Druid fullDestination is ${fullDestination} in EVENT_portDestination

	/if (${Me.Book[${fullDestination}]}) {
		/bc [+y+] Memorizing ${fullDestination} ...
		/memorize "${fullDestination}|gem8"
	} else {
		/bc [+r+] ${fullDestination} not found in my spellboook ...
	}
	
/return

| --------------------------------------------------------------------------------------|
| -- Casts shrink on anyone over 2.40 in size						                   -|
| --------------------------------------------------------------------------------------|

|SUB EVENT_shrinkGroup
#event druidShrinkGroup "<#*#> Shrink your group!"
SUB EVENT_druidShrinkGroup

	| ---------- If you don't have a shrink item, exit
	/if (!${FindItem[Bracelet of the Shadow Hive].ID}) /return

	| ---------- Cycle through all group members
	/declare i int local
	/for i 0 to ${Group}

		| ---------- Target the group member
		/target id ${Group.Member[${i}].ID}
		/delay 2s ${Target.ID}==${Group.Member[${i}].ID}

		| ---------- If the target is larger than 2.40, loop through shrinking them
		:shrinkLoop
		/if (${Target.Height} > 2.40) {
			/if (${FindItem[Bracelet of the Shadow Hive].ID}) /call e3_cast ${Group.Member[${i}].ID} "Bracelet of the Shadow Hive"
			/delay 5s !${Me.Casting.ID}
			/goto :shrinkLoop
		}
		
	/next i

/return

| --------------------------------------------------------------------------------------|
| -- Casts EB on anyone "DROWNING!"   								                   -|
| --------------------------------------------------------------------------------------|
|**
#event drowning "<#1#> DROWNING!"

SUB EVENT_drowning(line, drowningTarget)

	/if (!${Me.CleanName.Equal[Myride]}) /return

	/declare ebSpell string local Enduring Breath

	/call e3_cast ${NetBots[${drowningTarget}].ID} "${ebSpell}"

/return

**|