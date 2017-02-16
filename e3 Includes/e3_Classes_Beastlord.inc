|----------------------------------------------------------------------------|
|- e3_Classes_Beastlord.inc v3.0 ( originally designed to work with e3.mac )-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Contains Beastlord specific functions related to,						-|
|-		Setup and Ini entry information										-|
|-		MainLoop, and AssistLoop hooks										-|
|-		Over-all functionality / flow										-|
|----------------------------------------------------------------------------|

| --------------------------------------------------------------------------------------------------------|
| -- Moves a BST and SHM out of cover to engagement position an attempt to slow Quarm					 -|
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

	| -- Use e3Cast and keep casting until "Sha's Legarthy" is successful.  Skip recast if the spell won't take hold
	:tryAgainSlow
	/call e3_Cast ${Spawn[Quarm].ID} "Sha's Revenge"

	| ---------- If the spell won't take hold (ie someone else slowed it, move back under cover)
	/if (${castReturn.Equal[NOHOLD]}) /goto :moveToCover
	
	| ---------- If the spell wasn't successful (fizzle/resist), try again
	/if (!${castReturn.Equal[SUCCESS]}) {
		/delay 4
		/goto :tryAgainSlow
	}

	/bc [+r+] Quarm:  BST Slow Successful

	:moveToCover
	| ---------- Get back under cover
	/call EVENT_Deploy "NULL" TimeQuarm

/return

Sub BST_Setup
/if (${Debug}) /echo |- BST_Setup ==>
	
/if (${Debug}) /echo <== BST_Setup -|
/return



Sub BST_Background_Events

/return



SUB BST_MacroSettings
/if (${Debug}) /echo |- BST_MacroSettings ==>

	| Adv Settings
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#1" background 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#2" basics 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#3" casting 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#4" begForBuffs 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#5" buffCheck 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#6" buy 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#7" pet 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#8" heal 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#9" assist 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#10" class 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#11" corpseSummon 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#12" loot 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#13" sell 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#14" smoothMoves 1
	/call WriteToIni "${advSettings_Ini},BST Included Setups,Include#15" Wait4Rez 1
	/delay 15

	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#1" "check_Debuffs" 1
	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#2" "check_DoTs" 1
	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#3" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#4" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#5" "check_Heals" 1
	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#6" "check_PetHeal" 1
	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#7" "check_Nukes" 1
	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#8" "check_Buffs" 1
	/call WriteToIni "${advSettings_Ini},BST Functions,BST Function#9" "check_Pets" 1

/if (${Debug}) /echo <== BST_MacroSettings -|
/RETURN



SUB BST_CharacterSettings
/if (${Debug}) /echo |- BST_CharacterSettings ==>

/if (${Debug}) /echo <== BST_CharacterSettings -|
/RETURN



Sub BST_Aliases
/if (${Debug}) /echo |- BST_Aliases ==>

/if (${Debug}) /echo <== BST_Aliases -|
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

| --------------------------------------------------------------------------------------------------------|
| -- Adds the include for pulling Praesertum's in Sseru													 -|
| --------------------------------------------------------------------------------------------------------|

|#include e3 Includes\e3x_Sseru.inc