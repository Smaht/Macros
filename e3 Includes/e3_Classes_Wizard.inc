|----------------------------------------------------------------------------|
|- e3_Classes_Wizard.inc v3.0 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Contains Wizard specific functions related to,							-|
|-		Setup and Ini entry information										-|
|-		MainLoop, and AssistLoop hooks										-|
|-		Over-all functionality / flow										-|
|----------------------------------------------------------------------------|



SUB check_Harvest

	/if (${autoHarvest} && ${Me.PctMana} < 70 && ${Me.PctHPs} > 35) {
		| AltAbility
		/if (${Me.PctMana} < 65 && ${Me.AltAbilityReady[Harvest of Druzzil]}) /call e3_Cast 0 "Harvest of Druzzil"

		/if (!${ActionTaken}) {
		
			/declare castName string local
			/declare i int local
			
			/for i 1 to ${harvestSpells.Size}
			
				/varset castName ${harvestSpells[${i}].Arg[1,/]}
				/call e3_Cast 0 "${harvestSpells[${i}]}/giveuptimer|${If[${Me.Gem[${castName}]} || ${FindItemCount[=${castName}]},0,80]}"

			/if (!${ActionTaken}) /next i
		}
	}

/RETURN



Sub WIZ_Setup
/if (${Debug}) /echo |- WIZ_Setup ==>
	
	/declare i int local
	
	/call INItoVar "${Character_Ini},Wizard,Evac Spell" Evac_Spell string outer
	/call INItoVar "${Character_Ini},Wizard,Auto-Harvest (On/Off)" autoHarvest bool outer
	/call IniToArray "${Character_Ini},Wizard,Harvest#" harvestSpells
	
	| Look for a familiar to cast
	/if (${Defined[SelfBuffs]}) {
		/for i 1 to ${SelfBuffs.Size}
			
			
		/next i
	}

/if (${Debug}) /echo <== WIZ_Setup -|
/return



Sub WIZ_Background_Events

	/if (${Me.Pet.ID} && ${Me.Pet.CleanName.Find[${Me.CleanName}`s Familiar]} && (${MacroQuest.Server.Equal[PEQTGC]} || ${MacroQuest.Server.Equal[EQTitan]})) /pet get lost

/return



SUB WIZ_MacroSettings
/if (${Debug}) /echo |- WIZ_MacroSettings ==>

	| AdvSettings
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#1" background 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#2" basics 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#3" casting 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#4" begForBuffs 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#5" buffCheck 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#6" buy 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#7" assist 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#8" class 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#9" corpseSummon 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#10" loot 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#11" sell 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#12" smoothMoves 1
	/call WriteToIni "${advSettings_Ini},Wizard Included Setups,Include#13" Wait4Rez 1
	/delay 15

	/call WriteToIni "${advSettings_Ini},WIZ Functions,WIZ Function#1" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},WIZ Functions,WIZ Function#2" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},WIZ Functions,WIZ Function#3" "check_Rain" 1
	/call WriteToIni "${advSettings_Ini},WIZ Functions,WIZ Function#4" "check_Nukes" 1
	/call WriteToIni "${advSettings_Ini},WIZ Functions,WIZ Function#5" "check_Harvest" 1
	/call WriteToIni "${advSettings_Ini},WIZ Functions,WIZ Function#6" "check_Buffs" 1

/if (${Debug}) /echo <== WIZ_MacroSettings -|
/RETURN



SUB WIZ_CharacterSettings
/if (${Debug}) /echo |- WIZ_CharacterSettings ==>

	/call WriteToIni "${Character_Ini},Wizard,Evac Spell"
	/call WriteToIni "${Character_Ini},Wizard,Auto-Harvest (On/Off)"
	/call WriteToIni "${Character_Ini},Wizard,Harvest#1"

/if (${Debug}) /echo <== WIZ_CharacterSettings -|
/RETURN

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

	| ---------------- Memorize and cast appropriate port spell if you are a wizard
	/declare fullDestination string local
	/varset fullDestination ${shortDestination} Portal

	| ---------------- Over-ride fullDestination if the destination is skyfire and Class.ShortName = WIZ
	/if (${shortDestination.Equal[skyfire]} || ${shortDestination.Equal[sf]}) /varset fullDestination Tishan's Relocation

	| ---------------- Over-ride fullDestination if the destination is ej and Class.ShortName = WIZ
	/if (${shortDestination.Equal[ej]}) /varset fullDestination Markar's Relocation

	| ---------------- Over-ride fullDestination if the destination is dl and Class.ShortName = WIZ
	/if (${shortDestination.Equal[dl]}) /varset fullDestination Combine Portal

	| ---------------- Over-ride fullDestination if the destination is sro and Class.ShortName = WIZ
	/if (${shortDestination.Equal[sro]}) /varset fullDestination Evacuate: Ro

	| ---------------- Debug info
	/if (${e3xUtilitiesDebug} || ${Debug}) /echo 220: Wizard fullDestination is ${fullDestination} in EVENT_portDestination

	/if (${Me.Book[${fullDestination}]}) {
		/bc [+y+] Memorizing ${fullDestination} ...
		/memorize "${fullDestination}|gem8"
	} else {
		/bc [+r+] ${fullDestination} not found in my spellboook ...
	}

/return