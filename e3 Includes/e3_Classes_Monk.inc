|------------------------------------------------------------------------|
|- e3_Classes_Monk.inc v3.0 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ											-|
|------------------------------------------------------------------------|
|- Contains Monk specific functions related to,							-|
|-		Setup and Ini entry information									-|
|-		MainLoop, and AssistLoop hooks									-|
|-		Over-all functionality / flow									-|
|------------------------------------------------------------------------|



Sub MNK_Setup
/if (${Debug}) /echo |- MNK_Setup ==>
	
/if (${Debug}) /echo <== MNK_Setup -|
/return



Sub MNK_Background_Events

/return



SUB MNK_MacroSettings
/if (${Debug}) /echo |- MNK_MacroSettings ==>

	| Adv Settings
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#1" background 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#2" basics 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#3" casting 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#4" begForBuffs 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#5" buffCheck 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#6" buy 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#7" assist 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#8" class 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#9" corpseSummon 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#10" loot 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#11" sell 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#12" smoothMoves 1
	/call WriteToIni "${advSettings_Ini},Monk Included Setups,Include#13" Wait4Rez 1
	/delay 15

	/call WriteToIni "${advSettings_Ini},MNK Functions,MNK Function#1" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},MNK Functions,MNK Function#2" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},MNK Functions,MNK Function#3" "check_Buffs" 1

/if (${Debug}) /echo <== MNK_MacroSettings -|
/RETURN



SUB MNK_CharacterSettings
/if (${Debug}) /echo |- MNK_CharacterSettings ==>

/if (${Debug}) /echo <== MNK_CharacterSettings -|
/RETURN



Sub MNK_Aliases
/if (${Debug}) /echo |- MNK_Aliases ==>

/if (${Debug}) /echo <== MNK_Aliases -|
/return