|----------------------------------------------------------------------------|
|- e3_Classes_Warrior.inc v3.0 ( originally designed to work with e3.mac )	-|
|- Written by Killians of PEQ												-|
|----------------------------------------------------------------------------|
|- Contains Warrior specific functions related to,							-|
|-		Setup and Ini entry information										-|
|-		MainLoop, and AssistLoop hooks										-|
|-		Over-all functionality / flow										-|
|----------------------------------------------------------------------------|



|--------------------|
|- attemptProvoke	-|
|--------------------|
SUB check_warProvoke(provokeDisc)
/if (${Debug}) /echo |- check_warProvoke ==>

	/if (!${Defined[MinBellowEndurance]}) {
		/if (${provokeDisc.Equal[Ancient: Chaos Cry]}) {
			/declare MinBellowEndurance int outer 300
		} else /if (${provokeDisc.Equal[Bellow of the Mastruq]}) {
			/declare MinBellowEndurance int outer 275
		} else /if (${provokeDisc.Equal[Incite]}) {
			/declare MinBellowEndurance int outer 250
		} else /if (${provokeDisc.Equal[Berate]}) {
			/declare MinBellowEndurance int outer 200
		} else /if (${provokeDisc.Equal[Bellow]}) {
			/declare MinBellowEndurance int outer 175
		} else /if (${provokeDisc.Equal[Provoke]}) {
			/declare MinBellowEndurance int outer 85
		}
	}
	
	/if (${Me.CombatAbilityReady[${provokeDisc}]}) {
	
		| check endurance
		/if (${Me.Endurance} < ${MinBellowEndurance}) {
			/if (${Debug}) /echo |- attemptProvoke -| I do not have enough endurance to use [${provokeDisc}].
		} else {
	
			/if (${provokeTimer_${Target.ID}} && ${Spawn[${Me.TargetOfTarget}].CleanName.Equal[${Me}]}) {
				/if (${Debug}) /echo |- attemptProvoke -| I still have aggro, and have not met my recast timer for [${provokeDisc}] [${provokeTimer_${Target.ID}}].
			} else {
	
				/call useDisc 0 "${provokeDisc}"
				/call CreateTimer provokeTimer_${Target.ID} 600
			}
		}
	}

/if (${Debug}) /echo <== check_warProvoke -|
/RETURN



Sub WAR_Setup
/if (${Debug}) /echo |- WAR_Setup ==>

	/call IniToVar "${MacroData_Ini},Class Specific Info,Warrior Provoke Disciplines" warriorBellows string outer

/if (${Debug}) /echo <== WAR_Setup -|
/return



Sub WAR_Background_Events

/return



SUB WAR_MacroSettings
/if (${Debug}) /echo |- WAR_MacroSettings ==>

	| MacroData settings
	/call WriteToIni "${MacroData_Ini},Class Specific Info,Warrior Provoke Disciplines" "Provoke,Bellow,Berate,Incite,Bellow of the Mastruq,Ancient: Chaos Cry" 0
	
	| AdvSettings
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#1" background 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#2" basics 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#3" casting 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#4" begForBuffs 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#5" buffCheck 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#6" buy 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#7" assist 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#8" class 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#9" corpseSummon 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#10" loot 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#11" sell 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#12" smoothMoves 1
	/call WriteToIni "${advSettings_Ini},Warrior Included Setups,Include#13" Wait4Rez 1
	/delay 15

	/call WriteToIni "${advSettings_Ini},WAR Functions,WAR Function#1" "check_Burns" 1
	/call WriteToIni "${advSettings_Ini},WAR Functions,WAR Function#2" "check_CombatBuffs" 1
	/call WriteToIni "${advSettings_Ini},WAR Functions,WAR Function#3" "check_Buffs" 1
	
/if (${Debug}) /echo <== WAR_MacroSettings -|
/RETURN



SUB WAR_CharacterSettings
/if (${Debug}) /echo |- WAR_CharacterSettings ==>

/if (${Debug}) /echo <== WAR_CharacterSettings -|
/RETURN

| --------------------------------------------------------------------------------------|
| -- States if soemthing is slowed, and what                                           -|
| --------------------------------------------------------------------------------------|

#EVENT DebuffTarget "#1# yawns."
#EVENT DebuffTarget "#1# loses their fighting edge."
#EVENT DebuffTarget "#1# slows down."
#EVENT DebuffTarget "#1# has been crippled."
#EVENT DebuffTarget "#1# looks very uncomfortable."
SUB EVENT_DebuffTarget(line, SlowedTarget)

	| ---------------- If I am not the single main toon, exit
	/if (!${Me.CleanName.Equal[${classLeader}]}) /return

	| ---------------- Shaman slow
	/if (${line.Find[yawn]}) {	
		/bc [+r+] Slowed ${SlowedTarget} (SHM)
		/if (${NetBots[${classLeader}].Raid}) /rs Slowed ${SlowedTarget} (SHM)
		/return
	}

	| ---------------- Shaman malos
	/if (${line.Find[uncomfortable]}) {	
		/bc [+r+] Malos/Malosinia on ${SlowedTarget} (SHM)
		/if (${NetBots[${classLeader}].Raid}) /rs Malos/Malosinia on ${SlowedTarget} (SHM)
		/return
	}

	| ---------------- Beastlord slow
	/if (${line.Find[fighting]}) {
		/bc [+r+] Slowed ${SlowedTarget} (BST)
		/if (${NetBots[${classLeader}].Raid}) /rs Slowed ${SlowedTarget} (BST)
		/return
	}

	| ---------------- Enchanter slow
	/if (${line.Find[slows]}) {
		/bc [+r+] Slowed ${SlowedTarget} (ENC)
		/if (${NetBots[${classLeader}].Raid}) /rs Slowed ${SlowedTarget} (ENC)
		/return
	}

	| ---------------- Cripple	
	/if (${line.Find[crippled]}) {
		/bc [+r+] Crippled ${SlowedTarget}
		/if (${NetBots[${classLeader}].Raid}) /rs Crippled ${SlowedTarget}
		/return
	}

/return

| --------------------------------------------------------------------------------------|
| -- Execute a set of predefined commands											   -|
| --------------------------------------------------------------------------------------|

#EVENT execCommand "[MQ2] Execute Command: #1#"
SUB EVENT_execCommand(line, commandSet)

	| ---------------- If I am not the single main toon, exit
	/if (!${Me.CleanName.Equal[${classLeader}]}) /return

	| ---------- Define the location of saved commands
	/if (!${Defined[execCommand_Ini]}) /declare execCommand_Ini string outer ${Ini[${MacroData_Ini},File Paths,Saved Commands]}

	| ---------- If the chEvent doesn't exist, exit
	/if (!${Ini[${execCommand_Ini}].Find[${commandSet}]}) {
		/bc [+y+] ${commandSet} not found!  Exiting ...
		/return commandSet_NOT_FOUND
	}

	| ---------- Create an array of saved commands from the config file
	/if (${Ini[${execCommand_Ini},${commandSet},Command#1].Length}) /call IniToArray "${execCommand_Ini},${commandSet},Command#" commandsToExecute

	| ---------- Execute the commands in the list
	/declare i int local
	/for i 1 to ${commandsToExecute.Size}

		/docommand ${commandsToExecute[${i}]}
		/delay 2

	/next i

	/echo Completed running ${commandSet}

	| ---------- Cleanup
	/if (${Defined[execCommand_Ini]}) /deletevar execCommand_Ini
	/if (${Defined[commandsToExecute]}) /deletevar commandsToExecute

/return
