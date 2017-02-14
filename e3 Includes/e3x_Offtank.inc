| --------------------------------------------------------------------------------------|
| -- e3x_Offtank v1.0	( extended functionality designed to work with e3.mac )	       -|
| -- Written by Smaht							                                       -|
| --------------------------------------------------------------------------------------|
| -- Contains extended functions for e3.mac to provide offtanking.		               -|
| --------------------------------------------------------------------------------------|


| --------------------------------------------------------------------------------------|
| -- Offtank routine when called from Assist.inc                                       -|
| -- Prior to assisting, if offtank=TRUE in the .ini file, this sub is called		   -|
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

sub offtankMobs(int mainAssistID)

	/declare offtankDebug bool local FALSE

	| ------------ Turn Taunt Off
	/varset AutoAggro FALSE

	| ------------ Debug Info
	/if (${Debug} || ${offtankDebug}) {
		/echo 00: AutoAggro set to ${AutoAggro}
		/echo 01: mainAssistID set to ${mainAssistID}
		/echo 02: offtankList set to ${offtankList}
	}

	| ------------ Declare offtank leash range
	/declare xy_offtankLeashRange int local 150
	/declare z_offtankLeashRange int local 100

	| ------------ Declare number of potential offtank targets
	/declare offtankPotentialTargets int local ${SpawnCount[npc radius ${xy_offtankLeashRange} zradius ${z_offtankLeashRange}]}

	| ------------ If offtankPotentialTargets is <= 1, then exit function (MA has it ... probably)
	/if (${offtankPotentialTargets} <= 1) {

		| ------------ Debug Info
		/if (${Debug} || ${offtankDebug}) /echo 10: Exiting routine as offtankPotentialTargets is <= 1
		
		/return ${mainAssistID}
	}
	
	| ------------ Debug Info
	/if (${Debug} || ${offtankDebug}) {
		/echo 20: xy_offtankLeashRange set to ${xy_offtankLeashRange}
		/echo 21: z_offtankLeashRange set to ${z_offtankLeashRange}
		/echo 22: offtankPotentialTargets set to ${offtankPotentialTargets}
	}

	/declare i int local
	/declare targetToCheck int local
	/declare lowPriTargets string local
	/declare medPriTargets string local
	/declare highPriTarget int local
	/declare offtankTargetID int local

	| ------------ Debug Info
	/if (${Debug} || ${offtankDebug}) /echo 30: protecteeList set to ${protecteeList}

	| ------------ Loop through all offtankPotentialTargets looking for ones not tanked by an offtank
	/for i 1 to ${offtankPotentialTargets}

		| ------------ Set targetToCheck to the ID of the mob to check on this loop
		/varset targetToCheck ${NearestSpawn[${i}, npc radius ${xy_offtankLeashRange} zradius ${z_offtankLeashRange}].ID}

		| ------------ Exit if targetToCheck would be of undesireable type OR targetToCheck is the mainAssistID
		/if (${Select[${Spawn[${targetToCheck}].Type},PC,Untargetable,Mount,Corpse,Chest,Trigger,Trap,Timer,Item]} || (${targetToCheck} == ${mainAssistID})) {
		
			| ------------ Debug Info
			/if (${Debug} || ${offtankDebug}) {
				/echo 40: Skipping i=${i} because targetToCheck (${targetToCheck}) is ${Spawn[${targetToCheck}].Type} OR
				/echo 41: Skipping 1=${i} because targetToCheck (${targetToCheck}) = mainAssistID (${mainAssistID})
			}
		
			/if (${i} == ${offtankPotentialTargets}) /goto :skipOutOfForLoop
			/next i
		}

		| ------------ Target the targetToCheck (then use ${Me.TargetOfTarget} to determine priority of targetToCheck)
		/call TrueTarget ${targetToCheck}

		| ------------ Short delay to allow TargetOfTarget to populate
		/delay 5

		| ------------ Debug Info
		/if (${Debug} || ${offtankDebug}) {
			/echo 50: i set to ${i}
			/echo 51: targetToCheck set to ${targetToCheck}
			/echo 52: ${Target.ID}
			/echo 53: Me.TargetOfTarget for targetToCheck is ${Me.TargetOfTarget} (${Me.TargetOfTarget.ID})
		}

		| ------------ Check ${Me.TargetOfTarget} (the target of targetToCheck) for priority
		| ------------ If high-priority, start offtanking immediately
		| ------------ If med-priority, finish for-loop looking for higher-priority targets
		| ------------ If low-priority, finish for-loop looking for higher-priority targets

		| ------------ High-priority check - targetToCheck has a PC targeted AND it's one of the listed importantbots
		/if (${Me.TargetOfTarget.Type.Equal[PC]} && (${Select[${Me.TargetOfTarget},${protecteeList}]} > 0)) {
			/varset offtankTargetID ${targetToCheck}

		| ------------ Debug Info
		/if (${Debug} || ${offtankDebug}) {
			/echo 60: High: (${Me.TargetOfTarget.Type.Equal[PC]} && ${Select[${Me.TargetOfTarget},${protecteeList}]})
			/echo 61: offtankTargetID set to ${offtankTargetID}
		}

		/goto :skipOutOfForLoop
		}

		| ------------ Med-priority check - targetToCheck has a PC targeted AND it's not one of the offtanks
		/if (${Me.TargetOfTarget.Type.Equal[PC]} && !${Select[${Me.TargetOfTarget},${offtankList}]}) {
			
			/if (!${medPriTargets.Length}) {
				/varset medPriTargets ${targetToCheck}
			} else {
				/varset medPriTargets ${medPriTargets},${targetToCheck}
			}

			| ------------ Debug Info
			/if (${Debug} || ${offtankDebug}) {
				/echo 70: medPriTargets set to ${medPriTargets}
				/echo 71: i is set to ${i}
				/echo 72: Med: (${Me.TargetOfTarget.Type.Equal[PC]} && !${Select[${Me.TargetOfTarget},${offtankList}]})
			}
		}

		| ------------ Low-priority check - targetToCheck has a PC targeted AND it's one of the offtanks
		/if (${Me.TargetOfTarget.Type.Equal[PC]} && ${Select[${Me.TargetOfTarget},${offtankList}]}) {
			/if (!${lowPriTargets.Length}) {
				/varset lowPriTargets ${targetToCheck}
			} else {
				/varset lowPriTargets ${lowPriTargets},${targetToCheck}
			}
			
			| ------------ Debug Info
			/if (${Debug} || ${offtankDebug}) {
				/echo 80: lowPriTargets set to ${lowPriTargets}
				/echo 81: i is set to ${i}
				/echo 82: Low: (${Me.TargetOfTarget.Type.Equal[PC]} && ${Select[${Me.TargetOfTarget},${offtankList}]})
			}
		}

	/next i

	:skipOutOfForLoop

	| ------------ If offtankTargetID is already set, then it's a high-priority target - skip to tanking
	/if (${offtankTargetID}) {
		
		| ------------ Debug Info
		/if (${Debug} || ${offtankDebug}) /echo 90: High Priority Target Chosen: offtankTargetID set to ${offtankTargetID}

		/goto :beginTanking
	}

	| ------------ If medPriTargets has a length, then pick one (the first, it's closest) and begin tanking
	/if (${medPriTargets.Length}) {

		| ------------ Pick a random target from the list, where:
		| ------------ b = ${Math.Calc[${medPriTargets.Count[,]} + 1]} is the number of elements in the list
		| ------------ And where ${Math.Calc[${Math.Rand[b]} + 1]} is a random element of it
		/declare b int local ${Math.Calc[${Math.Rand[${Math.Calc[${medPriTargets.Count[,]} + 1]}]} + 1]}

		| ------------ Set the offtankTargetID to the random element {b} as defined above
		/varset offtankTargetID ${medPriTargets.Arg[${b},,]}

		| ------------ Debug Info
		/if (${Debug} || ${offtankDebug}) {
			/echo 100: Med Priority Target Chosen: offtankTargetID set to ${offtankTargetID}
			/echo 101: medPriTargets set to ${medPriTargets} | ${medPriTargets.Arg[${b},,]} should be offtankTargetID
			/echo 102: Random element number chosen: ${b}
		}

		/goto :beginTanking
	}

	| ------------ If lowPriTargets has a length, then pick one (the first, it's closest) and begin tanking
	/if (${lowPriTargets.Length}) {

		| ------------ Pick a random target from the list, where:
		| ------------ c = ${Math.Calc[${lowPriTargets.Count[,]} + 1]} is the number of elements in the list
		| ------------ And where ${Math.Calc[${Math.Rand[c]} + 1]} is a random element of it
		/declare c int local ${Math.Calc[${Math.Rand[${Math.Calc[${lowPriTargets.Count[,]} + 1]}]} + 1]}

		| ------------ Set the offtankTargetID to the random element {c} as defined above
		/varset offtankTargetID ${lowPriTargets.Arg[${c},,]}

		| ------------ Debug Info
		/if (${Debug} || ${offtankDebug}) {
			/echo 110: Low Priority Target Chosen: offtankTargetID set to ${offtankTargetID}
			/echo 111: lowPriTargets set to ${lowPriTargets} | ${lowPriTargets.Arg[${c},,]} should be offtankTargetID
			/echo 112: Random element number chosen: ${c}
		}

		/goto :beginTanking
	}

	| ------------ Should never get this far, but if it does, return the to Assist.inc with the original target (no offtanking)
	| ------------ Debug Info
		/if (${Debug} || ${offtankDebug}) {
			/echo 120:  Script should never get to this point.  Should have skipped to :beginTanking
			/echo 121: Script probably here because there were untargetable mob types in the SpawnCount
			/echo 122: Returning with mainAssistID (${mainAssistID})
		}
	
	/return ${mainAssistID}

	:beginTanking

|**
	| ---------- Commented out, while it works, there are simply too many "baby names" that the tank will then ignore

	| ---------- If the mob is a named mob, don't tank, go back to {mainAssistID}
	/if (${Spawn[named id ${offtankTargetID}].ID}) {
		/bc [+y+] NOT Offtanking: ${Spawn[${offtankTargetID}].CleanName} (ID: ${offtankTargetID})
		/return ${mainAssistID}
	}
**|
	| ------------ Turn on AutoAggro as there is an offtanking target to tank
	/varset AutoAggro TRUE

	| ------------ Debug Info
	/if (${Debug} || ${offtankDebug}) {
		/echo 130: offtankTargetID set to ${offtankTargetID}
		/echo 131: AutoAggro set to ${AutoAggro}
	}	
	
	/bc [+r+] Offtanking: ${Spawn[${offtankTargetID}].CleanName} (ID: ${offtankTargetID})
	
/return ${offtankTargetID}

| ----------------------------------------------------|
| -- Offtank status check 							 -|
| ----------------------------------------------------|

#event offtankStatus "#*# Offtank status"
Sub event_offtankStatus

	/declare tempAssistTarget int local

	/echo 01: Target.ID set to ${Target.ID}

	/call offtankMobs ${Target.ID}
	/varset tempAssistTarget ${Macro.Return}

	/echo 02: tempAssistTarget set to ${tempAssistTarget}

/return
