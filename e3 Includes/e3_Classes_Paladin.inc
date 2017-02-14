|----------------------------------------------------------------------------|
|- e3_Classes_Paladin.inc v3.0 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Contains Paladin specific functions related to,							-|
|-		Setup and Ini entry information										-|
|-		MainLoop, and AssistLoop hooks										-|
|-		Over-all functionality / flow										-|
|----------------------------------------------------------------------------|

SUB check_Yaulp
/if (${Debug}) /echo |- check_clrYaulp ==>
	
	/if (${AutoYaulp} && !${medBreak})  {	

		/declare castName string local ${yaulpSpell.Arg[1,/]}

		| Auto-yaulp spell
		/if (!${Bool[${Me.Buff[${castName}]}]} && ${Me.PctMana} < 95) {
			/if (${Target.ID}) /declare tempTarget int local ${Target.ID}
			/squelch /target clear
			/call e3_Cast 0 "${yaulpSpell}/giveuptimer|${If[${Me.Gem[${castName}]} || ${FindItemCount[=${castName}]},0,80]}"
			/if (${tempTarget}) /call TrueTarget ${tempTarget}
		}
	}
	
/if (${Debug}) /echo <== check_clrYaulp -|
/RETURN

Sub PAL_Setup
/if (${Debug}) /echo |- PAL_Setup ==>
	
	/call InitoVar "${Character_Ini},Paladin,Auto-Yaulp (On/Off)" AutoYaulp bool outer
	/call InitoVar "${Character_Ini},Paladin,Yaulp Spell" yaulpSpell string outer
	
/if (${Debug}) /echo <== PAL_Setup -|
/return



Sub PAL_Background_Events

/return



SUB PAL_MacroSettings
/if (${Debug}) /echo |- PAL_MacroSettings ==>

	| Adv Settings
	/call WriteToIni "${advSettings_Ini},PAL Functions,PAL Function#1" "check_Yaulp" 1
	/call WriteToIni "${advSettings_Ini},PAL Functions,PAL Function#2" "check_Heals" 1
	/call WriteToIni "${advSettings_Ini},PAL Functions,PAL Function#3" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},PAL Functions,PAL Function#4" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},PAL Functions,PAL Function#5" "check_Nukes" 1
	/call WriteToIni "${advSettings_Ini},PAL Functions,PAL Function#6" "check_Buffs" 1

/if (${Debug}) /echo <== PAL_MacroSettings -|
/RETURN



SUB PAL_CharacterSettings
/if (${Debug}) /echo |- PAL_CharacterSettings ==>

	/call WriteToIni "${Character_Ini},Paladin,Auto-Yaulp (On/Off)" On 0
	/call WriteToIni "${Character_Ini},Paladin,Yaulp Spell"
	
/if (${Debug}) /echo <== PAL_CharacterSettings -|
/RETURN



Sub PAL_Aliases
/if (${Debug}) /echo |- PAL_Aliases ==>

/if (${Debug}) /echo <== PAL_Aliases -|
/return