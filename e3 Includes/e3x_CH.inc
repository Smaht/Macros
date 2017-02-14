| -----------------------------------------------------------------------------
| -- ch.mac - Basics originally created by pinkfloydx33
| --    	- Heavily modified by Smaht and incorporated into Killian's e3
| --        - e3x_CH.inc
| --        - Version 3.0, last modified 2016/05/17
| -----------------------------------------------------------------------------

| -----------------------------------------------------------------------------
| -- Start the CH loop.  run events and loop
| -----------------------------------------------------------------------------

#Event chEnabled "#*#CH Mode Enabled: #1#"

Sub event_chEnabled(line, chEvent)

	| ---------- Define the location of deployment locations
	/if (!${Defined[chEvent_Ini]}) /declare chEvent_Ini string outer ${Ini[${MacroData_Ini},File Paths,CH Events]}

	| ---------- If the chEvent doesn't exist, exit
	/if (!${Ini[${chEvent_Ini}].Find[${chEvent}]}) {
		/bc [+y+] ${chEvent} not found!  Exiting ...
		/return chEvent_NOT_FOUND
	}

	| ------------ If you aren't a CLR,DRU,SHM exit
	/if (!${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /return CH_CLASS_INVALID

	| ---------- Declare some starting variables for use in setup
	/declare chDebug bool outer FALSE
	/declare chDebugChannel string outer echo
	/declare chChainMemberShip int outer
	/declare i int local

	| ---------- Read event variables from the config file
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#1 - Cleric#1].Length}) /call IniToArray "${chEvent_Ini},${chEvent},Chain#1 - Cleric#" chChain1Healers
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#1 - Tank#1].Length}) /call IniToArray "${chEvent_Ini},${chEvent},Chain#1 - Tank#" chChain1Tanks
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#1 - Timing].Length}) /call IniToVar "${chEvent_Ini},${chEvent},Chain#1 - Timing" chChain1Timing int outer
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#1 - Type].Length}) /call IniToVar "${chEvent_Ini},${chEvent},Chain#1 - Type" chChain1Type string outer

	/if (${Ini[${chEvent_Ini},${chEvent},Chain#2 - Cleric#1].Length}) /call IniToArray "${chEvent_Ini},${chEvent},Chain#2 - Cleric#" chChain2Healers
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#2 - Tank#1].Length}) /call IniToArray "${chEvent_Ini},${chEvent},Chain#2 - Tank#" chChain2Tanks
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#2 - Timing].Length}) /call IniToVar "${chEvent_Ini},${chEvent},Chain#2 - Timing" chChain2Timing int outer
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#2 - Type].Length}) /call IniToVar "${chEvent_Ini},${chEvent},Chain#2 - Type" chChain2Type string outer

	/if (${Ini[${chEvent_Ini},${chEvent},Chain#3 - Cleric#1].Length}) /call IniToArray "${chEvent_Ini},${chEvent},Chain#3 - Cleric#" chChain3Healers
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#3 - Tank#1].Length}) /call IniToArray "${chEvent_Ini},${chEvent},Chain#3 - Tank#" chChain3Tanks
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#3 - Timing].Length}) /call IniToVar "${chEvent_Ini},${chEvent},Chain#3 - Timing" chChain3Timing int outer
	/if (${Ini[${chEvent_Ini},${chEvent},Chain#3 - Type].Length}) /call IniToVar "${chEvent_Ini},${chEvent},Chain#3 - Type" chChain3Type string outer

	| ---------- Debug info
	/if (${chDebug}) {

		/if (${Defined[chChain1Healers]}) {
			/for i 1 to ${chChain1Healers.Size}
				/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_1_${i}: {chChain1Healers[${i}]} = ${chChain1Healers[${i}]}
			/next i
		}
		/if (${Defined[chChain1Tanks]}) {
			/for i 1 to ${chChain1Tanks.Size}
				/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_1_${i}: {chChain1Tanks[${i}]} = ${chChain1Tanks[${i}]}
			/next i
		}
		/if (${Defined[chChain1Timing]}) /docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_1: {chChain1Timing} = ${chChain1Timing}
		/if (${Defined[chChain1Type]}) /docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_1: {chChain1Type} = ${chChain1Type}
		
		/if (${Defined[chChain2Healers]}) {
			/for i 1 to ${chChain2Healers.Size}
				/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_2_${i}: {chChain2Healers[${i}]} = ${chChain2Healers[${i}]}
			/next i
		}
		/if (${Defined[chChain2Tanks]}) {
			/for i 1 to ${chChain2Tanks.Size}
				/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_2_${i}: {chChain2Tanks[${i}]} = ${chChain2Tanks[${i}]}
			/next i
		}
		/if (${Defined[chChain2Timing]}) /docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_2: {chChain2Timing} = ${chChain2Timing}
		/if (${Defined[chChain2Type]}) /docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_2: {chChain2Type} = ${chChain2Type}

		/if (${Defined[chChain3Healers]}) {
			/for i 1 to ${chChain3Healers.Size}
				/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_3_${i}: {chChain3Healers[${i}]} = ${chChain3Healers[${i}]}
			/next i
		}
		/if (${Defined[chChain3Tanks]}) {
			/for i 1 to ${chChain3Tanks.Size}
				/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_3_${i}: {chChain3Tanks[${i}]} = ${chChain3Tanks[${i}]}
			/next i
		}
		/if (${Defined[chChain3Timing]}) /docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_3: {chChain3Timing} = ${chChain3Timing}
		/if (${Defined[chChain3Type]}) /docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_3: {chChain3Type} = ${chChain3Type}
	}

	| ---------- Verify that you are a member of this {chEvent}.  Set {chChainMemberShip} appropriately.  Exit if not a member.
	/if (${Defined[chChain1Healers]} && !${chChainMemberShip}) {
		/for i 1 to ${chChain1Healers.Size}
			/if (${chChain1Healers[${i}].Equal[${Me.CleanName}]}) {
				/varset chChainMemberShip 1

				| ---------- Debug Info
				/if (${chDebug}) {
					/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_000: ${Me.CleanName} found on {chChain1Healers[${i}]}
					/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_001: {chChainMemberShip} = ${chChainMemberShip}
				}

				| ---------- Skip to the good part
				/bc [+r+] Coach!  I made team #${chChainMemberShip} for ${chEvent}!
				/goto :startCH
			}
		/next i
	}

	/if (${Defined[chChain2Healers]} && !${chChainMemberShip}) {
		/for i 1 to ${chChain2Healers.Size}
			/if (${chChain2Healers[${i}].Equal[${Me.CleanName}]}) {
				/varset chChainMemberShip 2

				| ---------- Debug Info
				/if (${chDebug}) {
					/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_010: ${Me.CleanName} found on {chChain2Healers[${i}]}
					/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_011: {chChainMemberShip} = ${chChainMemberShip}
				}

				| ---------- Skip to the good part
				/bc [+r+] Coach!  I made team #${chChainMemberShip} for ${chEvent}!
				/goto :startCH
			}
		/next i
	}

	/if (${Defined[chChain3Healers]} && !${chChainMemberShip}) {
		/for i 1 to ${chChain3Healers.Size}
			/if (${chChain3Healers[${i}].Equal[${Me.CleanName}]}) {
				/varset chChainMemberShip 3

				| ---------- Debug Info
				/if (${chDebug}) {
					/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_020: ${Me.CleanName} found on {chChain3Healers[${i}]}
					/docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_021: {chChainMemberShip} = ${chChainMemberShip}
				}

				| ---------- Skip to the good part
				/bc [+r+] Coach!  I made team #${chChainMemberShip} for ${chEvent}!
				/goto :startCH
			}
		/next i
	}

	| ---------- You aren't a member of this chEvent.  Exit
	/if (${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: e3x_CH_005: I am not a member of ${chEvent}.  Exiting and doing nothing
	/goto :cleanupAndExit
	/return

	| ---------- Chain membership determined, and which chain.  Move to the actual healing
	:startCH

	| ---------- Mount if you can
    |/call EVENT_mountUp

	| ---------- You are a CH member, set the appropriate variable for checking against later
	/if (${Defined[chExitVar]}) {
		/varset chExitVar FALSE 
		} else {
			/declare chExitVar bool outer FALSE
		}   

	| ------------ Declare some variables for use in the CH rotation
	/declare prevCler string outer
	/declare nextCler string outer
	/declare clerCount int outer
	/declare curClerPos int outer
	/declare waitTime int outer ${chChain${chChainMemberShip}Timing}
	/declare rotation_Active bool outer FALSE
	/declare chatChannel string outer bc
	/declare healSpell string outer
	/declare chTarget string outer ${chChain${chChainMemberShip}Tanks[1]}
	/declare chTargetID int outer ${Spawn[pc ${chChain${chChainMemberShip}Tanks[1]}].ID}

	| ---------- Setup prevCler, nextCler, clerCount and curClerPos
	/call ArrayToString chChain${chChainMemberShip}Healers clericList
	/call clerList

	| ---------- Call the prep routine for setting appropriate spell
	/call chPrep
	/if (${Macro.Return.Equal[FAILED]}) {
		/bc {+r+} CH spell setup failed.  Ending CH mode
		/return CH_SPELL_SETUP_FAIL
	}

	| ------------ Run CH mode events ONLY.  Do not exit loop until specifically told
	:loop
    /doevents nextCH
    /doevents setTank
    /doevents startRotation
    /doevents stopRotation
    /doevents chDisabled
    
    | ------------ Exit out of CH Mode
    /if (${chExitVar}) /goto :exitloop

    /goto :loop

    :exitloop

    /bc [+r+] Exiting CH Loop

    | ------------ Cleanup outer variables
    :cleanupAndExit
	/if (${Defined[clericList]}) /deletevar clericList
	/if (${Defined[chDebug]}) /deletevar chDebug
	/if (${Defined[chDebugChannel]}) /deletevar chDebugChannel
	/if (${Defined[prevCler]}) /deletevar prevCler
	/if (${Defined[nextCler]}) /deletevar nextCler
	/if (${Defined[clerCount]}) /deletevar clerCount
	/if (${Defined[curClerPos]}) /deletevar curClerPos
	/if (${Defined[waitTime]}) /deletevar waitTime
	/if (${Defined[rotation_Active]}) /deletevar rotation_Active
	/if (${Defined[chTarget]}) /deletevar chTarget
	/if (${Defined[chTargetID]}) /deletevar chTargetID
	/if (${Defined[chatChannel]}) /deletevar chatChannel
	/if (${Defined[healSpell]}) /deletevar healSpell
	/if (${Defined[chExitVar]}) /deletevar chExitVar
	/if (${Defined[chChainMemberShip]}) /deletevar chChainMemberShip

    | ------------ Dismount if mounted
    /delay 11s
    |/call EVENT_dismountOnZone

/return 


| ---------------------------------------
variablers to cleanup later
chDebug
chChainMemberShip
chExitVar

| -----------------------------------------------------------------------------
| -- End the CH loop
| -----------------------------------------------------------------------------

#Event chDisabled "#*#CH Mode Disabled#*#"

Sub event_chDisabled

    | ------------ If you are a CLR,DRU,SHM reset your ${tanks} array and delete {chTarget}
    /if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /call resetTanksArray

    | ------------ If not a member of the CH, ignore this event
    /if (!${Defined[chExitVar]}) /return NOT_CH_MEMBER

    | ------------ Set the chExitVar to TRUE to force an exit of the CH loop
    /varset chExitVar TRUE

    | ------------ Debug info
    /if (${Debug} || ${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: 00: chExitVar is now set to ${chExitVar}

/return

| -----------------------------------------------------------------------------
| -- When called upon, cast the next CH
| -----------------------------------------------------------------------------

#Event nextCH "#*# You are the next cleric #1#!"

Sub event_nextCH(line, nextUp)

    | ------------ If not a member of the CH, ignore this event
    /if (!${Defined[chExitVar]}) /return NOT_CH_MEMBER

    /if (${nextUp.Equal[${Me.CleanName}]}) {
        /echo Waiting ${waitTime} seconds to CH
        /delay ${waitTime}
        /doevents setTank
        /if (${rotation_Active}) /call castCH
    }
}

/return

| -----------------------------------------------------------------------------
| -- Set the tank target
| -----------------------------------------------------------------------------

#Event setTank "#*#Chain#1# Tank: #2#"

Sub event_setTank(line, int team, tank)

    | ------------ If not a member of the CH, ignore the rest
    /if (!${Defined[chExitVar]}) /return NOT_CH_MEMBER

    /if (${chChainMemberShip} == ${team}) {

        | ------------ Set chTarget to be ${tank}
        /varset chTarget ${tank}

        | ------------ Set chTargetID to be the ID of ${tank}
        /varset chTargetID ${Spawn[pc ${tank}].ID}

        | ------------ Debug info
        /if (${Debug} || ${chDebug}) {
            /docommand /${chDebugChannel} ${Me.CleanName}: setTank00: Tank has been set to ${chTarget}
            /docommand /${chDebugChannel} ${Me.CleanName}: setTank01: Tank.ID has been set to ${chTargetID}
            /docommand /${chDebugChannel} ${Me.CleanName}: setTank02: {team} = ${team}
            /docommand /${chDebugChannel} ${Me.CleanName}: setTank03: {chChainMemberShip} = ${chChainMemberShip}
        }
    }

/return

| -----------------------------------------------------------------------------
| -- Set the cleric list
| -- Requires the list to be in the format of Cler1,Cler2,Cler3,etc
| -----------------------------------------------------------------------------

Sub clerList

    | ------------ If not a member of the CH, ignore this event
    /if (!${Defined[chExitVar]}) /return NOT_CH_MEMBER

    | ------------ Set the number of clerics in the list
    /varcalc clerCount ${clericList.Count[,]} + 1
    
    | ------------ Set the position of the cleric in the list
    /declare i int local
    /for i 1 to ${clerCount}
        /if (${clericList.Arg[${i},,].Equal[${Me.CleanName}]}) /varset curClerPos ${i}
        
        | ------------ Debug Info
        /if (${Debug} || ${chDebug}) {
            /docommand /${chDebugChannel} ${Me.CleanName}: 30: i is set to ${i}
            /docommand /${chDebugChannel} ${Me.CleanName}: 31: clericList.Arg[${i}] is ${clericList.Arg[${i},,]}
        }
        
        /next i

    | ------------ Debug Info
    /if (${Debug} || ${chDebug}) {
        /docommand /${chDebugChannel} ${Me.CleanName}: 20: clerCount is ${clerCount}
        /docommand /${chDebugChannel} ${Me.CleanName}: 21: curClerPos is ${curClerPos}
    }

    | ------------ If I am the first cleric in the list, set nextCler/prevCler variables and return
    /if (${curClerPos} == 1) {
        /varset prevCler ${clericList.Arg[${clerCount},,]}
        | ------------ If there are two or more clerics in the list, set nextCler to the next cleric
        /if (${clerCount} > 1) /varset nextCler ${clericList.Arg[${Math.Calc[${curClerPos}+1].Int},,]}
        | ------------ If I am the only cleric in the list, set nextCler to myself
        /if (${clerCount} == 1) /varset nextCler ${clericList.Arg[1,,]}

        | ------------ Debug info
        /if (${Debug} || ${chDebug}) {
            /docommand /${chDebugChannel} ${Me.CleanName}: 50: nextCler set to ${nextCler} in the case of curClerPos = 1
            /docommand /${chDebugChannel} ${Me.CleanName}: 51: prevCler set to ${prevCler} in the case of curClerPos = 1
        }

        /return
    } 

    | ------------ If I am the last cleric in the list, set nextCler/prevCler variables and return
    /if (${curClerPos} == ${clerCount}) {
    	/varset nextCler ${clericList.Arg[1,,]}
        | ------------ If there are two or more clerics in the list, set prevCler to the previous cleric
        /if (${clerCount} > 1) /varset prevCler ${clericList.Arg[${Math.Calc[${curClerPos}-1].Int},,]}
        | ------------ If I am the only cleric in the list, set prevCler to myself
        /if (${clerCount} == 1) /varset prevCler ${clericList.Arg[1,,]}

        | ------------ Debug info
        /if (${Debug} || ${chDebug}) {
            /docommand /${chDebugChannel} ${Me.CleanName}: 60: nextCler set to ${nextCler} in the case of curClerPos = clerCount
            /docommand /${chDebugChannel} ${Me.CleanName}: 61: prevCler set to ${prevCler} in the case of curClerPos = clerCount
        }

        /return
    } 

    | ------------ If there are three (3) or more clerics in the list, set nextCler/prevCler variables and return
    /if (${clerCount} > 2) {
        /varset nextCler ${clericList.Arg[${Math.Calc[${curClerPos}+1].Int},,]}
        /varset prevCler ${clericList.Arg[${Math.Calc[${curClerPos}-1].Int},,]}

        | ------------ Debug info
        /if (${Debug} || ${chDebug}) {
            /docommand /${chDebugChannel} ${Me.CleanName}: 70: nextCler set to ${nextCler} in the case of clerCount > 2
            /docommand /${chDebugChannel} ${Me.CleanName}: 71: prevCler set to ${prevCler} in the case of clerCount > 2
        }

        /return
    }
 
/return

| -----------------------------------------------------------------------------
| -- Start the heal chain
| -----------------------------------------------------------------------------

#Event startRotation "#*#Start CH#*#"

Sub event_startRotation

    | ------------ If you are a CLR,DRU,SHM delete your ${tanks} array
    |/if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /deletevar tanks
    /if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /call RemoveArrayElement tanks Smaht

    | ------------ If not a member of the CH, ignore this event
    /if (!${Defined[chExitVar]}) /return NOT_CH_MEMBER

    | ------------ Set the rotation TRUE for all clerics
    /varset rotation_Active TRUE

    | ------------ Debug info
    /if (${Debug} || ${chDebug}) {
        /docommand /${chDebugChannel} ${Me.CleanName}: 80: rotation_Active is now set to ${rotation_Active}
        /docommand /${chDebugChannel} ${Me.CleanName}: 81: chTarget is ${chTarget} with an ID of ${chTargetID}
    }

    | ------------ If I am the first cleric in the list, start the ball rolling
    /if (${curClerPos} == 1) /call castCH

/return

| -----------------------------------------------------------------------------
| -- End the heal chain
| -----------------------------------------------------------------------------

#Event stopRotation "#*#Stop CH#*#"

Sub event_stopRotation

    | ------------ If you are a CLR,DRU,SHM reset your ${tanks} array
    /if (${Select[${Me.Class.ShortName},CLR,DRU,SHM]}) /call resetTanksArray

    | ------------ If not a member of the CH, ignore this event
    /if (!${Defined[chExitVar]}) /return NOT_CH_MEMBER

    | ------------ Set the rotation FALSE for all clerics
    /varset rotation_Active FALSE

    | ------------ Debug info
    /if (${Debug} || ${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: 90: rotation_Active is now set to ${rotation_Active}

/return

| -----------------------------------------------------------------------------
| -- Cast CH on chTargetID
| -----------------------------------------------------------------------------

Sub castCH

    | ------------ When called upon, cast CH on chTarget/chTargetID
    /stand
    
    /if (${Int[${Spawn[id ${chTargetID}].Distance3D}]} < ${Int[${Spell[${healSpell}].MyRange}]}) {
    	/bc Chain${chChainMemberShip} on ${chTarget} (ID:${chTargetID}). Next cleric ${nextCler}.  CurMana: ${Me.PctMana}
    } else {
     	/bc [+y+] WARNING! ${chTarget} (ID:${chTargetID}) OOR!  Range is ${Int[${Spawn[id ${chTargetID}].Distance3D}]} yards!
	}

    /bct ${nextCler} You are the next cleric ${nextCler}!
    
    | ---------- Verify the tank exists, don't cast if it doesn't
    /call verifyCHTarget
    /if (${chExitVar}) /return

    /casting "${healSpell}" -targetid|${chTargetID} -maxtries|4
    /sit

    | ------------ Debug info
    /if (${Debug} || ${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: 100: Commpleted callCH on ${chTarget}/${chTargetID}

/return

| -----------------------------------------------------------------------------
| -- Set healSpell to the appropriate spell based on class
| -----------------------------------------------------------------------------

Sub chPrep

    | ------------ Based on class, pick a heal spell.  Pick the one appropriate to the type of heal

    /if (${Me.Class.ShortName.Equal[CLR]}) {
        /if (${chChain${chChainMemberShip}Type.Equal[CH]}) /varset healSpell Complete Heal
        /if (${chChain${chChainMemberShip}Type.Equal[Fast]}) /varset healSpell Holy Light

        | ------------ Debug info
        /if (${Debug} || ${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: 100: In CLR healSpell set to ${healSpell}

    } else /if (${Me.Class.ShortName.Equal[DRU]}) {
        /if (${chChain${chChainMemberShip}Type.Equal[CH]}) /varset healSpell Karana's Renewal
        /if (${chChain${chChainMemberShip}Type.Equal[Fast]}) /varset healSpell Sylvan Infusion

        | ------------ Debug info
        /if (${Debug} || ${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: 110: in DRU healSpell set to ${healSpell}
    
    } else /if (${Me.Class.ShortName.Equal[SHM]}) {
        /if (${chChain${chChainMemberShip}Type.Equal[CH]}) /varset healSpell Kragg's Mending
        /if (${chChain${chChainMemberShip}Type.Equal[Fast]}) /varset healSpell Daluda's Mending

        | ------------ Debug info
        /if (${Debug} || ${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: 120: In SHM healSpell set to ${healSpell}

    }

    /if (${healSpell.Equal[NULL]}) /return FAILED

    | ---------- If healSpell not already memmed, mem it into the DefaultGem
    /if (!${Me.Gem[${healSpell}]}) {
        /memorize "${healSpell}|${DefaultGem}"
    }

/return

| -----------------------------------------------------------------------------
| -- Reset ${tanks} array
| -----------------------------------------------------------------------------

Sub resetTanksArray

    /if (${Ini[${Character_Ini},Heals,Tank#1].Length})  /call IniToArray "${Character_Ini},Heals,Tank#" tanks

/return

| -----------------------------------------------------------------------------
| -- Verifies the CH target is alive and in zone, otherwise moves on to the next CH tank it if exists, exits CH mode if it doesn't
| -----------------------------------------------------------------------------

Sub verifyCHTarget

	| ------------ Debug info
	/if (${Debug} || ${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: verifyCHTarget_010: chTarget = ${chTarget}

	| ---------- If chTarget is in zone, return to casting loop and cast
	/if (${SpawnCount[pc ${chTarget}]}) {

		| ------------ Debug info
        /if (${Debug} || ${chDebug}) /docommand /${chDebugChannel} ${Me.CleanName}: verifyCHTarget_020: Found ${chTarget} (${chTargetID}) alive for chain #${chChainMemberShip} - casting CH!

        /return
	} else {

		| ---------- chTarget isn't in the zone, move to the next chTank for the appropriate chain, or exit the CH chain entirely if no more tanks available
		/declare i int local

		/for i 1 to ${chChain${chChainMemberShip}Tanks.Size}

			| ------------ Debug info
        	/if (${Debug} || ${chDebug}) {
        		/docommand /${chDebugChannel} ${Me.CleanName}: verifyCHTarget_030: {i} = ${i}
        		/docommand /${chDebugChannel} ${Me.CleanName}: verifyCHTarget_031: {chChain${chChainMemberShip}Tanks[${i}]} = ${chChain${chChainMemberShip}Tanks[${i}]}
        	}

			| ---------- If this iterations tank = chTarget && {i} is less than the size of the array, set the chTarget to the next element in the array and return
			/if (${chChain${chChainMemberShip}Tanks[${i}].Equal[${chTarget}]} && (${i} < ${chChain${chChainMemberShip}Tanks.Size})) {
				/call event_setTank "NULL" ${chChainMemberShip} ${chChain${chChainMemberShip}Tanks[${Math.Calc[${i} + 1]}]}
				/return
			}

		/next i

	}

	| ---------- If you got this far, then you didn't find a tank to switch to - exit CH chain and get back to regular healing
	/call event_chDisabled

/return
