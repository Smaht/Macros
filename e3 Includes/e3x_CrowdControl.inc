| --------------------------------------------------------------------------------------|
| -- e3x_CrowdControlv1.9.6	( extended functionality designed to work with e3.mac )	   -|
| -- Written by Smaht							                                       -|
| --------------------------------------------------------------------------------------|
| -- Contains extended functions for e3.mac to provide crown control.	               -|
| --------------------------------------------------------------------------------------|
| -- Version 1.9,6:  - Immunity tested and functional                                  -|
| --                 - Added pets to "skipped" mob types                               -|
| --------------------------------------------------------------------------------------|
| -- Version 1.9,5:  - All major bugs worked out                                       -|
| --                 - Bards spell casting fixed                                       -|
| --                 - Fixes made for mobs than no longer have corpses                 -|
| --                 - Bards mez target list fixed to no more than 4 mobs              -|
| --                 - Imunity as yet untested                                         -|
| --------------------------------------------------------------------------------------|
| -- Version 1.0.0:  Initial Build                                                     -|
| --------------------------------------------------------------------------------------|


| --------------------------------------------------------------------------------------|
| -- Crown Control routine when called from Assist.inc                                 -|
| -- Prior to assisting, if crowdcontrol=TRUE in the .ini file, this sub is called	   -|
| -- If the following conditions:                                                      -|
| ----- Not already offtanked                                                          -|
| ----- NPC has aggro on a PC                                                          -|
| ----- The target is within a specific distance                                       -|
| ----- The target wasn't the original assist call                                     -|
| -- Then the following actions are taken:                                             -|
| ----- Turn AutoAggro on                                                              -|
| ----- Advertise in /bc the target you are offtanking                                 -|
| ----- Return to assist function with the new offtanking target                       -|
| -- Else:                                                                             -|
| ----- Ensure taunt is off, and exit                                                  -|
| --------------------------------------------------------------------------------------|


| ---------------------------------------------------------------------------------------------------------------|
| -- controlCrowdControl (CCC) is the routine run to determine who does what for crowd control 					-|
| -- This routine requires that a [Crowd Control] section exists in the .ini files 								-|
| -- This routine will work only for enchanter or bards															-|
| -- This routine is only run for the FIRST entry in of the "CC Members" 										-|
| -- The on/off function for this routine is therefore if {Me.CleanName} is the first entry in the lists		-|
| -- This routine will take the list of "CC Members" from the .ini file and share the CC 						-|
| -- amongst them, calling for CC to each of the Members 														-|
| ---------------------------------------------------------------------------------------------------------------|

sub controlCrowdControl(int mainAssistID)
	
	| ---------- Build the string used later on
	/call ArrayToString MyCCTargets crowdControlList

	| ---------- Some basic variables
	/declare controlCrowdControlDebug bool local TRUE
	/declare controlCrowdControlDebugChannel string local echo

	| ---------- Debug info
	/if (${Debug} || ${controlCrowdControlDebug}) {
	 	/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC000: Starting logs for controlCrowdControl
	 	/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC001: {mainAssistID} set to ${mainAssistID}
	 	/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC002: {crowdControlList} set to ${crowdControlList}
	}

	| ------------ Declare offtank leash range
	/declare xy_crowdControlRange int local 150
	/declare z_crowdControlRange int local 100

	| ------------ Declare number of potential offtank targets
	/declare crowdControlPotentialTargets int local ${SpawnCount[npc radius ${xy_crowdControlRange} zradius ${z_crowdControlRange}]}

	| ------------ If offtankPotentialTargets is <= 1, then exit function (MA has it ... probably)
	/if (${crowdControlPotentialTargets} <= 1) {

		| ------------ Debug Info
		/if (${Debug} || ${controlCrowdControlDebug}) /docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC010: Exiting routine as crowdControlPotentialTargets is <= 1
		
		/return ${mainAssistID}
	}
	
	| ------------ Debug Info
	/if (${Debug} || ${controlCrowdControlDebug}) {
		/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC020: xy_crowdControlRange set to ${xy_crowdControlRange}
		/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC021: z_crowdControlRange set to ${z_crowdControlRange}
		/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC022: crowdControlPotentialTargets set to ${crowdControlPotentialTargets}
	}

	/declare i int local
	/declare targetToCheck int local
	/declare lowPriTargets string local
	/declare highPriTargets string local
	/declare crowdControlTargetID int local

	| ------------ Debug Info
	/if (${Debug} || ${controlCrowdControlDebug}) /docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC030: protecteeList set to ${protecteeList}

	| ------------ Loop through all offtankPotentialTargets looking for ones not tanked by an offtank
	/for i 1 to ${crowdControlPotentialTargets}

		| ------------ Set targetToCheck to the ID of the mob to check on this loop
		/varset targetToCheck ${NearestSpawn[${i}, npc radius ${xy_crowdControlRange} zradius ${z_crowdControlRange}].ID}

		| ------------ Exit if targetToCheck would be of undesireable type OR targetToCheck is the mainAssistID
		/if (${Select[${Spawn[${targetToCheck}].Type},PC,Untargetable,Mount,Corpse,Chest,Trigger,Trap,Timer,Item,Pet]} || (${targetToCheck} == ${mainAssistID})) {
		
			| ------------ Debug Info
			/if (${Debug} || ${controlCrowdControlDebug}) {
				/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC040: Skipping i=${i} because targetToCheck (${targetToCheck}) is ${Spawn[${targetToCheck}].Type} OR
				/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC041: Skipping i=${i} because targetToCheck (${targetToCheck}) = mainAssistID (${mainAssistID})
			}
		
			/if (${i} == ${crowdControlPotentialTargets}) /goto :skipOutOfForLoop
			/next i
		}

		| ------------ Target the targetToCheck (then use ${Me.TargetOfTarget} to determine priority of targetToCheck)
		/call TrueTarget ${targetToCheck}

		| ------------ Short delay to allow TargetOfTarget to populate
		/delay 5

		| ------------ Debug Info
		/if (${Debug} || ${controlCrowdControlDebug}) {
			/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC050: i set to ${i}
			/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC051: targetToCheck set to ${targetToCheck}
			/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC052: ${Target.ID}
			/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC053: Me.TargetOfTarget for targetToCheck is ${Me.TargetOfTarget} (${Me.TargetOfTarget.ID})
		}

		| ------------ Check ${Me.TargetOfTarget} (the target of targetToCheck) for priority
		| ------------ If high-priority, start crowncontrolling immediately
		| ------------ If med-priority, finish for-loop looking for higher-priority targets
		| ------------ If low-priority, finish for-loop looking for higher-priority targets

		| ------------ High-priority check - targetToCheck has a PC targeted AND it's one of the listed protectees
		/if (${Me.TargetOfTarget.Type.Equal[PC]} && (${Select[${Me.TargetOfTarget},${protecteeList}]} > 0)) {
			
			/if (!${highPriTargets.Length}) {
				/varset highPriTargets ${targetToCheck}
			} else {
				/varset highPriTargets ${highPriTargets},${targetToCheck}
			}

			| ------------ Debug Info
			/if (${Debug} || ${controlCrowdControlDebug}) {
				/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC060: highPriTargets set to ${highPriTargets}
				/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC061: i is set to ${i}
				/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC062: High: (${Me.TargetOfTarget.Type.Equal[PC]} && ${Select[${Me.TargetOfTarget},${protecteeList}]})
			}
		}

		| ------------ Low-priority check - targetToCheck has a PC targeted AND it's one of the offtanks
		/if (${Me.TargetOfTarget.Type.Equal[PC]}) {
			/if (!${lowPriTargets.Length}) {
				/varset lowPriTargets ${targetToCheck}
			} else {
				/varset lowPriTargets ${lowPriTargets},${targetToCheck}
			}
			
			| ------------ Debug Info
			/if (${Debug} || ${controlCrowdControlDebug}) {
				/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC070: lowPriTargets set to ${lowPriTargets}
				/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC071: i is set to ${i}
				/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC072: Low: (${Me.TargetOfTarget.Type.Equal[PC]} && ${Select[${Me.TargetOfTarget},${offtankList}]})
			}
		}

	/next i
	:skipOutOfForLoop

	| ---------- Buid the lists for each crowd controller
	| ---------- Start by concatenating the lists above in priority order
	/declare crowdControlTargets string local

	| ---------- If highPriTargets has entries, add it first to the crowdControlTargets list
	/if (${highPriTargets.Length}) /varset crowdControlTargets ${highPriTargets}

	| ---------- If lowPriTargets has entries, add it to the crowdControlTargets list
	/if (${lowPriTargets.Length}) {

		/if (${crowdControlTargets.Length}) {
			/varset crowdControlTargets ${crowdControlTargets},${lowPriTargets}
		} else {
			/varset crowdControlTargets ${lowPriTargets}
		}
	}

	| ------------ Debug Info
	/if (${Debug} || ${controlCrowdControlDebug}) {
		/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC080: {highPriTargets} = ${highPriTargets}
		/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC081: {lowPriTargets} = ${lowPriTargets}
		/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC082: {crowdControlTargets} = ${crowdControlTargets}
	}	
	
	| ---------- Declare a list for each crowd controller in the list found
	| ---------- Where ${Math.Calc[${crowdControlList.Count[,]} + 1]} is the number of people in the list
 	/for i 1 to ${Math.Calc[${crowdControlList.Count[,]} + 1]}

 		/declare crowdController_${i}_List string local

 		| ---------- Debug info
 		/if (${Debug} || ${controlCrowdControlDebug}) /docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC090: Declared crowdController_${i}_List

 	/next i

 	| ---------- Write each list for each crowd controller
 	/declare j int local 1
 	/declare k int local 1
 	/declare l int local 0
 	/for j 1 to ${Math.Calc[${crowdControlTargets.Count[,]} + 1]}

 		| ---------- If the crowdController is a bard, and already has 4 in his list, skip him
 		:mezBardTooMany
 		/if (${Spawn[pc ${crowdControlList.Arg[${k},,]}].Class.ShortName.Equal[BRD]} && (${Math.Calc[${crowdController_${k}_List.Count[,]} + 1]} == 4)) {

 			| ---------- Exit if you have checked all chars and their lists are too long
 			/if (${l} ==  ${Math.Calc[${crowdControlList.Count[,]} + 1]}) /goto :endForLoop1

 			| ---------- Increase {k} to move the next entry to the next list
 			/if (${k} < ${Math.Calc[${crowdControlList.Count[,]} + 1]}) {
 				/varcalc k ${k} + 1
  			} else {
 				/varset k 1
 			}
 			
 			/varcalc l ${l} + 1
 			/goto :mezBardTooMany			
 		}

		| ---------- If the crowdController_X_List is empty, set it to the value, otherwise append it to the list
		/if (!${crowdController_${k}_List.Length}) {
			/varset crowdController_${k}_List ${crowdControlTargets.Arg[${j},,]}
		} else {
			/varset crowdController_${k}_List ${crowdController_${k}_List},${crowdControlTargets.Arg[${j},,]}
		}

 		| ---------- Increase {k} to move the next entry to the next list
 		/if (${k} < ${Math.Calc[${crowdControlList.Count[,]} + 1]}) {
 			/varcalc k ${k} + 1
  		} else {
 			/varset k 1
 		}

 		| ------------ Debug Info
		/if (${Debug} || ${controlCrowdControlDebug}) {
			/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC100: {j} = ${j}
			/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC101: {k} = ${k}
			/docommand /${controlCrowdControlDebugChannel} ${Me.CleanName}: CCC102: {crowdController_${k}_List} = ${crowdController_${k}_List}
		}

	:endForLoop1
	/next j

	| ---------- For each list call out the crowd control assignements and then move on to your own (list in reverse)
	/for i ${Math.Calc[${crowdControlList.Count[,]} + 1]} downto 1

		| ---------- Call out the mez list for everyone else
		/bc ${crowdControlList.Arg[${i},,]} mez the following: ${crowdController_${i}_List}

	:endForLoop2
	/next i

	| ---------- Cleanup
	/deletevar crowdControlList

/return ${mainAssistID}


| ---------------------------------------------------------------------------------------------------------------|
| -- This routine is for individual CC Members as defined in the character .ini files							-|
| -- The above definition need only exist in the "controlling members" .ini file 								-|
| -- The list of CC Members can be as many/few as desired														-|
| -- Each CC Member must however have a [Crowd Control] section in their .ini to define at a minimum			-|
| -- the "CC Spell" and optionally "CC Extension"																-|
| -- You must be either a BRD or ENC to operate in this section													-|
| ---------------------------------------------------------------------------------------------------------------|

#event crowdControlMobs "<#*#> #1# mez the following: #2#"
Sub event_crowdControlMobs(string line, string mezAgent, string targetMezMobs)

	| ---------- Exit if I am not the {mezAgent}, if I am not a bard/ench or if {mezSpell} isn't defined
	/if (!${Me.CleanName.Equal[${mezAgent}]} || !${Select[${Me.Class.ShortName},ENC,BRD]}) /return

	| ---------- Declare a few useful variables
	/declare mezZoneID int local ${Zone.ID}
	/declare crowdControlMobsDebug bool local TRUE
	/declare crowdControlMobsDebugChannel string local echo
	/declare mobsToMez bool local FALSE
	/declare mezSpell string local
	/declare mezSpellLevel string local
	/declare i int local

	| ---------- Define {mezSpell} and {mezSpellLevel}
	/varset mezSpell ${mezSpellFullInfo.Arg[1,/]}
	/if (${mezSpellFullInfo.Find[/Level|]}) {
			/call argueString Level "${mezSpellFullInfo}"
			/varset mezSpellLevel ${Macro.Return}
	}

	| ------------ Debug Info
	/if (${Debug} || ${crowdControlMobsDebug}) {
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM010: Starting logs for CCM
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM011: {mezSpell} = ${mezSpell}
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM012: {mezSpellLevel} = ${mezSpellLevel}
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM013: {mezSpellFullInfo} = ${mezSpellFullInfo}
	}

	/if (!${Defined[mezSpell]}) {
		/bc [+r+] {mezSpell} not defined!  Mezzing failed for ${targetMezMobs}!
		/return
	}
	
	| ---------- Prep for future mezzing - make sure spells are memmed
	/memorize "${mezSpell}|${DefaultGem}"
	/delay 10s !${Window[SpellBookWnd].Open}

	| ---------- Build and populate an array of entries according to the size/number of target mobs
	| ---------- Array structure:  Mob.ID | Macro.RunTime | Mezzable
	| ---------- Where mezzable mobs have a value of 1 (all mobs initially), and unmezzable mobs have this value set to 0
	| ---------- Where an unmezzable mob as defined here is either: a) truly unmezzable, b) a corpse, or c) of higher level than the spell allows
	/declare arrayTargetMobs[${Math.Calc[${targetMezMobs.Count[,]} + 1]},3] int local 1
	/for i 1 to ${Math.Calc[${targetMezMobs.Count[,]} + 1]}

		/varset arrayTargetMobs[${i},1] ${Int[${targetMezMobs.Arg[${i},,]}]}
		/if (${Spawn[id ${Int[${targetMezMobs.Arg[${i},,]}]}].Level} > ${mezSpellLevel} || !${mezSpellLevel}) /varset arrayTargetMobs[${i},3] 0

		| ------------ Debug Info
		/if (${Debug} || ${crowdControlMobsDebug}) {
			/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM020: {i} = ${i}
			/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM021: {arrayTargetMobs[${i},1]} = ${arrayTargetMobs[${i},1]}
			/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM022: {arrayTargetMobs[${i},2]} = ${arrayTargetMobs[${i},2]}
			/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM023: {arrayTargetMobs[${i},3]} = ${arrayTargetMobs[${i},3]}
			/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM024: {Spawn[id ${Int[${targetMezMobs.Arg[${i},,]}]}].Level} = ${Spawn[id ${Int[${targetMezMobs.Arg[${i},,]}]}].Level}
		}

	/next i

	| ---------- Alarm if you can't mez as many mobs as you've been given
	/if (${Defined[mezExtension]}) {
		/declare mezDuration float local ${Math.Calc[(60 * ${Spell[${mezSpell}].Duration} / 10) * (1 + (${mezExtension} / 100))]}
	} else {
		/declare mezDuration float local ${Math.Calc[60 * ${Spell[${mezSpell}].Duration} / 10]}
	}
	/if (${Me.Class.ShortName.Equal[BRD]}) /declare mezCastCycle float local 3.2
	/if (${Me.Class.ShortName.Equal[ENC]}) /declare mezCastCycle float local ${Math.Calc[${Spell[${mezSpell}].MyCastTime} + 2]}

	| ---------- If the difference between what you can mez, and what you can mez is < 0, alarm
	| -- ${Float[${Math.Calc[${mezDuration} / ${mezCastCycle}]}].Int} = Number of mobs you can mez
	| -- ${Math.Calc[${targetMezMobs.Count[,]} + 1]} = Number of mobs you have been asked to mez
	/if (${Math.Calc[${Float[${Math.Calc[${mezDuration} / ${mezCastCycle}]}].Int} - ${Math.Calc[${targetMezMobs.Count[,]} + 1]}]} < 0) /bc [+r+] I have been asked to mez too many mobs! (${Math.Calc[${Float[${Math.Calc[${mezDuration} / ${mezCastCycle}]}].Int} - ${Math.Calc[${targetMezMobs.Count[,]} + 1]}]})

	| ------------ Debug Info
	/if (${Debug} || ${crowdControlMobsDebug}) {
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM030: {mezDuration} = ${mezDuration}
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM031: {mezCastCycle} = ${mezCastCycle}
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM032: {Float[${Math.Calc[${mezDuration} / ${mezCastCycle}]}].Int} = ${Float[${Math.Calc[${mezDuration} / ${mezCastCycle}]}].Int}
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM033: {Math.Calc[${targetMezMobs.Count[,]} + 1]} = ${Math.Calc[${targetMezMobs.Count[,]} + 1]}
		/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM034: {Math.Calc[${Float[${Math.Calc[${mezDuration} / ${mezCastCycle}]}].Int} - ${Math.Calc[${targetMezMobs.Count[,]} = ${Math.Calc[${Float[${Math.Calc[${mezDuration} / ${mezCastCycle}]}].Int} - ${Math.Calc[${targetMezMobs.Count[,]}]}]}
	}

	| ---------- Mez loop
	:mezLoop

	| ---------- Exit if you zone or die
	/if (${Zone.ID} != ${mezZoneID}) {
		| ------------ Debug Info
		/if (${Debug} || ${crowdControlMobsDebug}) /docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM040: Exiting CCM because {Zone.ID} (${Zone.ID}) does not equal {mezZoneID} (${mezZoneID})
		/return
	}

	| ---------- Loop through array, check to see if any mobs are left to mez (${arrayTargetMobs[${i},4]} = 1)
	/varset mobsToMez FALSE
	/for i 1 to ${Math.Calc[${targetMezMobs.Count[,]} + 1]}
		/if (${arrayTargetMobs[${i},3]} == 1) /varset mobsToMez TRUE
	/next i

	| ---------- Exit this routine if there are no longer any mobs to mez
	/if (!${mobsToMez}) {
		| ------------ Debug Info
		/if (${Debug} || ${crowdControlMobsDebug}) /docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM050: Exiting CCM as there are no longer any mobs to mez
		/return
	}

	| ---------- Loop through mobs, mez them as they come up
	/for i 1 to ${Math.Calc[${targetMezMobs.Count[,]} + 1]}

		| ---------- Mark the mob as dead (0) if it's a corpse
		/if (${Spawn[id ${Int[${targetMezMobs.Arg[${i},,]}]}].Type.Equal[Corpse]} || !${Spawn[id ${Int[${targetMezMobs.Arg[${i},,]}]}].ID}) /varset arrayTargetMobs[${i},3] 0

		| ---------- Exit to the next mob if it's been previously marked as dead or unmezzable
		/if (${arrayTargetMobs[${i},3]} == 0) {

			| ------------ Debug Info
			/if (${Debug} || ${crowdControlMobsDebug}) {
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM060: {i} = ${i}
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM061: {arrayTargetMobs[${i},1]} = ${arrayTargetMobs[${i},1]} (Mob.ID)
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM062: {arrayTargetMobs[${i},2]} = ${arrayTargetMobs[${i},2]} (Macro.RunTime)
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM063: {arrayTargetMobs[${i},3]} = ${arrayTargetMobs[${i},3]} (Dead/Ummezzable)
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM064: {Target.Type.Equal[Corpse]} = ${Target.Type.Equal[Corpse]}
			}

			/goto :endMezLoop
		}

		| ---------- Target the mob
		/call TrueTarget ${arrayTargetMobs[${i},1]}
		/delay 3

		| ---------- IF {the mob has no target (still mezzed) AND the Macro.RunTime is less than the mez timer), skip
		/if (${Me.TargetOfTarget.Type.Equal[PC]} && ${Macro.RunTime} < ${arrayTargetMobs[${i},2]}) {
		
			| ------------ Debug Info
			/if (${Debug} || ${crowdControlMobsDebug}) {
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM070: {i} = ${i}
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM071: {arrayTargetMobs[${i},1]} = ${arrayTargetMobs[${i},1]} (Mob.ID)
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM072: {arrayTargetMobs[${i},2]} = ${arrayTargetMobs[${i},2]} (Macro.RunTime)
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM073: {arrayTargetMobs[${i},3]} = ${arrayTargetMobs[${i},3]} (Dead/Ummezzable)
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM074: {Me.TargetOfTarget.Type.Equal[PC]} = ${Me.TargetOfTarget.Type.Equal[PC]}
				/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM075: {Macro.RunTime} = ${Macro.RunTime}
			}

			/goto :endMezLoop
		}

		| ---------- Cast mez, loop if unsuccessful
		:recastMez
		/if (${Me.Class.ShortName.Equal[ENC]}) {
			/call e3_Cast ${arrayTargetMobs[${i},1]} "${mezSpell}"
			:completeTheCast
			/if (${Me.Casting.ID}) {
				/delay 2
				/goto :completeTheCast
			}
/echo ${castReturn}
		}

		/if (${Me.Class.ShortName.Equal[BRD]}) {

			/twist stop
			/twist once ${Me.Gem[${mezSpell}]}
			/delay 38
			/twist stop

			| ---------- Check for resists
			/varset castReturn SUCCESS
			/doevents Resisted2
			/doevents Resisted
			/doevents Immune
		}

		| ---------- Mark the mob as unmezzable (0) if it's unmezzable
		/if (${castReturn.Equal[IMMUNE]}) {

			| ------------ Debug Info
			/if (${Debug} || ${crowdControlMobsDebug}) /docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM080: Setting {arrayTargetMobs[${i},3]} to 0 as the mob is unmezzable

			/varset arrayTargetMobs[${i},3] 0
			/bc [+y+] Warning! ${Spawn[id ${arrayTargetMobs[${i},1]}].CleanName} (ID: ${arrayTargetMobs[${i},1]}) is not mezzable!
			/goto :endMezLoop
		}
		/if (!${castReturn.Equal[SUCCESS]}) {

			| ------------ Debug Info
			/if (${Debug} || ${crowdControlMobsDebug}) /docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM090: Looping to :recastMez as the cast was unsuccessful

			/delay 3
			/if (!${Target.ID}) /goto :endMezLoop
			/goto :recastMez
		}

		| ---------- Update the timer
		| ---------- Timer is set to: CurrentTime + mezDuration - Setup/Recast. ie when should you be recasting by
		| ---------- Where Setup/Recast time is:  CastTime + 2.5secs of Spell Recovery
		/varset arrayTargetMobs[${i},2] ${Math.Calc[${Macro.RunTime} + ${mezDuration} - ${Spell[${mezSpell}].CastTime} - 3]}

		:endMezLoop
		| ------------ Debug Info
		/if (${Debug} || ${crowdControlMobsDebug}) {
			/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM100: {castReturn} = ${castReturn} (Success of the {mezSell} cast)
			/docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM101: {arrayTargetMobs[${i},2]} = ${arrayTargetMobs[${i},2]}
		}

	/next i
	/goto :mezLoop

	| ------------ Debug Info
	/if (${Debug} || ${crowdControlMobsDebug}) /docommand /${crowdControlMobsDebugChannel} ${Me.CleanName}: CCM110: Should never get to this point, should exit elsewhere

	/if (${Me.Class.ShortName.Equal[BRD]}) /twist ${songSet}

/return
