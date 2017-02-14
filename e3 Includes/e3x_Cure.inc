
| ---------------------------------------------------------
| -- Cure routine
| ---------------------------------------------------------

| ---------------------------------------------------------
| -- Changelog
| -- April 02, 2016: Modified to solve for range related issues (ie skip if not in range)

#event cureEnable "#*#Cure Enable: #1#'"
#event cureEnable "#*#Cure Enable: #1#"
Sub event_cureEnable(line, cureAgent)

	| ------------ If you aren't the cureAgent, exit
	/if (!${Me.Name.Equal[${cureAgent}]}) /return CURE_AGENT_INVALID

	| ------------ If you aren't a PAL,CLR,DRU,SHM exit
	/if (!${Select[${Me.Class.ShortName},PAL,CLR,DRU,SHM,BST]}) /return CURE_CLASS_INVALID

	| ------------ Notify start of cure mode
	/bc [+r+] Cure mode started!

	/declare cureDebug bool local FALSE

	/if (${Defined[cureExitVar]}) {
		/varset cureExitVar FALSE 
		} else {
		/declare cureExitVar bool outer FALSE
		}	

	| ------------ Set priority order for Tanks, Protectees and all other bots (otherbotList)
	| ------------ Values can be FIRST, SECOND, THIRD or ONLY (not a lot of error checking, be smart)
	/declare cureTankPri string local FIRST
	/declare cureProtecteePri string local SECOND
	/declare cureAllPri string local THIRD

	/declare cureList string local

	| ------------ Create a list of "other bots" equal to NetBots.List minus the offtankList and protecteeList
	/declare otherbotList string local
	/declare i int local

	/for i 1 to ${NetBots.Counts}

		| ------------ If you are NOT a member of either the offtankList or the protecteeList, add to otherbotList
		/if (!(${Select[${NetBots.Client.Arg[${i}]},${offtankList}]} || ${Select[${NetBots.Client.Arg[${i}]},${protecteeList}]})) {

			| ------------ If the otherbotList is empty, set it to {NetBots.Client.Arg[i]}, otherwise append {NetBots.Client.Arg[i]} to the list
			/if (!${otherbotList.Length}) {
				/varset otherbotList ${NetBots.Client.Arg[${i}]}
			} else {
				/varset otherbotList ${otherbotList},${NetBots.Client.Arg[${i}]}
			}

			| ------------ Debug Info
			/if (${cureDebug} || ${Debug}) {
				/echo 00: i set to ${i}
				/echo 01: NetBots.Client.Arg[${i}] set to ${NetBots.Client.Arg[${i}]}
				/echo 02: otherbotList set to ${otherbotList}
				/echo 03: Length of otherbotList is ${Math.Calc[${otherbotList.Count[,]} + 1]}
			}

		}
	/next i

	| ------------ Set cureList in order of priority based on Priorities set above
	| ------------ If one of the priorities is set to ONLY, set cureList and skip to startCure
	/if (${cureTankPri.Equal[ONLY]} || ${cureProtecteePri.Equal[ONLY]} || ${cureAllPri.Equal[ONLY]}) {
			/if (${cureTankPri.Equal[ONLY]}) /varset cureList ${offtankList}
			/if (${cureProtecteePri.Equal[ONLY]}) /varset cureList ${protecteeList}
			/if (${cureAllPri.Equal[ONLY]}) /varset cureList ${otherbotList}
			/goto :startCure
	}

	| ------------ If no groups set to ONLY, then create cureList based on order of Priority
	/declare j int local
	/for j 1 to 3

		/if (${j} == 1) {
			/if (${cureTankPri.Equal[FIRST]} && ${offtankList.Length}) /varset cureList ${offtankList}
			/if (${cureProtecteePri.Equal[FIRST]} && ${protecteeList.Length}) /varset cureList ${protecteeList}
			/if (${cureAllPri.Equal[FIRST]} && ${otherbotList.Length}) /varset cureList ${otherbotList}
		} else /if (${j} == 2) {
			/if (${cureTankPri.Equal[SECOND]} && ${offtankList.Length}) /varset cureList ${cureList},${offtankList}
			/if (${cureProtecteePri.Equal[SECOND]} && ${protecteeList.Length}) /varset cureList ${cureList},${protecteeList}
			/if (${cureAllPri.Equal[SECOND]} && ${otherbotList.Length}) /varset cureList ${cureList},${otherbotList}
		} else {
			/if (${cureTankPri.Equal[THIRD]} && ${offtankList.Length}) /varset cureList ${cureList},${offtankList}
			/if (${cureProtecteePri.Equal[THIRD]} && ${protecteeList.Length}) /varset cureList ${cureList},${protecteeList}
			/if (${cureAllPri.Equal[THIRD]} && ${otherbotList.Length}) /varset cureList ${cureList},${otherbotList}
		}

	/next j

	/declare p int local
	/declare q int local
	/declare cureListLength int local ${Math.Calc[${cureList.Count[,]} + 1]}
	:startCure

	| ------------ Debug Info
	/if (${cureDebug} || ${Debug}) {
		/echo 10: cureList is ${cureList}
		/echo 11: length of cureList is ${cureListLength}
	}

	| ------------ Loop through all entries in the cureList
	/for p 1 to ${cureListLength}

/stick off
/attack off

		| ------------ Reverse the direction of lookups if you aren't a paladin (paladin being my primary cure bot).  Part #1
		/if (!${Me.Class.ShortName.Equal[PAL]}) {
			/varset q ${p}
			/varset p ${Math.Calc[${cureListLength} - ${p} + 1]}
		}


		:startp

		| ------------ For each entry in the cureList, call determineTargetDebuff to see if they are debuffed
		/call determineTargetDebuff ${cureList.Arg[${p},,]}

		| ------------ Debug Info
		/if (${cureDebug} || ${Debug}) {
			/echo 20: p set to ${p}
			/echo 21: cureList[${p}] set to ${cureList.Arg[${p},,]}
			/echo 22: Macro.Return from determineTargetDebuff set to ${Macro.Return}
			/echo 23: targetDetrimental set to ${Macro.Return.Arg[1,_]}
			/echo 24: cureTargetDetrimentalSpellID set to ${Macro.Return.Arg[2,_]}
			/echo 25: targetDetrimentalType set to ${Macro.Return.Arg[3,_]}
			/echo 26: targetDetrimentalCounters set to ${Macro.Return.Arg[4,_]}
		}

		| ------------ If they are debuffed as per determineTargetDebuff, call cleanseTargetDebuff
		| ------------ Only apply for targetDetrimentalType Poison, Disease or Curse AND if the spell aliases file contains an entry for this type of cure for your class AND if the distance to the cureTarget is less than 100units away
		/if (${Macro.Return.Arg[1,_]} && (${Macro.Return.Arg[3,_].Equal[Poison]} || ${Macro.Return.Arg[3,_].Equal[Disease]} || ${Macro.Return.Arg[3,_].Equal[Curse]}) && ${Ini[${SpellAliases_Ini},${Me.Class},cure${Macro.Return.Arg[3,_]}].Length} && (${Spawn[=${cureList.Arg[${p},,]}].Distance} < 100)) {

			/call cleanseTargetDebuff ${cureList.Arg[${p},,]} ${Macro.Return.Arg[2,_]} ${Macro.Return.Arg[3,_]} ${Macro.Return.Arg[4,_]}

			| ------------ Debug Info
			/if (${cureDebug} || ${Debug}) {
					/echo 30: Macro.Return set to ${Macro.Return}
			}

			| ------------ Short delay inserted after a cleanse to allow for NetBots buff refresh timer
			/delay 5
			/goto :startp
		}

		| ------------ Reverse the direction of lookups if you aren't a paladin (paladin being my primary cure bot).  Part #2
		/if (!${Me.Class.ShortName.Equal[PAL]}) /varset p ${q}

	/next p
	
	| ------------ Uncomment the following to force e3x_Cure.inc to only run for one iteration.  Leave it commented to loop it until cureExitVar is set
	|/goto :endCure

	| ------------ Check to see if YOU need to be running
	/doevents cursed

	| ------------ Check to see if you should stop
	/doevents cureDisable
	/if (${cureExitVar}) /goto :endCure

	| ------------ Loop again until event_cureDisable is called
	/goto :startCure

	:endCure
	/target clear

	| ------------ Notify end of cure mode
	/bc [+r+] Cure mode ended!

	| ------------ Cleanup outer variables
	/deletevar cureExitVar

/return

| ---------------------------------------------------------
| -- Disable Cures
| ---------------------------------------------------------


#event cureDisable "#*#Cure Disable#*#"
Sub event_cureDisable

    | ------------ If not already curing, ignore this event
    /if (!${Defined[cureExitVar]}) /return NOT_CURE_MEMBER

    | ------------ Set the cureExitVar to TRUE to force an exit of the cure loop
	/varset cureExitVar TRUE

/return

| ---------------------------------------------------------
| -- Take a character string as input, and return data regarding the debuff status of the character string
| -- This does not take into account multiple debuffs.  It simply returns the first one
| ---------------------------------------------------------

Sub determineTargetDebuff(cureTarget)

	/declare cureDebug bool local FALSE

	| ------------ Macro.Return values
	/declare cureTargetDetrimental bool local FALSE
	/declare cureTargetDetrimentalSpell string local NULL
	/declare cureTargetDetrimentalSpellID int local 0
	/declare cureTargetDetrimentalType string local FALSE
	/declare cureTargetDetrimentalCounters int local 0

	| ------------ Other variables
	/declare cureTargetBuffList string local ${NetBots[${cureTarget}].Buff}

	| ------------ Debug Info
	/if (${cureDebug} || ${Debug}) {
		/echo 100: cureTarget set to ${cureTarget}
		/echo 101: cureTargetBuffList set to ${cureTargetBuffList}
		/echo 102: length of cureTargetBuffList set to ${cureTargetBuffList.Count[ ]}
	}

	| ------------ For each buff found, loop through to determine type
	/declare k int local 0
	/for k 1 to ${cureTargetBuffList.Count[ ]}

		| ------------ If the buff in question is of type detrimental, set some variables and cureExitVar
		/if (${Debuff[${cureTargetBuffList.Arg[${k}]}].Detrimentals}) {

			/varset cureTargetDetrimental TRUE
			/varset cureTargetDetrimentalSpell ${Spell[${cureTargetBuffList.Arg[${k}]}]}
			/varset cureTargetDetrimentalSpellID ${cureTargetBuffList.Arg[${k}]}
			/varset cureTargetDetrimentalType ${Spell[${cureTargetBuffList.Arg[${k}]}].CounterType}
			/varset cureTargetDetrimentalCounters ${Debuff[${cureTargetBuffList.Arg[${k}]}].Counters}

			| ------------ Debug Info
			/if (${cureDebug} || ${Debug}) {
				/echo 110: k set to ${k}
				/echo 111: cureTargetDetrimental set to ${cureTargetDetrimental}
				/echo 112: cureTargetDetrimentalSpell set to ${cureTargetDetrimentalSpell}
				/echo 113: cureTargetDetrimentalSpellID set to ${cureTargetDetrimentalSpellID}
				/echo 114: cureTargetDetrimentalType set to ${cureTargetDetrimentalType}
				/echo 115: cureTargetDetrimentalCounters set to ${cureTargetDetrimentalCounters}
			}

			| ------------ If the debuff you found is curable (of type Poison, Disease or Curse) then exit with these paramters, otherwise keep looking
			/if ((${cureTargetDetrimentalType.Equal[Poison]} || ${cureTargetDetrimentalType.Equal[Disease]} || ${cureTargetDetrimentalType.Equal[Curse]}) && ${Ini[${SpellAliases_Ini},${Me.Class},cure${cureTargetDetrimentalType}].Length}) {

				/if (${cureDebug} || ${Debug}) /echo 120: Found a spell of type Poison, Disease, Curse.  Exiting for cleanse

				/goto :exit
			}
		}
	/next k

	:exit

/return ${cureTargetDetrimental}_${cureTargetDetrimentalSpellID}_${cureTargetDetrimentalType}_${cureTargetDetrimentalCounters}

| ---------------------------------------------------------
| -- Cleanses the debuff from the target provided
| -- This assumes that spell aliases called curePoison, cureDisease and cureCurse are setup in the spell aliases file
| ---------------------------------------------------------

Sub cleanseTargetDebuff(cureTarget, cureSpellID, cureType, cureCounters)

	/declare cureDebug bool local FALSE

	/if (${cureDebug} || ${Debug}) {
		/echo 200: cureTarget set to ${cureTarget}
		/echo 201: cureSpellID set to ${cureSpellID}
		/echo 202: cureType set to ${cureType}
		/echo 203: cureCounters set to ${cureCounters}
	}

	| ------------ If the spell aliases file doesn't contain the appropriate cureType, exit
	| ------------ Otherwise set cureSpellName from the spell aliases file based upon cureType
	| ------------ Otherwise set cureSpellToCast to cureSpellName with a GiveUpTimer (required for e3_cast or it won't mem spells)
	/if (!${Ini[${SpellAliases_Ini},${Me.Class},cure${cureType}].Length}) {
		/return INVALID_CURE
	} else {
		/declare cureSpellName string local ${Ini[${SpellAliases_Ini},${Me.Class},cure${cureType}]}
		/declare cureSpellToCast string local ${cureSpellName}/GiveUpTimer|${Math.Calc[${Spell[${cureSpellName}].RecastTime}+30]}s
	}

	/if (${cureDebug} || ${Debug}) {
		/echo 210: cureSpellName set to ${cureSpellName}
		/echo 211: cureSpellToCast set to ${cureSpellToCast}
	}

	:curingTarget

	| ------------ Check to see if debuff is still on cureTarget, exit if cleansed (by this char or another) OR if the target has moved out of range
	/if (!${Select[${cureSpellID},${NetBots[${cureTarget}].Buff.Replace[ ,,]}]} || (${Spawn[=${cureTarget}].Distance} > 100)) /return

	/if (!${Defined[cureSpellDelay]}) /declare cureSpellDelay int local 1

	| ------------ If {cureSpellName} is a spell from your spell book
	/if (${Me.Book[${cureSpellName}]}) {

		| ----------- Determine spell delay time
		/varset cureSpellDelay ${Spell[${cureSpellName}].MyCastTime}

		| ------------ Cast appropriate cure spell as defined by cureType
		/bc [+y+] Casting ${cureSpellName} on ${cureTarget} to cure ${Spell[${cureSpellID}]}
		/call e3_cast ${NetBots[${cureTarget}].ID} "${cureSpellToCast}"

		| ------------ Debug Info
		/if (${cureDebug} || ${Debug}) {
			/echo 220: Macro.Return for cure cast in cleanseTargetDebuff is ${Macro.Return}
			/echo 221: Will wait ${Math.Calc[${cureSpellDelay} + 0.5]}s (MyCastTime/ItemCastTime + .5s) before continuing
		}
	
		/delay ${Math.Calc[${cureSpellDelay} + 0.5]}s
	}

	| ------------ If {cureSpellName} is from an item
	/if (${FindItem[${cureSpellName}].ID}) {

		| ----------- Determine spell delay time
		/varset cureSpellDelay ${FindItem[${cureSpellName}].CastTime}

		| ----------- Determine some item info
		/if (!${Defined[cureSpellItemSlot]}) {
			/declare cureSpellItemSlot string local ${FindItem[${cureSpellName}].WornSlot[1].Name}
		} else {
			/varset cureSpellItemSlot ${FindItem[${cureSpellName}].WornSlot[1].Name}
		}

		/if (!${Defined[cureSpellItemName]}) {
			/declare cureSpellItemName string local ${InvSlot[${cureSpellItemSlot}].Item.Name}
		} else {
			/varset cureSpellItemName ${InvSlot[${cureSpellItemSlot}].Item.Name}
		}

		| ------------ Debug Info
		/if (${cureDebug} || ${Debug}) {
			/echo 230: cureSpellItemSlot set to ${cureSpellItemSlot}
			/echo 231: cureSpellItemName set to ${cureSpellItemName}
			/echo 232: ${NetBots[${cureTarget}].ID} "${cureSpellToCast}"
			/echo 233:  ${Math.Calc[${Spell[${cureSpellName}].MyCastTime} + 0.5]}s : ${Spell[${cureSpellName}].MyCastTime} : ${cureSpellName}
		}

		| ------------ Cast appropriate cure spell as defined by cureType
		/bc [+y+] Casting ${cureSpellName} on ${cureTarget} to cure ${Spell[${cureSpellID}]}
		/call e3_cast ${NetBots[${cureTarget}].ID} "${cureSpellToCast}"
	
		| ------------ Debug Info
		/if (${cureDebug} || ${Debug}) {
			/echo 240: Macro.Return for cure cast in cleanseTargetDebuff is ${Macro.Return}
			/echo 241: Will wait ${Math.Calc[${cureSpellDelay} + 0.5]}s (MyCastTime/ItemCastTime + .5s) before continuing
		}

		/delay ${Math.Calc[${cureSpellDelay} + 0.5]}s

		| ------------ Debug Info
		/if (${cureDebug} || ${Debug}) /echo /exchange ${cureSpellItemName} ${cureSpellItemSlot}

		/exchange "${cureSpellItemName}" ${cureSpellItemSlot}

	}

	| ------------ Loop again to cure
	/goto :curingTarget

/return