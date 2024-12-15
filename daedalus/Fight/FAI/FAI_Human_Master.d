META
{
    Parser = Fight
    MergeMode = 1
    Engine = G2A
};

// ***************************
// Kampf - AI Human Master (4)
// ***************************

// ------- Ich habe Gegner im Fokus (kann treffen) -------
instance FA_MY_W_FOCUS_4(C_FightAI)
{
	// move[0] = MOVE_FRONTATTACK;
	// move[1] = MOVE_ATTACK;
    // move[2] = MOVE_WAIT; // --- Wait sorgt für besseres Parade-Fenster ---
    
    move[0] = MOVE_MASTERATTACK; // Gothic 1
    move[1] = MOVE_WAIT; // --- Wait sorgt für besseres Parade-Fenster ---
};
