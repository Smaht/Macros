|----------------------------------------------------------------------------|
|- e3_Classes_Ranger.inc v3.0 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Contains Ranger specific functions related to,							-|
|-		Setup and Ini entry information										-|
|-		MainLoop, and AssistLoop hooks										-|
|-		Over-all functionality / flow										-|
|----------------------------------------------------------------------------|



Sub RNG_Setup
/if (${Debug}) /echo |- RNG_Setup ==>
	
/if (${Debug}) /echo <== RNG_Setup -|
/return



Sub RNG_Background_Events

/return



SUB RNG_MacroSettings
/if (${Debug}) /echo |- RNG_MacroSettings ==>

	| Adv Settings
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#1" background 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#2" basics 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#3" casting 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#4" begForBuffs 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#5" buffCheck 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#6" buy 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#7" heal 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#8" assist 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#9" class 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#10" corpseSummon 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#11" loot 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#12" sell 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#13" smoothMoves 1
	/call WriteToIni "${advSettings_Ini},RNG Included Setups,Include#14" Wait4Rez 1
	/delay 15

	/call WriteToIni "${advSettings_Ini},RNG Functions,RNG Function#1" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},RNG Functions,RNG Function#2" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},RNG Functions,RNG Function#3" "check_Heals"	 1
	/call WriteToIni "${advSettings_Ini},RNG Functions,RNG Function#4" "check_DoTs" 1
	/call WriteToIni "${advSettings_Ini},RNG Functions,RNG Function#5" "check_Nukes" 1
	/call WriteToIni "${advSettings_Ini},RNG Functions,RNG Function#6" "check_Buffs" 1

/if (${Debug}) /echo <== RNG_MacroSettings -|
/RETURN



SUB RNG_CharacterSettings
/if (${Debug}) /echo |- RNG_CharacterSettings ==>

/if (${Debug}) /echo <== RNG_CharacterSettings -|
/RETURN



Sub RNG_Aliases
/if (${Debug}) /echo |- RNG_Aliases ==>

/if (${Debug}) /echo <== RNG_Aliases -|
/return