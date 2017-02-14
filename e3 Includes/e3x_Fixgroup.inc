| --------------------------------------------------------------------------------------|
| -- Adds remaining characters to an already created group 					           -|
| --------------------------------------------------------------------------------------|

#EVENT fixGroup "<#*#> Fix Grouping #1#"
SUB fixGroup(line, groupName)

	| ---------- If you are a group leader AND the group exists in the .ini, determine which group you are in and create an array of the "other members"
	/if (${Select[${Group.Leader},${Me}]} && ${Ini[${Group_Ini}].Find[${groupName}]) {

		| ---------- Determine countSubGroup
		/declare countSubGroup int local ${Ini[${Group_Ini}].Count[|]}
		| ---------- Solving the formatting problem with Haskell |]
		/declare subGroupFound bool local FALSE
		/declare groupMembers[6] string local
		/declare i int local 1
		/declare g int local 0

		| ---------- Loop through each SubGroup to determine which group you are a member of
		| ---------- While doing so, write an array of groupMembers for use later
		/for i 1 to ${countSubGroup}

			| ---------- Loop through the members of each SubGroup looking for you
			/for g 0 to 5

				| ---------- Set subGroupFound to TRUE if you are in this group
				/if (${Ini[${Group_Ini},${Ini[${Group_Ini}].Arg[${i},|]},GroupMember#${g}].Equal[${Me.CleanName}]}) /varset subGroupFound TRUE

				| ---------- Build the array in case this is the subgroup in qestion
				/varset groupMembers[${g}] ${Ini[${Group_Ini},${Ini[${Group_Ini}].Arg[${i},|]},GroupMember#${g}]}

			/next g

			| ---------- Exit if you found yourself
			/if (${subGroupFound}) /goto :foundGroup

		/next i

		:foundGroup
		| ---------- Cycle through groupMembers and invite those who aren't already grouped
		/declare h int local 0
		/for h 1 to 6

			| ---------- If the groupMember is !${Me} AND is not already in the group then invite him
			/if (!${groupMembers[${h}].Equal[${Me.CleanName}]} && !${Group.Member[${groupMembers[${h}]}]}) /invite ${groupMembers[${h}]}

		/next h

		/declare e int local 0
		/delay 100 ${Group} == 6
		/if (${Group} != 6) {
			/for e 1 to 5
				/if (${groupMembers[${e}].NotEqual[PLACEHOLDER]} && !${Group.Member[${groupMembers[${e}]}]}) /bc [${groupMembers[${e}]}] Did not join the group...
				/next e
			}

	}

/return