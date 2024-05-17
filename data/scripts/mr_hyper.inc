Common_EventScript_MrHyper::
    lock
    faceplayer
    goto_if_set FLAG_MET_MR_HYPER, EventScript_MrHyperGreeting
	message gText_MrHyper_Introduction
	waitmessage
	setflag FLAG_MET_MR_HYPER
	goto EventScript_CheckPlayerHasCap
	end

EventScript_MrHyperGreeting::
    message gText_MrHyper_Greeting
	waitmessage
	goto EventScript_CheckPlayerHasCap
	end

EventScript_CheckPlayerHasCap::
	call EventScript_GetPlayersCaps
	goto EventScript_ShowOptions
	end

EventScript_GetPlayersCaps::
	setvar VAR_TEMP_1, 0
	checkitem ITEM_BOTTLE_CAP
	call_if_eq VAR_RESULT, TRUE, EventScript_HasBottleCap
	checkitem ITEM_RUSTED_CAP
	call_if_eq VAR_RESULT, TRUE, EventScript_HasRustedCap
	checkitem ITEM_GOLD_CAP
	call_if_eq VAR_RESULT, TRUE, EventScript_HasGoldCap
	return

EventScript_HasBottleCap::
	addvar VAR_TEMP_1, 1
	return

EventScript_HasRustedCap::
	addvar VAR_TEMP_1, 2
	return

EventScript_HasGoldCap::
	addvar VAR_TEMP_1, 4
	return

EventScript_ShowOptions::
	switch VAR_TEMP_1
    case 0, EventScript_CapOptionsNone
	case 1, EventScript_CapOptionsB
	case 2, EventScript_CapOptionsR
	case 3, EventScript_CapOptionsBR
	case 4, EventScript_CapOptionsG
	case 5, EventScript_CapOptionsBG
	case 6, EventScript_CapOptionsRG
	case 7, EventScript_CapOptionsBRG
	end

EventScript_CapOptionsNone::
	multichoice 0, 0, MULTI_CAPS_NONE, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperInfo
	case 1, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
    end

EventScript_CapOptionsB::
	multichoice 0, 0, MULTI_CAPS_B, FALSE
	switch VAR_RESULT
	case 0, EventScript_TradeBottleCap
	case 1, EventScript_MrHyperInfo
	case 2, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsR::
	multichoice 0, 0, MULTI_CAPS_R, FALSE
	switch VAR_RESULT
	case 0, EventScript_TradeRustedCap
	case 1, EventScript_MrHyperInfo
	case 2, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsBR::
	multichoice 0, 0, MULTI_CAPS_BR, FALSE
	switch VAR_RESULT
    case 0, EventScript_TradeBottleCap
    case 1, EventScript_TradeRustedCap
	case 2, EventScript_MrHyperInfo
	case 3, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsG::
	multichoice 0, 0, MULTI_CAPS_G, FALSE
	switch VAR_RESULT
    case 0, EventScript_TradeGoldCap
	case 1, EventScript_MrHyperInfo
	case 2, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsBG::
	multichoice 0, 0, MULTI_CAPS_BG, FALSE
	switch VAR_RESULT
    case 0, EventScript_TradeBottleCap
    case 1, EventScript_TradeGoldCap
	case 2, EventScript_MrHyperInfo
	case 3, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsRG::
	multichoice 0, 0, MULTI_CAPS_RG, FALSE
	switch VAR_RESULT
	case 0, EventScript_TradeRustedCap
    case 1, EventScript_TradeGoldCap
	case 2, EventScript_MrHyperInfo
	case 3, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsBRG::
	multichoice 0, 0, MULTI_CAPS_BRG, FALSE
	switch VAR_RESULT
    case 0, EventScript_TradeBottleCap
    case 1, EventScript_TradeRustedCap
    case 2, EventScript_TradeGoldCap
	case 3, EventScript_MrHyperInfo
	case 4, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_TradeBottleCap::
	msgbox gText_MrHyper_TradeBottleCap, MSGBOX_DEFAULT
	setvar VAR_0x8005, ITEM_BOTTLE_CAP
	special ChooseMonForHyperTraining
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, EventScript_MrHyperExit
	release
    end

EventScript_TradeRustedCap::
	msgbox gText_MrHyper_TradeRustedCap, MSGBOX_DEFAULT
	setvar VAR_0x8005, ITEM_RUSTED_CAP
	special ChooseMonForHyperTraining
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, EventScript_MrHyperExit
	release
    end

EventScript_TradeGoldCap::
	msgbox gText_MrHyper_TradeGoldCap, MSGBOX_DEFAULT
	setvar VAR_0x8005, ITEM_GOLD_CAP
	special ChooseMonForHyperTraining
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, EventScript_MrHyperExit
	release
    end

EventScript_MrHyperInfo::
    msgbox gText_MrHyper_Info, MSGBOX_DEFAULT
	goto EventScript_MrHyperGreeting
	end    

EventScript_MrHyperExit::
    msgbox gText_MrHyper_Exit, MSGBOX_DEFAULT
	release
	end
