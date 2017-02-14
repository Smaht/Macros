|----------------------------------------------------------------------------|
|- e3_Classes_Shaman.inc v3.0 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Contains Shaman specific functions related to,							-|
|-		Setup and Ini entry information										-|
|-		MainLoop, and AssistLoop hooks										-|
|-		Over-all functionality / flow										-|
|----------------------------------------------------------------------------|



|--------------------------------------------------------|
|- autoCanni											-|
|--------------------------------------------------------|
|- Uses cannibalize spells and AAs when under 95% mana.	-|
|- Spells must be memmed to be used automatically.		-|
|--------------------------------------------------------|
SUB check_Canni
/if (${Debug}) /echo |- autoCanni ==>

	| Check if Auto-Canni is on
	/if (${AutoCanni}) {
	
		| AltAbility Cannibalization.
		/if (${Me.PctHPs} > 65 && ${Me.PctMana} < 65 && ${Me.AltAbilityReady[Cannibalization]}) /call e3_Cast 0 "Cannibalization"

		/if (!${ActionTaken} && !${medBreak}) {
		
			/declare castName string local
			/declare minMana int local
			/declare i int local
			
			/for i 1 to ${canniSpells.Size}
			
				/varset castName ${canniSpells[${i}].Arg[1,/]}
				
				| Get MinMana
				/if (${canniSpells[${i}].Find[/MinMana|]}) {
					/call argueString MinMana "${canniSpells[${i}]}"
					/varset minMana ${Macro.Return}
				} else {
					/varset minMana 90
				}
				
				/if (${Me.PctMana} < ${minMana} && ${Me.PctHPs} > 35) /call e3_Cast 0 "${canniSpells[${i}]}/giveuptimer|${If[${Me.Gem[${castName}]} || ${FindItemCount[=${castName}]},0,80]}"

			/if (!${ActionTaken}) /next i
		}
	}
	
/if (${Debug}) /echo <== autoCanni -|
/RETURN

| --------------------------------------------------------------------------------------------------------|
| -- Moves a SHM out of cover to engagement position an attempt to slow Quarm					 -|
| --------------------------------------------------------------------------------------------------------|

#Event slowQuarm "<#*#> Slow Quarm!"
Sub EVENT_slowQuarm(line)

	| ---------- /moveto "out of cover" location from which to slow.  Pause 1s to have time to actually begin moving if you are in fact going to move
	/moveto loc -1148, 214
	/delay 10

	| ---------- Wait until you are no longer moving, then re-target the ${deployRaidTarget} and face them
	:stillMoving
	/if (${Me.Moving}) {
		/delay 5
		/goto :stillMoving
	}

	/target Quarm
	/face fast

	| -- Use e3Cast and keep casting until "Malos" is successful
	:tryAgainMalo
	/call e3_Cast ${Spawn[Quarm].ID} "Malos"
	
	/if (!${castReturn.Equal[SUCCESS]}) {
		/delay 4
		/goto :tryAgainMalo
	}

	/bc [+r+] Quarm:  SHM Malos Successful

	| -- Use e3Cast and keep casting until "Turgur's Insects" is succesful
	:tryAgainSlow
	/call e3_Cast ${Spawn[Quarm].ID} "Turgur's Insects"
	
	/if (!${castReturn.Equal[SUCCESS]}) {
		/delay 4
		/goto :tryAgainSlow
	}

	/bc [+r+] Quarm:  SHM Slow Successful

	| ---------- Get back under cover
	/call EVENT_Deploy "NULL" TimeQuarm

/return

Sub SHM_Setup
/if (${Debug}) /echo |- SHM_Setup ==>
	
	/call IniToVar "${Character_Ini},Shaman,Auto-Canni (On/Off)" AutoCanni bool outer
	/call IniToArray "${Character_Ini},Shaman,Canni#" canniSpells
	
/if (${Debug}) /echo <== SHM_Setup -|
/return



Sub SHM_Background_Events
/return



SUB SHM_MacroSettings
/if (${Debug}) /echo |- SHM_MacroSettings ==>

	| Adv Settings
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#1" "check_Debuffs" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#2" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#3" "check_Heals" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#4" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#5" "check_DoTs" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#6" "check_petHeal" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#7" "check_Rain" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#8" "check_Nukes" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#9" "check_Buffs" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#10" "check_Pets" 1
	/call WriteToIni "${advSettings_Ini},SHM Functions,SHM Function#11" "check_Canni" 1

/if (${Debug}) /echo <== SHM_MacroSettings -|
/RETURN



SUB SHM_CharacterSettings
/if (${Debug}) /echo |- SHM_CharacterSettings ==>

	/call WriteToIni "${Character_Ini},Shaman,Auto-Canni (On/Off)"
	/call WriteToIni "${Character_Ini},Shaman,Canni#1"

/if (${Debug}) /echo <== SHM_CharacterSettings -|
/RETURN



Sub SHM_Aliases
/if (${Debug}) /echo |- SHM_Aliases ==>

/if (${Debug}) /echo <== SHM_Aliases -|
/return

| --------------------------------------------------------------------------------------|
| -- Casts shrink on anyone over 2.40 in size						                   -|
| --------------------------------------------------------------------------------------|

|SUB EVENT_shrinkGroup
#event shamanShrinkGroup "<#*#> Shrink your group!"
SUB EVENT_shamanShrinkGroup

	| ---------- You are a shaman, cast group shrink twice
	/call e3_cast ${Me.ID} "Tiny Terror/giveuptimer|45s"
	/delay 10s ${Me.SpellReady[Tiny Terror]}
	/call e3_cast ${Me.ID} "Tiny Terror/giveuptimer|45s"

/return


| --------------------------------------------------------------------------------------|
| -- Casts shrink on anyone over 2.40 in size						                   -|
| --------------------------------------------------------------------------------------|

|SUB EVENT_shrinkGroup
#event beastlordShrinkGroup "<#*#> Shrink your group!"
SUB EVENT_beastlordShrinkGroup

	| ---------- Cycle through all group members
	/declare i int local
	/for i 0 to ${Group}

		| ---------- Target the group member
		/target id ${Group.Member[${i}].ID}
		/delay 2s ${Target.ID}==${Group.Member[${i}].ID}

		| ---------- If the target is larger than 2.40, loop through shrinking them
		:shrinkLoop
		/if (${Target.Height} > 2.40) {
			/call e3_cast ${Group.Member[${i}].ID} "Shrink/giveuptimer|45s"
			/delay 5s !${Me.Casting.ID}
			/goto :shrinkLoop
		}
		
	/next i

/return