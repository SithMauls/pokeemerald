extern const u16 gTutorMoves[TUTOR_MOVE_COUNT];

#define TUTOR(move) (1u << (TUTOR_##move))

extern const u32 sTutorLearnsets[];