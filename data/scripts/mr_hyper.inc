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
	checkitem ITEM_GOLD_CAP
	call_if_eq VAR_RESULT, TRUE, EventScript_HasGoldCap
	checkitem ITEM_RUSTED_CAP
	call_if_eq VAR_RESULT, TRUE, EventScript_HasRustedCap
	return

EventScript_HasBottleCap::
	addvar VAR_TEMP_1, 1
	return

EventScript_HasGoldCap::
	addvar VAR_TEMP_1, 2
	return

EventScript_HasRustedCap::
	addvar VAR_TEMP_1, 4
	return

EventScript_ShowOptions::
	switch VAR_TEMP_1
    case 0, EventScript_CapOptionsNone
	case 1, EventScript_CapOptionsB
	case 2, EventScript_CapOptionsG
	case 3, EventScript_CapOptionsBG
	case 4, EventScript_CapOptionsR
	case 5, EventScript_CapOptionsBR
	case 6, EventScript_CapOptionsGR
	case 7, EventScript_CapOptionsBGR
	end

EventScript_CapOptionsNone::
	multichoice 0, 0, MULTI_CAPS_NONE, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperExplain
	case 1, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
    end

EventScript_CapOptionsB::
	multichoice 0, 0, MULTI_CAPS_B, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperExplain
    case 1, EventScript_TradeBottleCap
	case 2, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsG::
	multichoice 0, 0, MULTI_CAPS_G, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperExplain
    case 1, EventScript_TradeGoldCap
	case 2, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsBG::
	multichoice 0, 0, MULTI_CAPS_BG, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperExplain
    case 1, EventScript_TradeBottleCap
    case 2, EventScript_TradeGoldCap
	case 3, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsR::
	multichoice 0, 0, MULTI_CAPS_R, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperExplain
    case 1, EventScript_TradeRustedCap
	case 2, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsBR::
	multichoice 0, 0, MULTI_CAPS_BR, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperExplain
    case 1, EventScript_TradeBottleCap
    case 2, EventScript_TradeRustedCap
	case 3, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsGR::
	multichoice 0, 0, MULTI_CAPS_GR, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperExplain
    case 1, EventScript_TradeGoldCap
    case 2, EventScript_TradeRustedCap
	case 3, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_CapOptionsBGR::
	multichoice 0, 0, MULTI_CAPS_BGR, FALSE
	switch VAR_RESULT
	case 0, EventScript_MrHyperExplain
    case 1, EventScript_TradeBottleCap
    case 2, EventScript_TradeGoldCap
    case 3, EventScript_TradeRustedCap
	case 4, EventScript_MrHyperExit
	goto EventScript_MrHyperExit
	end

EventScript_TradeBottleCap::
	msgbox gText_MrHyper_TradeBottleCap, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_BOTTLE_CAP
	special ChooseMonForHyperTraining
	waitstate
    release
    end

EventScript_TradeGoldCap::
	msgbox gText_MrHyper_TradeGoldCap, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_GOLD_CAP
	special ChooseMonForHyperTraining
	waitstate
    release
    end

EventScript_TradeRustedCap::
	msgbox gText_MrHyper_TradeRustedCap, MSGBOX_DEFAULT
	setvar VAR_0x8004, ITEM_RUSTED_CAP
	special ChooseMonForHyperTraining
	waitstate
    release
    end

EventScript_MrHyperExplain::
    msgbox gText_MrHyper_Explain, MSGBOX_DEFAULT
	goto EventScript_MrHyperGreeting
	end    

EventScript_MrHyperExit::
    msgbox gText_MrHyper_Exit, MSGBOX_DEFAULT
	release
	end
