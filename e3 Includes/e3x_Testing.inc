| --------------------------------------------------------------------------------------------------------|
| -- Testing stuff																						 -|
| --------------------------------------------------------------------------------------------------------|

#event test1 "#*#test1#*#"
SUB EVENT_test1

	/echo event test01 entered
	:tryAgain
	/call e3_Cast ${Spawn[Smaal].ID} "Turgur's Insects"
	/echo ${castReturn}
	/if (!${castReturn.Equal[SUCCESS]}) {
	|/if (${castReturn.Equal[RESISTED]}) {
		/echo resisted loop
		/delay 4
		/goto :tryAgain
	}

/return

#event test2 "#*#test2#*#"
SUB EVENT_test2

	/echo event test02 entered
	/declare tempString string local one,two,three
	/echo ${tempString}
	/echo ---------
	/echo 1: ${tempString.Arg[1,,]}
	/echo 2: ${tempString.Arg[2,,]}
	/echo 3: ${tempString.Arg[3,,]}
	/echo ---------
	/declare tempStringCounter int local ${tempString.Count[,]}
	/varcalc tempStringCounter ${tempStringCounter} + 1
	/echo Count of items is ${tempStringCounter}
	/echo ---------
	/echo ${Math.Rand[3]} - straight random
	/echo ${Math.Rand[${tempStringCounter}]} - Random of my number (${tempStringCounter})
	/echo ---------
	/declare b int local ${Math.Calc[${Math.Rand[${Math.Calc[${tempString.Count[,]} + 1]}]} + 1]}
	/echo ${b}

/return

#event test3 "#*#test3#*#"
SUB EVENT_test3

	/echo event test03 entered
	/echo ${NetBots.Client}
	/echo ${NetBots.Counts}

	/declare HealString string local
	/declare i int local

	/echo spacer
	/echo ${HealString}

	/declare randomStart int local
	/varset randomStart ${Math.Rand[${NetBots.Counts}]}

	/echo randomStart = ${randomStart}
	/varcalc randomStart ${randomStart} + 1
	/echo randomStart = ${randomStart}

	/for i 1 to ${NetBots.Counts}

		/if (${Math.Calc[${i}+${randomStart}]} <= ${NetBots.Counts}) {
			|/echo ${NetBots.Client[${Math.Calc[${i}+${randomStart}]}]}
			/varset HealString ${HealString} ${NetBots.Client[${Math.Calc[${i}+${randomStart}]}]}
			} else {

			|/echo ${NetBots.Client[${Math.Calc[${i}+${randomStart}-${NetBots.Counts}]}]}
			/varset HealString ${HealString} ${NetBots.Client[${Math.Calc[${i}+${randomStart}-${NetBots.Counts}]}]}
		}


	|	/echo ${i}
	|	/echo ${Math.Calc[${i}+${randomStart}]}
	|	/echo ${NetBots.Client[${Math.Calc[${i}+${randomStart}]}]}

	/next i

	/echo ${HealString}
	/echo ${HealString.Length}
	/echo ${HealString.Count[ ]}
	/echo ${HealString.Arg[1, ]}
	/echo ${HealString.Arg[2, ]}
	/echo ${HealString.Arg[${NetBots.Counts}, ]}

/return

#event test4 "#*#test4#*#"
SUB EVENT_test4

	/target silent
	/delay 2s
	/doability taunt

/return



|Flopp Hitty timee mylift club lance