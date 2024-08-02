#include "global.h"
#include "battle_main.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "international_string_util.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "m4a.h"
#include "overworld.h"
#include "palette.h"
#include "pokedex.h"
#include "pokedex_area_screen.h"
#include "pokedex_cry_screen.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "trainer_pokemon_sprites.h"
#include "trig.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "data/pokemon/egg_moves.h"
#include "constants/items.h"
#include "data/tmhm_moves.h"
#include "constants/party_menu.h"
#include "data/pokemon/tutor_learnsets.h"
#include "data/text/move_descriptions.h"
#include "data/pokemon/evolution.h"
#include "data/text/egg_groups.h"
#include "item_icon.h"
#include "constants/abilities.h"
#include "pokedex_strings.h"

enum
{
    PAGE_MAIN,
    PAGE_INFO,
    PAGE_SEARCH,
    PAGE_SEARCH_RESULTS,
    PAGE_UNK,
    PAGE_SIZE,
    PAGE_MOVES,
    PAGE_AREA,
    PAGE_CRY
};

enum
{
    SIZE_SCREEN,
    MOVES_SCREEN,
    AREA_SCREEN,
    CRY_SCREEN,
    CANCEL_SCREEN,
    SCREEN_COUNT
};

enum
{
    //SEARCH_NAME,
    //SEARCH_COLOR,
    SEARCH_TYPE_LEFT,
    SEARCH_TYPE_RIGHT,
    SEARCH_GROUP_LEFT,
    SEARCH_GROUP_RIGHT,
    SEARCH_ABILITY,
    SEARCH_MOVE,
    SEARCH_MOVETYPE,
    SEARCH_ORDER,
    SEARCH_MODE,
    SEARCH_OK,
    SEARCH_COUNT,
    SEARCH_ABILITY_ABC,
    SEARCH_ABILITY_DEF,
    SEARCH_ABILITY_GHI,
    SEARCH_ABILITY_JKL,
    SEARCH_ABILITY_MNO,
    SEARCH_ABILITY_PQR,
    SEARCH_ABILITY_STU,
    SEARCH_ABILITY_VWX,
    SEARCH_MOVE_BUG,
    SEARCH_MOVE_DARK,
    SEARCH_MOVE_DRAGON,
    SEARCH_MOVE_ELECTRIC,
    SEARCH_MOVE_FIGHTING,
    SEARCH_MOVE_FIRE,
    SEARCH_MOVE_FLYING,
    SEARCH_MOVE_GHOST,
    SEARCH_MOVE_GRASS,
    SEARCH_MOVE_GROUND,
    SEARCH_MOVE_ICE,
    SEARCH_MOVE_NORMAL_AE,
    SEARCH_MOVE_NORMAL_FL,
    SEARCH_MOVE_NORMAL_MR,
    SEARCH_MOVE_NORMAL_S,
    SEARCH_MOVE_NORMAL_TZ,
    SEARCH_MOVE_POISON,
    SEARCH_MOVE_PSYCHIC,
    SEARCH_MOVE_ROCK,
    SEARCH_MOVE_STEEL,
    SEARCH_MOVE_WATER,
};

enum
{
    SEARCH_TOPBAR_SEARCH,
    SEARCH_TOPBAR_SHIFT,
    SEARCH_TOPBAR_CANCEL,
    SEARCH_TOPBAR_COUNT
};

enum
{
    MOVETYPE_ANY,
    MOVETYPE_LEVEL_UP,
    MOVETYPE_TMHM,
    MOVETYPE_TUTOR,
    MOVETYPE_EGG_MOVE,
};

enum
{
   ORDER_NUMERICAL,
   ORDER_ALPHABETICAL,
   ORDER_WEIGHT,
   //ORDER_LIGHTEST,
   ORDER_HEIGHT,
   //ORDER_SMALLEST
   ORDER_HP,
   ORDER_ATTACK,
   ORDER_DEFENSE,
   ORDER_SP_ATK,
   ORDER_SP_DEF,
   ORDER_SPEED,
   ORDER_STATS
};

enum
{
    NAME_ABC = 1,
    NAME_DEF,
    NAME_GHI,
    NAME_JKL,
    NAME_MNO,
    NAME_PQR,
    NAME_STU,
    NAME_VWX,
    NAME_YZ
};

enum
{
    SEARCH_TYPE_BUG = 1,
    SEARCH_TYPE_DARK,
    SEARCH_TYPE_DRAGON,
    SEARCH_TYPE_ELECTRIC,
    SEARCH_TYPE_FIGHTING,
    SEARCH_TYPE_FIRE,
    SEARCH_TYPE_FLYING,
    SEARCH_TYPE_GHOST,
    SEARCH_TYPE_GRASS,
    SEARCH_TYPE_GROUND,
    SEARCH_TYPE_ICE,
    SEARCH_TYPE_NORMAL_AE,
    SEARCH_TYPE_NORMAL_FL,
    SEARCH_TYPE_NORMAL_MR,
    SEARCH_TYPE_NORMAL_S,
    SEARCH_TYPE_NORMAL_TZ,
    SEARCH_TYPE_POISON,
    SEARCH_TYPE_PSYCHIC,
    SEARCH_TYPE_ROCK,
    SEARCH_TYPE_STEEL,
    SEARCH_TYPE_WATER,
};

enum {
    WIN_INFO,
    WIN_FOOTPRINT,
    WIN_CRY_WAVE,
    WIN_VU_METER,
    WIN_MOVES,
    WIN_MOVES_DESC,
    WIN_MOVES_BATTLE_LABELS,
    WIN_MOVES_BATTLE_VALUES,
};

enum {
    MOVE_LEVEL_UP,
    MOVE_EGG,
    MOVE_TM,
    MOVE_HM,
    MOVE_TUTOR,
    MOVE_PRE_EVOLUTION,
};

#define MOVE_SELECTOR_SPRITES_COUNT 10
#define TYPE_ICON_SPRITE_COUNT (MAX_MON_MOVES + 1)
// for the spriteIds field in PokemonSummaryScreenData
enum
{
    SPRITE_ARR_ID_MON,
    SPRITE_ARR_ID_BALL,
    SPRITE_ARR_ID_STATUS,
    SPRITE_ARR_ID_TYPE, // 2 for mon types, 5 for move types(4 moves and 1 to learn), used interchangeably, because mon types and move types aren't shown on the same screen
    SPRITE_ARR_ID_MOVE_SELECTOR1 = SPRITE_ARR_ID_TYPE + TYPE_ICON_SPRITE_COUNT, // 10 sprites that make up the selector
    SPRITE_ARR_ID_MOVE_SELECTOR2 = SPRITE_ARR_ID_MOVE_SELECTOR1 + MOVE_SELECTOR_SPRITES_COUNT,
    SPRITE_ARR_ID_COUNT = SPRITE_ARR_ID_MOVE_SELECTOR2 + MOVE_SELECTOR_SPRITES_COUNT
};

// For scrolling search parameter
#define MAX_SEARCH_PARAM_ON_SCREEN   6
#define MAX_SEARCH_PARAM_CURSOR_POS  (MAX_SEARCH_PARAM_ON_SCREEN - 1)

#define MAX_MONS_ON_SCREEN 4

#define LIST_SCROLL_STEP         16

#define POKEBALL_ROTATION_TOP    64
#define POKEBALL_ROTATION_BOTTOM (POKEBALL_ROTATION_TOP - 16)

// Coordinates of the Pokémon sprite on its page (info/cry screens)
#define MON_PAGE_X 52
#define MON_PAGE_Y 56

static EWRAM_DATA struct PokedexView *sPokedexView = NULL;
static EWRAM_DATA u16 sLastSelectedPokemon = 0;
static EWRAM_DATA u8 sPokeBallRotation = 0;
static EWRAM_DATA struct PokedexListItem *sPokedexListItem = NULL;
static EWRAM_DATA struct MovesListItem *sMovesListItem = NULL;
static EWRAM_DATA struct MovesView *sMovesView = NULL;

// This is written to, but never read.
u8 gUnusedPokedexU8;
void (*gPokedexVBlankCB)(void);

struct SearchOptionText
{
    const u8 *description;
    const u8 *title;
};

struct SearchOption
{
    const struct SearchOptionText *texts;
    u8 taskDataCursorPos;
    u8 taskDataScrollOffset;
    u16 numOptions;
};

struct SearchMenuTopBarItem
{
    const u8 *description;
    u8 highlightX;
    u8 highlightY;
    u8 highlightWidth;
};

struct SearchMenuItem
{
    const u8 *description;
    u8 titleBgX;
    u8 titleBgY;
    u8 titleBgWidth;
    u8 selectionBgX;
    u8 selectionBgY;
    u8 selectionBgWidth;
};

struct PokedexListItem
{
    u16 dexNum;
    u16 seen:1;
    u16 owned:1;
};

struct PokedexView
{
    struct PokedexListItem pokedexList[NATIONAL_DEX_COUNT + 1];
    u16 pokemonListCount;
    u16 selectedPokemon;
    u16 selectedPokemonBackup;
    u16 dexMode;
    u16 dexModeBackup;
    u16 dexOrder;
    u16 dexOrderBackup;
    u16 seenCount;
    u16 ownCount;
    u16 monSpriteIds[MAX_MONS_ON_SCREEN];
    u16 selectedMonSpriteId;
    u16 pokeBallRotationStep;
    u16 pokeBallRotationBackup;
    u8 pokeBallRotation;
    u8 initialVOffset;
    u8 scrollTimer;
    u8 scrollDirection;
    s16 listVOffset;
    s16 listMovingVOffset;
    u16 scrollMonIncrement;
    u16 maxScrollTimer;
    u16 scrollSpeed;
    u16 unkArr1[4]; // Cleared, never read
    u8 filler[8];
    u8 currentPage;
    u8 currentPageBackup;
    bool8 isSearchResults:1;
    u8 selectedScreen;
    u8 screenSwitchState;
    u8 menuIsOpen;
    u16 menuCursorPos;
    s16 menuY;     //Menu Y position (inverted because we use REG_BG0VOFS for this)
    u8 unkArr2[8]; // Cleared, never read
    u8 unkArr3[8]; // Cleared, never read
};

struct MovesListItem
{
    u8 type;
    u16 index;
    u16 move;
};

struct MovesView
{
    struct MovesListItem movesList[100];
    u16 movesListCount;
    u16 selectedMove;
    u8 initialVOffset;
    u8 scrollTimer;
    u8 scrollDirection;
    s16 listVOffset;
    s16 listMovingVOffset;
    u16 scrollMovesIncrement;
    u16 maxScrollTimer;
    u16 scrollSpeed;
    s16 menuY;
    u8 spriteIds[SPRITE_ARR_ID_COUNT];
    s16 spriteYPos[SPRITE_ARR_ID_COUNT];
    s32 bg3VOffsetBuffer;
    u8 categorySpriteId;
    bool8 inMenu;
};

#define TAG_MOVE_TYPES 30002

static const struct OamData sOamData_MoveTypes =
{
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 2,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sSpriteAnim_TypeNormal[] = {
    ANIMCMD_FRAME(TYPE_NORMAL * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeFighting[] = {
    ANIMCMD_FRAME(TYPE_FIGHTING * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeFlying[] = {
    ANIMCMD_FRAME(TYPE_FLYING * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypePoison[] = {
    ANIMCMD_FRAME(TYPE_POISON * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeGround[] = {
    ANIMCMD_FRAME(TYPE_GROUND * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeRock[] = {
    ANIMCMD_FRAME(TYPE_ROCK * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeBug[] = {
    ANIMCMD_FRAME(TYPE_BUG * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeGhost[] = {
    ANIMCMD_FRAME(TYPE_GHOST * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeSteel[] = {
    ANIMCMD_FRAME(TYPE_STEEL * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeMystery[] = {
    ANIMCMD_FRAME(TYPE_MYSTERY * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeFire[] = {
    ANIMCMD_FRAME(TYPE_FIRE * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeWater[] = {
    ANIMCMD_FRAME(TYPE_WATER * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeGrass[] = {
    ANIMCMD_FRAME(TYPE_GRASS * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeElectric[] = {
    ANIMCMD_FRAME(TYPE_ELECTRIC * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypePsychic[] = {
    ANIMCMD_FRAME(TYPE_PSYCHIC * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeIce[] = {
    ANIMCMD_FRAME(TYPE_ICE * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeDragon[] = {
    ANIMCMD_FRAME(TYPE_DRAGON * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_TypeDark[] = {
    ANIMCMD_FRAME(TYPE_DARK * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_CategoryCool[] = {
    ANIMCMD_FRAME((CONTEST_CATEGORY_COOL + NUMBER_OF_MON_TYPES) * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_CategoryBeauty[] = {
    ANIMCMD_FRAME((CONTEST_CATEGORY_BEAUTY + NUMBER_OF_MON_TYPES) * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_CategoryCute[] = {
    ANIMCMD_FRAME((CONTEST_CATEGORY_CUTE + NUMBER_OF_MON_TYPES) * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_CategorySmart[] = {
    ANIMCMD_FRAME((CONTEST_CATEGORY_SMART + NUMBER_OF_MON_TYPES) * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_CategoryTough[] = {
    ANIMCMD_FRAME((CONTEST_CATEGORY_TOUGH + NUMBER_OF_MON_TYPES) * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_CategoryPhysical[] = {
    ANIMCMD_FRAME((CATEGORY_PHYSICAL + NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT) * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_CategorySpecial[] = {
    ANIMCMD_FRAME((CATEGORY_SPECIAL + NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT) * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};
static const union AnimCmd sSpriteAnim_CategoryStatus[] = {
    ANIMCMD_FRAME((CATEGORY_STATUS + NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT) * 8, 0, FALSE, FALSE),
    ANIMCMD_END
};


static const union AnimCmd *const sSpriteAnimTable_MoveTypes[NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT + CATEGORIES_COUNT] = {
    sSpriteAnim_TypeNormal,
    sSpriteAnim_TypeFighting,
    sSpriteAnim_TypeFlying,
    sSpriteAnim_TypePoison,
    sSpriteAnim_TypeGround,
    sSpriteAnim_TypeRock,
    sSpriteAnim_TypeBug,
    sSpriteAnim_TypeGhost,
    sSpriteAnim_TypeSteel,
    sSpriteAnim_TypeMystery,
    sSpriteAnim_TypeFire,
    sSpriteAnim_TypeWater,
    sSpriteAnim_TypeGrass,
    sSpriteAnim_TypeElectric,
    sSpriteAnim_TypePsychic,
    sSpriteAnim_TypeIce,
    sSpriteAnim_TypeDragon,
    sSpriteAnim_TypeDark,
    sSpriteAnim_CategoryCool,
    sSpriteAnim_CategoryBeauty,
    sSpriteAnim_CategoryCute,
    sSpriteAnim_CategorySmart,
    sSpriteAnim_CategoryTough,
    sSpriteAnim_CategoryPhysical,
    sSpriteAnim_CategorySpecial,
    sSpriteAnim_CategoryStatus,
};

static const struct CompressedSpriteSheet sSpriteSheet_MoveTypes =
{
    .data = gMoveTypes_Gfx,
    .size = (NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT + CATEGORIES_COUNT) * 0x100,
    .tag = TAG_MOVE_TYPES
};
static const struct SpriteTemplate sSpriteTemplate_MoveTypes =
{
    .tileTag = TAG_MOVE_TYPES,
    .paletteTag = TAG_MOVE_TYPES,
    .oam = &sOamData_MoveTypes,
    .anims = sSpriteAnimTable_MoveTypes,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const u8 sMoveTypeToOamPaletteNum[NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT + CATEGORIES_COUNT] =
{
    [TYPE_NORMAL] = 12,
    [TYPE_FIGHTING] = 12,
    [TYPE_FLYING] = 13,
    [TYPE_POISON] = 13,
    [TYPE_GROUND] = 12,
    [TYPE_ROCK] = 12,
    [TYPE_BUG] = 14,
    [TYPE_GHOST] = 13,
    [TYPE_STEEL] = 12,
    [TYPE_MYSTERY] = 14,
    [TYPE_FIRE] = 12,
    [TYPE_WATER] = 13,
    [TYPE_GRASS] = 14,
    [TYPE_ELECTRIC] = 12,
    [TYPE_PSYCHIC] = 13,
    [TYPE_ICE] = 13,
    [TYPE_DRAGON] = 14,
    [TYPE_DARK] = 12,
    [NUMBER_OF_MON_TYPES + CONTEST_CATEGORY_COOL] = 12,
    [NUMBER_OF_MON_TYPES + CONTEST_CATEGORY_BEAUTY] = 13,
    [NUMBER_OF_MON_TYPES + CONTEST_CATEGORY_CUTE] = 13,
    [NUMBER_OF_MON_TYPES + CONTEST_CATEGORY_SMART] = 14,
    [NUMBER_OF_MON_TYPES + CONTEST_CATEGORY_TOUGH] = 12,
    [NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT + CATEGORY_PHYSICAL] = 15,
    [NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT + CATEGORY_SPECIAL] = 15,
    [NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT + CATEGORY_STATUS] = 15,
};

// this file's functions
static void CB2_Pokedex(void);
static void Task_OpenPokedexMainPage(u8);
static void Task_HandlePokedexInput(u8);
static void Task_WaitForScroll(u8);
static void Task_HandlePokedexStartMenuInput(u8);
static void Task_OpenInfoScreenAfterMonMovement(u8);
static void Task_WaitForExitInfoScreen(u8);
static void Task_WaitForExitSearch(u8);
static void Task_ClosePokedex(u8);
static void Task_OpenSearchResults(u8);
static void Task_HandleSearchResultsInput(u8);
static void Task_WaitForSearchResultsScroll(u8);
static void Task_HandleSearchResultsStartMenuInput(u8);
static void Task_OpenSearchResultsInfoScreenAfterMonMovement(u8);
static void Task_WaitForExitSearchResultsInfoScreen(u8);
static void Task_ReturnToPokedexFromSearchResults(u8);
static void Task_ClosePokedexFromSearchResultsStartMenu(u8);
static bool8 LoadPokedexListPage(u8);
static void LoadPokedexBgPalette(bool8);
static void FreeWindowAndBgBuffers(void);
static void CreatePokedexList(u8, u8);
static void CreateMonDexNum(u16, u8, u8, u16);
static void CreateCaughtBall(u16, u8, u8, u16);
static u8 CreateMonName(u16, u8, u8);
static void ClearMonListEntry(u8 x, u8 y, u16 unused);
static void CreateMonSpritesAtPos(u16, u16);
static bool8 UpdateDexListScroll(u8, u8, u8);
static u16 TryDoPokedexScroll(u16, u16);
static void UpdateSelectedMonSpriteId(void);
static u8 ClearMonSprites(void);
static u16 GetPokemonSpriteToDisplay(u16);
static u32 CreatePokedexMonSprite(u16, s16, s16);
static void CreateInterfaceSprites(u8);
static void SpriteCB_MoveMonForInfoScreen(struct Sprite *sprite);
static void SpriteCB_Scrollbar(struct Sprite *sprite);
static void SpriteCB_ScrollArrow(struct Sprite *sprite);
static void SpriteCB_DexListInterfaceText(struct Sprite *sprite);
static void SpriteCB_RotatingPokeBall(struct Sprite *sprite);
static void SpriteCB_SeenOwnInfo(struct Sprite *sprite);
static void SpriteCB_DexListStartMenuCursor(struct Sprite *sprite);
static void SpriteCB_PokedexListMonSprite(struct Sprite *sprite);
static u8 LoadInfoScreen(struct PokedexListItem *, u8 monSpriteId);
static bool8 IsInfoScreenScrolling(u8);
static u8 StartInfoScreenScroll(struct PokedexListItem *, u8);
static void Task_LoadInfoScreen(u8);
static void Task_HandleInfoScreenInput(u8);
static void Task_SwitchScreensFromInfoScreen(u8);
static void Task_LoadInfoScreenWaitForFade(u8);
static void Task_ExitInfoScreen(u8);
static void Task_LoadAreaScreen(u8);
static void Task_WaitForAreaScreenInput(u8 taskId);
static void Task_SwitchScreensFromAreaScreen(u8);
static void Task_LoadCryScreen(u8);
static void Task_HandleCryScreenInput(u8);
static void Task_SwitchScreensFromCryScreen(u8);
static void LoadPlayArrowPalette(bool8);
static void Task_LoadSizeScreen(u8);
static void Task_HandleSizeScreenInput(u8);
static void Task_SwitchScreensFromSizeScreen(u8);
static void LoadScreenSelectBarMain(u16);
static void LoadScreenSelectBarSubmenu(u16);
static void HighlightScreenSelectBarItem(u8, u16);
static void HighlightSubmenuScreenSelectBarItem(u8, u16);
static void Task_DisplayCaughtMonDexPage(u8);
static void Task_HandleCaughtMonPageInput(u8);
static void Task_ExitCaughtMonPage(u8);
static void SpriteCB_SlideCaughtMonToCenter(struct Sprite *sprite);
static void PrintMonInfo(u32 num, u32, u32 owned, u32 newEntry);
static void PrintMonHeight(u16 height, u8 left, u8 top);
static void PrintMonWeight(u16 weight, u8 left, u8 top);
static void ResetOtherVideoRegisters(u16);
static u8 PrintCryScreenSpeciesName(u8, u16, u8, u8);
static void PrintDecimalNum(u8 windowId, u16 num, u8 left, u8 top);
static void DrawFootprint(u8 windowId, u16 dexNum);
static u16 CreateSizeScreenTrainerPic(u16, s16, s16, s8);
static u16 GetNextPosition(u8, u16, u16, u16);
static u8 LoadSearchMenu(void);
static void Task_LoadSearchMenu(u8);
static void Task_SwitchToSearchMenuTopBar(u8);
static void Task_HandleSearchTopBarInput(u8);
static void Task_SwitchToSearchMenu(u8);
static void Task_HandleSearchMenuInput(u8);
static void Task_StartPokedexSearch(u8);
static void Task_WaitAndCompleteSearch(u8);
static void Task_SearchCompleteWaitForInput(u8);
static void Task_SelectSearchMenuItem(u8);
static void Task_HandleSearchParameterInput(u8);
static void Task_ExitSearch(u8);
static void Task_ExitSearchWaitForFade(u8);
static void HighlightSelectedSearchTopBarItem(u8);
static void HighlightSelectedSearchMenuItem(u8, u8);
static void PrintSelectedSearchParameters(u8);
static void DrawOrEraseSearchParameterBox(bool8);
static void PrintSearchParameterText(u8);
static u16 GetSearchModeSelection(u8 taskId, u8 option);
static void SetDefaultSearchModeAndOrder(u8);
static void CreateSearchParameterScrollArrows(u8);
static void EraseAndPrintSearchTextBox(const u8 *);
static void EraseSelectorArrow(u32);
static void PrintSelectorArrow(u32);
static void PrintSearchParameterTitle(u32, const u8 *);
static void ClearSearchParameterBoxText(void);
static void Task_LoadMovesScreen(u8);
static void CreateMovesList(void);
static s32 FindSpeciesInEggMoves(u16 species);
static u16 GetPreEvolution(u16 species);
static void CreateMoveSpritesAtPos(u16 selectedMove, u16 ignored);
static void ClearMoveListEntry(u8 x, u8 y, u16 unused);
static void CreateMoveListEntry(u8 position, u16 b, u16 ignored);
static void CreateMovePrefix(u8 type, u16 index, u8 left, u8 top);
static u8 CreateMoveName(u16 move, u8 left, u8 top);
static void Task_HandleMovesScreenInput(u8);
static void Task_HandleMovesScreenInput2(u8);
static u16 TryDoMovesScroll(u16 selectedMove, u16 ignored, u8 taskId);
static void Task_WaitForMovesScroll(u8 taskId);
static bool8 UpdateMovesListScroll(u8 direction, u8 monMoveIncrement, u8 scrollTimerMax);
static void Task_SwitchScreensFromMovesScreen(u8);
static void PrintMoveData(u8 windowId, u8 fontId, const u8 *str, u8 left, u8 top);
static void CreateMoveTypeIcons(void);
static void SetMoveTypeIcons(void);
static void SetTypeSpritePosAndPal(u8 typeId, u8 x, u8 y, u8 spriteArrayId);
static void ResetSpriteIds(void);
static void SyncBg3VOffset();
static void PrintMoveInfo(u16 moveIndex);
static void DrawEVYield(struct SpeciesInfo species, u16 *tilemapBuffer);

// const rom data
#include "data/pokemon/pokedex_orders.h"

static const struct OamData sOamData_ScrollBar =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x8),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sOamData_ScrollArrow =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x8),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x8),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sOamData_InterfaceText =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sOamData_RotatingPokeBall =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_WINDOW,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(32x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(32x32),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sOamData_SeenOwnText =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(64x32),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(64x32),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const struct OamData sOamData_Dex8x16 =
{
    .y = DISPLAY_HEIGHT,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(8x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(8x16),
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0
};

static const union AnimCmd sSpriteAnim_ScrollBar[] =
{
    ANIMCMD_FRAME(3, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_ScrollArrow[] =
{
    ANIMCMD_FRAME(1, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_RotatingPokeBall[] =
{
    ANIMCMD_FRAME(16, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_StartButton[] =
{
    ANIMCMD_FRAME(48, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_SearchText[] =
{
    ANIMCMD_FRAME(40, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_SelectButton[] =
{
    ANIMCMD_FRAME(32, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_MenuText[] =
{
    ANIMCMD_FRAME(56, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_SeenText[] =
{
    ANIMCMD_FRAME(64, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_OwnText[] =
{
    ANIMCMD_FRAME(96, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennText[] =
{
    ANIMCMD_FRAME(160, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalText[] =
{
    ANIMCMD_FRAME(168, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit0[] =
{
    ANIMCMD_FRAME(128, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit1[] =
{
    ANIMCMD_FRAME(130, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit2[] =
{
    ANIMCMD_FRAME(132, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit3[] =
{
    ANIMCMD_FRAME(134, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit4[] =
{
    ANIMCMD_FRAME(136, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit5[] =
{
    ANIMCMD_FRAME(138, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit6[] =
{
    ANIMCMD_FRAME(140, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit7[] =
{
    ANIMCMD_FRAME(142, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit8[] =
{
    ANIMCMD_FRAME(144, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_HoennSeenOwnDigit9[] =
{
    ANIMCMD_FRAME(146, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit0[] =
{
    ANIMCMD_FRAME(176, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit1[] =
{
    ANIMCMD_FRAME(178, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit2[] =
{
    ANIMCMD_FRAME(180, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit3[] =
{
    ANIMCMD_FRAME(182, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit4[] =
{
    ANIMCMD_FRAME(184, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit5[] =
{
    ANIMCMD_FRAME(186, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit6[] =
{
    ANIMCMD_FRAME(188, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit7[] =
{
    ANIMCMD_FRAME(190, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit8[] =
{
    ANIMCMD_FRAME(192, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_NationalSeenOwnDigit9[] =
{
    ANIMCMD_FRAME(194, 30),
    ANIMCMD_END
};

static const union AnimCmd sSpriteAnim_DexListStartMenuCursor[] =
{
    ANIMCMD_FRAME(4, 30),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable_ScrollBar[] =
{
    sSpriteAnim_ScrollBar
};

static const union AnimCmd *const sSpriteAnimTable_ScrollArrow[] =
{
    sSpriteAnim_ScrollArrow
};

static const union AnimCmd *const sSpriteAnimTable_RotatingPokeBall[] =
{
    sSpriteAnim_RotatingPokeBall
};

static const union AnimCmd *const sSpriteAnimTable_InterfaceText[] =
{
    sSpriteAnim_StartButton,
    sSpriteAnim_SearchText,
    sSpriteAnim_SelectButton,
    sSpriteAnim_MenuText
};

static const union AnimCmd *const sSpriteAnimTable_SeenOwnText[] =
{
    sSpriteAnim_SeenText,
    sSpriteAnim_OwnText
};

static const union AnimCmd *const sSpriteAnimTable_HoennNationalText[] =
{
    sSpriteAnim_HoennText,
    sSpriteAnim_NationalText
};

static const union AnimCmd *const sSpriteAnimTable_HoennSeenOwnNumber[] =
{
    sSpriteAnim_HoennSeenOwnDigit0,
    sSpriteAnim_HoennSeenOwnDigit1,
    sSpriteAnim_HoennSeenOwnDigit2,
    sSpriteAnim_HoennSeenOwnDigit3,
    sSpriteAnim_HoennSeenOwnDigit4,
    sSpriteAnim_HoennSeenOwnDigit5,
    sSpriteAnim_HoennSeenOwnDigit6,
    sSpriteAnim_HoennSeenOwnDigit7,
    sSpriteAnim_HoennSeenOwnDigit8,
    sSpriteAnim_HoennSeenOwnDigit9
};

static const union AnimCmd *const sSpriteAnimTable_NationalSeenOwnNumber[] =
{
    sSpriteAnim_NationalSeenOwnDigit0,
    sSpriteAnim_NationalSeenOwnDigit1,
    sSpriteAnim_NationalSeenOwnDigit2,
    sSpriteAnim_NationalSeenOwnDigit3,
    sSpriteAnim_NationalSeenOwnDigit4,
    sSpriteAnim_NationalSeenOwnDigit5,
    sSpriteAnim_NationalSeenOwnDigit6,
    sSpriteAnim_NationalSeenOwnDigit7,
    sSpriteAnim_NationalSeenOwnDigit8,
    sSpriteAnim_NationalSeenOwnDigit9
};

static const union AnimCmd *const sSpriteAnimTable_DexListStartMenuCursor[] =
{
    sSpriteAnim_DexListStartMenuCursor
};

#define TAG_DEX_INTERFACE 4096 // Tile and pal tag used for all interface sprites.

static const struct SpriteTemplate sScrollBarSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_ScrollBar,
    .anims = sSpriteAnimTable_ScrollBar,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_Scrollbar,
};

static const struct SpriteTemplate sScrollArrowSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_ScrollArrow,
    .anims = sSpriteAnimTable_ScrollArrow,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_ScrollArrow,
};

static const struct SpriteTemplate sInterfaceTextSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_InterfaceText,
    .anims = sSpriteAnimTable_InterfaceText,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_DexListInterfaceText,
};

static const struct SpriteTemplate sRotatingPokeBallSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_RotatingPokeBall,
    .anims = sSpriteAnimTable_RotatingPokeBall,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_RotatingPokeBall,
};

static const struct SpriteTemplate sSeenOwnTextSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_SeenOwnText,
    .anims = sSpriteAnimTable_SeenOwnText,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SeenOwnInfo,
};

static const struct SpriteTemplate sHoennNationalTextSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_InterfaceText,
    .anims = sSpriteAnimTable_HoennNationalText,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SeenOwnInfo,
};

static const struct SpriteTemplate sHoennDexSeenOwnNumberSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_Dex8x16,
    .anims = sSpriteAnimTable_HoennSeenOwnNumber,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SeenOwnInfo,
};

static const struct SpriteTemplate sNationalDexSeenOwnNumberSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_Dex8x16,
    .anims = sSpriteAnimTable_NationalSeenOwnNumber,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_SeenOwnInfo,
};

static const struct SpriteTemplate sDexListStartMenuCursorSpriteTemplate =
{
    .tileTag = TAG_DEX_INTERFACE,
    .paletteTag = TAG_DEX_INTERFACE,
    .oam = &sOamData_Dex8x16,
    .anims = sSpriteAnimTable_DexListStartMenuCursor,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_DexListStartMenuCursor,
};

static const struct CompressedSpriteSheet sInterfaceSpriteSheet[] =
{
    {gPokedexInterface_Gfx, 0x2000, TAG_DEX_INTERFACE},
    {0}
};

static const struct SpritePalette sInterfaceSpritePalette[] =
{
    {gPokedexBgHoenn_Pal, TAG_DEX_INTERFACE},
    {0}
};

// By scroll speed. Last element of each unused
static const u8 sScrollMonIncrements[] = {4, 8, 16, 32, 32};
static const u8 sScrollTimers[] = {8, 4, 2, 1, 1};

static const struct BgTemplate sPokedex_BgTemplate[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 12,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 13,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 15,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    }
};

static const struct WindowTemplate sPokemonList_WindowTemplate[] =
{
    {
        .bg = 2,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 32,
        .height = 32,
        .paletteNum = 0,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sCaughtBall_Gfx[] = INCBIN_U8("graphics/pokedex/caught_ball.4bpp");

#include "data/pokemon/pokedex_text.h"
#include "data/pokemon/pokedex_entries.h"

static const u16 sSizeScreenSilhouette_Pal[] = INCBIN_U16("graphics/pokedex/size_silhouette.gbapal");

static const struct BgTemplate sInfoScreen_BgTemplate[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 12,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 13,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 15,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    }
};

static const struct WindowTemplate sInfoScreen_WindowTemplates[] =
{
    [WIN_INFO] =
    {
        .bg = 2,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 32,
        .height = 20,
        .paletteNum = 0,
        .baseBlock = 1,
    },
    [WIN_FOOTPRINT] =
    {
        .bg = 2,
        .tilemapLeft = 25,
        .tilemapTop = 8,
        .width = 2,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 641,
    },
    [WIN_CRY_WAVE] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 12,
        .width = 32,
        .height = 7,
        .paletteNum = 8,
        .baseBlock = 645,
    },
    [WIN_VU_METER] =
    {
        .bg = 2,
        .tilemapLeft = 18,
        .tilemapTop = 3,
        .width = 10,
        .height = 8,
        .paletteNum = 9,
        .baseBlock = 869,
    },
    [WIN_MOVES] =
    {
        .bg = 3,
        .tilemapLeft = 12,
        .tilemapTop = 0,
        .width = 16,
        .height = 32,
        .paletteNum = 0,
        .baseBlock = 1,
    },
    [WIN_MOVES_DESC] =
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 16,
        .width = 28,
        .height = 4,
        .paletteNum = 0,
        .baseBlock = 513,
    },
    [WIN_MOVES_BATTLE_LABELS] =
    {
        .bg = 1,
        .tilemapLeft = 1,
        .tilemapTop = 4,
        .width = 6,
        .height = 10,
        .paletteNum = 0,
        .baseBlock = 625,
    },
    [WIN_MOVES_BATTLE_VALUES]
    {
        .bg = 1,
        .tilemapLeft = 7,
        .tilemapTop = 4,
        .width = 3,
        .height = 10,
        .paletteNum = 0,
        .baseBlock = 685,
    },
    DUMMY_WIN_TEMPLATE
};

static const struct BgTemplate sNewEntryInfoScreen_BgTemplate[] =
{
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 3,
        .charBaseIndex = 1,
        .mapBaseIndex = 15,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
};

static const struct WindowTemplate sNewEntryInfoScreen_WindowTemplates[] =
{
    [WIN_INFO] =
    {
        .bg = 2,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 32,
        .height = 20,
        .paletteNum = 0,
        .baseBlock = 1,
    },
    [WIN_FOOTPRINT] =
    {
        .bg = 2,
        .tilemapLeft = 25,
        .tilemapTop = 8,
        .width = 2,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 641,
    },
    DUMMY_WIN_TEMPLATE
};

#include "data/pokemon_graphics/footprint_table.h"

/*
// First character in range followed by number of characters in range for upper and lowercase
static const u8 sLetterSearchRanges[][4] =
{
    {}, // Name not specified, shouldn't be reached
    [NAME_ABC] = {CHAR_A, 3, CHAR_a, 3},
    [NAME_DEF] = {CHAR_D, 3, CHAR_d, 3},
    [NAME_GHI] = {CHAR_G, 3, CHAR_g, 3},
    [NAME_JKL] = {CHAR_J, 3, CHAR_j, 3},
    [NAME_MNO] = {CHAR_M, 3, CHAR_m, 3},
    [NAME_PQR] = {CHAR_P, 3, CHAR_p, 3},
    [NAME_STU] = {CHAR_S, 3, CHAR_s, 3},
    [NAME_VWX] = {CHAR_V, 3, CHAR_v, 3},
    [NAME_YZ]  = {CHAR_Y, 2, CHAR_y, 2},
};

#define LETTER_IN_RANGE_UPPER(letter, range) \
    ((letter) >= sLetterSearchRanges[range][0]                                  \
  && (letter) < sLetterSearchRanges[range][0] + sLetterSearchRanges[range][1])  \

#define LETTER_IN_RANGE_LOWER(letter, range) \
    ((letter) >= sLetterSearchRanges[range][2]                                  \
  && (letter) < sLetterSearchRanges[range][2] + sLetterSearchRanges[range][3])  \
*/

static const struct SearchMenuTopBarItem sSearchMenuTopBarItems[SEARCH_TOPBAR_COUNT] =
{
    [SEARCH_TOPBAR_SEARCH] =
    {
        .description = gText_SearchForPkmnBasedOnParameters,
        .highlightX = 0,
        .highlightY = 0,
        .highlightWidth = 5,
    },
    [SEARCH_TOPBAR_SHIFT] =
    {
        .description = gText_SwitchPokedexListings,
        .highlightX = 6,
        .highlightY = 0,
        .highlightWidth = 5,
    },
    [SEARCH_TOPBAR_CANCEL] =
    {
        .description = gText_ReturnToPokedex,
        .highlightX = 12,
        .highlightY = 0,
        .highlightWidth = 5,
    },
};

static const struct SearchMenuItem sSearchMenuItems[SEARCH_COUNT] =
{
    /*
    [SEARCH_NAME] =
    {
        .description = gText_ListByFirstLetter,
        .titleBgX = 0,
        .titleBgY = 2,
        .titleBgWidth = 5,
        .selectionBgX = 5,
        .selectionBgY = 2,
        .selectionBgWidth = 12,
    },
    [SEARCH_COLOR] =
    {
        .description = gText_ListByBodyColor,
        .titleBgX = 0,
        .titleBgY = 4,
        .titleBgWidth = 5,
        .selectionBgX = 5,
        .selectionBgY = 4,
        .selectionBgWidth = 12,
    },
    */
    [SEARCH_TYPE_LEFT] =
    {
        .description = gText_ListByType,
        .titleBgX = 0,
        .titleBgY = 2,
        .titleBgWidth = 5,
        .selectionBgX = 5,
        .selectionBgY = 2,
        .selectionBgWidth = 6,
    },
    [SEARCH_TYPE_RIGHT] =
    {
        .description = gText_ListByType,
        .titleBgX = 0,
        .titleBgY = 2,
        .titleBgWidth = 5,
        .selectionBgX = 11,
        .selectionBgY = 2,
        .selectionBgWidth = 6,
    },
    [SEARCH_GROUP_LEFT] =
    {
        .description = gText_ListByGroup,
        .titleBgX = 0,
        .titleBgY = 4,
        .titleBgWidth = 5,
        .selectionBgX = 5,
        .selectionBgY = 4,
        .selectionBgWidth = 6,
    },
    [SEARCH_GROUP_RIGHT] =
    {
        .description = gText_ListByGroup,
        .titleBgX = 0,
        .titleBgY = 4,
        .titleBgWidth = 5,
        .selectionBgX = 11,
        .selectionBgY = 4,
        .selectionBgWidth = 6,
    },
    [SEARCH_ABILITY] =
    {
        .description = gText_ListByAbility,
        .titleBgX = 0,
        .titleBgY = 6,
        .titleBgWidth = 5,
        .selectionBgX = 5,
        .selectionBgY = 6,
        .selectionBgWidth = 12,
    },
    [SEARCH_MOVE] =
    {
        .description = gText_ListByMove,
        .titleBgX = 0,
        .titleBgY = 8,
        .titleBgWidth = 5,
        .selectionBgX = 5,
        .selectionBgY = 8,
        .selectionBgWidth = 8,
    },
    [SEARCH_MOVETYPE] =
    {
        .description = gText_ListByMoveType,
        .titleBgX = 0,
        .titleBgY = 8,
        .titleBgWidth = 5,
        .selectionBgX = 13,
        .selectionBgY = 8,
        .selectionBgWidth = 4,
    },
    [SEARCH_ORDER] =
    {
        .description = gText_SelectPokedexListingMode,
        .titleBgX = 0,
        .titleBgY = 10,
        .titleBgWidth = 5,
        .selectionBgX = 5,
        .selectionBgY = 10,
        .selectionBgWidth = 12,
    },
    [SEARCH_MODE] =
    {
        .description = gText_SelectPokedexMode,
        .titleBgX = 0,
        .titleBgY = 12,
        .titleBgWidth = 5,
        .selectionBgX = 5,
        .selectionBgY = 12,
        .selectionBgWidth = 7,
    },
    [SEARCH_OK] =
    {
        .description = gText_ExecuteSearchSwitch,
        .titleBgX = 12,
        .titleBgY = 12,
        .titleBgWidth = 5,
        .selectionBgX = 0,
        .selectionBgY = 0,
        .selectionBgWidth = 0,
    },
};

// Left, Right, Up, Down
static const u8 sSearchMovementMap_SearchNatDex[SEARCH_COUNT][4] =
{
    /*
    [SEARCH_NAME] =
    {
        0xFF,
        0xFF,
        0xFF,
        //SEARCH_COLOR
        SEARCH_GROUP_LEFT
    },
    [SEARCH_COLOR] =
    {
        0xFF,
        0xFF,
        SEARCH_NAME,
        SEARCH_TYPE_LEFT
    },
    */
    [SEARCH_TYPE_LEFT] =
    {
        0xFF,
        SEARCH_TYPE_RIGHT,
        0xFF,
        SEARCH_GROUP_LEFT
    },
    [SEARCH_TYPE_RIGHT] =
    {   SEARCH_TYPE_LEFT,
        0xFF,
        0xFF,
        SEARCH_GROUP_RIGHT
    },
    [SEARCH_GROUP_LEFT] =
    {
        0xFF,
        SEARCH_GROUP_RIGHT,
        SEARCH_TYPE_LEFT,
        SEARCH_ABILITY
    },
    [SEARCH_GROUP_RIGHT] =
    {   SEARCH_GROUP_LEFT,
        0xFF,
        SEARCH_TYPE_RIGHT,
        SEARCH_ABILITY
    },
    [SEARCH_ABILITY] = 
    {
        0xFF,
        0xFF,
        SEARCH_GROUP_LEFT,
        SEARCH_MOVE
    },
    [SEARCH_MOVE] =
    {
        0xFF,
        SEARCH_MOVETYPE,
        SEARCH_ABILITY,
        SEARCH_ORDER
    },
    [SEARCH_MOVETYPE] =
    {
        SEARCH_MOVE,
        0xFF,
        SEARCH_ABILITY,
        SEARCH_ORDER
    },
    [SEARCH_ORDER] =
    {
        0xFF,
        0xFF,
        SEARCH_MOVE,
        SEARCH_MODE
    },
    [SEARCH_MODE] =
    {
        0xFF,
        SEARCH_OK,
        SEARCH_ORDER,
        SEARCH_OK
    },
    [SEARCH_OK] =
    {
        SEARCH_MODE,
        0xFF,
        SEARCH_ORDER,
        0xFF
    },
};

// Left, Right, Up, Down
static const u8 sSearchMovementMap_ShiftNatDex[SEARCH_COUNT][4] =
{
    /*
    [SEARCH_NAME] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_COLOR] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    */
    [SEARCH_TYPE_LEFT] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_TYPE_RIGHT] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_GROUP_LEFT] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_GROUP_RIGHT] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_ABILITY] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_MOVE] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_MOVETYPE] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_ORDER] =
    {
        0xFF,
        0xFF,
        0xFF,
        SEARCH_MODE
    },
    [SEARCH_MODE] =
    {
        0xFF,
        SEARCH_OK,
        SEARCH_ORDER,
        SEARCH_OK
    },
    [SEARCH_OK] =
    {
        SEARCH_MODE,
        0xFF,
        SEARCH_ORDER,
        0xFF
    },
};

// Left, Right, Up, Down
static const u8 sSearchMovementMap_SearchHoennDex[SEARCH_COUNT][4] =
{
    /*
    [SEARCH_NAME] =
    {
        0xFF,
        0xFF,
        0xFF,
        //SEARCH_COLOR
        SEARCH_GROUP_LEFT
    },
    [SEARCH_COLOR] =
    {
        0xFF,
        0xFF,
        SEARCH_NAME,
        SEARCH_TYPE_LEFT
    },
    */
    [SEARCH_TYPE_LEFT] =
    {
        0xFF,
        SEARCH_TYPE_RIGHT,
        0xFF,
        SEARCH_GROUP_LEFT
    },
    [SEARCH_TYPE_RIGHT] =
    {   SEARCH_TYPE_LEFT,
        0xFF,
        0xFF,
        SEARCH_GROUP_RIGHT
    },
    [SEARCH_GROUP_LEFT] =
    {
        0xFF,
        SEARCH_GROUP_RIGHT,
        SEARCH_TYPE_LEFT,
        SEARCH_ABILITY
    },
    [SEARCH_GROUP_RIGHT] =
    {   SEARCH_GROUP_LEFT,
        0xFF,
        SEARCH_TYPE_RIGHT,
        SEARCH_ABILITY
    },
    [SEARCH_ABILITY] = 
    {
        0xFF,
        0xFF,
        SEARCH_GROUP_LEFT,
        SEARCH_MOVE
    },
    [SEARCH_MOVE]
    {
        0xFF,
        SEARCH_MOVETYPE,
        SEARCH_ABILITY,
        SEARCH_ORDER
    },
    [SEARCH_MOVETYPE] =
    {
        SEARCH_MOVE,
        0xFF,
        SEARCH_ABILITY,
        SEARCH_ORDER
    },
    [SEARCH_ORDER] =
    {
        0xFF,
        0xFF,
        SEARCH_MOVE,
        SEARCH_OK
    },
    [SEARCH_MODE] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_OK] =
    {
        0xFF,
        0xFF,
        SEARCH_ORDER,
        0xFF
    },
};

// Left, Right, Up, Down
static const u8 sSearchMovementMap_ShiftHoennDex[SEARCH_COUNT][4] =
{
    /*
    [SEARCH_NAME] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_COLOR] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    */
    [SEARCH_TYPE_LEFT] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_TYPE_RIGHT] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_GROUP_LEFT] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_GROUP_RIGHT] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_ABILITY] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_MOVE] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
        [SEARCH_MOVETYPE] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_ORDER] =
    {
        0xFF,
        0xFF,
        0xFF,
        SEARCH_OK
    },
    [SEARCH_MODE] =
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF
    },
    [SEARCH_OK] =
    {
        0xFF,
        0xFF,
        SEARCH_ORDER,
        0xFF
    },
};

static const struct SearchOptionText sDexModeOptions[] =
{
    [DEX_MODE_HOENN]    = {gText_DexHoennDescription, gText_DexHoennTitle},
    [DEX_MODE_NATIONAL] = {gText_DexNatDescription,   gText_DexNatTitle},
    {},
};

static const struct SearchOptionText sDexOrderOptions[] =
{
    [ORDER_NUMERICAL]    = {gText_DexSortNumericalDescription, gText_DexSortNumericalTitle},
    [ORDER_ALPHABETICAL] = {gText_DexSortAtoZDescription,      gText_DexSortAtoZTitle},
    [ORDER_WEIGHT]       = {gText_DexSortWeightDescription,    gText_DexSortWeightTitle},
    //[ORDER_LIGHTEST]     = {gText_DexSortLightestDescription,  gText_DexSortLightestTitle},
    [ORDER_HEIGHT]       = {gText_DexSortHeightDescription,    gText_DexSortHeightTitle},
    //[ORDER_SMALLEST]     = {gText_DexSortSmallestDescription,  gText_DexSortSmallestTitle},
    [ORDER_HP]           = {gText_DexSortHPDescription,        gText_DexSortHPTitle},
    [ORDER_ATTACK]       = {gText_DexSortAttackDescription,    gText_DexSortAttackTitle},
    [ORDER_DEFENSE]      = {gText_DexSortDefenseDescription,   gText_DexSortDefenseTitle},
    [ORDER_SP_ATK]       = {gText_DexSortSpAtkDescription,     gText_DexSortSpAtkTitle},
    [ORDER_SP_DEF]       = {gText_DexSortSpDefDescription,     gText_DexSortSpDefTitle},
    [ORDER_SPEED]        = {gText_DexSortSpeedDescription,     gText_DexSortSpeedTitle},
    [ORDER_STATS]        = {gText_DexSortStatsDescription,     gText_DexSortStatsTitle},
    {},
};

/*
static const struct SearchOptionText sDexSearchNameOptions[] =
{
    {gText_DexEmptyString, gText_DexSearchDontSpecify},
    [NAME_ABC] = {gText_DexEmptyString, gText_DexSearchAlphaABC},
    [NAME_DEF] = {gText_DexEmptyString, gText_DexSearchAlphaDEF},
    [NAME_GHI] = {gText_DexEmptyString, gText_DexSearchAlphaGHI},
    [NAME_JKL] = {gText_DexEmptyString, gText_DexSearchAlphaJKL},
    [NAME_MNO] = {gText_DexEmptyString, gText_DexSearchAlphaMNO},
    [NAME_PQR] = {gText_DexEmptyString, gText_DexSearchAlphaPQR},
    [NAME_STU] = {gText_DexEmptyString, gText_DexSearchAlphaSTU},
    [NAME_VWX] = {gText_DexEmptyString, gText_DexSearchAlphaVWX},
    [NAME_YZ]  = {gText_DexEmptyString, gText_DexSearchAlphaYZ},
    {},
};

static const struct SearchOptionText sDexSearchColorOptions[] =
{
    {gText_DexEmptyString, gText_DexSearchDontSpecify},
    [BODY_COLOR_RED + 1]    = {gText_DexEmptyString, gText_DexSearchColorRed},
    [BODY_COLOR_BLUE + 1]   = {gText_DexEmptyString, gText_DexSearchColorBlue},
    [BODY_COLOR_YELLOW + 1] = {gText_DexEmptyString, gText_DexSearchColorYellow},
    [BODY_COLOR_GREEN + 1]  = {gText_DexEmptyString, gText_DexSearchColorGreen},
    [BODY_COLOR_BLACK + 1]  = {gText_DexEmptyString, gText_DexSearchColorBlack},
    [BODY_COLOR_BROWN + 1]  = {gText_DexEmptyString, gText_DexSearchColorBrown},
    [BODY_COLOR_PURPLE + 1] = {gText_DexEmptyString, gText_DexSearchColorPurple},
    [BODY_COLOR_GRAY + 1]   = {gText_DexEmptyString, gText_DexSearchColorGray},
    [BODY_COLOR_WHITE + 1]  = {gText_DexEmptyString, gText_DexSearchColorWhite},
    [BODY_COLOR_PINK + 1]   = {gText_DexEmptyString, gText_DexSearchColorPink},
    {},
};
*/

static const struct SearchOptionText sDexSearchGroupOptions[] =
{
    {gText_DexEmptyString, gText_DexSearchTypeNone},
    {gText_DexEmptyString, gText_DexSearchGroupAmorphous},
    {gText_DexEmptyString, gText_DexSearchGroupBug},
    {gText_DexEmptyString, gText_DexSearchGroupDitto},
    {gText_DexEmptyString, gText_DexSearchGroupDragon},
    {gText_DexEmptyString, gText_DexSearchGroupFairy},
    {gText_DexEmptyString, gText_DexSearchGroupField},
    {gText_DexEmptyString, gText_DexSearchGroupFlying},
    {gText_DexEmptyString, gText_DexSearchGroupGrass},
    {gText_DexEmptyString, gText_DexSearchGroupHumanLike},
    {gText_DexEmptyString, gText_DexSearchGroupMineral},
    {gText_DexEmptyString, gText_DexSearchGroupMonster},
    {gText_DexEmptyString, gText_DexSearchGroupWater1},
    {gText_DexEmptyString, gText_DexSearchGroupWater2},
    {gText_DexEmptyString, gText_DexSearchGroupWater3},
    {gText_DexEmptyString, gText_DexSearchGroupUnknown},
    {},
};

static const struct SearchOptionText sDexSearchAbilityInitialOptions[] =
{
    {gText_DexEmptyString, gText_DexSearchTypeNone},
    [NAME_ABC]  = {gText_DexEmptyString, gText_DexSearchAlphaABC},
    [NAME_DEF]  = {gText_DexEmptyString, gText_DexSearchAlphaDEF},
    [NAME_GHI]  = {gText_DexEmptyString, gText_DexSearchAlphaGHI},
    [NAME_JKL]  = {gText_DexEmptyString, gText_DexSearchAlphaJKL},
    [NAME_MNO]  = {gText_DexEmptyString, gText_DexSearchAlphaMNO},
    [NAME_PQR]  = {gText_DexEmptyString, gText_DexSearchAlphaPQR},
    [NAME_STU]  = {gText_DexEmptyString, gText_DexSearchAlphaSTU},
    [NAME_VWX]  = {gText_DexEmptyString, gText_DexSearchAlphaVWX},
    {},
};

static const struct SearchOptionText sDexSearchMoveInitialOptions[] =
{
    {gText_DexEmptyString, gText_DexSearchTypeNone},
    [SEARCH_TYPE_BUG]       = {gText_DexEmptyString, gText_DexSearchTypeBug},
    [SEARCH_TYPE_DARK]      = {gText_DexEmptyString, gText_DexSearchTypeDark},
    [SEARCH_TYPE_DRAGON]    = {gText_DexEmptyString, gText_DexSearchTypeDragon},
    [SEARCH_TYPE_ELECTRIC]  = {gText_DexEmptyString, gText_DexSearchTypeElectric},
    [SEARCH_TYPE_FIGHTING]  = {gText_DexEmptyString, gText_DexSearchTypeFighting},
    [SEARCH_TYPE_FIRE]      = {gText_DexEmptyString, gText_DexSearchTypeFire},
    [SEARCH_TYPE_FLYING]    = {gText_DexEmptyString, gText_DexSearchTypeFlying},
    [SEARCH_TYPE_GHOST]     = {gText_DexEmptyString, gText_DexSearchTypeGhost},
    [SEARCH_TYPE_GRASS]     = {gText_DexEmptyString, gText_DexSearchTypeGrass},
    [SEARCH_TYPE_GROUND]    = {gText_DexEmptyString, gText_DexSearchTypeGround},
    [SEARCH_TYPE_ICE]       = {gText_DexEmptyString, gText_DexSearchTypeIce},
    [SEARCH_TYPE_NORMAL_AE] = {gText_DexEmptyString, gText_DexSearchTypeNormalAE},
    [SEARCH_TYPE_NORMAL_FL] = {gText_DexEmptyString, gText_DexSearchTypeNormalFL},
    [SEARCH_TYPE_NORMAL_MR] = {gText_DexEmptyString, gText_DexSearchTypeNormalMR},
    [SEARCH_TYPE_NORMAL_S]  = {gText_DexEmptyString, gText_DexSearchTypeNormalS},
    [SEARCH_TYPE_NORMAL_TZ] = {gText_DexEmptyString, gText_DexSearchTypeNormalTZ},
    [SEARCH_TYPE_POISON]    = {gText_DexEmptyString, gText_DexSearchTypePoison},
    [SEARCH_TYPE_PSYCHIC]   = {gText_DexEmptyString, gText_DexSearchTypePsychic},
    [SEARCH_TYPE_ROCK]      = {gText_DexEmptyString, gText_DexSearchTypeRock},
    [SEARCH_TYPE_STEEL]     = {gText_DexEmptyString, gText_DexSearchTypeSteel},
    [SEARCH_TYPE_WATER]     = {gText_DexEmptyString, gText_DexSearchTypeWater},
    {},
};

static const struct SearchOptionText sDexSearchAbilityOptionsABC[] =
{
    {sText_AbilityDescription_AirLock,      gAbilityNames[ABILITY_AIR_LOCK]},
    {sText_AbilityDescription_ArenaTrap,    gAbilityNames[ABILITY_ARENA_TRAP]},
    {sText_AbilityDescription_BattleArmor,  gAbilityNames[ABILITY_BATTLE_ARMOR]},
    {sText_AbilityDescription_Blaze,        gAbilityNames[ABILITY_BLAZE]},
    {sText_AbilityDescription_Cacophony,    gAbilityNames[ABILITY_CACOPHONY]},
    {sText_AbilityDescription_Chlorophyll,  gAbilityNames[ABILITY_CHLOROPHYLL]},
    {sText_AbilityDescription_ClearBody,    gAbilityNames[ABILITY_CLEAR_BODY]},
    {sText_AbilityDescription_CloudNine,    gAbilityNames[ABILITY_CLOUD_NINE]},
    {sText_AbilityDescription_ColorChange,  gAbilityNames[ABILITY_COLOR_CHANGE]},
    {sText_AbilityDescription_CompoundEyes, gAbilityNames[ABILITY_COMPOUND_EYES]},
    {sText_AbilityDescription_CuteCharm,    gAbilityNames[ABILITY_CUTE_CHARM]},
    {},
};

static const struct SearchOptionText sDexSearchAbilityOptionsDEF[] =
{
    {sText_AbilityDescription_Damp,         gAbilityNames[ABILITY_DAMP]},
    {sText_AbilityDescription_Drizzle,      gAbilityNames[ABILITY_DRIZZLE]},
    {sText_AbilityDescription_Drought,      gAbilityNames[ABILITY_DROUGHT]},
    {sText_AbilityDescription_EarlyBird,    gAbilityNames[ABILITY_EARLY_BIRD]},
    {sText_AbilityDescription_EffectSpore,  gAbilityNames[ABILITY_EFFECT_SPORE]},
    {sText_AbilityDescription_FlameBody,    gAbilityNames[ABILITY_FLAME_BODY]},
    {sText_AbilityDescription_FlashFire,    gAbilityNames[ABILITY_FLASH_FIRE]},
    {sText_AbilityDescription_Forecast,     gAbilityNames[ABILITY_FORECAST]},
    {},
};

static const struct SearchOptionText sDexSearchAbilityOptionsGHI[] =
{
    {sText_AbilityDescription_Guts,         gAbilityNames[ABILITY_GUTS]},
    {sText_AbilityDescription_HugePower,    gAbilityNames[ABILITY_HUGE_POWER]},
    {sText_AbilityDescription_Hustle,       gAbilityNames[ABILITY_HUSTLE]},
    {sText_AbilityDescription_HyperCutter,  gAbilityNames[ABILITY_HYPER_CUTTER]},
    {sText_AbilityDescription_Illuminate,   gAbilityNames[ABILITY_ILLUMINATE]},
    {sText_AbilityDescription_Immunity,     gAbilityNames[ABILITY_IMMUNITY]},
    {sText_AbilityDescription_InnerFocus,   gAbilityNames[ABILITY_INNER_FOCUS]},
    {sText_AbilityDescription_Insomnia,     gAbilityNames[ABILITY_INSOMNIA]},
    {sText_AbilityDescription_Intimidate,   gAbilityNames[ABILITY_INTIMIDATE]},
    {},
};

static const struct SearchOptionText sDexSearchAbilityOptionsJKL[] =
{
    {sText_AbilityDescription_KeenEye,      gAbilityNames[ABILITY_KEEN_EYE]},
    {sText_AbilityDescription_Levitate,     gAbilityNames[ABILITY_LEVITATE]},
    {sText_AbilityDescription_LightningRod, gAbilityNames[ABILITY_LIGHTNING_ROD]},
    {sText_AbilityDescription_Limber,       gAbilityNames[ABILITY_LIMBER]},
    {sText_AbilityDescription_LiquidOoze,   gAbilityNames[ABILITY_LIQUID_OOZE]},
    {},
};

static const struct SearchOptionText sDexSearchAbilityOptionsMNO[] =
{
    {sText_AbilityDescription_MagmaArmor,   gAbilityNames[ABILITY_MAGMA_ARMOR]},
    {sText_AbilityDescription_MagnetPull,   gAbilityNames[ABILITY_MAGNET_PULL]},
    {sText_AbilityDescription_MarvelScale,  gAbilityNames[ABILITY_MARVEL_SCALE]},
    {sText_AbilityDescription_Minus,        gAbilityNames[ABILITY_MINUS]},
    {sText_AbilityDescription_NaturalCure,  gAbilityNames[ABILITY_NATURAL_CURE]},
    {sText_AbilityDescription_Oblivious,    gAbilityNames[ABILITY_OBLIVIOUS]},
    {sText_AbilityDescription_Overgrow,     gAbilityNames[ABILITY_OVERGROW]},
    {sText_AbilityDescription_OwnTempo,     gAbilityNames[ABILITY_OWN_TEMPO]},
    {},
};

static const struct SearchOptionText sDexSearchAbilityOptionsPQR[] =
{
    {sText_AbilityDescription_Pickup,       gAbilityNames[ABILITY_PICKUP]},
    {sText_AbilityDescription_Plus,         gAbilityNames[ABILITY_PLUS]},
    {sText_AbilityDescription_PoisonPoint,  gAbilityNames[ABILITY_POISON_POINT]},
    {sText_AbilityDescription_Pressure,     gAbilityNames[ABILITY_PRESSURE]},
    {sText_AbilityDescription_PurePower,    gAbilityNames[ABILITY_PURE_POWER]},
    {sText_AbilityDescription_RainDish,     gAbilityNames[ABILITY_RAIN_DISH]},
    {sText_AbilityDescription_RockHead,     gAbilityNames[ABILITY_ROCK_HEAD]},
    {sText_AbilityDescription_RoughSkin,    gAbilityNames[ABILITY_ROUGH_SKIN]},
    {sText_AbilityDescription_RunAway,      gAbilityNames[ABILITY_RUN_AWAY]},
    {},
};

static const struct SearchOptionText sDexSearchAbilityOptionsSTU[] =
{
    {sText_AbilityDescription_SandStream,   gAbilityNames[ABILITY_SAND_STREAM]},
    {sText_AbilityDescription_SandVeil,     gAbilityNames[ABILITY_SAND_VEIL]},
    {sText_AbilityDescription_SereneGrace,  gAbilityNames[ABILITY_SERENE_GRACE]},
    {sText_AbilityDescription_ShadowTag,    gAbilityNames[ABILITY_SHADOW_TAG]},
    {sText_AbilityDescription_ShedSkin,     gAbilityNames[ABILITY_SHED_SKIN]},
    {sText_AbilityDescription_ShellArmor,   gAbilityNames[ABILITY_SHELL_ARMOR]},
    {sText_AbilityDescription_ShieldDust,   gAbilityNames[ABILITY_SHIELD_DUST]},
    {sText_AbilityDescription_Soundproof,   gAbilityNames[ABILITY_SOUNDPROOF]},
    {sText_AbilityDescription_SpeedBoost,   gAbilityNames[ABILITY_SPEED_BOOST]},
    {sText_AbilityDescription_Static,       gAbilityNames[ABILITY_STATIC]},
    {sText_AbilityDescription_Stench,       gAbilityNames[ABILITY_STENCH]},
    {sText_AbilityDescription_StickyHold,   gAbilityNames[ABILITY_STICKY_HOLD]},
    {sText_AbilityDescription_Sturdy,       gAbilityNames[ABILITY_STURDY]},
    {sText_AbilityDescription_SuctionCups,  gAbilityNames[ABILITY_SUCTION_CUPS]},
    {sText_AbilityDescription_Swarm,        gAbilityNames[ABILITY_SWARM]},
    {sText_AbilityDescription_SwiftSwim,    gAbilityNames[ABILITY_SWIFT_SWIM]},
    {sText_AbilityDescription_Synchronize,  gAbilityNames[ABILITY_SYNCHRONIZE]},
    {sText_AbilityDescription_ThickFat,     gAbilityNames[ABILITY_THICK_FAT]},
    {sText_AbilityDescription_Torrent,      gAbilityNames[ABILITY_TORRENT]},
    {sText_AbilityDescription_Trace,        gAbilityNames[ABILITY_TRACE]},
    {sText_AbilityDescription_Truant,       gAbilityNames[ABILITY_TRUANT]},
    {},
};

static const struct SearchOptionText sDexSearchAbilityOptionsVWX[] =
{
    {sText_AbilityDescription_VitalSpirit,  gAbilityNames[ABILITY_VITAL_SPIRIT]},
    {sText_AbilityDescription_VoltAbsorb,   gAbilityNames[ABILITY_VOLT_ABSORB]},
    {sText_AbilityDescription_WaterAbsorb,  gAbilityNames[ABILITY_WATER_ABSORB]},
    {sText_AbilityDescription_WaterVeil,    gAbilityNames[ABILITY_WATER_VEIL]},
    {sText_AbilityDescription_WhiteSmoke,   gAbilityNames[ABILITY_WHITE_SMOKE]},
    {sText_AbilityDescription_WonderGuard,  gAbilityNames[ABILITY_WONDER_GUARD]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsBug[] =
{
    {sFuryCutterDescription,    gMoveNames[MOVE_FURY_CUTTER]},
    {sLeechLifeDescription,     gMoveNames[MOVE_LEECH_LIFE]},
    {sMegahornDescription,      gMoveNames[MOVE_MEGAHORN]},
    {sPinMissileDescription,    gMoveNames[MOVE_PIN_MISSILE]},
    {sSignalBeamDescription,    gMoveNames[MOVE_SIGNAL_BEAM]},
    {sSilverWindDescription,    gMoveNames[MOVE_SILVER_WIND]},
    {sSpiderWebDescription,     gMoveNames[MOVE_SPIDER_WEB]},
    {sStringShotDescription,    gMoveNames[MOVE_STRING_SHOT]},
    {sTailGlowDescription,      gMoveNames[MOVE_TAIL_GLOW]},
    {sTwineedleDescription,     gMoveNames[MOVE_TWINEEDLE]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsDark[] =
{
    {sBeatUpDescription,        gMoveNames[MOVE_BEAT_UP]},
    {sBiteDescription,          gMoveNames[MOVE_BITE]},
    {sCrunchDescription,        gMoveNames[MOVE_CRUNCH]},
    {sFaintAttackDescription,   gMoveNames[MOVE_FAINT_ATTACK]},
    {sFakeTearsDescription,     gMoveNames[MOVE_FAKE_TEARS]},
    {sFlatterDescription,       gMoveNames[MOVE_FLATTER]},
    {sKnockOffDescription,      gMoveNames[MOVE_KNOCK_OFF]},
    {sMementoDescription,       gMoveNames[MOVE_MEMENTO]},
    {sPursuitDescription,       gMoveNames[MOVE_PURSUIT]},
    {sSnatchDescription,        gMoveNames[MOVE_SNATCH]},
    {sTauntDescription,         gMoveNames[MOVE_TAUNT]},
    {sThiefDescription,         gMoveNames[MOVE_THIEF]},
    {sTormentDescription,       gMoveNames[MOVE_TORMENT]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsDragon[] =
{
    {sDragonClawDescription,    gMoveNames[MOVE_DRAGON_CLAW]},
    {sDragonDanceDescription,   gMoveNames[MOVE_DRAGON_DANCE]},
    {sDragonRageDescription,    gMoveNames[MOVE_DRAGON_RAGE]},
    {sDragonBreathDescription,  gMoveNames[MOVE_DRAGON_BREATH]},
    {sOutrageDescription,       gMoveNames[MOVE_OUTRAGE]},
    {sTwisterDescription,       gMoveNames[MOVE_TWISTER]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsElectric[] =
{
    {sChargeDescription,        gMoveNames[MOVE_CHARGE]},
    {sShockWaveDescription,     gMoveNames[MOVE_SHOCK_WAVE]},
    {sSparkDescription,         gMoveNames[MOVE_SPARK]},
    {sThunderDescription,       gMoveNames[MOVE_THUNDER]},
    {sThunderWaveDescription,   gMoveNames[MOVE_THUNDER_WAVE]},
    {sThunderboltDescription,   gMoveNames[MOVE_THUNDERBOLT]},
    {sThunderPunchDescription,  gMoveNames[MOVE_THUNDER_PUNCH]},
    {sThunderShockDescription,  gMoveNames[MOVE_THUNDER_SHOCK]},
    {sVoltTackleDescription,    gMoveNames[MOVE_VOLT_TACKLE]},
    {sZapCannonDescription,     gMoveNames[MOVE_ZAP_CANNON]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsFighting[] =
{
    {sArmThrustDescription,     gMoveNames[MOVE_ARM_THRUST]},
    {sBrickBreakDescription,    gMoveNames[MOVE_BRICK_BREAK]},
    {sBulkUpDescription,        gMoveNames[MOVE_BULK_UP]},
    {sCounterDescription,       gMoveNames[MOVE_COUNTER]},
    {sCrossChopDescription,     gMoveNames[MOVE_CROSS_CHOP]},
    {sDetectDescription,        gMoveNames[MOVE_DETECT]},
    {sDoubleKickDescription,    gMoveNames[MOVE_DOUBLE_KICK]},
    {sDynamicPunchDescription,  gMoveNames[MOVE_DYNAMIC_PUNCH]},
    {sFocusPunchDescription,    gMoveNames[MOVE_FOCUS_PUNCH]},
    {sHiJumpKickDescription,    gMoveNames[MOVE_HI_JUMP_KICK]},
    {sJumpKickDescription,      gMoveNames[MOVE_JUMP_KICK]},
    {sKarateChopDescription,    gMoveNames[MOVE_KARATE_CHOP]},
    {sLowKickDescription,       gMoveNames[MOVE_LOW_KICK]},
    {sMachPunchDescription,     gMoveNames[MOVE_MACH_PUNCH]},
    {sRevengeDescription,       gMoveNames[MOVE_REVENGE]},
    {sReversalDescription,      gMoveNames[MOVE_REVERSAL]},
    {sRockSmashDescription,     gMoveNames[MOVE_ROCK_SMASH]},
    {sRollingKickDescription,   gMoveNames[MOVE_ROLLING_KICK]},
    {sSeismicTossDescription,   gMoveNames[MOVE_SEISMIC_TOSS]},
    {sSkyUppercutDescription,   gMoveNames[MOVE_SKY_UPPERCUT]},
    {sSubmissionDescription,    gMoveNames[MOVE_SUBMISSION]},
    {sSuperpowerDescription,    gMoveNames[MOVE_SUPERPOWER]},
    {sTripleKickDescription,    gMoveNames[MOVE_TRIPLE_KICK]},
    {sVitalThrowDescription,    gMoveNames[MOVE_VITAL_THROW]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsFire[] =
{
    {sBlastBurnDescription,     gMoveNames[MOVE_BLAST_BURN]},
    {sBlazeKickDescription,     gMoveNames[MOVE_BLAZE_KICK]},
    {sEmberDescription,         gMoveNames[MOVE_EMBER]},
    {sEruptionDescription,      gMoveNames[MOVE_ERUPTION]},
    {sFireBlastDescription,     gMoveNames[MOVE_FIRE_BLAST]},
    {sFirePunchDescription,     gMoveNames[MOVE_FIRE_PUNCH]},
    {sFireSpinDescription,      gMoveNames[MOVE_FIRE_SPIN]},
    {sFlameWheelDescription,    gMoveNames[MOVE_FLAME_WHEEL]},
    {sFlamethrowerDescription,  gMoveNames[MOVE_FLAMETHROWER]},
    {sHeatWaveDescription,      gMoveNames[MOVE_HEAT_WAVE]},
    {sOverheatDescription,      gMoveNames[MOVE_OVERHEAT]},
    {sSacredFireDescription,    gMoveNames[MOVE_SACRED_FIRE]},
    {sSunnyDayDescription,      gMoveNames[MOVE_SUNNY_DAY]},
    {sWillOWispDescription,     gMoveNames[MOVE_WILL_O_WISP]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsFlying[] =
{
    {sAerialAceDescription,     gMoveNames[MOVE_AERIAL_ACE]},
    {sAeroblastDescription,     gMoveNames[MOVE_AEROBLAST]},
    {sAirCutterDescription,     gMoveNames[MOVE_AIR_CUTTER]},
    {sBounceDescription,        gMoveNames[MOVE_BOUNCE]},
    {sDrillPeckDescription,     gMoveNames[MOVE_DRILL_PECK]},
    {sFeatherDanceDescription,  gMoveNames[MOVE_FEATHER_DANCE]},
    {sFlyDescription,           gMoveNames[MOVE_FLY]},
    {sGustDescription,          gMoveNames[MOVE_GUST]},
    {sMirrorMoveDescription,    gMoveNames[MOVE_MIRROR_MOVE]},
    {sPeckDescription,          gMoveNames[MOVE_PECK]},
    {sSkyAttackDescription,     gMoveNames[MOVE_SKY_ATTACK]},
    {sWingAttackDescription,    gMoveNames[MOVE_WING_ATTACK]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsGhost[] =
{
    {sAstonishDescription,      gMoveNames[MOVE_ASTONISH]},
    {sConfuseRayDescription,    gMoveNames[MOVE_CONFUSE_RAY]},
    {sDestinyBondDescription,   gMoveNames[MOVE_DESTINY_BOND]},
    {sGrudgeDescription,        gMoveNames[MOVE_GRUDGE]},
    {sLickDescription,          gMoveNames[MOVE_LICK]},
    {sNightShadeDescription,    gMoveNames[MOVE_NIGHT_SHADE]},
    {sNightmareDescription,     gMoveNames[MOVE_NIGHTMARE]},
    {sShadowBallDescription,    gMoveNames[MOVE_SHADOW_BALL]},
    {sShadowPunchDescription,   gMoveNames[MOVE_SHADOW_PUNCH]},
    {sSpiteDescription,         gMoveNames[MOVE_SPITE]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsGrass[] =
{
    {sAbsorbDescription,        gMoveNames[MOVE_ABSORB]},
    {sAromatherapyDescription,  gMoveNames[MOVE_AROMATHERAPY]},
    {sBulletSeedDescription,    gMoveNames[MOVE_BULLET_SEED]},
    {sCottonSporeDescription,   gMoveNames[MOVE_COTTON_SPORE]},
    {sFrenzyPlantDescription,   gMoveNames[MOVE_FRENZY_PLANT]},
    {sGigaDrainDescription,     gMoveNames[MOVE_GIGA_DRAIN]},
    {sGrassWhistleDescription,  gMoveNames[MOVE_GRASS_WHISTLE]},
    {sIngrainDescription,       gMoveNames[MOVE_INGRAIN]},
    {sLeafBladeDescription,     gMoveNames[MOVE_LEAF_BLADE]},
    {sLeechSeedDescription,     gMoveNames[MOVE_LEECH_SEED]},
    {sMagicalLeafDescription,   gMoveNames[MOVE_MAGICAL_LEAF]},
    {sMegaDrainDescription,     gMoveNames[MOVE_MEGA_DRAIN]},
    {sNeedleArmDescription,     gMoveNames[MOVE_NEEDLE_ARM]},
    {sPetalDanceDescription,    gMoveNames[MOVE_PETAL_DANCE]},
    {sRazorLeafDescription,     gMoveNames[MOVE_RAZOR_LEAF]},
    {sSleepPowderDescription,   gMoveNames[MOVE_SLEEP_POWDER]},
    {sSolarBeamDescription,     gMoveNames[MOVE_SOLAR_BEAM]},
    {sSporeDescription,         gMoveNames[MOVE_SPORE]},
    {sStunSporeDescription,     gMoveNames[MOVE_STUN_SPORE]},
    {sSynthesisDescription,     gMoveNames[MOVE_SYNTHESIS]},
    {sVineWhipDescription,      gMoveNames[MOVE_VINE_WHIP]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsGround[] =
{
    {sBoneClubDescription,      gMoveNames[MOVE_BONE_CLUB]},
    {sBoneRushDescription,      gMoveNames[MOVE_BONE_RUSH]},
    {sBonemerangDescription,    gMoveNames[MOVE_BONEMERANG]},
    {sDigDescription,           gMoveNames[MOVE_DIG]},
    {sEarthquakeDescription,    gMoveNames[MOVE_EARTHQUAKE]},
    {sFissureDescription,       gMoveNames[MOVE_FISSURE]},
    {sMagnitudeDescription,     gMoveNames[MOVE_MAGNITUDE]},
    {sMudShotDescription,       gMoveNames[MOVE_MUD_SHOT]},
    {sMudSportDescription,      gMoveNames[MOVE_MUD_SPORT]},
    {sMudSlapDescription,       gMoveNames[MOVE_MUD_SLAP]},
    {sSandTombDescription,      gMoveNames[MOVE_SAND_TOMB]},
    {sSandAttackDescription,    gMoveNames[MOVE_SAND_ATTACK]},
    {sSpikesDescription,        gMoveNames[MOVE_SPIKES]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsIce[] =
{
    {sAuroraBeamDescription,    gMoveNames[MOVE_AURORA_BEAM]},
    {sBlizzardDescription,      gMoveNames[MOVE_BLIZZARD]},
    {sHailDescription,          gMoveNames[MOVE_HAIL]},
    {sHazeDescription,          gMoveNames[MOVE_HAZE]},
    {sIceBallDescription,       gMoveNames[MOVE_ICE_BALL]},
    {sIceBeamDescription,       gMoveNames[MOVE_ICE_BEAM]},
    {sIcePunchDescription,      gMoveNames[MOVE_ICE_PUNCH]},
    {sIcicleSpearDescription,   gMoveNames[MOVE_ICICLE_SPEAR]},
    {sIcyWindDescription,       gMoveNames[MOVE_ICY_WIND]},
    {sMistDescription,          gMoveNames[MOVE_MIST]},
    {sPowderSnowDescription,    gMoveNames[MOVE_POWDER_SNOW]},
    {sSheerColdDescription,     gMoveNames[MOVE_SHEER_COLD]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsNormalAE[] =
{
    {sAssistDescription,        gMoveNames[MOVE_ASSIST]},
    {sAttractDescription,       gMoveNames[MOVE_ATTRACT]},
    {sBarrageDescription,       gMoveNames[MOVE_BARRAGE]},
    {sBatonPassDescription,     gMoveNames[MOVE_BATON_PASS]},
    {sBellyDrumDescription,     gMoveNames[MOVE_BELLY_DRUM]},
    {sBideDescription,          gMoveNames[MOVE_BIDE]},
    {sBindDescription,          gMoveNames[MOVE_BIND]},
    {sBlockDescription,         gMoveNames[MOVE_BLOCK]},
    {sBodySlamDescription,      gMoveNames[MOVE_BODY_SLAM]},
    {sCamouflageDescription,    gMoveNames[MOVE_CAMOUFLAGE]},
    {sCharmDescription,         gMoveNames[MOVE_CHARM]},
    {sCometPunchDescription,    gMoveNames[MOVE_COMET_PUNCH]},
    {sConstrictDescription,     gMoveNames[MOVE_CONSTRICT]},
    {sConversionDescription,    gMoveNames[MOVE_CONVERSION]},
    {sConversion2Description,   gMoveNames[MOVE_CONVERSION_2]},
    {sCovetDescription,         gMoveNames[MOVE_COVET]},
    {sCrushClawDescription,     gMoveNames[MOVE_CRUSH_CLAW]},
    {sCutDescription,           gMoveNames[MOVE_CUT]},
    {sDefenseCurlDescription,   gMoveNames[MOVE_DEFENSE_CURL]},
    {sDisableDescription,       gMoveNames[MOVE_DISABLE]},
    {sDizzyPunchDescription,    gMoveNames[MOVE_DIZZY_PUNCH]},
    {sDoubleTeamDescription,    gMoveNames[MOVE_DOUBLE_TEAM]},
    {sDoubleEdgeDescription,    gMoveNames[MOVE_DOUBLE_EDGE]},
    {sDoubleSlapDescription,    gMoveNames[MOVE_DOUBLE_SLAP]},
    {sEggBombDescription,       gMoveNames[MOVE_EGG_BOMB]},
    {sEncoreDescription,        gMoveNames[MOVE_ENCORE]},
    {sEndeavorDescription,      gMoveNames[MOVE_ENDEAVOR]},
    {sEndureDescription,        gMoveNames[MOVE_ENDURE]},
    {sExplosionDescription,     gMoveNames[MOVE_EXPLOSION]},
    {sExtremeSpeedDescription,  gMoveNames[MOVE_EXTREME_SPEED]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsNormalFL[] =
{
    {sFacadeDescription,        gMoveNames[MOVE_FACADE]},
    {sFakeOutDescription,       gMoveNames[MOVE_FAKE_OUT]},
    {sFalseSwipeDescription,    gMoveNames[MOVE_FALSE_SWIPE]},
    {sFlailDescription,         gMoveNames[MOVE_FLAIL]},
    {sFlashDescription,         gMoveNames[MOVE_FLASH]},
    {sFocusEnergyDescription,   gMoveNames[MOVE_FOCUS_ENERGY]},
    {sFollowMeDescription,      gMoveNames[MOVE_FOLLOW_ME]},
    {sForesightDescription,     gMoveNames[MOVE_FORESIGHT]},
    {sFrustrationDescription,   gMoveNames[MOVE_FRUSTRATION]},
    {sFuryAttackDescription,    gMoveNames[MOVE_FURY_ATTACK]},
    {sFurySwipesDescription,    gMoveNames[MOVE_FURY_SWIPES]},
    {sGlareDescription,         gMoveNames[MOVE_GLARE]},
    {sGrowlDescription,         gMoveNames[MOVE_GROWL]},
    {sGrowthDescription,        gMoveNames[MOVE_GROWTH]},
    {sGuillotineDescription,    gMoveNames[MOVE_GUILLOTINE]},
    {sHardenDescription,        gMoveNames[MOVE_HARDEN]},
    {sHeadbuttDescription,      gMoveNames[MOVE_HEADBUTT]},
    {sHealBellDescription,      gMoveNames[MOVE_HEAL_BELL]},
    {sHelpingHandDescription,   gMoveNames[MOVE_HELPING_HAND]},
    {sHiddenPowerDescription,   gMoveNames[MOVE_HIDDEN_POWER]},
    {sHornAttackDescription,    gMoveNames[MOVE_HORN_ATTACK]},
    {sHornDrillDescription,     gMoveNames[MOVE_HORN_DRILL]},
    {sHowlDescription,          gMoveNames[MOVE_HOWL]},
    {sHyperBeamDescription,     gMoveNames[MOVE_HYPER_BEAM]},
    {sHyperFangDescription,     gMoveNames[MOVE_HYPER_FANG]},
    {sHyperVoiceDescription,    gMoveNames[MOVE_HYPER_VOICE]},
    {sLeerDescription,          gMoveNames[MOVE_LEER]},
    {sLockOnDescription,        gMoveNames[MOVE_LOCK_ON]},
    {sLovelyKissDescription,    gMoveNames[MOVE_LOVELY_KISS]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsNormalMR[] =
{
    {sMeanLookDescription,      gMoveNames[MOVE_MEAN_LOOK]},
    {sMegaKickDescription,      gMoveNames[MOVE_MEGA_KICK]},
    {sMegaPunchDescription,     gMoveNames[MOVE_MEGA_PUNCH]},
    {sMetronomeDescription,     gMoveNames[MOVE_METRONOME]},
    {sMilkDrinkDescription,     gMoveNames[MOVE_MILK_DRINK]},
    {sMimicDescription,         gMoveNames[MOVE_MIMIC]},
    {sMindReaderDescription,    gMoveNames[MOVE_MIND_READER]},
    {sMinimizeDescription,      gMoveNames[MOVE_MINIMIZE]},
    {sMoonlightDescription,     gMoveNames[MOVE_MOONLIGHT]},
    {sMorningSunDescription,    gMoveNames[MOVE_MORNING_SUN]},
    {sNaturePowerDescription,   gMoveNames[MOVE_NATURE_POWER]},
    {sOdorSleuthDescription,    gMoveNames[MOVE_ODOR_SLEUTH]},
    {sPainSplitDescription,     gMoveNames[MOVE_PAIN_SPLIT]},
    {sPayDayDescription,        gMoveNames[MOVE_PAY_DAY]},
    {sPerishSongDescription,    gMoveNames[MOVE_PERISH_SONG]},
    {sPoundDescription,         gMoveNames[MOVE_POUND]},
    {sPresentDescription,       gMoveNames[MOVE_PRESENT]},
    {sProtectDescription,       gMoveNames[MOVE_PROTECT]},
    {sPsychUpDescription,       gMoveNames[MOVE_PSYCH_UP]},
    {sQuickAttackDescription,   gMoveNames[MOVE_QUICK_ATTACK]},
    {sRageDescription,          gMoveNames[MOVE_RAGE]},
    {sRapidSpinDescription,     gMoveNames[MOVE_RAPID_SPIN]},
    {sRazorWindDescription,     gMoveNames[MOVE_RAZOR_WIND]},
    {sRecoverDescription,       gMoveNames[MOVE_RECOVER]},
    {sRecycleDescription,       gMoveNames[MOVE_RECYCLE]},
    {sRefreshDescription,       gMoveNames[MOVE_REFRESH]},
    {sReturnDescription,        gMoveNames[MOVE_RETURN]},
    {sRoarDescription,          gMoveNames[MOVE_ROAR]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsNormalS[] =
{
    {sSafeguardDescription,     gMoveNames[MOVE_SAFEGUARD]},
    {sScaryFaceDescription,     gMoveNames[MOVE_SCARY_FACE]},
    {sScratchDescription,       gMoveNames[MOVE_SCRATCH]},
    {sScreechDescription,       gMoveNames[MOVE_SCREECH]},
    {sSecretPowerDescription,   gMoveNames[MOVE_SECRET_POWER]},
    {sSelfDestructDescription,  gMoveNames[MOVE_SELF_DESTRUCT]},
    {sSharpenDescription,       gMoveNames[MOVE_SHARPEN]},
    {sSingDescription,          gMoveNames[MOVE_SING]},
    {sSketchDescription,        gMoveNames[MOVE_SKETCH]},
    {sSkullBashDescription,     gMoveNames[MOVE_SKULL_BASH]},
    {sSlackOffDescription,      gMoveNames[MOVE_SLACK_OFF]},
    {sSlamDescription,          gMoveNames[MOVE_SLAM]},
    {sSlashDescription,         gMoveNames[MOVE_SLASH]},
    {sSleepTalkDescription,     gMoveNames[MOVE_SLEEP_TALK]},
    {sSmellingSaltDescription,  gMoveNames[MOVE_SMELLING_SALT]},
    {sSmokescreenDescription,   gMoveNames[MOVE_SMOKESCREEN]},
    {sSnoreDescription,         gMoveNames[MOVE_SNORE]},
    {sSoftBoiledDescription,    gMoveNames[MOVE_SOFT_BOILED]},
    {sSonicBoomDescription,     gMoveNames[MOVE_SONIC_BOOM]},
    {sSpikeCannonDescription,   gMoveNames[MOVE_SPIKE_CANNON]},
    {sSpitUpDescription,        gMoveNames[MOVE_SPIT_UP]},
    {sSplashDescription,        gMoveNames[MOVE_SPLASH]},
    {sStockpileDescription,     gMoveNames[MOVE_STOCKPILE]},
    {sStompDescription,         gMoveNames[MOVE_STOMP]},
    {sStrengthDescription,      gMoveNames[MOVE_STRENGTH]},
    {sStruggleDescription,      gMoveNames[MOVE_STRUGGLE]},
    {sSubstituteDescription,    gMoveNames[MOVE_SUBSTITUTE]},
    {sSuperFangDescription,     gMoveNames[MOVE_SUPER_FANG]},
    {sSupersonicDescription,    gMoveNames[MOVE_SUPERSONIC]},
    {sSwaggerDescription,       gMoveNames[MOVE_SWAGGER]},
    {sSwallowDescription,       gMoveNames[MOVE_SWALLOW]},
    {sSweetKissDescription,     gMoveNames[MOVE_SWEET_KISS]},
    {sSweetScentDescription,    gMoveNames[MOVE_SWEET_SCENT]},
    {sSwiftDescription,         gMoveNames[MOVE_SWIFT]},
    {sSwordsDanceDescription,   gMoveNames[MOVE_SWORDS_DANCE]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsNormalTZ[] =
{
    {sTackleDescription,        gMoveNames[MOVE_TACKLE]},
    {sTailWhipDescription,      gMoveNames[MOVE_TAIL_WHIP]},
    {sTakeDownDescription,      gMoveNames[MOVE_TAKE_DOWN]},
    {sTeeterDanceDescription,   gMoveNames[MOVE_TEETER_DANCE]},
    {sThrashDescription,        gMoveNames[MOVE_THRASH]},
    {sTickleDescription,        gMoveNames[MOVE_TICKLE]},
    {sTransformDescription,     gMoveNames[MOVE_TRANSFORM]},
    {sTriAttackDescription,     gMoveNames[MOVE_TRI_ATTACK]},
    {sUproarDescription,        gMoveNames[MOVE_UPROAR]},
    {sViceGripDescription,      gMoveNames[MOVE_VICE_GRIP]},
    {sWeatherBallDescription,   gMoveNames[MOVE_WEATHER_BALL]},
    {sWhirlwindDescription,     gMoveNames[MOVE_WHIRLWIND]},
    {sWishDescription,          gMoveNames[MOVE_WISH]},
    {sWrapDescription,          gMoveNames[MOVE_WRAP]},
    {sYawnDescription,          gMoveNames[MOVE_YAWN]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsPoison[] =
{
    {sAcidDescription,          gMoveNames[MOVE_ACID]},
    {sAcidArmorDescription,     gMoveNames[MOVE_ACID_ARMOR]},
    {sPoisonFangDescription,    gMoveNames[MOVE_POISON_FANG]},
    {sPoisonGasDescription,     gMoveNames[MOVE_POISON_GAS]},
    {sPoisonStingDescription,   gMoveNames[MOVE_POISON_STING]},
    {sPoisonTailDescription,    gMoveNames[MOVE_POISON_TAIL]},
    {sPoisonPowderDescription,  gMoveNames[MOVE_POISON_POWDER]},
    {sSludgeDescription,        gMoveNames[MOVE_SLUDGE]},
    {sSludgeBombDescription,    gMoveNames[MOVE_SLUDGE_BOMB]},
    {sSmogDescription,          gMoveNames[MOVE_SMOG]},
    {sToxicDescription,         gMoveNames[MOVE_TOXIC]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsPsychic[] =
{
    {sAgilityDescription,       gMoveNames[MOVE_AGILITY]},
    {sAmnesiaDescription,       gMoveNames[MOVE_AMNESIA]},
    {sBarrierDescription,       gMoveNames[MOVE_BARRIER]},
    {sCalmMindDescription,      gMoveNames[MOVE_CALM_MIND]},
    {sConfusionDescription,     gMoveNames[MOVE_CONFUSION]},
    {sCosmicPowerDescription,   gMoveNames[MOVE_COSMIC_POWER]},
    {sDreamEaterDescription,    gMoveNames[MOVE_DREAM_EATER]},
    {sExtrasensoryDescription,  gMoveNames[MOVE_EXTRASENSORY]},
    {sFutureSightDescription,   gMoveNames[MOVE_FUTURE_SIGHT]},
    {sHypnosisDescription,      gMoveNames[MOVE_HYPNOSIS]},
    {sImprisonDescription,      gMoveNames[MOVE_IMPRISON]},
    {sKinesisDescription,       gMoveNames[MOVE_KINESIS]},
    {sLightScreenDescription,   gMoveNames[MOVE_LIGHT_SCREEN]},
    {sLusterPurgeDescription,   gMoveNames[MOVE_LUSTER_PURGE]},
    {sMagicCoatDescription,     gMoveNames[MOVE_MAGIC_COAT]},
    {sMeditateDescription,      gMoveNames[MOVE_MEDITATE]},
    {sMirrorCoatDescription,    gMoveNames[MOVE_MIRROR_COAT]},
    {sMistBallDescription,      gMoveNames[MOVE_MIST_BALL]},
    {sPsybeamDescription,       gMoveNames[MOVE_PSYBEAM]},
    {sPsychicDescription,       gMoveNames[MOVE_PSYCHIC]},
    {sPsychoBoostDescription,   gMoveNames[MOVE_PSYCHO_BOOST]},
    {sPsywaveDescription,       gMoveNames[MOVE_PSYWAVE]},
    {sReflectDescription,       gMoveNames[MOVE_REFLECT]},
    {sRestDescription,          gMoveNames[MOVE_REST]},
    {sRolePlayDescription,      gMoveNames[MOVE_ROLE_PLAY]},
    {sSkillSwapDescription,     gMoveNames[MOVE_SKILL_SWAP]},
    {sTeleportDescription,      gMoveNames[MOVE_TELEPORT]},
    {sTrickDescription,         gMoveNames[MOVE_TRICK]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsRock[] =
{
    {sAncientPowerDescription,  gMoveNames[MOVE_ANCIENT_POWER]},
    {sRockBlastDescription,     gMoveNames[MOVE_ROCK_BLAST]},
    {sRockSlideDescription,     gMoveNames[MOVE_ROCK_SLIDE]},
    {sRockThrowDescription,     gMoveNames[MOVE_ROCK_THROW]},
    {sRockTombDescription,      gMoveNames[MOVE_ROCK_TOMB]},
    {sRolloutDescription,       gMoveNames[MOVE_ROLLOUT]},
    {sSandstormDescription,     gMoveNames[MOVE_SANDSTORM]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsSteel[] =
{
    {sDoomDesireDescription,    gMoveNames[MOVE_DOOM_DESIRE]},
    {sIronDefenseDescription,   gMoveNames[MOVE_IRON_DEFENSE]},
    {sIronTailDescription,      gMoveNames[MOVE_IRON_TAIL]},
    {sMetalClawDescription,     gMoveNames[MOVE_METAL_CLAW]},
    {sMetalSoundDescription,    gMoveNames[MOVE_METAL_SOUND]},
    {sMeteorMashDescription,    gMoveNames[MOVE_METEOR_MASH]},
    {sSteelWingDescription,     gMoveNames[MOVE_STEEL_WING]},
    {},
};

static const struct SearchOptionText sDexSearchMoveOptionsWater[] =
{
    {sBubbleDescription,        gMoveNames[MOVE_BUBBLE]},
    {sBubbleBeamDescription,    gMoveNames[MOVE_BUBBLE_BEAM]},
    {sClampDescription,         gMoveNames[MOVE_CLAMP]},
    {sCrabhammerDescription,    gMoveNames[MOVE_CRABHAMMER]},
    {sDiveDescription,          gMoveNames[MOVE_DIVE]},
    {sHydroCannonDescription,   gMoveNames[MOVE_HYDRO_CANNON]},
    {sHydroPumpDescription,     gMoveNames[MOVE_HYDRO_PUMP]},
    {sMuddyWaterDescription,    gMoveNames[MOVE_MUDDY_WATER]},
    {sOctazookaDescription,     gMoveNames[MOVE_OCTAZOOKA]},
    {sRainDanceDescription,     gMoveNames[MOVE_RAIN_DANCE]},
    {sSurfDescription,          gMoveNames[MOVE_SURF]},
    {sWaterGunDescription,      gMoveNames[MOVE_WATER_GUN]},
    {sWaterPulseDescription,    gMoveNames[MOVE_WATER_PULSE]},
    {sWaterSportDescription,    gMoveNames[MOVE_WATER_SPORT]},
    {sWaterSpoutDescription,    gMoveNames[MOVE_WATER_SPOUT]},
    {sWaterfallDescription,     gMoveNames[MOVE_WATERFALL]},
    {sWhirlpoolDescription,     gMoveNames[MOVE_WHIRLPOOL]},
    {sWithdrawDescription,      gMoveNames[MOVE_WITHDRAW]},
    {},
};

static const struct SearchOptionText sDexSearchMoveTypeOptions[] = 
{
    {gText_DexEmptyString, sText_Any},
    {gText_DexEmptyString, sText_LevelUp},
    {gText_DexEmptyString, sText_TMHM},
    {gText_DexEmptyString, sText_Tutor},
    {gText_DexEmptyString, sText_EggMove},
    {},
};

static const struct SearchOptionText sDexSearchTypeOptions[NUMBER_OF_MON_TYPES + 1] = // + 2 for "None" and terminator, - 1 for Mystery
{
    {gText_DexEmptyString, gText_DexSearchTypeNone},
    {gText_DexEmptyString, gTypeNames[TYPE_BUG]},
    {gText_DexEmptyString, gTypeNames[TYPE_DARK]},
    {gText_DexEmptyString, gTypeNames[TYPE_DRAGON]},
    {gText_DexEmptyString, gText_DexSearchTypeElectric},
    {gText_DexEmptyString, gTypeNames[TYPE_FIGHTING]},
    {gText_DexEmptyString, gTypeNames[TYPE_FIRE]},
    {gText_DexEmptyString, gTypeNames[TYPE_FLYING]},
    {gText_DexEmptyString, gTypeNames[TYPE_GHOST]},
    {gText_DexEmptyString, gTypeNames[TYPE_GRASS]},
    {gText_DexEmptyString, gTypeNames[TYPE_GROUND]},
    {gText_DexEmptyString, gTypeNames[TYPE_ICE]},
    {gText_DexEmptyString, gTypeNames[TYPE_NORMAL]},
    {gText_DexEmptyString, gTypeNames[TYPE_POISON]},
    {gText_DexEmptyString, gText_DexSearchTypePsychic},
    {gText_DexEmptyString, gTypeNames[TYPE_ROCK]},
    {gText_DexEmptyString, gTypeNames[TYPE_STEEL]},
    {gText_DexEmptyString, gTypeNames[TYPE_WATER]},
    {},
};

static const u8 sPokedexModes[] = {DEX_MODE_HOENN, DEX_MODE_NATIONAL};
static const u8 sOrderOptions[] =
{
    ORDER_NUMERICAL,
    ORDER_ALPHABETICAL,
    ORDER_WEIGHT,
    //ORDER_LIGHTEST,
    ORDER_HEIGHT,
    //ORDER_SMALLEST,
    ORDER_HP,
    ORDER_ATTACK,
    ORDER_DEFENSE,
    ORDER_SP_ATK,
    ORDER_SP_DEF,
    ORDER_SPEED,
    ORDER_STATS,
};

static const u8 sDexSearchTypeIds[NUMBER_OF_MON_TYPES] =
{
    TYPE_NONE,
    TYPE_BUG,
    TYPE_DARK,
    TYPE_DRAGON,
    TYPE_ELECTRIC,
    TYPE_FIGHTING,
    TYPE_FIRE,
    TYPE_FLYING,
    TYPE_GHOST,
    TYPE_GRASS,
    TYPE_GROUND,
    TYPE_ICE,
    TYPE_NORMAL,
    TYPE_POISON,
    TYPE_PSYCHIC,
    TYPE_ROCK,
    TYPE_STEEL,
    TYPE_WATER,
};

static const u8 sDexSearchGroupIds[EGG_GROUPS_COUNT] =
{
    EGG_GROUP_NONE,
    EGG_GROUP_AMORPHOUS,
    EGG_GROUP_BUG,
    EGG_GROUP_DITTO,
    EGG_GROUP_DRAGON,
    EGG_GROUP_FAIRY,
    EGG_GROUP_FIELD,
    EGG_GROUP_FLYING,
    EGG_GROUP_GRASS,
    EGG_GROUP_HUMAN_LIKE,
    EGG_GROUP_MINERAL,
    EGG_GROUP_MONSTER,
    EGG_GROUP_WATER_1,
    EGG_GROUP_WATER_2,
    EGG_GROUP_WATER_3,
    EGG_GROUP_NO_EGGS_DISCOVERED,
};

static const u16 sDexSearchMoveIdsBug[] =
{
    MOVE_FURY_CUTTER,
    MOVE_LEECH_LIFE,
    MOVE_MEGAHORN,
    MOVE_PIN_MISSILE,
    MOVE_SIGNAL_BEAM,
    MOVE_SILVER_WIND,
    MOVE_SPIDER_WEB,
    MOVE_STRING_SHOT,
    MOVE_TAIL_GLOW,
    MOVE_TWINEEDLE,
};

static const u16 sDexSearchMoveIdsDark[] =
{
    MOVE_BEAT_UP,
    MOVE_BITE,
    MOVE_CRUNCH,
    MOVE_FAINT_ATTACK,
    MOVE_FAKE_TEARS,
    MOVE_FLATTER,
    MOVE_KNOCK_OFF,
    MOVE_MEMENTO,
    MOVE_PURSUIT,
    MOVE_SNATCH,
    MOVE_TAUNT,
    MOVE_THIEF,
    MOVE_TORMENT,
};

static const u16 sDexSearchMoveIdsDragon[] =
{
    MOVE_DRAGON_CLAW,
    MOVE_DRAGON_DANCE,
    MOVE_DRAGON_RAGE,
    MOVE_DRAGON_BREATH,
    MOVE_OUTRAGE,
    MOVE_TWISTER,
};

static const u16 sDexSearchMoveIdsElectric[] =
{
    MOVE_CHARGE,
    MOVE_SHOCK_WAVE,
    MOVE_SPARK,
    MOVE_THUNDER,
    MOVE_THUNDER_WAVE,
    MOVE_THUNDERBOLT,
    MOVE_THUNDER_PUNCH,
    MOVE_THUNDER_SHOCK,
    MOVE_VOLT_TACKLE,
    MOVE_ZAP_CANNON,
};

static const u16 sDexSearchMoveIdsFighting[] =
{
    MOVE_ARM_THRUST,
    MOVE_BRICK_BREAK,
    MOVE_BULK_UP,
    MOVE_COUNTER,
    MOVE_CROSS_CHOP,
    MOVE_DETECT,
    MOVE_DOUBLE_KICK,
    MOVE_DYNAMIC_PUNCH,
    MOVE_FOCUS_PUNCH,
    MOVE_HI_JUMP_KICK,
    MOVE_JUMP_KICK,
    MOVE_KARATE_CHOP,
    MOVE_LOW_KICK,
    MOVE_MACH_PUNCH,
    MOVE_REVENGE,
    MOVE_REVERSAL,
    MOVE_ROCK_SMASH,
    MOVE_ROLLING_KICK,
    MOVE_SEISMIC_TOSS,
    MOVE_SKY_UPPERCUT,
    MOVE_SUBMISSION,
    MOVE_SUPERPOWER,
    MOVE_TRIPLE_KICK,
    MOVE_VITAL_THROW,
};

static const u16 sDexSearchMoveIdsFire[] =
{
    MOVE_BLAST_BURN,
    MOVE_BLAZE_KICK,
    MOVE_EMBER,
    MOVE_ERUPTION,
    MOVE_FIRE_BLAST,
    MOVE_FIRE_PUNCH,
    MOVE_FIRE_SPIN,
    MOVE_FLAME_WHEEL,
    MOVE_FLAMETHROWER,
    MOVE_HEAT_WAVE,
    MOVE_OVERHEAT,
    MOVE_SACRED_FIRE,
    MOVE_SUNNY_DAY,
    MOVE_WILL_O_WISP,
};

static const u16 sDexSearchMoveIdsFlying[] =
{
    MOVE_AERIAL_ACE,
    MOVE_AEROBLAST,
    MOVE_AIR_CUTTER,
    MOVE_BOUNCE,
    MOVE_DRILL_PECK,
    MOVE_FEATHER_DANCE,
    MOVE_FLY,
    MOVE_GUST,
    MOVE_MIRROR_MOVE,
    MOVE_PECK,
    MOVE_SKY_ATTACK,
    MOVE_WING_ATTACK,
};

static const u16 sDexSearchMoveIdsGhost[] =
{
    MOVE_ASTONISH,
    MOVE_CONFUSE_RAY,
    MOVE_DESTINY_BOND,
    MOVE_GRUDGE,
    MOVE_LICK,
    MOVE_NIGHT_SHADE,
    MOVE_NIGHTMARE,
    MOVE_SHADOW_BALL,
    MOVE_SHADOW_PUNCH,
    MOVE_SPITE,
};

static const u16 sDexSearchMoveIdsGrass[] =
{
    MOVE_ABSORB,
    MOVE_AROMATHERAPY,
    MOVE_BULLET_SEED,
    MOVE_COTTON_SPORE,
    MOVE_FRENZY_PLANT,
    MOVE_GIGA_DRAIN,
    MOVE_GRASS_WHISTLE,
    MOVE_INGRAIN,
    MOVE_LEAF_BLADE,
    MOVE_LEECH_SEED,
    MOVE_MAGICAL_LEAF,
    MOVE_MEGA_DRAIN,
    MOVE_NEEDLE_ARM,
    MOVE_PETAL_DANCE,
    MOVE_RAZOR_LEAF,
    MOVE_SLEEP_POWDER,
    MOVE_SOLAR_BEAM,
    MOVE_SPORE,
    MOVE_STUN_SPORE,
    MOVE_SYNTHESIS,
    MOVE_VINE_WHIP,
};

static const u16 sDexSearchMoveIdsGround[] =
{
    MOVE_BONE_CLUB,
    MOVE_BONE_RUSH,
    MOVE_BONEMERANG,
    MOVE_DIG,
    MOVE_EARTHQUAKE,
    MOVE_FISSURE,
    MOVE_MAGNITUDE,
    MOVE_MUD_SHOT,
    MOVE_MUD_SPORT,
    MOVE_MUD_SLAP,
    MOVE_SAND_TOMB,
    MOVE_SAND_ATTACK,
    MOVE_SPIKES,
};

static const u16 sDexSearchMoveIdsIce[] =
{
    MOVE_AURORA_BEAM,
    MOVE_BLIZZARD,
    MOVE_HAIL,
    MOVE_HAZE,
    MOVE_ICE_BALL,
    MOVE_ICE_BEAM,
    MOVE_ICE_PUNCH,
    MOVE_ICICLE_SPEAR,
    MOVE_ICY_WIND,
    MOVE_MIST,
    MOVE_POWDER_SNOW,
    MOVE_SHEER_COLD,
};

static const u16 sDexSearchMoveIdsNormalAE[] =
{
    MOVE_ASSIST,
    MOVE_ATTRACT,
    MOVE_BARRAGE,
    MOVE_BATON_PASS,
    MOVE_BELLY_DRUM,
    MOVE_BIDE,
    MOVE_BIND,
    MOVE_BLOCK,
    MOVE_BODY_SLAM,
    MOVE_CAMOUFLAGE,
    MOVE_CHARM,
    MOVE_COMET_PUNCH,
    MOVE_CONSTRICT,
    MOVE_CONVERSION,
    MOVE_CONVERSION_2,
    MOVE_COVET,
    MOVE_CRUSH_CLAW,
    MOVE_CUT,
    MOVE_DEFENSE_CURL,
    MOVE_DISABLE,
    MOVE_DIZZY_PUNCH,
    MOVE_DOUBLE_TEAM,
    MOVE_DOUBLE_EDGE,
    MOVE_DOUBLE_SLAP,
    MOVE_EGG_BOMB,
    MOVE_ENCORE,
    MOVE_ENDEAVOR,
    MOVE_ENDURE,
    MOVE_EXPLOSION,
    MOVE_EXTREME_SPEED,
};

static const u16 sDexSearchMoveIdsNormalFL[] =
{
    MOVE_FACADE,
    MOVE_FAKE_OUT,
    MOVE_FALSE_SWIPE,
    MOVE_FLAIL,
    MOVE_FLASH,
    MOVE_FOCUS_ENERGY,
    MOVE_FOLLOW_ME,
    MOVE_FORESIGHT,
    MOVE_FRUSTRATION,
    MOVE_FURY_ATTACK,
    MOVE_FURY_SWIPES,
    MOVE_GLARE,
    MOVE_GROWL,
    MOVE_GROWTH,
    MOVE_GUILLOTINE,
    MOVE_HARDEN,
    MOVE_HEADBUTT,
    MOVE_HEAL_BELL,
    MOVE_HELPING_HAND,
    MOVE_HIDDEN_POWER,
    MOVE_HORN_ATTACK,
    MOVE_HORN_DRILL,
    MOVE_HOWL,
    MOVE_HYPER_BEAM,
    MOVE_HYPER_FANG,
    MOVE_HYPER_VOICE,
    MOVE_LEER,
    MOVE_LOCK_ON,
    MOVE_LOVELY_KISS,
};

static const u16 sDexSearchMoveIdsNormalMR[] =
{
    MOVE_MEAN_LOOK,
    MOVE_MEGA_KICK,
    MOVE_MEGA_PUNCH,
    MOVE_METRONOME,
    MOVE_MILK_DRINK,
    MOVE_MIMIC,
    MOVE_MIND_READER,
    MOVE_MINIMIZE,
    MOVE_MOONLIGHT,
    MOVE_MORNING_SUN,
    MOVE_NATURE_POWER,
    MOVE_ODOR_SLEUTH,
    MOVE_PAIN_SPLIT,
    MOVE_PAY_DAY,
    MOVE_PERISH_SONG,
    MOVE_POUND,
    MOVE_PRESENT,
    MOVE_PROTECT,
    MOVE_PSYCH_UP,
    MOVE_QUICK_ATTACK,
    MOVE_RAGE,
    MOVE_RAPID_SPIN,
    MOVE_RAZOR_WIND,
    MOVE_RECOVER,
    MOVE_RECYCLE,
    MOVE_REFRESH,
    MOVE_RETURN,
    MOVE_ROAR,
};

static const u16 sDexSearchMoveIdsNormalS[] =
{
    MOVE_SAFEGUARD,
    MOVE_SCARY_FACE,
    MOVE_SCRATCH,
    MOVE_SCREECH,
    MOVE_SECRET_POWER,
    MOVE_SELF_DESTRUCT,
    MOVE_SHARPEN,
    MOVE_SING,
    MOVE_SKETCH,
    MOVE_SKULL_BASH,
    MOVE_SLACK_OFF,
    MOVE_SLAM,
    MOVE_SLASH,
    MOVE_SLEEP_TALK,
    MOVE_SMELLING_SALT,
    MOVE_SMOKESCREEN,
    MOVE_SNORE,
    MOVE_SOFT_BOILED,
    MOVE_SONIC_BOOM,
    MOVE_SPIKE_CANNON,
    MOVE_SPIT_UP,
    MOVE_SPLASH,
    MOVE_STOCKPILE,
    MOVE_STOMP,
    MOVE_STRENGTH,
    MOVE_STRUGGLE,
    MOVE_SUBSTITUTE,
    MOVE_SUPER_FANG,
    MOVE_SUPERSONIC,
    MOVE_SWAGGER,
    MOVE_SWALLOW,
    MOVE_SWEET_KISS,
    MOVE_SWEET_SCENT,
    MOVE_SWIFT,
    MOVE_SWORDS_DANCE,
};

static const u16 sDexSearchMoveIdsNormalTZ[] =
{
    MOVE_TACKLE,
    MOVE_TAIL_WHIP,
    MOVE_TAKE_DOWN,
    MOVE_TEETER_DANCE,
    MOVE_THRASH,
    MOVE_TICKLE,
    MOVE_TRANSFORM,
    MOVE_TRI_ATTACK,
    MOVE_UPROAR,
    MOVE_VICE_GRIP,
    MOVE_WEATHER_BALL,
    MOVE_WHIRLWIND,
    MOVE_WISH,
    MOVE_WRAP,
    MOVE_YAWN,
};

static const u16 sDexSearchMoveIdsPoison[] =
{
    MOVE_ACID,
    MOVE_ACID_ARMOR,
    MOVE_POISON_FANG,
    MOVE_POISON_GAS,
    MOVE_POISON_STING,
    MOVE_POISON_TAIL,
    MOVE_POISON_POWDER,
    MOVE_SLUDGE,
    MOVE_SLUDGE_BOMB,
    MOVE_SMOG,
    MOVE_TOXIC,
};

static const u16 sDexSearchMoveIdsPsychic[] =
{
    MOVE_AGILITY,
    MOVE_AMNESIA,
    MOVE_BARRIER,
    MOVE_CALM_MIND,
    MOVE_CONFUSION,
    MOVE_COSMIC_POWER,
    MOVE_DREAM_EATER,
    MOVE_EXTRASENSORY,
    MOVE_FUTURE_SIGHT,
    MOVE_HYPNOSIS,
    MOVE_IMPRISON,
    MOVE_KINESIS,
    MOVE_LIGHT_SCREEN,
    MOVE_LUSTER_PURGE,
    MOVE_MAGIC_COAT,
    MOVE_MEDITATE,
    MOVE_MIRROR_COAT,
    MOVE_MIST_BALL,
    MOVE_PSYBEAM,
    MOVE_PSYCHIC,
    MOVE_PSYCHO_BOOST,
    MOVE_PSYWAVE,
    MOVE_REFLECT,
    MOVE_REST,
    MOVE_ROLE_PLAY,
    MOVE_SKILL_SWAP,
    MOVE_TELEPORT,
    MOVE_TRICK,
};

static const u16 sDexSearchMoveIdsRock[] =
{
    MOVE_ANCIENT_POWER,
    MOVE_ROCK_BLAST,
    MOVE_ROCK_SLIDE,
    MOVE_ROCK_THROW,
    MOVE_ROCK_TOMB,
    MOVE_ROLLOUT,
    MOVE_SANDSTORM,
};

static const u16 sDexSearchMoveIdsSteel[] =
{
    MOVE_DOOM_DESIRE,
    MOVE_IRON_DEFENSE,
    MOVE_IRON_TAIL,
    MOVE_METAL_CLAW,
    MOVE_METAL_SOUND,
    MOVE_METEOR_MASH,
    MOVE_STEEL_WING,
};

static const u16 sDexSearchMoveIdsWater[] =
{
    MOVE_BUBBLE,
    MOVE_BUBBLE_BEAM,
    MOVE_CLAMP,
    MOVE_CRABHAMMER,
    MOVE_DIVE,
    MOVE_HYDRO_CANNON,
    MOVE_HYDRO_PUMP,
    MOVE_MUDDY_WATER,
    MOVE_OCTAZOOKA,
    MOVE_RAIN_DANCE,
    MOVE_SURF,
    MOVE_WATER_GUN,
    MOVE_WATER_PULSE,
    MOVE_WATER_SPORT,
    MOVE_WATER_SPOUT,
    MOVE_WATERFALL,
    MOVE_WHIRLPOOL,
    MOVE_WITHDRAW,
};

static const u8 sDexSearchMoveTypeIds[] =
{
    MOVETYPE_ANY,
    MOVETYPE_LEVEL_UP,
    MOVETYPE_TMHM,
    MOVETYPE_TUTOR,
    MOVETYPE_EGG_MOVE,
};

static const u8 sDexSearchAbilityIdsABC[] =
{
    ABILITY_AIR_LOCK,
    ABILITY_ARENA_TRAP,
    ABILITY_BATTLE_ARMOR,
    ABILITY_BLAZE,
    ABILITY_CACOPHONY,
    ABILITY_CHLOROPHYLL,
    ABILITY_CLEAR_BODY,
    ABILITY_CLOUD_NINE,
    ABILITY_COLOR_CHANGE,
    ABILITY_COMPOUND_EYES,
    ABILITY_CUTE_CHARM,
};

static const u8 sDexSearchAbilityIdsDEF[] =
{
    ABILITY_DAMP,
    ABILITY_DRIZZLE,
    ABILITY_DROUGHT,
    ABILITY_EARLY_BIRD,
    ABILITY_EFFECT_SPORE,
    ABILITY_FLAME_BODY,
    ABILITY_FLASH_FIRE,
    ABILITY_FORECAST,
};

static const u8 sDexSearchAbilityIdsGHI[] =
{
    ABILITY_GUTS,
    ABILITY_HUGE_POWER,
    ABILITY_HUSTLE,
    ABILITY_HYPER_CUTTER,
    ABILITY_ILLUMINATE,
    ABILITY_IMMUNITY,
    ABILITY_INNER_FOCUS,
    ABILITY_INSOMNIA,
    ABILITY_INTIMIDATE,
};

static const u8 sDexSearchAbilityIdsJKL[] =
{
    ABILITY_KEEN_EYE,
    ABILITY_LEVITATE,
    ABILITY_LIGHTNING_ROD,
    ABILITY_LIMBER,
    ABILITY_LIQUID_OOZE,
};

static const u8 sDexSearchAbilityIdsMNO[] =
{
    ABILITY_MAGMA_ARMOR,
    ABILITY_MAGNET_PULL,
    ABILITY_MARVEL_SCALE,
    ABILITY_MINUS,
    ABILITY_NATURAL_CURE,
    ABILITY_OBLIVIOUS,
    ABILITY_OVERGROW,
    ABILITY_OWN_TEMPO,
};

static const u8 sDexSearchAbilityIdsPQR[] =
{
    ABILITY_PICKUP,
    ABILITY_PLUS,
    ABILITY_POISON_POINT,
    ABILITY_PRESSURE,
    ABILITY_PURE_POWER,
    ABILITY_RAIN_DISH,
    ABILITY_ROCK_HEAD,
    ABILITY_ROUGH_SKIN,
    ABILITY_RUN_AWAY,
};

static const u8 sDexSearchAbilityIdsSTU[] =
{
    ABILITY_SAND_STREAM,
    ABILITY_SAND_VEIL,
    ABILITY_SERENE_GRACE,
    ABILITY_SHADOW_TAG,
    ABILITY_SHED_SKIN,
    ABILITY_SHELL_ARMOR,
    ABILITY_SHIELD_DUST,
    ABILITY_SOUNDPROOF,
    ABILITY_SPEED_BOOST,
    ABILITY_STATIC,
    ABILITY_STENCH,
    ABILITY_STICKY_HOLD,
    ABILITY_STURDY,
    ABILITY_SUCTION_CUPS,
    ABILITY_SWARM,
    ABILITY_SWIFT_SWIM,
    ABILITY_SYNCHRONIZE,
    ABILITY_THICK_FAT,
    ABILITY_TORRENT,
    ABILITY_TRACE,
    ABILITY_TRUANT,
};

static const u8 sDexSearchAbilityIdsVWX[] =
{
    ABILITY_VITAL_SPIRIT,
    ABILITY_VOLT_ABSORB,
    ABILITY_WATER_ABSORB,
    ABILITY_WATER_VEIL,
    ABILITY_WHITE_SMOKE,
    ABILITY_WONDER_GUARD,
};

static const u8 sDexSearchAbilityIds[ABILITIES_COUNT] =
{
    ABILITY_NONE,
};

static const u8 sDexSearchMoveIds[MOVES_COUNT] =
{
    MOVE_NONE,
};

// Number pairs are the task data for tracking the cursor pos and scroll offset of each option list
// See task data defines above Task_LoadSearchMenu
static const struct SearchOption sSearchOptions[] =
{
    //[SEARCH_NAME]        = {sDexSearchNameOptions,  6,  7, ARRAY_COUNT(sDexSearchNameOptions) - 1},
    //[SEARCH_COLOR]      = {sDexSearchColorOptions, 8,  9, ARRAY_COUNT(sDexSearchColorOptions) - 1},
    [SEARCH_TYPE_LEFT]      = {sDexSearchTypeOptions,               6,  7, ARRAY_COUNT(sDexSearchTypeOptions) - 1},
    [SEARCH_TYPE_RIGHT]     = {sDexSearchTypeOptions,               8,  9, ARRAY_COUNT(sDexSearchTypeOptions) - 1},
    [SEARCH_GROUP_LEFT]     = {sDexSearchGroupOptions,             10, 11, ARRAY_COUNT(sDexSearchGroupOptions) - 1},
    [SEARCH_GROUP_RIGHT]    = {sDexSearchGroupOptions,             12, 13, ARRAY_COUNT(sDexSearchGroupOptions) - 1},
    [SEARCH_ABILITY]        = {sDexSearchAbilityInitialOptions,    14, 15, ARRAY_COUNT(sDexSearchAbilityInitialOptions) - 1},
    [SEARCH_MOVE]           = {sDexSearchMoveInitialOptions,       24, 25, ARRAY_COUNT(sDexSearchMoveInitialOptions) - 1},
    [SEARCH_MOVETYPE]       = {sDexSearchMoveTypeOptions,          32, 33, ARRAY_COUNT(sDexSearchMoveTypeOptions) - 1},
    [SEARCH_ORDER]          = {sDexOrderOptions,                    4,  5, ARRAY_COUNT(sDexOrderOptions) - 1},
    [SEARCH_MODE]           = {sDexModeOptions,                     2,  3, ARRAY_COUNT(sDexModeOptions) - 1},
    [SEARCH_ABILITY_ABC]    = {sDexSearchAbilityOptionsABC,        28, 29, ARRAY_COUNT(sDexSearchAbilityOptionsABC) - 1},
    [SEARCH_ABILITY_DEF]    = {sDexSearchAbilityOptionsDEF,        28, 29, ARRAY_COUNT(sDexSearchAbilityOptionsDEF) - 1},
    [SEARCH_ABILITY_GHI]    = {sDexSearchAbilityOptionsGHI,        28, 29, ARRAY_COUNT(sDexSearchAbilityOptionsGHI) - 1},
    [SEARCH_ABILITY_JKL]    = {sDexSearchAbilityOptionsJKL,        28, 29, ARRAY_COUNT(sDexSearchAbilityOptionsJKL) - 1},
    [SEARCH_ABILITY_MNO]    = {sDexSearchAbilityOptionsMNO,        28, 29, ARRAY_COUNT(sDexSearchAbilityOptionsMNO) - 1},
    [SEARCH_ABILITY_PQR]    = {sDexSearchAbilityOptionsPQR,        28, 29, ARRAY_COUNT(sDexSearchAbilityOptionsPQR) - 1},
    [SEARCH_ABILITY_STU]    = {sDexSearchAbilityOptionsSTU,        28, 29, ARRAY_COUNT(sDexSearchAbilityOptionsSTU) - 1},
    [SEARCH_ABILITY_VWX]    = {sDexSearchAbilityOptionsVWX,        28, 29, ARRAY_COUNT(sDexSearchAbilityOptionsVWX) - 1},
    [SEARCH_MOVE_BUG]       = {sDexSearchMoveOptionsBug,           30, 31, ARRAY_COUNT(sDexSearchMoveOptionsBug) - 1},
    [SEARCH_MOVE_DARK]      = {sDexSearchMoveOptionsDark,          30, 31, ARRAY_COUNT(sDexSearchMoveOptionsDark) - 1},
    [SEARCH_MOVE_DRAGON]    = {sDexSearchMoveOptionsDragon,        30, 31, ARRAY_COUNT(sDexSearchMoveOptionsDragon) - 1},
    [SEARCH_MOVE_ELECTRIC]  = {sDexSearchMoveOptionsElectric,      30, 31, ARRAY_COUNT(sDexSearchMoveOptionsElectric) - 1},
    [SEARCH_MOVE_FIGHTING]  = {sDexSearchMoveOptionsFighting,      30, 31, ARRAY_COUNT(sDexSearchMoveOptionsFighting) - 1},
    [SEARCH_MOVE_FIRE]      = {sDexSearchMoveOptionsFire,          30, 31, ARRAY_COUNT(sDexSearchMoveOptionsFire) - 1},
    [SEARCH_MOVE_FLYING]    = {sDexSearchMoveOptionsFlying,        30, 31, ARRAY_COUNT(sDexSearchMoveOptionsFlying) - 1},
    [SEARCH_MOVE_GHOST]     = {sDexSearchMoveOptionsGhost,         30, 31, ARRAY_COUNT(sDexSearchMoveOptionsGhost) - 1},
    [SEARCH_MOVE_GRASS]     = {sDexSearchMoveOptionsGrass,         30, 31, ARRAY_COUNT(sDexSearchMoveOptionsGrass) - 1},
    [SEARCH_MOVE_GROUND]    = {sDexSearchMoveOptionsGround,        30, 31, ARRAY_COUNT(sDexSearchMoveOptionsGround) - 1},
    [SEARCH_MOVE_ICE]       = {sDexSearchMoveOptionsIce,           30, 31, ARRAY_COUNT(sDexSearchMoveOptionsIce) - 1},
    [SEARCH_MOVE_NORMAL_AE] = {sDexSearchMoveOptionsNormalAE,      30, 31, ARRAY_COUNT(sDexSearchMoveOptionsNormalAE) - 1},
    [SEARCH_MOVE_NORMAL_FL] = {sDexSearchMoveOptionsNormalFL,      30, 31, ARRAY_COUNT(sDexSearchMoveOptionsNormalFL) - 1},
    [SEARCH_MOVE_NORMAL_MR] = {sDexSearchMoveOptionsNormalMR,      30, 31, ARRAY_COUNT(sDexSearchMoveOptionsNormalMR) - 1},
    [SEARCH_MOVE_NORMAL_S]  = {sDexSearchMoveOptionsNormalS,       30, 31, ARRAY_COUNT(sDexSearchMoveOptionsNormalS) - 1},
    [SEARCH_MOVE_NORMAL_TZ] = {sDexSearchMoveOptionsNormalTZ,      30, 31, ARRAY_COUNT(sDexSearchMoveOptionsNormalTZ) - 1},
    [SEARCH_MOVE_POISON]    = {sDexSearchMoveOptionsPoison,        30, 31, ARRAY_COUNT(sDexSearchMoveOptionsPoison) - 1},
    [SEARCH_MOVE_PSYCHIC]   = {sDexSearchMoveOptionsPsychic,       30, 31, ARRAY_COUNT(sDexSearchMoveOptionsPsychic) - 1},
    [SEARCH_MOVE_ROCK]      = {sDexSearchMoveOptionsRock,          30, 31, ARRAY_COUNT(sDexSearchMoveOptionsRock) - 1},
    [SEARCH_MOVE_STEEL]     = {sDexSearchMoveOptionsSteel,         30, 31, ARRAY_COUNT(sDexSearchMoveOptionsSteel) - 1},
    [SEARCH_MOVE_WATER]     = {sDexSearchMoveOptionsWater,         30, 31, ARRAY_COUNT(sDexSearchMoveOptionsWater) - 1},
};

static const struct BgTemplate sSearchMenu_BgTemplate[] =
{
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 12,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 13,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    },
    {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 14,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0
    },
    {
        .bg = 3,
        .charBaseIndex = 0,
        .mapBaseIndex = 15,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    }
};

static const struct WindowTemplate sSearchMenu_WindowTemplate[] =
{
    {
        .bg = 2,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 32,
        .height = 20,
        .paletteNum = 0,
        .baseBlock = 0x0001,
    },
    DUMMY_WIN_TEMPLATE
};

// .text

void ResetPokedex(void)
{
    u32 i;

    sLastSelectedPokemon = 0;
    sPokeBallRotation = POKEBALL_ROTATION_TOP;
    gUnusedPokedexU8 = 0;
    gSaveBlock2Ptr->pokedex.mode = DEX_MODE_HOENN;
    gSaveBlock2Ptr->pokedex.order = 0;
    gSaveBlock2Ptr->pokedex.nationalMagic = 0;
    gSaveBlock2Ptr->pokedex.unknown2 = 0;
    gSaveBlock2Ptr->pokedex.unownPersonality = 0;
    gSaveBlock2Ptr->pokedex.spindaPersonality = 0;
    gSaveBlock2Ptr->pokedex.unknown3 = 0;
    DisableNationalPokedex();
    for (i = 0; i < NUM_DEX_FLAG_BYTES; i++)
    {
        gSaveBlock2Ptr->pokedex.owned[i] = 0;
        gSaveBlock2Ptr->pokedex.seen[i] = 0;
        gSaveBlock1Ptr->seen1[i] = 0;
        gSaveBlock1Ptr->seen2[i] = 0;
    }
}

void ResetPokedexScrollPositions(void)
{
    sLastSelectedPokemon = 0;
    sPokeBallRotation = POKEBALL_ROTATION_TOP;
}

static void VBlankCB_Pokedex(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    if (sPokedexView->currentPage == PAGE_MOVES)
        SyncBg3VOffset();
    TransferPlttBuffer();
}

static void ResetPokedexView(struct PokedexView *pokedexView)
{
    u32 i;

    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        pokedexView->pokedexList[i].dexNum = 0xFFFF;
        pokedexView->pokedexList[i].seen = FALSE;
        pokedexView->pokedexList[i].owned = FALSE;
    }
    pokedexView->pokedexList[NATIONAL_DEX_COUNT].dexNum = 0;
    pokedexView->pokedexList[NATIONAL_DEX_COUNT].seen = FALSE;
    pokedexView->pokedexList[NATIONAL_DEX_COUNT].owned = FALSE;
    pokedexView->pokemonListCount = 0;
    pokedexView->selectedPokemon = 0;
    pokedexView->selectedPokemonBackup = 0;
    pokedexView->dexMode = DEX_MODE_HOENN;
    pokedexView->dexModeBackup = DEX_MODE_HOENN;
    pokedexView->dexOrder = ORDER_NUMERICAL;
    pokedexView->dexOrderBackup = ORDER_NUMERICAL;
    pokedexView->seenCount = 0;
    pokedexView->ownCount = 0;
    for (i = 0; i < MAX_MONS_ON_SCREEN; i++)
        pokedexView->monSpriteIds[i] = 0xFFFF;
    pokedexView->pokeBallRotationStep = 0;
    pokedexView->pokeBallRotationBackup = 0;
    pokedexView->pokeBallRotation = 0;
    pokedexView->initialVOffset = 0;
    pokedexView->scrollTimer = 0;
    pokedexView->scrollDirection = 0;
    pokedexView->listVOffset = 0;
    pokedexView->listMovingVOffset = 0;
    pokedexView->scrollMonIncrement = 0;
    pokedexView->maxScrollTimer = 0;
    pokedexView->scrollSpeed = 0;
    for (i = 0; i < ARRAY_COUNT(pokedexView->unkArr1); i++)
        pokedexView->unkArr1[i] = 0;
    pokedexView->currentPage = PAGE_MAIN;
    pokedexView->currentPageBackup = PAGE_MAIN;
    pokedexView->isSearchResults = FALSE;
    pokedexView->selectedScreen = SIZE_SCREEN;
    pokedexView->screenSwitchState = 0;
    pokedexView->menuIsOpen = 0;
    pokedexView->menuCursorPos = 0;
    pokedexView->menuY = 0;
    for (i = 0; i < ARRAY_COUNT(pokedexView->unkArr2); i++)
        pokedexView->unkArr2[i] = 0;
    for (i = 0; i < ARRAY_COUNT(pokedexView->unkArr3); i++)
        pokedexView->unkArr3[i] = 0;
}

void CB2_OpenPokedex(void)
{
    switch (gMain.state)
    {
    case 0:
    default:
        SetVBlankCallback(NULL);
        ResetOtherVideoRegisters(0);
        DmaFillLarge16(3, 0, (u8 *)VRAM, VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        gMain.state = 1;
        break;
    case 1:
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        ResetPaletteFade();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
        ResetAllPicSprites();
        gMain.state++;
        break;
    case 2:
        sPokedexView = AllocZeroed(sizeof(struct PokedexView));
        ResetPokedexView(sPokedexView);
        CreateTask(Task_OpenPokedexMainPage, 0);
        sPokedexView->dexMode = gSaveBlock2Ptr->pokedex.mode;
        if (!IsNationalPokedexEnabled())
            sPokedexView->dexMode = DEX_MODE_HOENN;
        sPokedexView->dexOrder = gSaveBlock2Ptr->pokedex.order;
        sPokedexView->selectedPokemon = sLastSelectedPokemon;
        sPokedexView->pokeBallRotation = sPokeBallRotation;
        sPokedexView->selectedScreen = SIZE_SCREEN;
        if (!IsNationalPokedexEnabled())
        {
            sPokedexView->seenCount = GetHoennPokedexCount(FLAG_GET_SEEN);
            sPokedexView->ownCount = GetHoennPokedexCount(FLAG_GET_CAUGHT);
        }
        else
        {
            sPokedexView->seenCount = GetNationalPokedexCount(FLAG_GET_SEEN);
            sPokedexView->ownCount = GetNationalPokedexCount(FLAG_GET_CAUGHT);
        }
        sPokedexView->initialVOffset = 8;
        gMain.state++;
        break;
    case 3:
        EnableInterrupts(1);
        SetVBlankCallback(VBlankCB_Pokedex);
        SetMainCallback2(CB2_Pokedex);
        CreatePokedexList(sPokedexView->dexMode, sPokedexView->dexOrder);
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x80);
        break;
    }
}

static void CB2_Pokedex(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void Task_OpenPokedexMainPage(u8 taskId)
{
    sPokedexView->isSearchResults = FALSE;
    if (LoadPokedexListPage(PAGE_MAIN))
        gTasks[taskId].func = Task_HandlePokedexInput;
}

#define tLoadScreenTaskId data[0]

static void Task_HandlePokedexInput(u8 taskId)
{
    SetGpuReg(REG_OFFSET_BG0VOFS, sPokedexView->menuY);

    if (sPokedexView->menuY)
    {
        sPokedexView->menuY -= 8;
    }
    else
    {
        if (JOY_NEW(A_BUTTON) && sPokedexView->pokedexList[sPokedexView->selectedPokemon].seen)
        {
            UpdateSelectedMonSpriteId();
            BeginNormalPaletteFade(~(1 << (gSprites[sPokedexView->selectedMonSpriteId].oam.paletteNum + 16)), 0, 0, 0x10, RGB_BLACK);
            gSprites[sPokedexView->selectedMonSpriteId].callback = SpriteCB_MoveMonForInfoScreen;
            gTasks[taskId].func = Task_OpenInfoScreenAfterMonMovement;
            PlaySE(SE_PIN);
            FreeWindowAndBgBuffers();
        }
        else if (JOY_NEW(START_BUTTON))
        {
            sPokedexView->menuY = 0;
            sPokedexView->menuIsOpen = TRUE;
            sPokedexView->menuCursorPos = 0;
            gTasks[taskId].func = Task_HandlePokedexStartMenuInput;
            PlaySE(SE_SELECT);
        }
        else if (JOY_NEW(SELECT_BUTTON))
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].tLoadScreenTaskId = LoadSearchMenu();
            sPokedexView->screenSwitchState = 0;
            sPokedexView->pokeBallRotationBackup = sPokedexView->pokeBallRotation;
            sPokedexView->selectedPokemonBackup = sPokedexView->selectedPokemon;
            sPokedexView->dexModeBackup = sPokedexView->dexMode;
            sPokedexView->dexOrderBackup = sPokedexView->dexOrder;
            gTasks[taskId].func = Task_WaitForExitSearch;
            PlaySE(SE_PC_LOGIN);
            FreeWindowAndBgBuffers();
        }
        else if (JOY_NEW(B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].func = Task_ClosePokedex;
            PlaySE(SE_PC_OFF);
        }
        else
        {
            //Handle D-pad
            sPokedexView->selectedPokemon = TryDoPokedexScroll(sPokedexView->selectedPokemon, 0xE);
            if (sPokedexView->scrollTimer)
                gTasks[taskId].func = Task_WaitForScroll;
        }
    }
}

static void Task_WaitForScroll(u8 taskId)
{
    if (UpdateDexListScroll(sPokedexView->scrollDirection, sPokedexView->scrollMonIncrement, sPokedexView->maxScrollTimer))
        gTasks[taskId].func = Task_HandlePokedexInput;
}

static void Task_HandlePokedexStartMenuInput(u8 taskId)
{
    SetGpuReg(REG_OFFSET_BG0VOFS, sPokedexView->menuY);

    //If menu is not open, slide it up, on screen
    if (sPokedexView->menuY != 80)
    {
        sPokedexView->menuY += 8;
    }
    else
    {
        if (JOY_NEW(A_BUTTON))
        {
            switch (sPokedexView->menuCursorPos)
            {
            case 0: //BACK TO LIST
            default:
                gMain.newKeys |= START_BUTTON;  //Exit menu
                break;
            case 1: //LIST TOP
                sPokedexView->selectedPokemon = 0;
                sPokedexView->pokeBallRotation = POKEBALL_ROTATION_TOP;
                ClearMonSprites();
                CreateMonSpritesAtPos(sPokedexView->selectedPokemon, 0xE);
                gMain.newKeys |= START_BUTTON;  //Exit menu
                break;
            case 2: //LIST BOTTOM
                sPokedexView->selectedPokemon = sPokedexView->pokemonListCount - 1;
                sPokedexView->pokeBallRotation = sPokedexView->pokemonListCount * 16 + POKEBALL_ROTATION_BOTTOM;
                ClearMonSprites();
                CreateMonSpritesAtPos(sPokedexView->selectedPokemon, 0xE);
                gMain.newKeys |= START_BUTTON;  //Exit menu
                break;
            case 3: //CLOSE POKéDEX
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                gTasks[taskId].func = Task_ClosePokedex;
                PlaySE(SE_PC_OFF);
                break;
            }
        }

        //Exit menu when Start or B is pressed
        if (JOY_NEW(START_BUTTON | B_BUTTON))
        {
            sPokedexView->menuIsOpen = FALSE;
            gTasks[taskId].func = Task_HandlePokedexInput;
            PlaySE(SE_SELECT);
        }
        else if (JOY_REPEAT(DPAD_UP) && sPokedexView->menuCursorPos != 0)
        {
            sPokedexView->menuCursorPos--;
            PlaySE(SE_SELECT);
        }
        else if (JOY_REPEAT(DPAD_DOWN) && sPokedexView->menuCursorPos < 3)
        {
            sPokedexView->menuCursorPos++;
            PlaySE(SE_SELECT);
        }
    }
}

// Opening the info screen from list view. Pokémon sprite is moving to its new position, wait for it to arrive
static void Task_OpenInfoScreenAfterMonMovement(u8 taskId)
{
    if (gSprites[sPokedexView->selectedMonSpriteId].x == MON_PAGE_X && gSprites[sPokedexView->selectedMonSpriteId].y == MON_PAGE_Y)
    {
        sPokedexView->currentPageBackup = sPokedexView->currentPage;
        gTasks[taskId].tLoadScreenTaskId = LoadInfoScreen(&sPokedexView->pokedexList[sPokedexView->selectedPokemon], sPokedexView->selectedMonSpriteId);
        gTasks[taskId].func = Task_WaitForExitInfoScreen;
    }
}

static void Task_WaitForExitInfoScreen(u8 taskId)
{
    if (gTasks[gTasks[taskId].tLoadScreenTaskId].isActive)
    {
        // While active, handle scroll input
        if (sPokedexView->currentPage == PAGE_INFO && !IsInfoScreenScrolling(gTasks[taskId].tLoadScreenTaskId) && TryDoInfoScreenScroll())
            StartInfoScreenScroll(&sPokedexView->pokedexList[sPokedexView->selectedPokemon], gTasks[taskId].tLoadScreenTaskId);
    }
    else
    {
        // Exiting, back to list view
        sLastSelectedPokemon = sPokedexView->selectedPokemon;
        sPokeBallRotation = sPokedexView->pokeBallRotation;
        gTasks[taskId].func = Task_OpenPokedexMainPage;
    }
}

static void Task_WaitForExitSearch(u8 taskId)
{
    if (!gTasks[gTasks[taskId].tLoadScreenTaskId].isActive)
    {
        ClearMonSprites();

        // Search produced results
        if (sPokedexView->screenSwitchState != 0)
        {
            sPokedexView->selectedPokemon = 0;
            sPokedexView->pokeBallRotation = POKEBALL_ROTATION_TOP;
            gTasks[taskId].func = Task_OpenSearchResults;
        }
        // Search didn't produce results
        else
        {
            sPokedexView->pokeBallRotation = sPokedexView->pokeBallRotationBackup;
            sPokedexView->selectedPokemon = sPokedexView->selectedPokemonBackup;
            sPokedexView->dexMode = sPokedexView->dexModeBackup;
            if (!IsNationalPokedexEnabled())
                sPokedexView->dexMode = DEX_MODE_HOENN;
            sPokedexView->dexOrder = sPokedexView->dexOrderBackup;
            gTasks[taskId].func = Task_OpenPokedexMainPage;
        }
    }
}

static void Task_ClosePokedex(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        gSaveBlock2Ptr->pokedex.mode = sPokedexView->dexMode;
        if (!IsNationalPokedexEnabled())
            gSaveBlock2Ptr->pokedex.mode = DEX_MODE_HOENN;
        gSaveBlock2Ptr->pokedex.order = sPokedexView->dexOrder;
        ClearMonSprites();
        FreeWindowAndBgBuffers();
        DestroyTask(taskId);
        SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        m4aMPlayVolumeControl(&gMPlayInfo_BGM, TRACKS_ALL, 0x100);
        Free(sPokedexView);
    }
}

static void Task_OpenSearchResults(u8 taskId)
{
    sPokedexView->isSearchResults = TRUE;
    if (LoadPokedexListPage(PAGE_SEARCH_RESULTS))
        gTasks[taskId].func = Task_HandleSearchResultsInput;
}

static void Task_HandleSearchResultsInput(u8 taskId)
{
    SetGpuReg(REG_OFFSET_BG0VOFS, sPokedexView->menuY);

    if (sPokedexView->menuY)
    {
        sPokedexView->menuY -= 8;
    }
    else
    {
        if (JOY_NEW(A_BUTTON) && sPokedexView->pokedexList[sPokedexView->selectedPokemon].seen)
        {
            u32 a;

            UpdateSelectedMonSpriteId();
            a = (1 << (gSprites[sPokedexView->selectedMonSpriteId].oam.paletteNum + 16));
            gSprites[sPokedexView->selectedMonSpriteId].callback = SpriteCB_MoveMonForInfoScreen;
            BeginNormalPaletteFade(~a, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].func = Task_OpenSearchResultsInfoScreenAfterMonMovement;
            PlaySE(SE_PIN);
            FreeWindowAndBgBuffers();
        }
        else if (JOY_NEW(START_BUTTON))
        {
            sPokedexView->menuY = 0;
            sPokedexView->menuIsOpen = TRUE;
            sPokedexView->menuCursorPos = 0;
            gTasks[taskId].func = Task_HandleSearchResultsStartMenuInput;
            PlaySE(SE_SELECT);
        }
        else if (JOY_NEW(SELECT_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].tLoadScreenTaskId = LoadSearchMenu();
            sPokedexView->screenSwitchState = 0;
            gTasks[taskId].func = Task_WaitForExitSearch;
            PlaySE(SE_PC_LOGIN);
            FreeWindowAndBgBuffers();
        }
        else if (JOY_NEW(B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].func = Task_ReturnToPokedexFromSearchResults;
            PlaySE(SE_PC_OFF);
        }
        else
        {
            //Handle D-pad
            sPokedexView->selectedPokemon = TryDoPokedexScroll(sPokedexView->selectedPokemon, 0xE);
            if (sPokedexView->scrollTimer)
                gTasks[taskId].func = Task_WaitForSearchResultsScroll;
        }
    }
}

static void Task_WaitForSearchResultsScroll(u8 taskId)
{
    if (UpdateDexListScroll(sPokedexView->scrollDirection, sPokedexView->scrollMonIncrement, sPokedexView->maxScrollTimer))
        gTasks[taskId].func = Task_HandleSearchResultsInput;
}

static void Task_HandleSearchResultsStartMenuInput(u8 taskId)
{
    SetGpuReg(REG_OFFSET_BG0VOFS, sPokedexView->menuY);

    if (sPokedexView->menuY != 96)
    {
        sPokedexView->menuY += 8;
    }
    else
    {
        if (JOY_NEW(A_BUTTON))
        {
            switch (sPokedexView->menuCursorPos)
            {
            case 0: //BACK TO LIST
            default:
                gMain.newKeys |= START_BUTTON;
                break;
            case 1: //LIST TOP
                sPokedexView->selectedPokemon = 0;
                sPokedexView->pokeBallRotation = POKEBALL_ROTATION_TOP;
                ClearMonSprites();
                CreateMonSpritesAtPos(sPokedexView->selectedPokemon, 0xE);
                gMain.newKeys |= START_BUTTON;
                break;
            case 2: //LIST BOTTOM
                sPokedexView->selectedPokemon = sPokedexView->pokemonListCount - 1;
                sPokedexView->pokeBallRotation = sPokedexView->pokemonListCount * 16 + POKEBALL_ROTATION_BOTTOM;
                ClearMonSprites();
                CreateMonSpritesAtPos(sPokedexView->selectedPokemon, 0xE);
                gMain.newKeys |= START_BUTTON;
                break;
            case 3: //BACK TO POKéDEX
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                gTasks[taskId].func = Task_ReturnToPokedexFromSearchResults;
                PlaySE(SE_TRUCK_DOOR);
                break;
            case 4: //CLOSE POKéDEX
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
                gTasks[taskId].func = Task_ClosePokedexFromSearchResultsStartMenu;
                PlaySE(SE_PC_OFF);
                break;
            }
        }

        //Exit menu when Start or B is pressed
        if (JOY_NEW(START_BUTTON | B_BUTTON))
        {
            sPokedexView->menuIsOpen = FALSE;
            gTasks[taskId].func = Task_HandleSearchResultsInput;
            PlaySE(SE_SELECT);
        }
        else if (JOY_REPEAT(DPAD_UP) && sPokedexView->menuCursorPos)
        {
            sPokedexView->menuCursorPos--;
            PlaySE(SE_SELECT);
        }
        else if (JOY_REPEAT(DPAD_DOWN) && sPokedexView->menuCursorPos < 4)
        {
            sPokedexView->menuCursorPos++;
            PlaySE(SE_SELECT);
        }
    }
}

static void Task_OpenSearchResultsInfoScreenAfterMonMovement(u8 taskId)
{
    if (gSprites[sPokedexView->selectedMonSpriteId].x == MON_PAGE_X && gSprites[sPokedexView->selectedMonSpriteId].y == MON_PAGE_Y)
    {
        sPokedexView->currentPageBackup = sPokedexView->currentPage;
        gTasks[taskId].tLoadScreenTaskId = LoadInfoScreen(&sPokedexView->pokedexList[sPokedexView->selectedPokemon], sPokedexView->selectedMonSpriteId);
        sPokedexView->selectedMonSpriteId = -1;
        gTasks[taskId].func = Task_WaitForExitSearchResultsInfoScreen;
    }
}

static void Task_WaitForExitSearchResultsInfoScreen(u8 taskId)
{
    if (gTasks[gTasks[taskId].tLoadScreenTaskId].isActive)
    {
        // While active, handle scroll input
        if (sPokedexView->currentPage == PAGE_INFO && !IsInfoScreenScrolling(gTasks[taskId].tLoadScreenTaskId) && TryDoInfoScreenScroll())
            StartInfoScreenScroll(&sPokedexView->pokedexList[sPokedexView->selectedPokemon], gTasks[taskId].tLoadScreenTaskId);
    }
    else
    {
        // Exiting, back to search results
        gTasks[taskId].func = Task_OpenSearchResults;
    }
}

static void Task_ReturnToPokedexFromSearchResults(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        sPokedexView->pokeBallRotation = sPokedexView->pokeBallRotationBackup;
        sPokedexView->selectedPokemon = sPokedexView->selectedPokemonBackup;
        sPokedexView->dexMode = sPokedexView->dexModeBackup;
        if (!IsNationalPokedexEnabled())
            sPokedexView->dexMode = DEX_MODE_HOENN;
        sPokedexView->dexOrder = sPokedexView->dexOrderBackup;
        gTasks[taskId].func = Task_OpenPokedexMainPage;
        ClearMonSprites();
        FreeWindowAndBgBuffers();
    }
}

static void Task_ClosePokedexFromSearchResultsStartMenu(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        sPokedexView->pokeBallRotation = sPokedexView->pokeBallRotationBackup;
        sPokedexView->selectedPokemon = sPokedexView->selectedPokemonBackup;
        sPokedexView->dexMode = sPokedexView->dexModeBackup;
        if (!IsNationalPokedexEnabled())
            sPokedexView->dexMode = DEX_MODE_HOENN;
        sPokedexView->dexOrder = sPokedexView->dexOrderBackup;
        gTasks[taskId].func = Task_ClosePokedex;
    }
}

#undef tLoadScreenTaskId

// For loading main pokedex page or Pokédex search results
static bool8 LoadPokedexListPage(u8 page)
{
    switch (gMain.state)
    {
    case 0:
    default:
        if (gPaletteFade.active)
            return 0;
        SetVBlankCallback(NULL);
        sPokedexView->currentPage = page;
        ResetOtherVideoRegisters(0);
        SetGpuReg(REG_OFFSET_BG2VOFS, sPokedexView->initialVOffset);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sPokedex_BgTemplate, ARRAY_COUNT(sPokedex_BgTemplate));
        SetBgTilemapBuffer(3, AllocZeroed(BG_SCREEN_SIZE));
        SetBgTilemapBuffer(2, AllocZeroed(BG_SCREEN_SIZE));
        SetBgTilemapBuffer(1, AllocZeroed(BG_SCREEN_SIZE));
        SetBgTilemapBuffer(0, AllocZeroed(BG_SCREEN_SIZE));
        DecompressAndLoadBgGfxUsingHeap(3, gPokedexMenu_Gfx, 0x2000, 0, 0);
        CopyToBgTilemapBuffer(1, gPokedexList_Tilemap, 0, 0);
        CopyToBgTilemapBuffer(3, gPokedexListUnderlay_Tilemap, 0, 0);
        if (page == PAGE_MAIN)
            CopyToBgTilemapBuffer(0, gPokedexStartMenuMain_Tilemap, 0, 0x280);
        else
            CopyToBgTilemapBuffer(0, gPokedexStartMenuSearchResults_Tilemap, 0, 0x280);
        ResetPaletteFade();
        if (page == PAGE_MAIN)
            sPokedexView->isSearchResults = FALSE;
        else
            sPokedexView->isSearchResults = TRUE;
        LoadPokedexBgPalette(sPokedexView->isSearchResults);
        InitWindows(sPokemonList_WindowTemplate);
        DeactivateAllTextPrinters();
        PutWindowTilemap(0);
        CopyWindowToVram(0, COPYWIN_FULL);
        gMain.state = 1;
        break;
    case 1:
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
        LoadCompressedSpriteSheet(&sInterfaceSpriteSheet[0]);
        LoadSpritePalettes(sInterfaceSpritePalette);
        CreateInterfaceSprites(page);
        gMain.state++;
        break;
    case 2:
        gMain.state++;
        break;
    case 3:
        if (page == PAGE_MAIN)
            CreatePokedexList(sPokedexView->dexMode, sPokedexView->dexOrder);
        CreateMonSpritesAtPos(sPokedexView->selectedPokemon, 0xE);
        sPokedexView->menuIsOpen = FALSE;
        sPokedexView->menuY = 0;
        CopyBgTilemapBufferToVram(0);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(3);
        gMain.state++;
        break;
    case 4:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_Pokedex);
        gMain.state++;
        break;
    case 5:
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_ALL | WININ_WIN1_ALL);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_ALL | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_OBJ);
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WIN1H, 0);
        SetGpuReg(REG_OFFSET_WIN1V, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON | DISPCNT_OBJWIN_ON);
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        gMain.state++;
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            gMain.state = 0;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

static void LoadPokedexBgPalette(bool8 isSearchResults)
{
    if (isSearchResults == TRUE)
        LoadPalette(gPokedexSearchResults_Pal + 1, BG_PLTT_ID(0) + 1, PLTT_SIZEOF(6 * 16 - 1));
    else if (!IsNationalPokedexEnabled())
        LoadPalette(gPokedexBgHoenn_Pal + 1, BG_PLTT_ID(0) + 1, PLTT_SIZEOF(6 * 16 - 1));
    else
        LoadPalette(gPokedexBgNational_Pal + 1, BG_PLTT_ID(0) + 1, PLTT_SIZEOF(6 * 16 - 1));
    LoadPalette(GetOverworldTextboxPalettePtr(), BG_PLTT_ID(15), PLTT_SIZE_4BPP);
}

static void FreeWindowAndBgBuffers(void)
{
    void *tilemapBuffer;

    FreeAllWindowBuffers();
    tilemapBuffer = GetBgTilemapBuffer(0);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(1);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(2);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(3);
    if (tilemapBuffer)
        Free(tilemapBuffer);
}

static void CreatePokedexList(u8 dexMode, u8 order)
{
    u32 vars[3]; //I have no idea why three regular variables are stored in an array, but whatever.
#define temp_dexCount   vars[0]
#define temp_isHoennDex vars[1]
#define temp_dexNum     vars[2]
    s32 i;

    sPokedexView->pokemonListCount = 0;

    switch (dexMode)
    {
    default:
    case DEX_MODE_HOENN:
        temp_dexCount = HOENN_DEX_COUNT;
        temp_isHoennDex = TRUE;
        break;
    case DEX_MODE_NATIONAL:
        if (IsNationalPokedexEnabled())
        {
            temp_dexCount = NATIONAL_DEX_COUNT;
            temp_isHoennDex = FALSE;
        }
        else
        {
            temp_dexCount = HOENN_DEX_COUNT;
            temp_isHoennDex = TRUE;
        }
        break;
    }

    switch (order)
    {
    case ORDER_NUMERICAL:
        if (temp_isHoennDex)
        {
            for (i = 0; i < temp_dexCount; i++)
            {
                temp_dexNum = HoennToNationalOrder(i + 1);
                sPokedexView->pokedexList[i].dexNum = temp_dexNum;
                sPokedexView->pokedexList[i].seen = GetSetPokedexFlag(temp_dexNum, FLAG_GET_SEEN);
                sPokedexView->pokedexList[i].owned = GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT);
                if (sPokedexView->pokedexList[i].seen)
                    sPokedexView->pokemonListCount = i + 1;
            }
        }
        else
        {
            s16 r5, r10;
            for (i = 0, r5 = 0, r10 = 0; i < temp_dexCount; i++)
            {
                temp_dexNum = i + 1;
                if (GetSetPokedexFlag(temp_dexNum, FLAG_GET_SEEN))
                    r10 = 1;
                if (r10)
                {
                    sPokedexView->pokedexList[r5].dexNum = temp_dexNum;
                    sPokedexView->pokedexList[r5].seen = GetSetPokedexFlag(temp_dexNum, FLAG_GET_SEEN);
                    sPokedexView->pokedexList[r5].owned = GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT);
                    if (sPokedexView->pokedexList[r5].seen)
                        sPokedexView->pokemonListCount = r5 + 1;
                    r5++;
                }
            }
        }
        break;
    case ORDER_ALPHABETICAL:
        for (i = 0; i < NUM_SPECIES - 1; i++)
        {
            temp_dexNum = gPokedexOrder_Alphabetical[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_SEEN))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT);
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    case ORDER_WEIGHT:
        for (i = NATIONAL_DEX_COUNT - 1; i >= 0; i--)
        {
            temp_dexNum = gPokedexOrder_Weight[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    /*
    case ORDER_LIGHTEST:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_Weight[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    */
    case ORDER_HEIGHT:
        for (i = NATIONAL_DEX_COUNT - 1; i >= 0; i--)
        {
            temp_dexNum = gPokedexOrder_Height[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    /*
    case ORDER_SMALLEST:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_Height[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    */
    case ORDER_HP:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_HP[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    case ORDER_ATTACK:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_Attack[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    case ORDER_DEFENSE:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_Defense[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    case ORDER_SP_ATK:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_SpAtk[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    case ORDER_SP_DEF:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_SpDef[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    case ORDER_SPEED:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_Speed[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    case ORDER_STATS:
        for (i = 0; i < NATIONAL_DEX_COUNT; i++)
        {
            temp_dexNum = gPokedexOrder_Stats[i];

            if (NationalToHoennOrder(temp_dexNum) <= temp_dexCount && GetSetPokedexFlag(temp_dexNum, FLAG_GET_CAUGHT))
            {
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].dexNum = temp_dexNum;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].seen = TRUE;
                sPokedexView->pokedexList[sPokedexView->pokemonListCount].owned = TRUE;
                sPokedexView->pokemonListCount++;
            }
        }
        break;
    }

    for (i = sPokedexView->pokemonListCount; i < NATIONAL_DEX_COUNT; i++)
    {
        sPokedexView->pokedexList[i].dexNum = 0xFFFF;
        sPokedexView->pokedexList[i].seen = FALSE;
        sPokedexView->pokedexList[i].owned = FALSE;
    }
}

static void PrintMonDexNumAndName(u8 windowId, u8 fontId, const u8 *str, u8 left, u8 top)
{
    u8 color[3];

    color[0] = TEXT_COLOR_TRANSPARENT;
    color[1] = TEXT_DYNAMIC_COLOR_6;
    color[2] = TEXT_COLOR_LIGHT_GRAY;
    AddTextPrinterParameterized4(windowId, fontId, left * 8, (top * 8) + 1, 0, 0, color, TEXT_SKIP_DRAW, str);
}

static void PrintMoveData(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y)
{
    u8 color[3];

    color[0] = TEXT_COLOR_TRANSPARENT;
    color[1] = TEXT_DYNAMIC_COLOR_6;
    color[2] = TEXT_COLOR_LIGHT_GRAY;
    AddTextPrinterParameterized4(windowId, fontId, x, y, 0, -2, color, TEXT_SKIP_DRAW, str);
}

// u16 ignored is passed but never used
static void CreateMonListEntry(u8 position, u16 b, u16 ignored)
{
    s32 entryNum;
    u32 i;
    u32 vOffset;

    switch (position)
    {
    case 0: // Initial
    default:
        entryNum = b - 5;
        for (i = 0; i <= 10; i++)
        {
            if (entryNum < 0 || entryNum >= NATIONAL_DEX_COUNT || sPokedexView->pokedexList[entryNum].dexNum == 0xFFFF)
            {
                ClearMonListEntry(17, i * 2, ignored);
            }
            else
            {
                ClearMonListEntry(17, i * 2, ignored);
                if (sPokedexView->pokedexList[entryNum].seen)
                {
                    CreateMonDexNum(entryNum, 0x12, i * 2, ignored);
                    CreateCaughtBall(sPokedexView->pokedexList[entryNum].owned, 0x11, i * 2, ignored);
                    CreateMonName(sPokedexView->pokedexList[entryNum].dexNum, 0x16, i * 2);
                }
                else
                {
                    CreateMonDexNum(entryNum, 0x12, i * 2, ignored);
                    CreateCaughtBall(FALSE, 0x11, i * 2, ignored);
                    CreateMonName(0, 0x16, i * 2);
                }
            }
            entryNum++;
        }
        break;
    case 1: // Up
        entryNum = b - 5;
        if (entryNum < 0 || entryNum >= NATIONAL_DEX_COUNT || sPokedexView->pokedexList[entryNum].dexNum == 0xFFFF)
        {
            ClearMonListEntry(17, sPokedexView->listVOffset * 2, ignored);
        }
        else
        {
            ClearMonListEntry(17, sPokedexView->listVOffset * 2, ignored);
            if (sPokedexView->pokedexList[entryNum].seen)
            {
                CreateMonDexNum(entryNum, 18, sPokedexView->listVOffset * 2, ignored);
                CreateCaughtBall(sPokedexView->pokedexList[entryNum].owned, 0x11, sPokedexView->listVOffset * 2, ignored);
                CreateMonName(sPokedexView->pokedexList[entryNum].dexNum, 0x16, sPokedexView->listVOffset * 2);
            }
            else
            {
                CreateMonDexNum(entryNum, 18, sPokedexView->listVOffset * 2, ignored);
                CreateCaughtBall(FALSE, 17, sPokedexView->listVOffset * 2, ignored);
                CreateMonName(0, 0x16, sPokedexView->listVOffset * 2);
            }
        }
        break;
    case 2: // Down
        entryNum = b + 5;
        vOffset = sPokedexView->listVOffset + 10;
        if (vOffset >= LIST_SCROLL_STEP)
            vOffset -= LIST_SCROLL_STEP;
        if (entryNum < 0 || entryNum >= NATIONAL_DEX_COUNT || sPokedexView->pokedexList[entryNum].dexNum == 0xFFFF)
            ClearMonListEntry(17, vOffset * 2, ignored);
        else
        {
            ClearMonListEntry(17, vOffset * 2, ignored);
            if (sPokedexView->pokedexList[entryNum].seen)
            {
                CreateMonDexNum(entryNum, 18, vOffset * 2, ignored);
                CreateCaughtBall(sPokedexView->pokedexList[entryNum].owned, 0x11, vOffset * 2, ignored);
                CreateMonName(sPokedexView->pokedexList[entryNum].dexNum, 0x16, vOffset * 2);
            }
            else
            {
                CreateMonDexNum(entryNum, 18, vOffset * 2, ignored);
                CreateCaughtBall(FALSE, 0x11, vOffset * 2, ignored);
                CreateMonName(0, 0x16, vOffset * 2);
            }
        }
        break;
    }
    CopyWindowToVram(0, COPYWIN_GFX);
}

// u16 ignored is passed but never used
static void CreateMoveListEntry(u8 position, u16 selectedMove, u16 ignored)
{
    s32 entryNum;
    u32 i;
    s32 vOffset;
    u32 textColors[][3] =
    {
        {0, 1, 2},
    };

    switch (position)
    {
    case 0: // Initial
    default:
        entryNum = selectedMove - 4; // 4 = list slots above selected move
        for (i = 0; i < 10; i++) // 10 = list slots on sreen at once
        {
            if (entryNum < 0 || entryNum >= sMovesView->movesListCount)
            {
                ClearMoveListEntry(0, i * 2, ignored);
                gSprites[sMovesView->spriteIds[i]].invisible = TRUE;
            }
            else
            {
                ClearMoveListEntry(0, i * 2, ignored);
                CreateMovePrefix(sMovesView->movesList[entryNum].type, sMovesView->movesList[entryNum].index, 0, i * 2);
                CreateMoveName(sMovesView->movesList[entryNum].move, 8, i * 2);
                gSprites[sMovesView->spriteIds[i]].invisible = FALSE;
                StartSpriteAnim(&gSprites[sMovesView->spriteIds[i]], gBattleMoves[sMovesView->movesList[entryNum].move].type);
                gSprites[sMovesView->spriteIds[i]].oam.paletteNum = sMoveTypeToOamPaletteNum[gBattleMoves[sMovesView->movesList[entryNum].move].type];
            }
            entryNum++;
        }
        break;
    case 1: // Pressed Up
        entryNum = selectedMove - 4;
        if (entryNum < 0 || entryNum >= sMovesView->movesListCount)
        {
            ClearMoveListEntry(0, sMovesView->listVOffset * 2, ignored);
        }
        else
        {
            ClearMoveListEntry(0, sMovesView->listVOffset * 2, ignored);
            CreateMovePrefix(sMovesView->movesList[entryNum].type, sMovesView->movesList[entryNum].index, 0, sMovesView->listVOffset * 2);
            CreateMoveName(sMovesView->movesList[entryNum].move, 8, sMovesView->listVOffset * 2);
        }
        break;
    case 2: // Pressed Down
        entryNum = selectedMove + 5;
        vOffset = sMovesView->listVOffset + 9;
        if (vOffset >= LIST_SCROLL_STEP)
            vOffset -= LIST_SCROLL_STEP;
        if (entryNum < 0 || entryNum >= sMovesView->movesListCount)
        {
            ClearMoveListEntry(0, vOffset * 2, ignored);
        }
        else
        {
            ClearMoveListEntry(0, vOffset * 2, ignored);
            CreateMovePrefix(sMovesView->movesList[entryNum].type, sMovesView->movesList[entryNum].index, 0, vOffset * 2);
            CreateMoveName(sMovesView->movesList[entryNum].move, 8, vOffset * 2);
        }
        break;
    case 3: // Pressed Left/Right
        entryNum = selectedMove - 4; // 4 = list slots above selected move
        vOffset = sMovesView->listVOffset;
        for (i = 0; i < 10; i++) // 10 = list slots on sreen at once
        {
            if (vOffset + i >= LIST_SCROLL_STEP)
                vOffset -= LIST_SCROLL_STEP;
            if (entryNum < 0 || entryNum >= sMovesView->movesListCount)
            {
                ClearMoveListEntry(0, (i + vOffset) * 2, ignored);
                gSprites[sMovesView->spriteIds[i]].invisible = TRUE;
            }
            else
            {
                ClearMoveListEntry(0, (i + vOffset) * 2, ignored);
                CreateMovePrefix(sMovesView->movesList[entryNum].type, sMovesView->movesList[entryNum].index, 0, (i + vOffset) * 2);
                CreateMoveName(sMovesView->movesList[entryNum].move, 8, (i + vOffset) * 2);
                gSprites[sMovesView->spriteIds[i]].invisible = FALSE;
                StartSpriteAnim(&gSprites[sMovesView->spriteIds[i]], gBattleMoves[sMovesView->movesList[entryNum].move].type);
                gSprites[sMovesView->spriteIds[i]].oam.paletteNum = sMoveTypeToOamPaletteNum[gBattleMoves[sMovesView->movesList[entryNum].move].type];
            }
            entryNum++;
        }
        break;
    }

    PrintMoveInfo(sMovesView->movesList[selectedMove].move);

    CopyWindowToVram(WIN_MOVES, COPYWIN_GFX);
    CopyWindowToVram(WIN_MOVES_BATTLE_LABELS, COPYWIN_GFX);
    CopyWindowToVram(WIN_MOVES_BATTLE_VALUES, COPYWIN_GFX);
    CopyWindowToVram(WIN_MOVES_DESC, COPYWIN_GFX);
}

static void PrintMoveInfo(u16 moveIndex)
{
    const u8 *text;

    // Clear windows
    FillWindowPixelBuffer(WIN_MOVES_BATTLE_LABELS, PIXEL_FILL(0));
    FillWindowPixelBuffer(WIN_MOVES_BATTLE_VALUES, PIXEL_FILL(0));
    FillWindowPixelBuffer(WIN_MOVES_DESC, PIXEL_FILL(0));
    
    // Print labels
    PrintMoveData(WIN_MOVES_BATTLE_LABELS, FONT_NORMAL, gText_Power, 0, 1);
    PrintMoveData(WIN_MOVES_BATTLE_LABELS, FONT_NORMAL, gText_Accuracy2, 0, 17);
    PrintMoveData(WIN_MOVES_BATTLE_LABELS, FONT_NORMAL, gText_EffectChance, 0, 33);
    PrintMoveData(WIN_MOVES_BATTLE_LABELS, FONT_NORMAL, gText_Priority, 0, 49);
    PrintMoveData(WIN_MOVES_BATTLE_LABELS, FONT_NORMAL, gText_PP, 38, 65);

    // Print Power value
    if (gBattleMoves[moveIndex].power < 2)
    {
        text = gText_ThreeDashes;
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[moveIndex].power, STR_CONV_MODE_RIGHT_ALIGN, 3);
        text = gStringVar1;
    }
    PrintMoveData(WIN_MOVES_BATTLE_VALUES, FONT_NORMAL, text, 5, 1);

    // Print Accuracy value
    if (gBattleMoves[moveIndex].accuracy == 0)
    {
        text = gText_ThreeDashes;
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[moveIndex].accuracy, STR_CONV_MODE_RIGHT_ALIGN, 3);
        text = gStringVar1;
    }
    PrintMoveData(WIN_MOVES_BATTLE_VALUES, FONT_NORMAL, text, 5, 17);

    // Print Secondary Effect Chance value
    if (gBattleMoves[moveIndex].secondaryEffectChance == 0)
    {
        text = gText_ThreeDashes;
    }
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[moveIndex].secondaryEffectChance, STR_CONV_MODE_RIGHT_ALIGN, 3);
        text = gStringVar1;
    }
    PrintMoveData(WIN_MOVES_BATTLE_VALUES, FONT_NORMAL, text, 5, 33);

    // Print Priority value
    if (gBattleMoves[moveIndex].priority < 0)
        PrintMoveData(WIN_MOVES_BATTLE_VALUES, FONT_NORMAL, gText_Dash, 11, 49);
    ConvertIntToDecimalStringN(gStringVar1, abs(gBattleMoves[moveIndex].priority), STR_CONV_MODE_RIGHT_ALIGN, 3);
    text = gStringVar1;
    PrintMoveData(WIN_MOVES_BATTLE_VALUES, FONT_NORMAL, text, 5, 49);

    // Print PP
    ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[moveIndex].pp, STR_CONV_MODE_RIGHT_ALIGN, 3);
    text = gStringVar1;
    PrintMoveData(WIN_MOVES_BATTLE_VALUES, FONT_NORMAL, text, 5, 65);

    // Update Category sprite
    StartSpriteAnim(&gSprites[sMovesView->categorySpriteId], NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT + gBattleMoves[moveIndex].category);

    // Print Description
    PrintMoveData(WIN_MOVES_DESC, FONT_NORMAL, gMoveDescriptionPointers[moveIndex - 1], 0, 1);
}

static void CreateMonDexNum(u16 entryNum, u8 left, u8 top, u16 unused)
{
    u8 text[6];
    u16 dexNum;

    memcpy(text, sText_No000, ARRAY_COUNT(text));
    dexNum = sPokedexView->pokedexList[entryNum].dexNum;
    if (sPokedexView->dexMode == DEX_MODE_HOENN)
        dexNum = NationalToHoennOrder(dexNum);
    text[2] = CHAR_0 + dexNum / 100;
    text[3] = CHAR_0 + (dexNum % 100) / 10;
    text[4] = CHAR_0 + (dexNum % 100) % 10;
    PrintMonDexNumAndName(0, FONT_NARROW, text, left, top);
}

static void CreateCaughtBall(bool16 owned, u8 x, u8 y, u16 unused)
{
    if (owned)
        BlitBitmapToWindow(0, sCaughtBall_Gfx, x * 8, y * 8, 8, 16);
    else
        FillWindowPixelRect(0, PIXEL_FILL(0), x * 8, y * 8, 8, 16);
}

static u8 CreateMonName(u16 num, u8 left, u8 top)
{
    const u8 *str;

    num = NationalPokedexNumToSpecies(num);
    if (num)
        str = gSpeciesNames[num];
    else
        str = sText_TenDashes;
    PrintMonDexNumAndName(0, FONT_NARROW, str, left, top);
    return StringLength(str);
}

static void CreateMovePrefix(u8 type, u16 index, u8 left, u8 top)
{
    u8 text[6];

    switch (type)
    {
    case MOVE_LEVEL_UP:
        memcpy(text, sText_Lv00, ARRAY_COUNT(text));

        if (index < 10)
        {
            text[2] = CHAR_0 + (index % 100) % 10;
        }
        else
        {
            text[2] = CHAR_0 + (index % 100) / 10;
            text[3] = CHAR_0 + (index % 100) % 10;
        }
        break;
    case MOVE_EGG:
        memcpy(text, sText_Egg, ARRAY_COUNT(text));
        break;
    case MOVE_TM:
        memcpy(text, sText_TM00, ARRAY_COUNT(text));
        text[2] = CHAR_0 + (index % 100) / 10;
        text[3] = CHAR_0 + (index % 100) % 10;
        break;
    case MOVE_HM:
        memcpy(text, sText_TM00, ARRAY_COUNT(text));
        text[0] = CHAR_H;
        text[2] = CHAR_0 + (index - 50 % 100) / 10;
        text[3] = CHAR_0 + (index - 50 % 100) % 10;
        break;
    case MOVE_TUTOR:
        memcpy(text, sText_Tutor, ARRAY_COUNT(text));
        break;
    case MOVE_PRE_EVOLUTION:
        memcpy(text, sText_Pre, ARRAY_COUNT(text));
        break;
    default:
        text[0] = '\0';
        break;
    }

    PrintMonDexNumAndName(WIN_MOVES, FONT_NARROW, text, left, top);
}

static u8 CreateMoveName(u16 move, u8 left, u8 top)
{
    const u8 *str;

    if (move)
        str = gMoveNames[move];
    PrintMonDexNumAndName(WIN_MOVES, FONT_NARROW, str, left, top);
    return StringLength(str);
}

static void ClearMonListEntry(u8 x, u8 y, u16 unused)
{
    FillWindowPixelRect(0, PIXEL_FILL(0), x * 8, y * 8, 0x60, 16);
}

static void ClearMoveListEntry(u8 x, u8 y, u16 unused)
{
    FillWindowPixelRect(4, PIXEL_FILL(0), x * 8, y * 8, 128, 16);
}

// u16 ignored is passed but never used
static void CreateMonSpritesAtPos(u16 selectedMon, u16 ignored)
{
    u32 i;
    u32 dexNum;
    u32 spriteId;

    gPaletteFade.bufferTransferDisabled = TRUE;

    for (i = 0; i < MAX_MONS_ON_SCREEN; i++)
        sPokedexView->monSpriteIds[i] = 0xFFFF;
    sPokedexView->selectedMonSpriteId = 0xFFFF;

    // Create top mon sprite
    dexNum = GetPokemonSpriteToDisplay(selectedMon - 1);
    if (dexNum != 0xFFFF)
    {
        spriteId = CreatePokedexMonSprite(dexNum, 0x60, 0x50);
        gSprites[spriteId].callback = SpriteCB_PokedexListMonSprite;
        gSprites[spriteId].data[5] = -32;
    }

    // Create mid mon sprite
    dexNum = GetPokemonSpriteToDisplay(selectedMon);
    if (dexNum != 0xFFFF)
    {
        spriteId = CreatePokedexMonSprite(dexNum, 0x60, 0x50);
        gSprites[spriteId].callback = SpriteCB_PokedexListMonSprite;
        gSprites[spriteId].data[5] = 0;
    }

    // Create bottom mon sprite
    dexNum = GetPokemonSpriteToDisplay(selectedMon + 1);
    if (dexNum != 0xFFFF)
    {
        spriteId = CreatePokedexMonSprite(dexNum, 0x60, 0x50);
        gSprites[spriteId].callback = SpriteCB_PokedexListMonSprite;
        gSprites[spriteId].data[5] = 32;
    }

    CreateMonListEntry(0, selectedMon, ignored);
    SetGpuReg(REG_OFFSET_BG2VOFS, sPokedexView->initialVOffset);

    sPokedexView->listVOffset = 0;
    sPokedexView->listMovingVOffset = 0;

    gPaletteFade.bufferTransferDisabled = FALSE;
}

static bool8 UpdateDexListScroll(u8 direction, u8 monMoveIncrement, u8 scrollTimerMax)
{
    u32 i;
    u32 step;

    if (sPokedexView->scrollTimer)
    {
        sPokedexView->scrollTimer--;
        switch (direction)
        {
        case 1: // Up
            for (i = 0; i < MAX_MONS_ON_SCREEN; i++)
            {
                if (sPokedexView->monSpriteIds[i] != 0xFFFF)
                    gSprites[sPokedexView->monSpriteIds[i]].data[5] += monMoveIncrement;
            }
            step = LIST_SCROLL_STEP * (scrollTimerMax - sPokedexView->scrollTimer) / scrollTimerMax;
            SetGpuReg(REG_OFFSET_BG2VOFS, sPokedexView->initialVOffset + sPokedexView->listMovingVOffset * LIST_SCROLL_STEP - step);
            sPokedexView->pokeBallRotation -= sPokedexView->pokeBallRotationStep;
            break;
        case 2: // Down
            for (i = 0; i < MAX_MONS_ON_SCREEN; i++)
            {
                if (sPokedexView->monSpriteIds[i] != 0xFFFF)
                    gSprites[sPokedexView->monSpriteIds[i]].data[5] -= monMoveIncrement;
            }
            step = LIST_SCROLL_STEP * (scrollTimerMax - sPokedexView->scrollTimer) / scrollTimerMax;
            SetGpuReg(REG_OFFSET_BG2VOFS, sPokedexView->initialVOffset + sPokedexView->listMovingVOffset * LIST_SCROLL_STEP + step);
            sPokedexView->pokeBallRotation += sPokedexView->pokeBallRotationStep;
            break;
        }
        return FALSE;
    }
    else
    {
        SetGpuReg(REG_OFFSET_BG2VOFS, sPokedexView->initialVOffset + sPokedexView->listVOffset * LIST_SCROLL_STEP);
        return TRUE;
    }
}

static bool8 UpdateMovesListScroll(u8 direction, u8 monMoveIncrement, u8 scrollTimerMax)
{
    s32 offset, newOffset, moveIndex;
    u32 step, moveType;
    u32 i;

    if (sMovesView->scrollTimer)
    {
        sMovesView->scrollTimer--;
        offset = sMovesView->listMovingVOffset * LIST_SCROLL_STEP;
        step = LIST_SCROLL_STEP * (scrollTimerMax - sMovesView->scrollTimer) / scrollTimerMax;
        newOffset = (direction == 1) ? (offset - step) : (offset + step);
        
        for (i = 0; i < 10; i++)
        {
            gSprites[sMovesView->spriteIds[i]].y = sMovesView->spriteYPos[i] + (direction == 1 ? step : -step);
        }
        sMovesView->bg3VOffsetBuffer = sMovesView->initialVOffset + newOffset;
        
        return FALSE;
    }
    else
    {
        for (i = 0; i < 10; i++)
        {
            struct Sprite* currentSprite = &gSprites[sMovesView->spriteIds[i]];
            currentSprite->y = sMovesView->spriteYPos[i];
            moveIndex = sMovesView->selectedMove + i - 4;
            
            if(moveIndex < 0 || moveIndex >= sMovesView->movesListCount)
            {
                currentSprite->invisible = TRUE;
            }
            else
            {
                currentSprite->invisible = FALSE;
                moveType = gBattleMoves[sMovesView->movesList[moveIndex].move].type;
                StartSpriteAnim(currentSprite, moveType);
                currentSprite->oam.paletteNum = sMoveTypeToOamPaletteNum[moveType];
            }
        }
        sMovesView->bg3VOffsetBuffer = sMovesView->initialVOffset + sMovesView->listVOffset * LIST_SCROLL_STEP;

        return TRUE;
    }
}

static void SyncBg3VOffset()
{
    SetGpuReg(REG_OFFSET_BG3VOFS, sMovesView->bg3VOffsetBuffer);
}

static void CreateScrollingPokemonSprite(u8 direction, u16 selectedMon)
{
    u16 dexNum;
    u8 spriteId;

    sPokedexView->listMovingVOffset = sPokedexView->listVOffset;
    switch (direction)
    {
    case 1: // up
        dexNum = GetPokemonSpriteToDisplay(selectedMon - 1);
        if (dexNum != 0xFFFF)
        {
            spriteId = CreatePokedexMonSprite(dexNum, 0x60, 0x50);
            gSprites[spriteId].callback = SpriteCB_PokedexListMonSprite;
            gSprites[spriteId].data[5] = -64;
        }
        if (sPokedexView->listVOffset > 0)
            sPokedexView->listVOffset--;
        else
            sPokedexView->listVOffset = LIST_SCROLL_STEP - 1;
        break;
    case 2: // down
        dexNum = GetPokemonSpriteToDisplay(selectedMon + 1);
        if (dexNum != 0xFFFF)
        {
            spriteId = CreatePokedexMonSprite(dexNum, 0x60, 0x50);
            gSprites[spriteId].callback = SpriteCB_PokedexListMonSprite;
            gSprites[spriteId].data[5] = 64;
        }
        if (sPokedexView->listVOffset < LIST_SCROLL_STEP - 1)
            sPokedexView->listVOffset++;
        else
            sPokedexView->listVOffset = 0;
        break;
    }
}

static void CreateScrollingMoveSprite(u8 direction)
{
    sMovesView->listMovingVOffset = sMovesView->listVOffset;
    switch (direction)
    {
    case 1: // up
        if (sMovesView->listVOffset > 0)
            sMovesView->listVOffset--;
        else
            sMovesView->listVOffset = LIST_SCROLL_STEP - 1;
        break;
    case 2: // down
        if (sMovesView->listVOffset < LIST_SCROLL_STEP - 1)
            sMovesView->listVOffset++;
        else
            sMovesView->listVOffset = 0;
        break;
    }
}

// u16 ignored is passed but never used
static u16 TryDoPokedexScroll(u16 selectedMon, u16 ignored)
{
    u32 scrollTimer;
    u32 scrollMonIncrement;
    u32 i;
    u32 startingPos;
    u32 scrollDir = 0;

    if (JOY_HELD(DPAD_UP) && (selectedMon > 0))
    {
        scrollDir = 1;
        selectedMon = GetNextPosition(1, selectedMon, 0, sPokedexView->pokemonListCount - 1);
        CreateScrollingPokemonSprite(1, selectedMon);
        CreateMonListEntry(1, selectedMon, ignored);
        PlaySE(SE_DEX_SCROLL);
    }
    else if (JOY_HELD(DPAD_DOWN) && (selectedMon < sPokedexView->pokemonListCount - 1))
    {
        scrollDir = 2;
        selectedMon = GetNextPosition(0, selectedMon, 0, sPokedexView->pokemonListCount - 1);
        CreateScrollingPokemonSprite(2, selectedMon);
        CreateMonListEntry(2, selectedMon, ignored);
        PlaySE(SE_DEX_SCROLL);
    }
    else if (JOY_NEW(DPAD_LEFT) && (selectedMon > 0))
    {
        startingPos = selectedMon;

        for (i = 0; i < 7; i++)
            selectedMon = GetNextPosition(1, selectedMon, 0, sPokedexView->pokemonListCount - 1);
        sPokedexView->pokeBallRotation += 16 * (selectedMon - startingPos);
        ClearMonSprites();
        CreateMonSpritesAtPos(selectedMon, 0xE);
        PlaySE(SE_DEX_PAGE);
    }
    else if (JOY_NEW(DPAD_RIGHT) && (selectedMon < sPokedexView->pokemonListCount - 1))
    {
        startingPos = selectedMon;
        for (i = 0; i < 7; i++)
            selectedMon = GetNextPosition(0, selectedMon, 0, sPokedexView->pokemonListCount - 1);
        sPokedexView->pokeBallRotation += 16 * (selectedMon - startingPos);
        ClearMonSprites();
        CreateMonSpritesAtPos(selectedMon, 0xE);
        PlaySE(SE_DEX_PAGE);
    }

    if (scrollDir == 0)
    {
        // Left/right input just snaps up/down, no scrolling
        sPokedexView->scrollSpeed = 0;
        return selectedMon;
    }

    scrollMonIncrement = sScrollMonIncrements[sPokedexView->scrollSpeed / 4];
    scrollTimer = sScrollTimers[sPokedexView->scrollSpeed / 4];
    sPokedexView->scrollTimer = scrollTimer;
    sPokedexView->maxScrollTimer = scrollTimer;
    sPokedexView->scrollMonIncrement = scrollMonIncrement;
    sPokedexView->scrollDirection = scrollDir;
    sPokedexView->pokeBallRotationStep = scrollMonIncrement / 2;
    UpdateDexListScroll(sPokedexView->scrollDirection, sPokedexView->scrollMonIncrement, sPokedexView->maxScrollTimer);
    if (sPokedexView->scrollSpeed < 12)
        sPokedexView->scrollSpeed++;
    return selectedMon;
}

// u16 ignored is passed but never used
static u16 TryDoMovesScroll(u16 selectedMove, u16 ignored, u8 taskId)
{
    u32 scrollTimer;
    u32 scrollMovesIncrement;
    u32 i;
    u32 startingPos;
    u32 scrollDir = 0;

    if (JOY_HELD(DPAD_UP))
    {
        if (selectedMove > 0)
        {
            scrollDir = 1;
            selectedMove = GetNextPosition(1, selectedMove, 0, sMovesView->movesListCount - 1);
            CreateScrollingMoveSprite(1);
            CreateMoveListEntry(1, selectedMove, ignored);
            PlaySE(SE_DEX_SCROLL);
        }
    }
    else if (JOY_HELD(DPAD_DOWN) && (selectedMove < sMovesView->movesListCount - 1))
    {
        scrollDir = 2;
        selectedMove = GetNextPosition(0, selectedMove, 0, sMovesView->movesListCount - 1);
        CreateScrollingMoveSprite(2);
        CreateMoveListEntry(2, selectedMove, ignored);
        PlaySE(SE_DEX_SCROLL);
    }
    else if (JOY_NEW(DPAD_LEFT) && (selectedMove > 0))
    {
        for (i = 0; i < 5; i++)
        {
            selectedMove = GetNextPosition(1, selectedMove, 0, sMovesView->movesListCount - 1);
            CreateScrollingMoveSprite(1);
        }
        CreateMoveListEntry(3, selectedMove, ignored);
        sMovesView->bg3VOffsetBuffer = sMovesView->initialVOffset + sMovesView->listVOffset * LIST_SCROLL_STEP;
        PlaySE(SE_DEX_PAGE);
    }
    else if (JOY_NEW(DPAD_RIGHT) && (selectedMove < sMovesView->movesListCount - 1))
    {
        for (i = 0; i < 5; i++)
        {
            selectedMove = GetNextPosition(0, selectedMove, 0, sMovesView->movesListCount - 1);
            CreateScrollingMoveSprite(2);
        }
        CreateMoveListEntry(3, selectedMove, ignored);
        sMovesView->bg3VOffsetBuffer = sMovesView->initialVOffset + sMovesView->listVOffset * LIST_SCROLL_STEP;
        PlaySE(SE_DEX_PAGE);
    }

    if (scrollDir == 0)
    {
        // Left/right input just snaps up/down, no scrolling
        sMovesView->scrollSpeed = 0;
        return selectedMove;
    }

    scrollMovesIncrement = sScrollMonIncrements[sMovesView->scrollSpeed / 4];
    scrollTimer = sScrollTimers[sMovesView->scrollSpeed / 4];
    sMovesView->scrollTimer = scrollTimer;
    sMovesView->maxScrollTimer = scrollTimer;
    sMovesView->scrollMovesIncrement = scrollMovesIncrement;
    sMovesView->scrollDirection = scrollDir;
    UpdateMovesListScroll(sMovesView->scrollDirection, sMovesView->scrollMovesIncrement, sMovesView->maxScrollTimer);
    if (sMovesView->scrollSpeed < 8)
        sMovesView->scrollSpeed++;
    return selectedMove;
}

static void SetTypeSpritePosAndPal(u8 typeId, u8 x, u8 y, u8 spriteId)
{
    struct Sprite *sprite = &gSprites[spriteId];
    StartSpriteAnim(sprite, typeId);
    sprite->oam.paletteNum = sMoveTypeToOamPaletteNum[typeId];
    sprite->oam.affineMode = ST_OAM_AFFINE_OFF;
    sprite->x = x + 16;
    sprite->y = y + 8;
}

static void SetMoveTypeIcons(void)
{
    u32 i;

    for (i = 0; i < 10; i++)
    {
        SetTypeSpritePosAndPal(TYPE_MYSTERY, 125, i * 16, sMovesView->spriteIds[i]);
        sMovesView->spriteYPos[i] = i * 16 + 8;
    }

    SetTypeSpritePosAndPal(NUMBER_OF_MON_TYPES + CONTEST_CATEGORIES_COUNT + CATEGORY_PHYSICAL, 8, 96, sMovesView->categorySpriteId);
}

static void ResetSpriteIds(void)
{
    u32 i;

    for (i = 0; i < 10; i++)
        sMovesView->spriteIds[i] = SPRITE_NONE;

    sMovesView->categorySpriteId = SPRITE_NONE;
}

static void CreateMoveTypeIcons(void)
{
    u32 i;
    for (i = 0; i < 10; i++)
    {
        if (sMovesView->spriteIds[i] == SPRITE_NONE)
            sMovesView->spriteIds[i] = CreateSprite(&sSpriteTemplate_MoveTypes, 0, 0, 2);
    }

    if (sMovesView->categorySpriteId == SPRITE_NONE)
        sMovesView->categorySpriteId = CreateSprite(&sSpriteTemplate_MoveTypes, 0, 0, 2);
}

static void UpdateSelectedMonSpriteId(void)
{
    u32 i;

    for (i = 0; i < MAX_MONS_ON_SCREEN; i++)
    {
        u16 spriteId = sPokedexView->monSpriteIds[i];

        if (gSprites[spriteId].x2 == 0 && gSprites[spriteId].y2 == 0 && spriteId != 0xFFFF)
            sPokedexView->selectedMonSpriteId = spriteId;
    }
}

bool8 TryDoInfoScreenScroll(void)
{
    u16 nextPokemon;
    u16 selectedPokemon = sPokedexView->selectedPokemon;

    if (JOY_NEW(DPAD_UP) && selectedPokemon)
    {
        nextPokemon = selectedPokemon;
        while (nextPokemon != 0)
        {
            nextPokemon = GetNextPosition(1, nextPokemon, 0, sPokedexView->pokemonListCount - 1);

            if (sPokedexView->pokedexList[nextPokemon].seen)
            {
                selectedPokemon = nextPokemon;
                break;
            }
        }

        if (sPokedexView->selectedPokemon == selectedPokemon)
            return FALSE;
        else
        {
            sPokedexView->selectedPokemon = selectedPokemon;
            sPokedexView->pokeBallRotation -= 16;
            return TRUE;
        }
    }
    else if (JOY_NEW(DPAD_DOWN) && selectedPokemon < sPokedexView->pokemonListCount - 1)
    {
        nextPokemon = selectedPokemon;
        while (nextPokemon < sPokedexView->pokemonListCount - 1)
        {
            nextPokemon = GetNextPosition(0, nextPokemon, 0, sPokedexView->pokemonListCount - 1);

            if (sPokedexView->pokedexList[nextPokemon].seen)
            {
                selectedPokemon = nextPokemon;
                break;
            }
        }

        if (sPokedexView->selectedPokemon == selectedPokemon)
            return FALSE;
        else
        {
            sPokedexView->selectedPokemon = selectedPokemon;
            sPokedexView->pokeBallRotation += 16;
            return TRUE;
        }
    }
    return FALSE;
}

static u8 ClearMonSprites(void)
{
    u32 i;

    for (i = 0; i < MAX_MONS_ON_SCREEN; i++)
    {
        if (sPokedexView->monSpriteIds[i] != 0xFFFF)
        {
            FreeAndDestroyMonPicSprite(sPokedexView->monSpriteIds[i]);
            sPokedexView->monSpriteIds[i] = 0xFFFF;
        }
    }
    return FALSE;
}

static u16 GetPokemonSpriteToDisplay(u16 species)
{
    if (species >= NATIONAL_DEX_COUNT || sPokedexView->pokedexList[species].dexNum == 0xFFFF)
        return 0xFFFF;
    else if (sPokedexView->pokedexList[species].seen)
        return sPokedexView->pokedexList[species].dexNum;
    else
        return 0;
}

static u32 CreatePokedexMonSprite(u16 num, s16 x, s16 y)
{
    u32 i;

    for (i = 0; i < MAX_MONS_ON_SCREEN; i++)
    {
        if (sPokedexView->monSpriteIds[i] == 0xFFFF)
        {
            u8 spriteId = CreateMonSpriteFromNationalDexNumber(num, x, y, i);

            gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
            gSprites[spriteId].oam.priority = 3;
            gSprites[spriteId].data[0] = 0;
            gSprites[spriteId].data[1] = i;
            gSprites[spriteId].data[2] = NationalPokedexNumToSpecies(num);
            sPokedexView->monSpriteIds[i] = spriteId;
            return spriteId;
        }
    }
    return 0xFFFF;
}

#define sIsDownArrow data[1]

static void CreateInterfaceSprites(u8 page)
{
    u8 spriteId;
    u16 digitNum;

    // Scroll arrows
    spriteId = CreateSprite(&sScrollArrowSpriteTemplate, 184, 4, 0);
    gSprites[spriteId].sIsDownArrow = FALSE;
    spriteId = CreateSprite(&sScrollArrowSpriteTemplate, 184, DISPLAY_HEIGHT - 4, 0);
    gSprites[spriteId].sIsDownArrow = TRUE;
    gSprites[spriteId].vFlip = TRUE;

    CreateSprite(&sScrollBarSpriteTemplate, 230, 20, 0);
    // Start button
    CreateSprite(&sInterfaceTextSpriteTemplate, 16, 120, 0);
    // Menu text
    spriteId = CreateSprite(&sInterfaceTextSpriteTemplate, 48, 120, 0);
    StartSpriteAnim(&gSprites[spriteId], 3);
    // Select button
    spriteId = CreateSprite(&sInterfaceTextSpriteTemplate, 16, DISPLAY_HEIGHT - 16, 0);
    StartSpriteAnim(&gSprites[spriteId], 2);
    gSprites[spriteId].data[2] = 0x80;
    // Search text
    spriteId = CreateSprite(&sInterfaceTextSpriteTemplate, 48, DISPLAY_HEIGHT - 16, 0);
    StartSpriteAnim(&gSprites[spriteId], 1);

    spriteId = CreateSprite(&sRotatingPokeBallSpriteTemplate, 0, DISPLAY_HEIGHT / 2, 2);
    gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
    gSprites[spriteId].oam.matrixNum = 30;
    gSprites[spriteId].data[0] = 30;
    gSprites[spriteId].data[1] = 0;

    spriteId = CreateSprite(&sRotatingPokeBallSpriteTemplate, 0, DISPLAY_HEIGHT / 2, 2);
    gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
    gSprites[spriteId].oam.matrixNum = 31;
    gSprites[spriteId].data[0] = 31;
    gSprites[spriteId].data[1] = 128;

    if (page == PAGE_MAIN)
    {
        bool32 drawNextDigit;

        if (!IsNationalPokedexEnabled())
        {
            // Seen text
            CreateSprite(&sSeenOwnTextSpriteTemplate, 32, 40, 1);
            // Own text
            spriteId = CreateSprite(&sSeenOwnTextSpriteTemplate, 32, 72, 1);
            StartSpriteAnim(&gSprites[spriteId], 1);

            // Seen value - 100s
            drawNextDigit = FALSE;
            spriteId = CreateSprite(&sHoennDexSeenOwnNumberSpriteTemplate, 24, 48, 1);
            digitNum = sPokedexView->seenCount / 100;
            StartSpriteAnim(&gSprites[spriteId], digitNum);
            if (digitNum != 0)
                drawNextDigit = TRUE;
            else
                gSprites[spriteId].invisible = TRUE;

            // Seen value - 10s
            spriteId = CreateSprite(&sHoennDexSeenOwnNumberSpriteTemplate, 32, 48, 1);
            digitNum = (sPokedexView->seenCount % 100) / 10;
            if (digitNum != 0 || drawNextDigit)
                StartSpriteAnim(&gSprites[spriteId], digitNum);
            else
                gSprites[spriteId].invisible = TRUE;

            // Seen value - 1s
            spriteId = CreateSprite(&sHoennDexSeenOwnNumberSpriteTemplate, 40, 48, 1);
            digitNum = (sPokedexView->seenCount % 100) % 10;
            StartSpriteAnim(&gSprites[spriteId], digitNum);

            // Owned value - 100s
            drawNextDigit = FALSE;
            spriteId = CreateSprite(&sHoennDexSeenOwnNumberSpriteTemplate, 24, 80, 1);
            digitNum = sPokedexView->ownCount / 100;
            StartSpriteAnim(&gSprites[spriteId], digitNum);
            if (digitNum != 0)
                drawNextDigit = TRUE;
            else
                gSprites[spriteId].invisible = TRUE;

            // Owned value - 10s
            spriteId = CreateSprite(&sHoennDexSeenOwnNumberSpriteTemplate, 32, 80, 1);
            digitNum = (sPokedexView->ownCount % 100) / 10;
            if (digitNum != 0 || drawNextDigit)
                StartSpriteAnim(&gSprites[spriteId], digitNum);
            else
                gSprites[spriteId].invisible = TRUE;

            // Owned value - 1s
            spriteId = CreateSprite(&sHoennDexSeenOwnNumberSpriteTemplate, 40, 80, 1);
            digitNum = (sPokedexView->ownCount % 100) % 10;
            StartSpriteAnim(&gSprites[spriteId], digitNum);
        }
        else
        {
            u16 seenOwnedCount;

            // Seen text
            CreateSprite(&sSeenOwnTextSpriteTemplate, 32, 40, 1);
            // Own text
            spriteId = CreateSprite(&sSeenOwnTextSpriteTemplate, 32, 76, 1);
            StartSpriteAnim(&gSprites[spriteId], 1);

            // Hoenn text (seen)
            CreateSprite(&sHoennNationalTextSpriteTemplate, 17, 45, 1);

            // National text (seen)
            spriteId = CreateSprite(&sHoennNationalTextSpriteTemplate, 17, 55, 1);
            StartSpriteAnim(&gSprites[spriteId], 1);

            // Hoenn text (own)
            CreateSprite(&sHoennNationalTextSpriteTemplate, 17, 81, 1);

            // National text (own)
            spriteId = CreateSprite(&sHoennNationalTextSpriteTemplate, 17, 91, 1);
            StartSpriteAnim(&gSprites[spriteId], 1);

            // Hoenn seen value - 100s
            seenOwnedCount = GetHoennPokedexCount(FLAG_GET_SEEN);
            drawNextDigit = FALSE;
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 40, 45, 1);
            digitNum = seenOwnedCount / 100;
            StartSpriteAnim(&gSprites[spriteId], digitNum);
            if (digitNum != 0)
                drawNextDigit = TRUE;
            else
                gSprites[spriteId].invisible = TRUE;

            // Hoenn seen value - 10s
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 48, 45, 1);
            digitNum = (seenOwnedCount % 100) / 10;
            if (digitNum != 0 || drawNextDigit)
                StartSpriteAnim(&gSprites[spriteId], digitNum);
            else
                gSprites[spriteId].invisible = TRUE;

            // Hoenn seen value - 1s
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 56, 45, 1);
            digitNum = (seenOwnedCount % 100) % 10;
            StartSpriteAnim(&gSprites[spriteId], digitNum);

            // National seen value - 100s
            drawNextDigit = FALSE;
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 40, 55, 1);
            digitNum = sPokedexView->seenCount / 100;
            StartSpriteAnim(&gSprites[spriteId], digitNum);
            if (digitNum != 0)
                drawNextDigit = TRUE;
            else
                gSprites[spriteId].invisible = TRUE;

            // National seen value - 10s
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 48, 55, 1);
            digitNum = (sPokedexView->seenCount % 100) / 10;
            if (digitNum != 0 || drawNextDigit)
                StartSpriteAnim(&gSprites[spriteId], digitNum);
            else
                gSprites[spriteId].invisible = TRUE;

            // National seen value - 1s
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 56, 55, 1);
            digitNum = (sPokedexView->seenCount % 100) % 10;
            StartSpriteAnim(&gSprites[spriteId], digitNum);

            seenOwnedCount = GetHoennPokedexCount(FLAG_GET_CAUGHT);

            // Hoenn owned value - 100s
            drawNextDigit = FALSE;
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 40, 81, 1);
            digitNum = seenOwnedCount / 100;
            StartSpriteAnim(&gSprites[spriteId], digitNum);
            if (digitNum != 0)
                drawNextDigit = TRUE;
            else
                gSprites[spriteId].invisible = TRUE;

            // Hoenn owned value - 10s
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 48, 81, 1);
            digitNum = (seenOwnedCount % 100) / 10;
            if (digitNum != 0 || drawNextDigit)
                StartSpriteAnim(&gSprites[spriteId], digitNum);
            else
                gSprites[spriteId].invisible = TRUE;

            // Hoenn owned value - 1s
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 56, 81, 1);
            digitNum = (seenOwnedCount % 100) % 10;
            StartSpriteAnim(&gSprites[spriteId], digitNum);

            // National owned value - 100s
            drawNextDigit = FALSE;
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 40, 91, 1);
            digitNum = sPokedexView->ownCount / 100;
            StartSpriteAnim(&gSprites[spriteId], digitNum);
            if (digitNum != 0)
                drawNextDigit = TRUE;
            else
                gSprites[spriteId].invisible = TRUE;

            // National owned value  - 10s
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 48, 91, 1);
            digitNum = (sPokedexView->ownCount % 100) / 10;
            if (digitNum != 0 || drawNextDigit)
                StartSpriteAnim(&gSprites[spriteId], digitNum);
            else
                gSprites[spriteId].invisible = TRUE;

            // National owned value - 1s
            spriteId = CreateSprite(&sNationalDexSeenOwnNumberSpriteTemplate, 56, 91, 1);
            digitNum = (sPokedexView->ownCount % 100) % 10;
            StartSpriteAnim(&gSprites[spriteId], digitNum);
        }
        spriteId = CreateSprite(&sDexListStartMenuCursorSpriteTemplate, 136, 96, 1);
        gSprites[spriteId].invisible = TRUE;
    }
    else // PAGE_SEARCH_RESULTS
    {
        spriteId = CreateSprite(&sDexListStartMenuCursorSpriteTemplate, 136, 80, 1);
        gSprites[spriteId].invisible = TRUE;
    }
}

static void SpriteCB_EndMoveMonForInfoScreen(struct Sprite *sprite)
{
    // Once mon is done moving there's nothing left to do
}

static void SpriteCB_SeenOwnInfo(struct Sprite *sprite)
{
    if (sPokedexView->currentPage != PAGE_MAIN)
        DestroySprite(sprite);
}

void SpriteCB_MoveMonForInfoScreen(struct Sprite *sprite)
{
    sprite->oam.priority = 0;
    sprite->oam.affineMode = ST_OAM_AFFINE_OFF;
    sprite->x2 = 0;
    sprite->y2 = 0;
    if (sprite->x != MON_PAGE_X || sprite->y != MON_PAGE_Y)
    {
        if (sprite->x > MON_PAGE_X)
            sprite->x--;
        if (sprite->x < MON_PAGE_X)
            sprite->x++;

        if (sprite->y > MON_PAGE_Y)
            sprite->y--;
        if (sprite->y < MON_PAGE_Y)
            sprite->y++;
    }
    else
    {
        sprite->callback = SpriteCB_EndMoveMonForInfoScreen;
    }
}

static void SpriteCB_PokedexListMonSprite(struct Sprite *sprite)
{
    u8 monId = sprite->data[1];

    if (sPokedexView->currentPage != PAGE_MAIN && sPokedexView->currentPage != PAGE_SEARCH_RESULTS)
    {
        FreeAndDestroyMonPicSprite(sPokedexView->monSpriteIds[monId]);
        sPokedexView->monSpriteIds[monId] = 0xFFFF;
    }
    else
    {
        u32 var;
        sprite->y2 = gSineTable[(u8)sprite->data[5]] * 76 / 256;
        var = SAFE_DIV(0x10000, gSineTable[sprite->data[5] + 64]);
        if (var > 0xFFFF)
            var = 0xFFFF;
        SetOamMatrix(sprite->data[1] + 1, 0x100, 0, 0, var);
        sprite->oam.matrixNum = monId + 1;

        if (sprite->data[5] > -64 && sprite->data[5] < 64)
        {
            sprite->invisible = FALSE;
            sprite->data[0] = 1;
        }
        else
        {
            sprite->invisible = TRUE;
        }

        if ((sprite->data[5] <= -64 || sprite->data[5] >= 64) && sprite->data[0] != 0)
        {
            FreeAndDestroyMonPicSprite(sPokedexView->monSpriteIds[monId]);
            sPokedexView->monSpriteIds[monId] = 0xFFFF;
        }
    }
}

static void SpriteCB_Scrollbar(struct Sprite *sprite)
{
    if (sPokedexView->currentPage != PAGE_MAIN && sPokedexView->currentPage != PAGE_SEARCH_RESULTS && sPokedexView->currentPage != PAGE_MOVES)
        DestroySprite(sprite);
    else if (sPokedexView->currentPage == PAGE_MOVES)
    {
        sprite->y2 = sMovesView->selectedMove * 88 / (sMovesView->movesListCount - 1);
    }
    else
        sprite->y2 = sPokedexView->selectedPokemon * 120 / (sPokedexView->pokemonListCount - 1);
}

static void SpriteCB_ScrollArrow(struct Sprite *sprite)
{
    if (sPokedexView->currentPage != PAGE_MAIN && sPokedexView->currentPage != PAGE_SEARCH_RESULTS)
    {
        DestroySprite(sprite);
    }
    else
    {
        u8 r0;

        if (sprite->sIsDownArrow)
        {
            if (sPokedexView->selectedPokemon == sPokedexView->pokemonListCount - 1)
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
            r0 = sprite->data[2];
        }
        else
        {
            if (sPokedexView->selectedPokemon == 0)
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
            r0 = sprite->data[2] - 128;
        }
        sprite->y2 = gSineTable[r0] / 64;
        sprite->data[2] = sprite->data[2] + 8;
        if (sPokedexView->menuIsOpen == FALSE && sPokedexView->menuY == 0 && sprite->invisible == FALSE)
            sprite->invisible = FALSE;
        else
            sprite->invisible = TRUE;
    }
}

static void SpriteCB_DexListInterfaceText(struct Sprite *sprite)
{
    if (sPokedexView->currentPage != PAGE_MAIN && sPokedexView->currentPage != PAGE_SEARCH_RESULTS)
        DestroySprite(sprite);
}

static void SpriteCB_RotatingPokeBall(struct Sprite *sprite)
{
    if (sPokedexView->currentPage != PAGE_MAIN && sPokedexView->currentPage != PAGE_SEARCH_RESULTS)
    {
        DestroySprite(sprite);
    }
    else
    {
        u8 val;
        s16 r3;
        s16 r0;

        val = sPokedexView->pokeBallRotation + sprite->data[1];
        r3 = gSineTable[val];
        r0 = gSineTable[val + 64];
        SetOamMatrix(sprite->data[0], r0, r3, -r3, r0);

        val = sPokedexView->pokeBallRotation + (sprite->data[1] + 64);
        r3 = gSineTable[val];
        r0 = gSineTable[val + 64];
        sprite->x2 = r0 * 40 / 256;
        sprite->y2 = r3 * 40 / 256;
    }
}

static void SpriteCB_DexListStartMenuCursor(struct Sprite *sprite)
{
    if (sPokedexView->currentPage != PAGE_MAIN && sPokedexView->currentPage != PAGE_SEARCH_RESULTS)
    {
        DestroySprite(sprite);
    }
    else
    {
        u16 r1 = sPokedexView->currentPage == PAGE_MAIN ? 80 : 96;

        if (sPokedexView->menuIsOpen && sPokedexView->menuY == r1)
        {
            sprite->invisible = FALSE;
            sprite->y2 = sPokedexView->menuCursorPos * 16;
            sprite->x2 = gSineTable[(u8)sprite->data[2]] / 64;
            sprite->data[2] += 8;
        }
        else
        {
            sprite->invisible = TRUE;
        }
    }
}

static void PrintInfoScreenText(const u8 *str, u8 left, u8 top)
{
    u8 color[3];
    color[0] = TEXT_COLOR_TRANSPARENT;
    color[1] = TEXT_DYNAMIC_COLOR_6;
    color[2] = TEXT_COLOR_LIGHT_GRAY;

    AddTextPrinterParameterized4(0, FONT_NORMAL, left, top, 0, 0, color, TEXT_SKIP_DRAW, str);
}

#define tScrolling       data[0]
#define tMonSpriteDone   data[1]
#define tBgLoaded        data[2]
#define tSkipCry         data[3]
#define tMonSpriteId     data[4]
#define tTrainerSpriteId data[5]

static u8 LoadInfoScreen(struct PokedexListItem *item, u8 monSpriteId)
{
    u8 taskId;

    sPokedexListItem = item;
    taskId = CreateTask(Task_LoadInfoScreen, 0);
    gTasks[taskId].tScrolling = FALSE;
    gTasks[taskId].tMonSpriteDone = TRUE; // Already has sprite from list view
    gTasks[taskId].tBgLoaded = FALSE;
    gTasks[taskId].tSkipCry = FALSE;
    gTasks[taskId].tMonSpriteId = monSpriteId;
    gTasks[taskId].tTrainerSpriteId = SPRITE_NONE;
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sInfoScreen_BgTemplate, ARRAY_COUNT(sInfoScreen_BgTemplate));
    SetBgTilemapBuffer(3, AllocZeroed(BG_SCREEN_SIZE));
    SetBgTilemapBuffer(2, AllocZeroed(BG_SCREEN_SIZE));
    SetBgTilemapBuffer(1, AllocZeroed(BG_SCREEN_SIZE));
    SetBgTilemapBuffer(0, AllocZeroed(BG_SCREEN_SIZE));
    InitWindows(sInfoScreen_WindowTemplates);
    DeactivateAllTextPrinters();

    return taskId;
}

static bool8 IsInfoScreenScrolling(u8 taskId)
{
    if (!gTasks[taskId].tScrolling && gTasks[taskId].func == Task_HandleInfoScreenInput)
        return FALSE;
    else
        return TRUE;
}

static u8 StartInfoScreenScroll(struct PokedexListItem *item, u8 taskId)
{
    sPokedexListItem = item;
    gTasks[taskId].tScrolling = TRUE;
    gTasks[taskId].tMonSpriteDone = FALSE;
    gTasks[taskId].tBgLoaded = FALSE;
    gTasks[taskId].tSkipCry = FALSE;
    return taskId;
}

static void Task_LoadInfoScreen(u8 taskId)
{
    switch (gMain.state)
    {
    case 0:
    default:
        if (!gPaletteFade.active)
        {
            u16 r2;

            sPokedexView->currentPage = PAGE_INFO;
            gPokedexVBlankCB = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            r2 = 0;
            if (gTasks[taskId].tMonSpriteDone)
                r2 += DISPCNT_OBJ_ON;
            if (gTasks[taskId].tBgLoaded)
                r2 |= DISPCNT_BG1_ON;
            ResetOtherVideoRegisters(r2);
            SetBgAttribute(0, BG_ATTR_CHARBASEINDEX, 2);
            SetBgAttribute(2, BG_ATTR_CHARBASEINDEX, 2);
            SetBgAttribute(3, BG_ATTR_CHARBASEINDEX, 0);
            gMain.state = 1;
        }
        break;
    case 1:
        DecompressAndLoadBgGfxUsingHeap(3, gPokedexMenu_Gfx, 0x2000, 0, 0);
        CopyToBgTilemapBuffer(3, gPokedexInfoScreen_Tilemap, 0, 0);
        FillWindowPixelBuffer(WIN_INFO, PIXEL_FILL(0));
        PutWindowTilemap(WIN_INFO);
        PutWindowTilemap(WIN_FOOTPRINT);
        DrawFootprint(WIN_FOOTPRINT, sPokedexListItem->dexNum);
        CopyWindowToVram(WIN_FOOTPRINT, COPYWIN_GFX);
        gMain.state++;
        break;
    case 2:
        LoadScreenSelectBarMain(0xD);
        HighlightScreenSelectBarItem(sPokedexView->selectedScreen, 0xD);
        LoadPokedexBgPalette(sPokedexView->isSearchResults);
        gMain.state++;
        break;
    case 3:
        gMain.state++;
        break;
    case 4:
        {
            struct SpeciesInfo species = gSpeciesInfo[NationalPokedexNumToSpecies(sPokedexListItem->dexNum)];
            u8 types[2];
            types[0] = species.types[0];
            types[1] = species.types[1];

            PrintMonInfo(sPokedexListItem->dexNum, sPokedexView->dexMode == DEX_MODE_HOENN ? FALSE : TRUE, sPokedexListItem->owned, 0);
            
            LoadCompressedSpriteSheet(&sSpriteSheet_MoveTypes);                         // Load spritesheet for move type sprites
            LoadCompressedPalette(gMoveTypes_Pal, OBJ_PLTT_ID(12), 4 * PLTT_SIZE_4BPP); // Load palettes for move type sprites
            sPokedexView->monSpriteIds[1] = CreateSprite(&sSpriteTemplate_MoveTypes, 0, 0, 2);
            SetTypeSpritePosAndPal(types[0], 96, 57, sPokedexView->monSpriteIds[1]);
            if (types[1] == types[0])
            {
                SetTypeSpritePosAndPal(types[0], 96, 65, sPokedexView->monSpriteIds[1]);
            }
            else
            {
                sPokedexView->monSpriteIds[2] = CreateSprite(&sSpriteTemplate_MoveTypes, 0, 0, 2);
                SetTypeSpritePosAndPal(types[1], 96, 73, sPokedexView->monSpriteIds[2]);
            }

            
            if (!sPokedexListItem->owned)
                LoadPalette(&gPlttBufferUnfaded[BG_PLTT_ID(0) + 1], BG_PLTT_ID(3) + 1, PLTT_SIZEOF(16 - 1));
            CopyWindowToVram(WIN_INFO, COPYWIN_FULL);
            CopyBgTilemapBufferToVram(1);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(3);
            gMain.state++;
        }
        break;
    case 5:
        if (!gTasks[taskId].tMonSpriteDone)
        {
            gTasks[taskId].tMonSpriteId = (u16)CreateMonSpriteFromNationalDexNumber(sPokedexListItem->dexNum, MON_PAGE_X, MON_PAGE_Y, 0);
            gSprites[gTasks[taskId].tMonSpriteId].oam.priority = 0;
        }
        gMain.state++;
        break;
    case 6:
        {
            u32 preservedPalettes = 0;

            if (gTasks[taskId].tBgLoaded)
                preservedPalettes = 0x14; // each bit represents a palette index
            if (gTasks[taskId].tMonSpriteDone)
                preservedPalettes |= (1 << (gSprites[gTasks[taskId].tMonSpriteId].oam.paletteNum + 16));
            BeginNormalPaletteFade(~preservedPalettes, 0, 16, 0, RGB_BLACK);
            SetVBlankCallback(gPokedexVBlankCB);
            gMain.state++;
        }
        break;
    case 7:
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        HideBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        gMain.state++;
        break;
    case 8:
        if (!gPaletteFade.active)
        {
            gMain.state++;
            if (!gTasks[taskId].tSkipCry)
            {
                StopCryAndClearCrySongs();
                PlayCry_NormalNoDucking(NationalPokedexNumToSpecies(sPokedexListItem->dexNum), 0, CRY_VOLUME_RS, CRY_PRIORITY_NORMAL);
            }
            else
            {
                gMain.state++;
            }
        }
        break;
    case 9:
        if (!IsCryPlayingOrClearCrySongs())
            gMain.state++;
        break;
    case 10:
        gTasks[taskId].tScrolling = FALSE;
        gTasks[taskId].tMonSpriteDone = FALSE; // Reload next time screen comes up
        gTasks[taskId].tBgLoaded = TRUE;
        gTasks[taskId].tSkipCry = TRUE;
        gTasks[taskId].func = Task_HandleInfoScreenInput;
        gMain.state = 0;
        break;
    }
}

static void FreeInfoScreenWindowAndBgBuffers(void)
{
    void *tilemapBuffer;

    FreeAllWindowBuffers();
    tilemapBuffer = GetBgTilemapBuffer(0);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(1);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(2);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(3);
    if (tilemapBuffer)
        Free(tilemapBuffer);
}

static void Task_HandleInfoScreenInput(u8 taskId)
{
    if (gTasks[taskId].tScrolling)
    {
        // Scroll up/down
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_LoadInfoScreenWaitForFade;
        PlaySE(SE_DEX_SCROLL);
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_ExitInfoScreen;
        PlaySE(SE_PC_OFF);
        return;
    }
    if (JOY_NEW(A_BUTTON))
    {
        switch (sPokedexView->selectedScreen)
        {
        case AREA_SCREEN:
            BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 16, RGB_BLACK);
            sPokedexView->screenSwitchState = 3;
            gTasks[taskId].func = Task_SwitchScreensFromInfoScreen;
            PlaySE(SE_PIN);
            break;
        case MOVES_SCREEN:
            BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
            sPokedexView->screenSwitchState = 2;
            gTasks[taskId].func = Task_SwitchScreensFromInfoScreen;
            PlaySE(SE_PIN);
            break;
        case CRY_SCREEN:
            BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
            sPokedexView->screenSwitchState = 4;
            gTasks[taskId].func = Task_SwitchScreensFromInfoScreen;
            PlaySE(SE_PIN);
            break;
        case SIZE_SCREEN:
            BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
            sPokedexView->screenSwitchState = 1;
            gTasks[taskId].func = Task_SwitchScreensFromInfoScreen;
            PlaySE(SE_PIN);
            break;
        case CANCEL_SCREEN:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
            gTasks[taskId].func = Task_ExitInfoScreen;
            PlaySE(SE_PC_OFF);
            break;
        }
        return;
    }
    if ((JOY_NEW(DPAD_LEFT)
     || (JOY_NEW(L_BUTTON) && gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
     && sPokedexView->selectedScreen > 0)
    {
        sPokedexView->selectedScreen--;
        HighlightScreenSelectBarItem(sPokedexView->selectedScreen, 0xD);
        PlaySE(SE_DEX_PAGE);
        return;
    }
    if ((JOY_NEW(DPAD_RIGHT)
     || (JOY_NEW(R_BUTTON) && gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
     && sPokedexView->selectedScreen < CANCEL_SCREEN)
    {
        sPokedexView->selectedScreen++;
        HighlightScreenSelectBarItem(sPokedexView->selectedScreen, 0xD);
        PlaySE(SE_DEX_PAGE);
        return;
    }
}

static void Task_SwitchScreensFromInfoScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FreeAndDestroyMonPicSprite(gTasks[taskId].tMonSpriteId);
        switch (sPokedexView->screenSwitchState)
        {
        case 1:
        default:
            gTasks[taskId].func = Task_LoadSizeScreen;
            break;
        case 2:
            gTasks[taskId].func = Task_LoadMovesScreen;
            break;
        case 3:
            gTasks[taskId].func = Task_LoadAreaScreen;
            break;
        case 4:
            gTasks[taskId].func = Task_LoadCryScreen;
            break;
        }
    }
}

static void Task_LoadInfoScreenWaitForFade(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        TRY_FREE_AND_SET_NULL(sMovesView);
        FreeAndDestroyMonPicSprite(gTasks[taskId].tMonSpriteId);
        gTasks[taskId].func = Task_LoadInfoScreen;
    }
}

static void Task_ExitInfoScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        TRY_FREE_AND_SET_NULL(sMovesView);
        FreeAndDestroyMonPicSprite(gTasks[taskId].tMonSpriteId);
        FreeInfoScreenWindowAndBgBuffers();
        DestroyTask(taskId);
    }
}

static void Task_LoadAreaScreen(u8 taskId)
{
    switch (gMain.state)
    {
    case 0:
    default:
        if (!gPaletteFade.active)
        {
            sPokedexView->currentPage = PAGE_AREA;
            gPokedexVBlankCB = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            ResetOtherVideoRegisters(DISPCNT_BG1_ON);
            sPokedexView->selectedScreen = AREA_SCREEN;
            SetBgAttribute(0, BG_ATTR_CHARBASEINDEX, 2);
            SetBgAttribute(2, BG_ATTR_CHARBASEINDEX, 2);
            SetBgAttribute(3, BG_ATTR_CHARBASEINDEX, 0);
            gMain.state = 1;
        }
        break;
    case 1:
        LoadScreenSelectBarSubmenu(0xD);
        HighlightSubmenuScreenSelectBarItem(AREA_SCREEN, 0xD);
        LoadPokedexBgPalette(sPokedexView->isSearchResults);
        SetGpuReg(REG_OFFSET_BG1CNT, BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(13) | BGCNT_16COLOR | BGCNT_TXT256x256);
        gMain.state++;
        break;
    case 2:
        ShowPokedexAreaScreen(NationalPokedexNumToSpecies(sPokedexListItem->dexNum), &sPokedexView->screenSwitchState);
        SetVBlankCallback(gPokedexVBlankCB);
        sPokedexView->screenSwitchState = 0;
        gMain.state = 0;
        gTasks[taskId].func = Task_WaitForAreaScreenInput;
        break;
    }
}

static void Task_WaitForAreaScreenInput(u8 taskId)
{
// See Task_HandlePokedexAreaScreenInput() in pokedex_area_screen.c
    if (sPokedexView->screenSwitchState == 4)
    {
        sPokedexListItem = &sPokedexView->pokedexList[sPokedexView->selectedPokemon];
        gTasks[taskId].func = Task_SwitchScreensFromAreaScreen;
    }
    else if (sPokedexView->screenSwitchState != 0)
        gTasks[taskId].func = Task_SwitchScreensFromAreaScreen;
}

static void Task_SwitchScreensFromAreaScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        switch (sPokedexView->screenSwitchState)
        {
        case 1:
        default:
            gTasks[taskId].func = Task_LoadInfoScreen;
            break;
        case 2:
            gTasks[taskId].func = Task_LoadMovesScreen;
            break;
        case 3:
            gTasks[taskId].func = Task_LoadCryScreen;
            break;
        case 4:
            gTasks[taskId].func = Task_LoadAreaScreen;
            break;
        }
    }
}

static void Task_LoadMovesScreen(u8 taskId)
{
    switch (gMain.state)
    {
    default:
    case 0:
        if (!gPaletteFade.active)
        {
            sPokedexView->currentPage = PAGE_MOVES;                                 // Set current page to Moves page
            sPokedexView->selectedScreen = MOVES_SCREEN;                            // Set selected screen to Moves screen

            gPokedexVBlankCB = gMain.vblankCallback;                                // Save current vblank callback
            SetVBlankCallback(NULL);                                                // Set vblank callback to NULL

            ResetOtherVideoRegisters(DISPCNT_BG1_ON);                               // Reset video registers, retain screen select bar
            FillBgTilemapBufferRect(3, 0, 0, 0, 32, 32, 0);                         // Clear bg3

            SetBgAttribute(0, BG_ATTR_CHARBASEINDEX, 0);
            SetBgAttribute(2, BG_ATTR_CHARBASEINDEX, 0);
            SetBgAttribute(3, BG_ATTR_CHARBASEINDEX, 2);

            gMain.state = 1;
        }
        break;
    case 1:
        DecompressAndLoadBgGfxUsingHeap(2, gPokedexMenu_Gfx, 0x2000, 0, 0);         // Load Pokedex menu tileset
        CopyToBgTilemapBuffer(0, gPokedexMovesUnderlay_Tilemap, 0, 0);              // Copy Moves Underlay tilemap to bg0
        CopyToBgTilemapBuffer(2, gPokedexMovesScreen_Tilemap, 0, 0);                // Copy Moves Screen tilemap to bg2

        PutWindowTilemap(WIN_MOVES);
        PutWindowTilemap(WIN_MOVES_BATTLE_LABELS);
        PutWindowTilemap(WIN_MOVES_BATTLE_VALUES);
        PutWindowTilemap(WIN_MOVES_DESC);

        gMain.state++;
        break;
    case 2:
        LoadScreenSelectBarSubmenu(0xD);                                            // Load screen select bar
        HighlightSubmenuScreenSelectBarItem(MOVES_SCREEN, 0xD);                     // Highlight Moves screen
        LoadPokedexBgPalette(sPokedexView->isSearchResults);
        gMain.state++;
        break;
    case 3:
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
        LoadCompressedSpriteSheet(&sInterfaceSpriteSheet[0]);                       // Load spritesheet for scrollbar sprite
        LoadSpritePalettes(sInterfaceSpritePalette);                                // Load palette for scrollbar sprite
        LoadCompressedSpriteSheet(&sSpriteSheet_MoveTypes);                         // Load spritesheet for move type sprites
        LoadCompressedPalette(gMoveTypes_Pal, OBJ_PLTT_ID(12), 4 * PLTT_SIZE_4BPP); // Load palettes for move type sprites
        gMain.state++;
        break;
    case 4:
        sMovesView = AllocZeroed(sizeof(struct MovesView));
        CreateMovesList();
        ResetSpriteIds();
        CreateMoveTypeIcons();
        SetMoveTypeIcons();
        CreateSprite(&sScrollBarSpriteTemplate, 230, 28, 0);
        CreateMoveSpritesAtPos(sMovesView->selectedMove, 0xE);
        gMain.state++;
        break;
    case 5:
        ResetPaletteFade();
        gMain.state++;
        break;
    case 6:
        {
            CopyBgTilemapBufferToVram(0);
            CopyBgTilemapBufferToVram(1);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(3);
        }
        gMain.state++;
        break;
    case 7:
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_Pokedex);
        gMain.state++;
        break;
    case 8:
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        gMain.state++;
        break;
    case 9:
        if (!gPaletteFade.active)
        {
            sPokedexView->screenSwitchState = 0;
            gMain.state = 0;
            gTasks[taskId].func = Task_HandleMovesScreenInput;        
        }
        break;
    }
}

static void Task_HandleMovesScreenInput(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 1;
        gTasks[taskId].func = Task_SwitchScreensFromMovesScreen;
        PlaySE(SE_PC_OFF);
        return;
    }
    if (JOY_NEW(DPAD_LEFT)
     || (JOY_NEW(L_BUTTON) && gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 2;
        gTasks[taskId].func = Task_SwitchScreensFromMovesScreen;
        PlaySE(SE_DEX_PAGE);
        return;
    }
    if (JOY_NEW(DPAD_RIGHT)
     || (JOY_NEW(R_BUTTON) && gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 3;
        gTasks[taskId].func = Task_SwitchScreensFromMovesScreen;
        PlaySE(SE_DEX_PAGE);
        return;
    }
    if (JOY_NEW(A_BUTTON))
    {
        LoadScreenSelectBarSubmenu(0xD);
        HighlightSubmenuScreenSelectBarItem(5, 0xD);
        gTasks[taskId].func = Task_HandleMovesScreenInput2;
        PlaySE(SE_DEX_PAGE);
        return;
    }
    else if (TryDoInfoScreenScroll())
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 4;
        sPokedexListItem = &sPokedexView->pokedexList[sPokedexView->selectedPokemon];
        gTasks[taskId].func = Task_SwitchScreensFromMovesScreen;
        PlaySE(SE_DEX_SCROLL);
    }

}

static void Task_HandleMovesScreenInput2(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        HighlightSubmenuScreenSelectBarItem(MOVES_SCREEN, 0xD);
        gTasks[taskId].func = Task_HandleMovesScreenInput;
        PlaySE(SE_DEX_PAGE);
        return;
    }
    else if (JOY_NEW(L_BUTTON) && gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR)
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 2;
        gTasks[taskId].func = Task_SwitchScreensFromMovesScreen;
        PlaySE(SE_DEX_PAGE);
        return;
    }
    else if (JOY_NEW(R_BUTTON) && gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR)
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 3;
        gTasks[taskId].func = Task_SwitchScreensFromMovesScreen;
        PlaySE(SE_DEX_PAGE);
        return;
    }
    else
    {
        sMovesView->selectedMove = TryDoMovesScroll(sMovesView->selectedMove, 0xE, taskId);
        if (sMovesView->scrollTimer)
            gTasks[taskId].func = Task_WaitForMovesScroll;
    }
}

static void Task_WaitForMovesScroll(u8 taskId)
{
    if (UpdateMovesListScroll(sMovesView->scrollDirection, sMovesView->scrollMovesIncrement, sMovesView->maxScrollTimer))
        gTasks[taskId].func = Task_HandleMovesScreenInput2;
}

static void Task_SwitchScreensFromMovesScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FillWindowPixelBuffer(WIN_MOVES_BATTLE_LABELS, PIXEL_FILL(0));
        FillWindowPixelBuffer(WIN_MOVES_BATTLE_VALUES, PIXEL_FILL(0));
        FillWindowPixelBuffer(WIN_MOVES_DESC, PIXEL_FILL(0));
        CopyWindowToVram(WIN_MOVES_BATTLE_LABELS, COPYWIN_FULL);
        CopyWindowToVram(WIN_MOVES_BATTLE_VALUES, COPYWIN_FULL);
        CopyWindowToVram(WIN_MOVES_DESC, COPYWIN_FULL);

        switch (sPokedexView->screenSwitchState)
        {
        default:
        case 1:
            gTasks[taskId].func = Task_LoadInfoScreen;
            break;
        case 2:
            gTasks[taskId].func = Task_LoadSizeScreen;
            break;
        case 3:
            gTasks[taskId].func = Task_LoadAreaScreen;
            break;
        case 4:
            gTasks[taskId].func = Task_LoadMovesScreen;
            break;
        }
    }
}

static void CreateMoveSpritesAtPos(u16 selectedMove, u16 ignored)
{
    gPaletteFade.bufferTransferDisabled = TRUE;

    CreateMoveListEntry(0, selectedMove, ignored);
    SetGpuReg(REG_OFFSET_BG3VOFS, sMovesView->initialVOffset);

    sMovesView->listVOffset = 0;
    sMovesView->listMovingVOffset = 0;

    gPaletteFade.bufferTransferDisabled = FALSE;
}

static void CreateMovesList(void)
{
    s32 i;
    u32 species = NationalPokedexNumToSpecies(sPokedexListItem->dexNum);
    u32 pre_species = species, j, moveExists;
    u32 numMoves = 0;
    const u16 *learnset = gLevelUpLearnsets[species];
    struct MovesListItem *moves = sMovesView->movesList;
    u32 moveType = MOVE_LEVEL_UP;

    // Level up moves
    for (i = 0; i < MAX_LEVEL_UP_MOVES; i++)
    {
        if (gLevelUpLearnsets[species][i] == LEVEL_UP_END)
        {
            i = 0;
            species = GetPreEvolution(species);
            if (species == SPECIES_NONE)
                break;
            pre_species = species;
            learnset = gLevelUpLearnsets[species];
            moveType = MOVE_PRE_EVOLUTION;
        }

        moveExists = FALSE;
        if (moveType == MOVE_PRE_EVOLUTION)
        {
            for (j = 0; j < numMoves; j++)
            {
                if (moves[j].move == (learnset[i] & LEVEL_UP_MOVE_ID))
                {
                    moveExists = TRUE;
                    break;
                }
            }
        }

        if (!moveExists) {
            moves[numMoves].type = moveType;
            moves[numMoves].move = learnset[i] & LEVEL_UP_MOVE_ID;
            moves[numMoves++].index = (learnset[i] & LEVEL_UP_MOVE_LV) >> 9;
        }
    }

    species = NationalPokedexNumToSpecies(sPokedexListItem->dexNum);

    // TMs
    for (i = 0; i <= ITEM_HM08 - ITEM_TM01; i++)
    {
        if (CanSpeciesLearnTMHM(species, i))
        {
            if (i < ITEM_HM01 - ITEM_TM01)
                moves[numMoves].type = MOVE_TM;
            else
                moves[numMoves].type = MOVE_HM;
            moves[numMoves].move = sTMHMMoves[i];
            moves[numMoves++].index = i + 1;
        }
    }

    // Tutor moves
    for (i = 0; i < TUTOR_MOVE_COUNT; i++)
    {
        if (sTutorLearnsets[species] & (1 << i))
        {
            moves[numMoves].type = MOVE_TUTOR;
            moves[numMoves++].move = gTutorMoves[i];
        }
    }

    // Egg moves
    i = FindSpeciesInEggMoves(pre_species);
    if (i != -1)
    {
        i++; // Skip the species value
        while (gEggMoves[i] <= EGG_MOVES_SPECIES_OFFSET)
        {
            moves[numMoves].type = MOVE_EGG;
            moves[numMoves++].move = gEggMoves[i++];
        }
    }

    sMovesView->movesListCount = numMoves;
}

// Helper function to find a species in the egg moves list
static s32 FindSpeciesInEggMoves(u16 species)
{
    s32 i;

    for (i = 0; i < gEggMovesCount; i++)
    {
        if (gEggMoves[i] == species + EGG_MOVES_SPECIES_OFFSET)
            return i;
    }
    return -1;
}

static u16 GetPreEvolution(u16 species)
{
    int i, j;

    for (i = 1; i < NUM_SPECIES; i++)
    {
        for (j = 0; j < EVOS_PER_MON; j++)
        {
            if (gEvolutionTable[i][j].targetSpecies == species)
            {
                return i;
            }
        }
    }
    return SPECIES_NONE;
}

static void Task_LoadCryScreen(u8 taskId)
{
    switch (gMain.state)
    {
    case 0:
    default:
        if (!gPaletteFade.active)
        {
            m4aMPlayStop(&gMPlayInfo_BGM);
            sPokedexView->currentPage = PAGE_CRY;
            gPokedexVBlankCB = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            ResetOtherVideoRegisters(DISPCNT_BG1_ON);
            sPokedexView->selectedScreen = CRY_SCREEN;
            SetBgAttribute(0, BG_ATTR_CHARBASEINDEX, 2);
            SetBgAttribute(2, BG_ATTR_CHARBASEINDEX, 2);
            SetBgAttribute(3, BG_ATTR_CHARBASEINDEX, 0);
            gMain.state = 1;
        }
        break;
    case 1:
        DecompressAndLoadBgGfxUsingHeap(3, &gPokedexMenu_Gfx, 0x2000, 0, 0);
        CopyToBgTilemapBuffer(3, &gPokedexCryScreen_Tilemap, 0, 0);
        FillWindowPixelBuffer(WIN_INFO, PIXEL_FILL(0));
        PutWindowTilemap(WIN_INFO);
        PutWindowTilemap(WIN_VU_METER);
        PutWindowTilemap(WIN_CRY_WAVE);
        gMain.state++;
        break;
    case 2:
        FillBgTilemapBufferRect(1, 0, 0, 0, 32, 32, 0);
        LoadScreenSelectBarSubmenu(0xD);
        HighlightSubmenuScreenSelectBarItem(CRY_SCREEN, 0xD);
        LoadPokedexBgPalette(sPokedexView->isSearchResults);
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        gMain.state++;
        break;
    case 4:
        PrintInfoScreenText(gText_CryOf, 82, 33);
        PrintCryScreenSpeciesName(0, sPokedexListItem->dexNum, 82, 49);
        gMain.state++;
        break;
    case 5:
        gTasks[taskId].tMonSpriteId = CreateMonSpriteFromNationalDexNumber(sPokedexListItem->dexNum, MON_PAGE_X - 4, MON_PAGE_Y, 0);
        gSprites[gTasks[taskId].tMonSpriteId].oam.priority = 0;
        gDexCryScreenState = 0;
        gMain.state++;
        break;
    case 6:
        {
            struct CryScreenWindow waveformWindow;

            waveformWindow.unk0 = 0x4020;
            waveformWindow.unk2 = 31;
            waveformWindow.paletteNo = 8;
            waveformWindow.yPos = 30;
            waveformWindow.xPos = 12;
            if (LoadCryWaveformWindow(&waveformWindow, 2))
            {
                gMain.state++;
                gDexCryScreenState = 0;
            }
        }
        break;
    case 7:
        {
            struct CryScreenWindow cryMeter;

            cryMeter.paletteNo = 9;
            cryMeter.xPos = 18;
            cryMeter.yPos = 3;
            if (LoadCryMeter(&cryMeter, 3))
                gMain.state++;
            CopyWindowToVram(WIN_VU_METER, COPYWIN_GFX);
            CopyWindowToVram(WIN_INFO, COPYWIN_FULL);
            CopyBgTilemapBufferToVram(0);
            CopyBgTilemapBufferToVram(1);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(3);
        }
        break;
    case 8:
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(gPokedexVBlankCB);
        gMain.state++;
        break;
    case 9:
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        ShowBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        gMain.state++;
        break;
    case 10:
        sPokedexView->screenSwitchState = 0;
        gMain.state = 0;
        gTasks[taskId].func = Task_HandleCryScreenInput;
        break;
    }
}

static void Task_HandleCryScreenInput(u8 taskId)
{
    UpdateCryWaveformWindow(2);

    if (IsCryPlaying())
        LoadPlayArrowPalette(TRUE);
    else
        LoadPlayArrowPalette(FALSE);

    if (JOY_NEW(A_BUTTON))
    {
        LoadPlayArrowPalette(TRUE);
        CryScreenPlayButton(NationalPokedexNumToSpecies(sPokedexListItem->dexNum));
        return;
    }
    else if (!gPaletteFade.active)
    {
        if (JOY_NEW(B_BUTTON))
        {
            BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
            m4aMPlayContinue(&gMPlayInfo_BGM);
            sPokedexView->screenSwitchState = 1;
            gTasks[taskId].func = Task_SwitchScreensFromCryScreen;
            PlaySE(SE_PC_OFF);
            return;
        }
        if (JOY_NEW(DPAD_LEFT)
         || (JOY_NEW(L_BUTTON) && gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
        {
            BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
            m4aMPlayContinue(&gMPlayInfo_BGM);
            sPokedexView->screenSwitchState = 2;
            gTasks[taskId].func = Task_SwitchScreensFromCryScreen;
            PlaySE(SE_DEX_PAGE);
            return;
        }
        else if (TryDoInfoScreenScroll())
        {
            BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
            sPokedexView->screenSwitchState = 3;
            sPokedexListItem = &sPokedexView->pokedexList[sPokedexView->selectedPokemon];
            gTasks[taskId].func = Task_SwitchScreensFromCryScreen;
            PlaySE(SE_DEX_SCROLL);
        }
    }
}

static void Task_SwitchScreensFromCryScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FreeCryScreen();
        FreeAndDestroyMonPicSprite(gTasks[taskId].tMonSpriteId);
        switch (sPokedexView->screenSwitchState)
        {
        default:
        case 1:
            gTasks[taskId].func = Task_LoadInfoScreen;
            break;
        case 2:
            gTasks[taskId].func = Task_LoadAreaScreen;
            break;
        case 3:
            gTasks[taskId].func = Task_LoadCryScreen;
            break;
        }
    }
}

static void LoadPlayArrowPalette(bool8 cryPlaying)
{
    u16 color;

    if (cryPlaying)
        color = RGB(18, 28, 0);
    else
        color = RGB(15, 21, 0);
    LoadPalette(&color, BG_PLTT_ID(5) + 13, PLTT_SIZEOF(1));
}

static void Task_LoadSizeScreen(u8 taskId)
{
    u8 spriteId;

    switch (gMain.state)
    {
    default:
    case 0:
        if (!gPaletteFade.active)
        {
            sPokedexView->currentPage = PAGE_SIZE;
            gPokedexVBlankCB = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            ResetOtherVideoRegisters(DISPCNT_BG1_ON);
            sPokedexView->selectedScreen = SIZE_SCREEN;
            SetBgAttribute(0, BG_ATTR_CHARBASEINDEX, 2);
            SetBgAttribute(2, BG_ATTR_CHARBASEINDEX, 2);
            SetBgAttribute(3, BG_ATTR_CHARBASEINDEX, 0);
            gMain.state = 1;
        }
        break;
    case 1:
        DecompressAndLoadBgGfxUsingHeap(3, gPokedexMenu_Gfx, 0x2000, 0, 0);
        CopyToBgTilemapBuffer(3, gPokedexSizeScreen_Tilemap, 0, 0);
        FillWindowPixelBuffer(WIN_INFO, PIXEL_FILL(0));
        PutWindowTilemap(WIN_INFO);
        gMain.state++;
        break;
    case 2:
        LoadScreenSelectBarSubmenu(0xD);
        HighlightSubmenuScreenSelectBarItem(SIZE_SCREEN, 0xD);
        LoadPokedexBgPalette(sPokedexView->isSearchResults);
        gMain.state++;
        break;
    case 3:
        {
            struct SpeciesInfo species = gSpeciesInfo[NationalPokedexNumToSpecies(sPokedexListItem->dexNum)];
            u8 abilities[2], eggGroups[2];

            DrawEVYield(species, GetBgTilemapBuffer(3));

            abilities[0] = species.abilities[0];
            abilities[1] = species.abilities[1];
            eggGroups[0] = species.eggGroups[0];
            eggGroups[1] = species.eggGroups[1];

            AddTextPrinterParameterized4(0, FONT_SMALL_NARROW, 120, 109, 0, 0, (u8[]){TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_LIGHT_GRAY}, TEXT_SKIP_DRAW, gText_50Percent);
            AddTextPrinterParameterized4(0, FONT_SMALL_NARROW, 152, 109, 0, 0, (u8[]){TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_6, TEXT_COLOR_LIGHT_GRAY}, TEXT_SKIP_DRAW, gText_5Percent);

            PrintInfoScreenText(gText_HP3, 25, 24);
            PrintInfoScreenText(gText_Attack3, 13, 40);
            PrintInfoScreenText(gText_Defense3, 10, 56);
            PrintInfoScreenText(gText_SpAtk3, 13, 72);
            PrintInfoScreenText(gText_SpDef3, 13, 88);
            PrintInfoScreenText(gText_Speed2, 16, 104);

            ConvertIntToDecimalStringN(gStringVar1, species.baseHP, STR_CONV_MODE_RIGHT_ALIGN, 3);
            PrintInfoScreenText(gStringVar1, 60, 24);
            ConvertIntToDecimalStringN(gStringVar1, species.baseAttack, STR_CONV_MODE_RIGHT_ALIGN, 3);
            PrintInfoScreenText(gStringVar1, 60, 40);
            ConvertIntToDecimalStringN(gStringVar1, species.baseDefense, STR_CONV_MODE_RIGHT_ALIGN, 3);
            PrintInfoScreenText(gStringVar1, 60, 56);
            ConvertIntToDecimalStringN(gStringVar1, species.baseSpAttack, STR_CONV_MODE_RIGHT_ALIGN, 3);
            PrintInfoScreenText(gStringVar1, 60, 72);
            ConvertIntToDecimalStringN(gStringVar1, species.baseSpDefense, STR_CONV_MODE_RIGHT_ALIGN, 3);
            PrintInfoScreenText(gStringVar1, 60, 88);
            ConvertIntToDecimalStringN(gStringVar1, species.baseSpeed, STR_CONV_MODE_RIGHT_ALIGN, 3);
            PrintInfoScreenText(gStringVar1, 60, 104);

            PrintInfoScreenText(gAbilityNames[abilities[0]], 8, 129);
            PrintInfoScreenText(gAbilityDescriptionPointers[abilities[0]], 88, 129);
            if (abilities[1] != 0 && abilities[0] != abilities[1])
            {
                PrintInfoScreenText(gAbilityNames[abilities[1]], 8, 144);
                PrintInfoScreenText(gAbilityDescriptionPointers[abilities[1]], 88, 144);
            }

            PrintInfoScreenText(gEggGroupNames[eggGroups[0]], 170 + GetStringCenterAlignXOffset(FONT_NORMAL, gEggGroupNames[eggGroups[0]], 60), 89);
            if (eggGroups[1] != 0 && eggGroups[0] != eggGroups[1])
                PrintInfoScreenText(gEggGroupNames[eggGroups[1]], 170 + GetStringCenterAlignXOffset(FONT_NORMAL, gEggGroupNames[eggGroups[1]], 60), 104);

            if (species.itemCommon != ITEM_NONE)
            {
                sPokedexView->monSpriteIds[2] = AddItemIconSprite(5110, 5110, species.itemCommon);
                gSprites[sPokedexView->monSpriteIds[2]].callback = SpriteCallbackDummy;
                gSprites[sPokedexView->monSpriteIds[2]].oam.priority = 0;
                gSprites[sPokedexView->monSpriteIds[2]].x2 = 116;
                gSprites[sPokedexView->monSpriteIds[2]].y2 = 108;
            }
            if (species.itemRare != ITEM_NONE)
            {
                sPokedexView->monSpriteIds[3] = AddItemIconSprite(5110, 5110, species.itemRare);
                gSprites[sPokedexView->monSpriteIds[3]].callback = SpriteCallbackDummy;
                gSprites[sPokedexView->monSpriteIds[3]].oam.priority = 0;
                gSprites[sPokedexView->monSpriteIds[3]].x2 = 148;
                gSprites[sPokedexView->monSpriteIds[3]].y2 = 108;
            }
            gMain.state++;
        }
        break;
    case 4:
        ResetPaletteFade();
        gMain.state++;
        break;
    case 5:
        spriteId = CreateSizeScreenTrainerPic(PlayerGenderToFrontTrainerPicId(gSaveBlock2Ptr->playerGender), 200, 56, 0);
        gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
        gSprites[spriteId].oam.matrixNum = 1;
        gSprites[spriteId].oam.priority = 0;
        gSprites[spriteId].y2 = gPokedexEntries[sPokedexListItem->dexNum].trainerOffset;
        SetOamMatrix(1, gPokedexEntries[sPokedexListItem->dexNum].trainerScale, 0, 0, gPokedexEntries[sPokedexListItem->dexNum].trainerScale);
        LoadPalette(sSizeScreenSilhouette_Pal, OBJ_PLTT_ID2(gSprites[spriteId].oam.paletteNum), PLTT_SIZE_4BPP);
        gTasks[taskId].tTrainerSpriteId = spriteId;
        gMain.state++;
        break;
    case 6:
        spriteId = CreateMonSpriteFromNationalDexNumber(sPokedexListItem->dexNum, 136, 56, 1);
        gSprites[spriteId].oam.affineMode = ST_OAM_AFFINE_NORMAL;
        gSprites[spriteId].oam.matrixNum = 2;
        gSprites[spriteId].oam.priority = 0;
        gSprites[spriteId].y2 = gPokedexEntries[sPokedexListItem->dexNum].pokemonOffset;
        SetOamMatrix(2, gPokedexEntries[sPokedexListItem->dexNum].pokemonScale, 0, 0, gPokedexEntries[sPokedexListItem->dexNum].pokemonScale);
        LoadPalette(sSizeScreenSilhouette_Pal, OBJ_PLTT_ID2(gSprites[spriteId].oam.paletteNum), PLTT_SIZE_4BPP);
        gTasks[taskId].tMonSpriteId = spriteId;
        CopyWindowToVram(WIN_INFO, COPYWIN_FULL);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(3);
        gMain.state++;
        break;
    case 7:
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(gPokedexVBlankCB);
        gMain.state++;
        break;
    case 8:
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        HideBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        gMain.state++;
        break;
    case 9:
        if (!gPaletteFade.active)
        {
            sPokedexView->screenSwitchState = 0;
            gMain.state = 0;
            gTasks[taskId].func = Task_HandleSizeScreenInput;
        }
        break;
    }
}

static void DrawEVYield(struct SpeciesInfo species, u16 *tilemapBuffer)
{
    u32 i, j;
    u16 evYields[] = 
    {
        species.evYield_HP,
        species.evYield_Attack,
        species.evYield_Defense,
        species.evYield_SpAttack,
        species.evYield_SpDefense,
        species.evYield_Speed
    };

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < evYields[i]; j++)
        {
            tilemapBuffer[103 + (64 * i) + j] = 0x502F;
            tilemapBuffer[135 + (64 * i) + j] = 0x503F;
        }
    }
}

static void Task_HandleSizeScreenInput(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 1;
        gTasks[taskId].func = Task_SwitchScreensFromSizeScreen;
        PlaySE(SE_PC_OFF);
    }
    else if (JOY_NEW(DPAD_RIGHT)
     || (JOY_NEW(R_BUTTON) && gSaveBlock2Ptr->optionsButtonMode == OPTIONS_BUTTON_MODE_LR))
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 2;
        gTasks[taskId].func = Task_SwitchScreensFromSizeScreen;
        PlaySE(SE_DEX_PAGE);
    }
    else if (TryDoInfoScreenScroll())
    {
        BeginNormalPaletteFade(PALETTES_ALL & ~(0x14), 0, 0, 0x10, RGB_BLACK);
        sPokedexView->screenSwitchState = 3;
        sPokedexListItem = &sPokedexView->pokedexList[sPokedexView->selectedPokemon];
        gTasks[taskId].func = Task_SwitchScreensFromSizeScreen;
        PlaySE(SE_DEX_SCROLL);
    }
}

static void Task_SwitchScreensFromSizeScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FreeAndDestroyMonPicSprite(gTasks[taskId].tMonSpriteId);
        FreeAndDestroyTrainerPicSprite(gTasks[taskId].tTrainerSpriteId);
        switch (sPokedexView->screenSwitchState)
        {
        default:
        case 1:
            gTasks[taskId].func = Task_LoadInfoScreen;
            break;
        case 2:
            gTasks[taskId].func = Task_LoadMovesScreen;
            break;
        case 3:
            gTasks[taskId].func = Task_LoadSizeScreen;
            break;
        }
    }
}

#undef tScrolling
#undef tMonSpriteDone
#undef tBgLoaded
#undef tSkipCry
#undef tMonSpriteId
#undef tTrainerSpriteId

static void LoadScreenSelectBarMain(u16 unused)
{
    CopyToBgTilemapBuffer(1, gPokedexScreenSelectBarMain_Tilemap, 0, 0);
}

static void LoadScreenSelectBarSubmenu(u16 unused)
{
    CopyToBgTilemapBuffer(1, gPokedexScreenSelectBarSubmenu_Tilemap, 0, 0);
}

static void HighlightScreenSelectBarItem(u8 selectedScreen, u16 unused)
{
    u32 i;
    u32 j;
    u16 *ptr = GetBgTilemapBuffer(1);
    u32 screenWidths[] = {5, 6, 5, 5, 7};
    u32 row = 1;

    for (i = 0; i < SCREEN_COUNT; i++)
    {
        u16 newPalette;

        do
        {
            newPalette = 0x4000;
            if (i == selectedScreen)
                newPalette = 0x2000;
        } while (0);

        for (j = 0; j < screenWidths[i]; j++)
        {
            ptr[row + j] = (ptr[row + j] % 0x1000) | newPalette;
            ptr[row + j + 0x20] = (ptr[row + j + 0x20] % 0x1000) | newPalette;
        }

        row += screenWidths[i];
    }
    CopyBgTilemapBufferToVram(1);
}

static void HighlightSubmenuScreenSelectBarItem(u8 selectedScreen, u16 b)
{
    u32 i;
    u32 j;
    u16 *ptr = GetBgTilemapBuffer(1);
    u32 screenWidths[] = {5, 6, 5, 5, 7};
    u32 row = 1;

    for (i = 0; i < SCREEN_COUNT; i++)
    {
        u32 newPalette;

        do
        {
            if (i == selectedScreen || i == 4)
                newPalette = 0x2000;
            else
                newPalette = 0x4000;
        } while (0);

        for (j = 0; j < screenWidths[i]; j++)
        {
            ptr[row + j] = (ptr[row + j] % 0x1000) | newPalette;
            ptr[row + j + 0x20] = (ptr[row + j + 0x20] % 0x1000) | newPalette;
        }

        row += screenWidths[i];
    }
    CopyBgTilemapBufferToVram(1);
}

#define tState         data[0]
#define tDexNum        data[1]
#define tPalTimer      data[2]
#define tMonSpriteId   data[3]
#define tOtIdLo        data[12]
#define tOtIdHi        data[13]
#define tPersonalityLo data[14]
#define tPersonalityHi data[15]

u8 DisplayCaughtMonDexPage(u16 dexNum, u32 otId, u32 personality)
{
    u8 taskId = CreateTask(Task_DisplayCaughtMonDexPage, 0);

    gTasks[taskId].tState = 0;
    gTasks[taskId].tDexNum = dexNum;
    gTasks[taskId].tOtIdLo = otId;
    gTasks[taskId].tOtIdHi = otId >> 16;
    gTasks[taskId].tPersonalityLo = personality;
    gTasks[taskId].tPersonalityHi = personality >> 16;
    return taskId;
}

static void Task_DisplayCaughtMonDexPage(u8 taskId)
{
    u8 spriteId;
    u16 dexNum = gTasks[taskId].tDexNum;

    switch (gTasks[taskId].tState)
    {
    case 0:
    default:
        if (!gPaletteFade.active)
        {
            gPokedexVBlankCB = gMain.vblankCallback;
            SetVBlankCallback(NULL);
            ResetOtherVideoRegisters(DISPCNT_BG0_ON);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sNewEntryInfoScreen_BgTemplate, ARRAY_COUNT(sNewEntryInfoScreen_BgTemplate));
            SetBgTilemapBuffer(3, AllocZeroed(BG_SCREEN_SIZE));
            SetBgTilemapBuffer(2, AllocZeroed(BG_SCREEN_SIZE));
            InitWindows(sNewEntryInfoScreen_WindowTemplates);
            DeactivateAllTextPrinters();
            gTasks[taskId].tState = 1;
        }
        break;
    case 1:
        DecompressAndLoadBgGfxUsingHeap(3, gPokedexMenu_Gfx, 0x2000, 0, 0);
        CopyToBgTilemapBuffer(3, gPokedexInfoScreen_Tilemap, 0, 0);
        FillWindowPixelBuffer(WIN_INFO, PIXEL_FILL(0));
        PutWindowTilemap(WIN_INFO);
        PutWindowTilemap(WIN_FOOTPRINT);
        DrawFootprint(WIN_FOOTPRINT, gTasks[taskId].tDexNum);
        CopyWindowToVram(WIN_FOOTPRINT, COPYWIN_GFX);
        ResetPaletteFade();
        LoadPokedexBgPalette(FALSE);
        gTasks[taskId].tState++;
        break;
    case 2:
        gTasks[taskId].tState++;
        break;
    case 3:
        {
            struct SpeciesInfo species = gSpeciesInfo[NationalPokedexNumToSpecies(dexNum)];
            u8 types[2];
            types[0] = species.types[0];
            types[1] = species.types[1];

            PrintMonInfo(dexNum, IsNationalPokedexEnabled(), 1, 1);

            LoadCompressedSpriteSheet(&sSpriteSheet_MoveTypes);                         // Load spritesheet for move type sprites
            LoadCompressedPalette(gMoveTypes_Pal, OBJ_PLTT_ID(12), 4 * PLTT_SIZE_4BPP); // Load palettes for move type sprites
            spriteId = CreateSprite(&sSpriteTemplate_MoveTypes, 0, 0, 2);
            SetTypeSpritePosAndPal(types[0], 96, 57, spriteId);
            if (types[1] == types[0])
            {
                SetTypeSpritePosAndPal(types[0], 96, 65, spriteId);
            }
            else
            {
                spriteId = CreateSprite(&sSpriteTemplate_MoveTypes, 0, 0, 2);
                SetTypeSpritePosAndPal(types[1], 96, 73, spriteId);
            }

            CopyWindowToVram(WIN_INFO, COPYWIN_FULL);
            CopyBgTilemapBufferToVram(2);
            CopyBgTilemapBufferToVram(3);
            gTasks[taskId].tState++;
        }
        break;
    case 4:
        spriteId = CreateMonSpriteFromNationalDexNumber(dexNum, MON_PAGE_X, MON_PAGE_Y, 0);
        gSprites[spriteId].oam.priority = 0;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK);
        SetVBlankCallback(gPokedexVBlankCB);
        gTasks[taskId].tMonSpriteId = spriteId;
        gTasks[taskId].tState++;
        break;
    case 5:
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        ShowBg(2);
        ShowBg(3);
        gTasks[taskId].tState++;
        break;
    case 6:
        if (!gPaletteFade.active)
        {
            PlayCry_Normal(NationalPokedexNumToSpecies(dexNum), 0);
            gTasks[taskId].tPalTimer = 0;
            gTasks[taskId].func = Task_HandleCaughtMonPageInput;
        }
        break;
    }
}

static void Task_HandleCaughtMonPageInput(u8 taskId)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        //BeginNormalPaletteFade(PALETTES_BG, 0, 0, 16, RGB_BLACK);
        BeginNormalPaletteFade(~(1 << (gSprites[gTasks[taskId].tMonSpriteId].oam.paletteNum + 16)), 0, 0, 0x10, RGB_BLACK);
        gSprites[gTasks[taskId].tMonSpriteId].callback = SpriteCB_SlideCaughtMonToCenter;
        gTasks[taskId].func = Task_ExitCaughtMonPage;
    }
    // Flicker caught screen color
    else if (++gTasks[taskId].tPalTimer & 16)
    {
        LoadPalette(gPokedexBgHoenn_Pal + 1, BG_PLTT_ID(3) + 1, PLTT_SIZEOF(7));
    }
    else
    {
        LoadPalette(gPokedexBgHoenn_Pal + 49, BG_PLTT_ID(3) + 1, PLTT_SIZEOF(7));
    }
}

static void Task_ExitCaughtMonPage(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        u16 species;
        u32 otId;
        u32 personality;
        u8 paletteNum;
        const u32 *lzPaletteData;
        void *buffer;

        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        FreeAllWindowBuffers();
        buffer = GetBgTilemapBuffer(2);
        if (buffer)
        {
            Free(buffer);
            if (gTasks[taskId].tMonSpriteId == 1)
                DestroySprite(&gSprites[0]);
            else
            {
                DestroySprite(&gSprites[0]);
                DestroySprite(&gSprites[1]);
            }
        }
        buffer = GetBgTilemapBuffer(3);
        if (buffer)
            Free(buffer);

        species = NationalPokedexNumToSpecies(gTasks[taskId].tDexNum);
        otId = ((u16)gTasks[taskId].tOtIdHi << 16) | (u16)gTasks[taskId].tOtIdLo;
        personality = ((u16)gTasks[taskId].tPersonalityHi << 16) | (u16)gTasks[taskId].tPersonalityLo;
        paletteNum = gSprites[gTasks[taskId].tMonSpriteId].oam.paletteNum;
        lzPaletteData = GetMonSpritePalFromSpeciesAndPersonality(species, otId, personality);
        LoadCompressedPalette(lzPaletteData, OBJ_PLTT_ID(paletteNum), PLTT_SIZE_4BPP);
        DestroyTask(taskId);
    }
}

static void SpriteCB_SlideCaughtMonToCenter(struct Sprite *sprite)
{
    if (sprite->x < DISPLAY_WIDTH / 2)
        sprite->x += 2;
    if (sprite->x > DISPLAY_WIDTH / 2)
        sprite->x -= 2;

    if (sprite->y < DISPLAY_HEIGHT / 2)
        sprite->y += 1;
    if (sprite->y > DISPLAY_HEIGHT / 2)
        sprite->y -= 1;
}

#undef tState
#undef tDexNum
#undef tPalTimer
#undef tMonSpriteId
#undef tOtIdLo
#undef tOtIdHi
#undef tPersonalityLo
#undef tPersonalityHi

// u32 value is re-used, but passed as a bool that's TRUE if national dex is enabled
static void PrintMonInfo(u32 num, u32 value, u32 owned, u32 newEntry)
{
    u8 str[16];
    u8 str2[32];
    u16 natNum;
    const u8 *name;
    const u8 *category;
    const u8 *description;

    if (newEntry)
        PrintInfoScreenText(gText_PokedexRegistration, GetStringCenterAlignXOffset(FONT_NORMAL, gText_PokedexRegistration, DISPLAY_WIDTH), 0);
    if (value == 0)
        value = NationalToHoennOrder(num);
    else
        value = num;
    ConvertIntToDecimalStringN(StringCopy(str, gText_NumberClear01), value, STR_CONV_MODE_LEADING_ZEROS, 3);
    PrintInfoScreenText(str, 0x60, 0x19);
    natNum = NationalPokedexNumToSpecies(num);
    if (natNum)
        name = gSpeciesNames[natNum];
    else
        name = sText_TenDashes2;
    PrintInfoScreenText(name, 0x84, 0x19);
    if (owned)
    {
        CopyMonCategoryText(num, str2);
        category = str2;
    }
    else
    {
        category = gText_5MarksPokemon;
    }
    PrintInfoScreenText(category, 0x64, 0x29);
    //PrintInfoScreenText(gText_HTHeight, 0x60, 0x39);
    //PrintInfoScreenText(gText_WTWeight, 0x60, 0x49);
    if (owned)
    {
        PrintMonHeight(gPokedexEntries[num].height, 137, 0x39);
        PrintMonWeight(gPokedexEntries[num].weight, 137, 0x49);
    }
    else
    {
        PrintInfoScreenText(gText_UnkHeight, 137, 0x39);
        PrintInfoScreenText(gText_UnkWeight, 137, 0x49);
    }
    if (owned)
        description = gPokedexEntries[num].description;
    else
        description = sExpandedPlaceholder_PokedexDescription;
    PrintInfoScreenText(description, GetStringCenterAlignXOffset(FONT_NORMAL, description, DISPLAY_WIDTH), 95);
}

static void PrintMonHeight(u16 height, u8 left, u8 top)
{
    u8 buffer[16];
    u32 inches, feet;
    u8 i = 0;

    inches = (height * 10000) / 254;
    if (inches % 10 >= 5)
        inches += 10;
    feet = inches / 120;
    inches = (inches - (feet * 120)) / 10;

    buffer[i++] = EXT_CTRL_CODE_BEGIN;
    buffer[i++] = EXT_CTRL_CODE_CLEAR_TO;
    if (feet / 10 == 0)
    {
        buffer[i++] = 18;
        buffer[i++] = feet + CHAR_0;
    }
    else
    {
        buffer[i++] = 12;
        buffer[i++] = feet / 10 + CHAR_0;
        buffer[i++] = (feet % 10) + CHAR_0;
    }
    buffer[i++] = CHAR_SGL_QUOTE_RIGHT;
    buffer[i++] = (inches / 10) + CHAR_0;
    buffer[i++] = (inches % 10) + CHAR_0;
    buffer[i++] = CHAR_DBL_QUOTE_RIGHT;
    buffer[i++] = EOS;
    PrintInfoScreenText(buffer, left, top);
}

static void PrintMonWeight(u16 weight, u8 left, u8 top)
{
    u8 buffer[16];
    bool32 output;
    u32 i;
    u32 lbs = (weight * 100000) / 4536;

    if (lbs % 10u >= 5)
        lbs += 10;
    i = 0;
    output = FALSE;

    if ((buffer[i] = (lbs / 100000) + CHAR_0) == CHAR_0 && !output)
    {
        buffer[i++] = CHAR_SPACER;
    }
    else
    {
        output = TRUE;
        i++;
    }

    lbs %= 100000;
    if ((buffer[i] = (lbs / 10000) + CHAR_0) == CHAR_0 && !output)
    {
        buffer[i++] = CHAR_SPACER;
    }
    else
    {
        output = TRUE;
        i++;
    }

    lbs %= 10000;
    if ((buffer[i] = (lbs / 1000) + CHAR_0) == CHAR_0 && !output)
    {
        buffer[i++] = CHAR_SPACER;
    }
    else
    {
        output = TRUE;
        i++;
    }

    lbs %= 1000;
    buffer[i++] = (lbs / 100) + CHAR_0;
    lbs %= 100;
    buffer[i++] = CHAR_PERIOD;
    buffer[i++] = (lbs / 10) + CHAR_0;
    buffer[i++] = CHAR_SPACE;
    buffer[i++] = CHAR_l;
    buffer[i++] = CHAR_b;
    buffer[i++] = CHAR_s;
    buffer[i++] = CHAR_PERIOD;
    buffer[i++] = EOS;
    PrintInfoScreenText(buffer, left, top);
}

const u8 *GetPokedexCategoryName(u16 dexNum) // unused
{
    return gPokedexEntries[dexNum].categoryName;
}

u16 GetPokedexHeightWeight(u16 dexNum, u8 data)
{
    switch (data)
    {
    case 0:  // height
        return gPokedexEntries[dexNum].height;
    case 1:  // weight
        return gPokedexEntries[dexNum].weight;
    default:
        return 1;
    }
}

s8 GetSetPokedexFlag(u16 nationalDexNo, u8 caseID)
{
    u8 index;
    u8 bit;
    u8 mask;
    s8 retVal;

    nationalDexNo--;
    index = nationalDexNo / 8;
    bit = nationalDexNo % 8;
    mask = 1 << bit;
    retVal = 0;
    switch (caseID)
    {
    case FLAG_GET_SEEN:
        if (gSaveBlock2Ptr->pokedex.seen[index] & mask)
        {
            if ((gSaveBlock2Ptr->pokedex.seen[index] & mask) == (gSaveBlock1Ptr->seen1[index] & mask)
             && (gSaveBlock2Ptr->pokedex.seen[index] & mask) == (gSaveBlock1Ptr->seen2[index] & mask))
                retVal = 1;
            else
            {
                gSaveBlock2Ptr->pokedex.seen[index] &= ~mask;
                gSaveBlock1Ptr->seen1[index] &= ~mask;
                gSaveBlock1Ptr->seen2[index] &= ~mask;
                retVal = 0;
            }
        }
        break;
    case FLAG_GET_CAUGHT:
        if (gSaveBlock2Ptr->pokedex.owned[index] & mask)
        {
            if ((gSaveBlock2Ptr->pokedex.owned[index] & mask) == (gSaveBlock2Ptr->pokedex.seen[index] & mask)
             && (gSaveBlock2Ptr->pokedex.owned[index] & mask) == (gSaveBlock1Ptr->seen1[index] & mask)
             && (gSaveBlock2Ptr->pokedex.owned[index] & mask) == (gSaveBlock1Ptr->seen2[index] & mask))
                retVal = 1;
            else
            {
                gSaveBlock2Ptr->pokedex.owned[index] &= ~mask;
                gSaveBlock2Ptr->pokedex.seen[index] &= ~mask;
                gSaveBlock1Ptr->seen1[index] &= ~mask;
                gSaveBlock1Ptr->seen2[index] &= ~mask;
                retVal = 0;
            }
        }
        break;
    case FLAG_SET_SEEN:
        gSaveBlock2Ptr->pokedex.seen[index] |= mask;
        gSaveBlock1Ptr->seen1[index] |= mask;
        gSaveBlock1Ptr->seen2[index] |= mask;
        break;
    case FLAG_SET_CAUGHT:
        gSaveBlock2Ptr->pokedex.owned[index] |= mask;
        break;
    }
    return retVal;
}

u16 GetNationalPokedexCount(u8 caseID)
{
    u32 count = 0;
    u32 i;

    for (i = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        switch (caseID)
        {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_SEEN))
                count++;
            break;
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
                count++;
            break;
        }
    }
    return count;
}

u16 GetHoennPokedexCount(u8 caseID)
{
    u32 count = 0;
    u32 i;

    for (i = 0; i < HOENN_DEX_COUNT; i++)
    {
        switch (caseID)
        {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_SEEN))
                count++;
            break;
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_CAUGHT))
                count++;
            break;
        }
    }
    return count;
}

u16 GetKantoPokedexCount(u8 caseID)
{
    u32 count = 0;
    u32 i;

    for (i = 0; i < KANTO_DEX_COUNT; i++)
    {
        switch (caseID)
        {
        case FLAG_GET_SEEN:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_SEEN))
                count++;
            break;
        case FLAG_GET_CAUGHT:
            if (GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
                count++;
            break;
        }
    }
    return count;
}

bool16 HasAllHoennMons(void)
{
    u32 i;

    // -2 excludes Jirachi and Deoxys
    for (i = 0; i < HOENN_DEX_COUNT - 2; i++)
    {
        if (!GetSetPokedexFlag(HoennToNationalOrder(i + 1), FLAG_GET_CAUGHT))
            return FALSE;
    }
    return TRUE;
}

bool8 HasAllKantoMons(void)
{
    u32 i;

    // -1 excludes Mew
    for (i = 0; i < KANTO_DEX_COUNT - 1; i++)
    {
        if (!GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
            return FALSE;
    }
    return TRUE;
}

bool16 HasAllMons(void)
{
    u32 i;

    // -1 excludes Mew
    for (i = 0; i < KANTO_DEX_COUNT - 1; i++)
    {
        if (!GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
            return FALSE;
    }

    // -3 excludes Lugia, Ho-Oh, and Celebi
    for (i = KANTO_DEX_COUNT; i < JOHTO_DEX_COUNT - 3; i++)
    {
        if (!GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
            return FALSE;
    }

    // -2 excludes Jirachi and Deoxys
    for (i = JOHTO_DEX_COUNT; i < NATIONAL_DEX_COUNT - 2; i++)
    {
        if (!GetSetPokedexFlag(i + 1, FLAG_GET_CAUGHT))
            return FALSE;
    }
    return TRUE;
}

static void ResetOtherVideoRegisters(u16 regBits)
{
    if (!(regBits & DISPCNT_BG0_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG0_ON);
        SetGpuReg(REG_OFFSET_BG0CNT, 0);
        SetGpuReg(REG_OFFSET_BG0HOFS, 0);
        SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    }
    if (!(regBits & DISPCNT_BG1_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG1_ON);
        SetGpuReg(REG_OFFSET_BG1CNT, 0);
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
        SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    }
    if (!(regBits & DISPCNT_BG2_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG2_ON);
        SetGpuReg(REG_OFFSET_BG2CNT, 0);
        SetGpuReg(REG_OFFSET_BG2HOFS, 0);
        SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    }
    if (!(regBits & DISPCNT_BG3_ON))
    {
        ClearGpuRegBits(0, DISPCNT_BG3_ON);
        SetGpuReg(REG_OFFSET_BG3CNT, 0);
        SetGpuReg(REG_OFFSET_BG3HOFS, 0);
        SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    }
    if (!(regBits & DISPCNT_OBJ_ON))
    {
        ClearGpuRegBits(0, DISPCNT_OBJ_ON);
        ResetSpriteData();
        FreeAllSpritePalettes();
        gReservedSpritePaletteCount = 8;
    }
}

static void PrintInfoSubMenuText(u8 windowId, const u8 *str, u8 left, u8 top)
{
    u8 color[3];
    color[0] = TEXT_COLOR_TRANSPARENT;
    color[1] = TEXT_DYNAMIC_COLOR_6;
    color[2] = TEXT_COLOR_LIGHT_GRAY;

    AddTextPrinterParameterized4(windowId, FONT_NORMAL, left, top, 0, 0, color, TEXT_SKIP_DRAW, str);
}

static void UNUSED UnusedPrintNum(u8 windowId, u16 num, u8 left, u8 top)
{
    u8 str[4];

    str[0] = CHAR_0 + num / 100;
    str[1] = CHAR_0 + (num % 100) / 10;
    str[2] = CHAR_0 + (num % 100) % 10;
    str[3] = EOS;
    PrintInfoSubMenuText(windowId, str, left, top);
}

static u8 PrintCryScreenSpeciesName(u8 windowId, u16 num, u8 left, u8 top)
{
    u8 str[POKEMON_NAME_LENGTH + 1];
    u32 i;

    for (i = 0; i < ARRAY_COUNT(str); i++)
        str[i] = EOS;
    num = NationalPokedexNumToSpecies(num);
    switch (num)
    {
    default:
        for (i = 0; gSpeciesNames[num][i] != EOS && i < POKEMON_NAME_LENGTH; i++)
            str[i] = gSpeciesNames[num][i];
        break;
    case 0:
        for (i = 0; i < 5; i++)
            str[i] = CHAR_HYPHEN;
        break;
    }
    PrintInfoSubMenuText(windowId, str, left, top);
    return i;
}

static void UNUSED UnusedPrintMonName(u8 windowId, const u8 *name, u8 left, u8 top)
{
    u8 str[POKEMON_NAME_LENGTH + 1];
    u32 i;
    u32 nameLength;

    for (i = 0; i < ARRAY_COUNT(str); i++)
        str[i] = CHAR_SPACE;
    for (nameLength = 0; name[nameLength] != CHAR_SPACE && nameLength < ARRAY_COUNT(str); nameLength++)
        ;
    for (i = 0; i < nameLength; i++)
        str[ARRAY_COUNT(str) - nameLength + i] = name[i];
#ifdef UBFIX
    str[ARRAY_COUNT(str) - 1] = EOS;
#else
    str[ARRAY_COUNT(str)] = EOS;
#endif
    PrintInfoSubMenuText(windowId, str, left, top);
}

// Unused in the English version, used to print height/weight in versions which use metric system.
static void UNUSED PrintDecimalNum(u8 windowId, u16 num, u8 left, u8 top)
{
    u8 str[6];
    bool8 outputted = FALSE;
    u8 result;

    result = num / 1000;
    if (result == 0)
    {
        str[0] = CHAR_SPACER;
        outputted = FALSE;
    }
    else
    {
        str[0] = CHAR_0 + result;
        outputted = TRUE;
    }

    result = (num % 1000) / 100;
    if (result == 0 && !outputted)
    {
        str[1] = CHAR_SPACER;
        outputted = FALSE;
    }
    else
    {
        str[1] = CHAR_0 + result;
        outputted = TRUE;
    }

    str[2] = CHAR_0 + ((num % 1000) % 100) / 10;
    str[3] = CHAR_DEC_SEPARATOR;
    str[4] = CHAR_0 + ((num % 1000) % 100) % 10;
    str[5] = EOS;
    PrintInfoSubMenuText(windowId, str, left, top);
}

// The footprints are drawn on WIN_FOOTPRINT, which uses BG palette 15 (loaded with graphics/text_window/message_box.gbapal)
// The footprint pixels are stored as 1BPP, and set to the below color index in this palette when converted to 4BPP.
#define FOOTPRINT_COLOR_IDX  2

#define NUM_FOOTPRINT_TILES  4

static void DrawFootprint(u8 windowId, u16 dexNum)
{
    u8 footprint4bpp[TILE_SIZE_4BPP * NUM_FOOTPRINT_TILES];
    const u8 * footprintGfx = gMonFootprintTable[NationalPokedexNumToSpecies(dexNum)];
    u16 tileIdx = 0;
    u16 i, j;

    for (i = 0; i < TILE_SIZE_1BPP * NUM_FOOTPRINT_TILES; i++)
    {
        u8 footprint1bpp = footprintGfx[i];

        // Convert the 8 pixels in the above 1BPP byte to 4BPP.
        // Each iteration creates one 4BPP byte (2 pixels),
        // so we need 4 iterations to do all 8 pixels.
        for (j = 0; j < 4; j++)
        {
            u8 tile = 0;
            if (footprint1bpp & (1 << (2 * j)))
                tile |= FOOTPRINT_COLOR_IDX; // Set pixel
            if (footprint1bpp & (2 << (2 * j)))
                tile |= FOOTPRINT_COLOR_IDX << 4; // Set pixel
            footprint4bpp[tileIdx] = tile;
            tileIdx++;
        }
    }
    CopyToWindowPixelBuffer(windowId, footprint4bpp, sizeof(footprint4bpp), 0);
}

// Ruby/Sapphire function.
static void UNUSED RS_DrawFootprint(u16 offset, u16 tileNum)
{
    *(u16 *)(VRAM + offset * 0x800 + 0x232) = 0xF000 + tileNum + 0;
    *(u16 *)(VRAM + offset * 0x800 + 0x234) = 0xF000 + tileNum + 1;
    *(u16 *)(VRAM + offset * 0x800 + 0x272) = 0xF000 + tileNum + 2;
    *(u16 *)(VRAM + offset * 0x800 + 0x274) = 0xF000 + tileNum + 3;
}

static u16 GetNextPosition(u8 direction, u16 position, u16 min, u16 max)
{
    switch (direction)
    {
    case 1: // Up/Left
        if (position > min)
            position--;
        break;
    case 0: // Down/Right
        if (position < max)
            position++;
        break;
    case 3: // Up/Left with loop (unused)
        if (position > min)
            position--;
        else
            position = max;
        break;
    case 2: // Down/Right with loop (unused)
        if (position < max)
            position++;
        else
            position = min;
        break;
    }
    return position;
}

// Unown and Spinda use the personality of the first seen individual of that species
// All others use personality 0
static u32 GetPokedexMonPersonality(u16 species)
{
    if (species == SPECIES_UNOWN || species == SPECIES_SPINDA)
    {
        if (species == SPECIES_UNOWN)
            return gSaveBlock2Ptr->pokedex.unownPersonality;
        else
            return gSaveBlock2Ptr->pokedex.spindaPersonality;
    }
    else
    {
        return 0;
    }
}

u16 CreateMonSpriteFromNationalDexNumber(u16 nationalNum, s16 x, s16 y, u16 paletteSlot)
{
    nationalNum = NationalPokedexNumToSpecies(nationalNum);
    return CreateMonPicSprite_HandleDeoxys(nationalNum, SHINY_ODDS, GetPokedexMonPersonality(nationalNum), TRUE, x, y, paletteSlot, TAG_NONE);
}

static u16 CreateSizeScreenTrainerPic(u16 species, s16 x, s16 y, s8 paletteSlot)
{
    return CreateTrainerPicSprite(species, TRUE, x, y, paletteSlot, TAG_NONE);
}

static u32 DoPokedexSearch(u32 dexMode, u32 order, u32 type1, u32 type2, u32 eggGroup1, u32 eggGroup2, u32 ability, u32 move, u32 moveType)
{
    u32 species, speciesCopy, resultsCount, tutorLearnset;
    u32 i, j, k;
    u32 groups[2];
    u32 types[2];
    u32 abilities[2];
    bool32 moveFound;
    bool32 isTM = FALSE;
    const u16 *levelUpLearnset;
    s32 tmId = -1;

    CreatePokedexList(dexMode, order);

    for (i = 0, resultsCount = 0; i < NATIONAL_DEX_COUNT; i++)
    {
        sPokedexView->pokedexList[resultsCount] = sPokedexView->pokedexList[i];
        resultsCount++;
    }
    sPokedexView->pokemonListCount = resultsCount;

    /*
    // Search by name
    if (abcGroup != 0xFF)
    {
        for (i = 0, resultsCount = 0; i < sPokedexView->pokemonListCount; i++)
        {
            u8 firstLetter;

            species = NationalPokedexNumToSpecies(sPokedexView->pokedexList[i].dexNum);
            firstLetter = gSpeciesNames[species][0];
            if (LETTER_IN_RANGE_UPPER(firstLetter, abcGroup) || LETTER_IN_RANGE_LOWER(firstLetter, abcGroup))
            {
                sPokedexView->pokedexList[resultsCount] = sPokedexView->pokedexList[i];
                resultsCount++;
            }
        }
        sPokedexView->pokemonListCount = resultsCount;
    }

    // Search by body color
    if (bodyColor != 0xFF)
    {
        for (i = 0, resultsCount = 0; i < sPokedexView->pokemonListCount; i++)
        {
            species = NationalPokedexNumToSpecies(sPokedexView->pokedexList[i].dexNum);

            if (bodyColor == gSpeciesInfo[species].bodyColor)
            {
                sPokedexView->pokedexList[resultsCount] = sPokedexView->pokedexList[i];
                resultsCount++;
            }
        }
        sPokedexView->pokemonListCount = resultsCount;
    }
    */

    // Search by type
    if (type1 != TYPE_NONE || type2 != TYPE_NONE)
    {
        if (type1 == TYPE_NONE)
        {
            type1 = type2;
            type2 = TYPE_NONE;
        }

        if (type2 == TYPE_NONE)
        {
            for (i = 0, resultsCount = 0; i < sPokedexView->pokemonListCount; i++)
            {
                species = NationalPokedexNumToSpecies(sPokedexView->pokedexList[i].dexNum);

                types[0] = gSpeciesInfo[species].types[0];
                types[1] = gSpeciesInfo[species].types[1];
                if (types[0] == type1 || types[1] == type1)
                {
                    sPokedexView->pokedexList[resultsCount] = sPokedexView->pokedexList[i];
                    resultsCount++;
                }
            }
        }
        else
        {
            for (i = 0, resultsCount = 0; i < sPokedexView->pokemonListCount; i++)
            {
                species = NationalPokedexNumToSpecies(sPokedexView->pokedexList[i].dexNum);

                types[0] = gSpeciesInfo[species].types[0];
                types[1] = gSpeciesInfo[species].types[1];
                if ((types[0] == type1 && types[1] == type2) || (types[0] == type2 && types[1] == type1))
                {
                    sPokedexView->pokedexList[resultsCount] = sPokedexView->pokedexList[i];
                    resultsCount++;
                }
            }
        }
        sPokedexView->pokemonListCount = resultsCount;
    }

    // Search by group
    if (eggGroup1 != EGG_GROUP_NONE || eggGroup2 != EGG_GROUP_NONE)
    {
        if (eggGroup1 == EGG_GROUP_NONE)
        {
            eggGroup1 = eggGroup2;
            eggGroup2 = EGG_GROUP_NONE;
        }

        if (eggGroup2 == EGG_GROUP_NONE)
        {
            for (i = 0, resultsCount = 0; i < sPokedexView->pokemonListCount; i++)
            {
                species = NationalPokedexNumToSpecies(sPokedexView->pokedexList[i].dexNum);

                groups[0] = gSpeciesInfo[species].eggGroups[0];
                groups[1] = gSpeciesInfo[species].eggGroups[1];
                if (groups[0] == eggGroup1 || groups[1] == eggGroup1)
                {
                    sPokedexView->pokedexList[resultsCount] = sPokedexView->pokedexList[i];
                    resultsCount++;
                }
            }
        }
        else
        {
            for (i = 0, resultsCount = 0; i < sPokedexView->pokemonListCount; i++)
            {
                species = NationalPokedexNumToSpecies(sPokedexView->pokedexList[i].dexNum);

                groups[0] = gSpeciesInfo[species].eggGroups[0];
                groups[1] = gSpeciesInfo[species].eggGroups[1];
                if ((groups[0] == eggGroup1 && groups[1] == eggGroup2) || (groups[0] == eggGroup2 && groups[1] == eggGroup1))
                {
                    sPokedexView->pokedexList[resultsCount] = sPokedexView->pokedexList[i];
                    resultsCount++;
                }
            }
        }
        sPokedexView->pokemonListCount = resultsCount;
    }

    // Search by ability
    if (ability != ABILITY_NONE)
    {
        for (i = 0, resultsCount = 0; i < sPokedexView->pokemonListCount; i++)
        {
            species = NationalPokedexNumToSpecies(sPokedexView->pokedexList[i].dexNum);

            abilities[0] = gSpeciesInfo[species].abilities[0];
            abilities[1] = gSpeciesInfo[species].abilities[1];
            if (abilities[0] == ability || abilities[1] == ability)
            {
                sPokedexView->pokedexList[resultsCount] = sPokedexView->pokedexList[i];
                resultsCount++;
            }
        }
        sPokedexView->pokemonListCount = resultsCount;
    }

    // Search by move
    if (move != MOVE_NONE)
    {
        // Is move TM?
        if (moveType == MOVETYPE_ANY || moveType == MOVETYPE_TMHM)
        {
            for (j = 0; j <= ITEM_HM08 - ITEM_TM01; j++)
            {
                if (sTMHMMoves[j] == move)
                {
                    isTM = TRUE;
                    tmId = j;
                    break;
                }
            }
        }

        for (i = 0, resultsCount = 0; i < sPokedexView->pokemonListCount; i++)
        {
            moveFound = FALSE;
            species = NationalPokedexNumToSpecies(sPokedexView->pokedexList[i].dexNum);
            speciesCopy = species;

            // TM/HM moves
            if (isTM && (moveType == MOVETYPE_ANY || moveType == MOVETYPE_TMHM) && CanSpeciesLearnTMHM(species, tmId))
            {
                moveFound = TRUE;
            }

            // Level up moves
            if (!moveFound && (moveType == MOVETYPE_ANY || moveType == MOVETYPE_LEVEL_UP))
            {
                levelUpLearnset = gLevelUpLearnsets[species];
                for (j = 0; levelUpLearnset[j] != LEVEL_UP_END; j++)
                {
                    if ((levelUpLearnset[j] & LEVEL_UP_MOVE_ID) == move)
                    {
                        moveFound = TRUE;
                        break;
                    }
                }
            }

            // Pre-evolution level up moves
            if (!moveFound && moveType == MOVETYPE_ANY)
            {
                species = GetPreEvolution(species);

                while (!moveFound && species != SPECIES_NONE)
                {
                    levelUpLearnset = gLevelUpLearnsets[species];
                    for (j = 0; levelUpLearnset[j] != LEVEL_UP_END; j++)
                    {
                        if ((levelUpLearnset[j] & LEVEL_UP_MOVE_ID) == move)
                        {
                            moveFound = TRUE;
                            break;
                        }
                    }

                    if (moveFound)
                        break;

                    species = GetPreEvolution(species);
                }

                species = speciesCopy;
            }

            // Tutor moves
            if (!moveFound && (moveType == MOVETYPE_ANY || moveType == MOVETYPE_TUTOR))
            {
                tutorLearnset = sTutorLearnsets[species];

                for (j = 0; j < TUTOR_MOVE_COUNT; j++)
                {
                    if (gTutorMoves[j] == move && (tutorLearnset & (1 << j)))
                    {
                        moveFound = TRUE;
                        break;
                    }
                }
            }

            // Egg moves
            if (!moveFound  && (moveType == MOVETYPE_ANY || moveType == MOVETYPE_EGG_MOVE))
            {
                while (species != SPECIES_NONE)
                {
                    speciesCopy = species;
                    species = GetPreEvolution(species);
                }

                j = FindSpeciesInEggMoves(speciesCopy);
                if (j != -1)
                {
                    for (k = 1; k <= 8; k++)
                    {
                        if (gEggMoves[j + k] > EGG_MOVES_SPECIES_OFFSET)
                            break;
                        if (gEggMoves[k + j] == move)
                        {
                            moveFound = TRUE;
                            break;
                        }
                    }
                }
            }

            if (moveFound)
            {
                sPokedexView->pokedexList[resultsCount++] = sPokedexView->pokedexList[i];
            }
        }
        sPokedexView->pokemonListCount = resultsCount;
    }

    if (sPokedexView->pokemonListCount != 0)
    {
        for (i = sPokedexView->pokemonListCount; i < NATIONAL_DEX_COUNT; i++)
        {
            sPokedexView->pokedexList[i].dexNum = 0xFFFF;
            sPokedexView->pokedexList[i].seen = FALSE;
            sPokedexView->pokedexList[i].owned = FALSE;
        }
    }

    return resultsCount;
}

static u8 LoadSearchMenu(void)
{
    return CreateTask(Task_LoadSearchMenu, 0);
}

static void PrintSearchText(const u8 *str, u32 x, u32 y)
{
    u8 color[3];

    color[0] = TEXT_COLOR_TRANSPARENT;
    color[1] = TEXT_DYNAMIC_COLOR_6;
    color[2] = TEXT_COLOR_DARK_GRAY;
    AddTextPrinterParameterized4(0, FONT_NORMAL, x, y, 0, 0, color, TEXT_SKIP_DRAW, str);
}

static void ClearSearchMenuRect(u32 x, u32 y, u32 width, u32 height)
{
    FillWindowPixelRect(0, PIXEL_FILL(0), x, y, width, height);
}

// Search task data
#define tTopBarItem              data[0]
#define tMenuItem                data[1]
#define tCursorPos_Mode          data[2]
#define tScrollOffset_Mode       data[3]
#define tCursorPos_Order         data[4]
#define tScrollOffset_Order      data[5]
//#define tCursorPos_Name          data[6]
//#define tScrollOffset_Name       data[7]
//#define tCursorPos_Color        data[8]
//#define tScrollOffset_Color     data[9]
#define tCursorPos_TypeLeft      data[6]
#define tScrollOffset_TypeLeft   data[7]
#define tCursorPos_TypeRight     data[8]
#define tScrollOffset_TypeRight  data[9]
#define tCursorPos_GroupLeft     data[10]
#define tScrollOffset_GroupLeft  data[11]
#define tCursorPos_GroupRight    data[12]
#define tScrollOffset_GroupRight data[13]
#define tCursorPos_Ability       data[14]
#define tScrollOffset_Ability    data[15]
#define tCursorPos_Move          data[24]
#define tScrollOffset_Move       data[25]
#define tCursorPos               data[26]
#define tScrollOffset            data[27]
#define tCursorPos_Ability_2     data[28]
#define tScrollOffset_Ability_2  data[29]
#define tCursorPos_Move_2        data[30]
#define tScrollOffset_Move_2     data[31]
#define tCursorPos_MoveType   	 data[32]
#define tScrollOffset_MoveType   data[33]

static void Task_LoadSearchMenu(u8 taskId)
{
    u32 i;

    switch (gMain.state)
    {
    default:
    case 0:
        if (!gPaletteFade.active)
        {
            sPokedexView->currentPage = PAGE_SEARCH;
            ResetOtherVideoRegisters(0);
            ResetBgsAndClearDma3BusyFlags(0);
            InitBgsFromTemplates(0, sSearchMenu_BgTemplate, ARRAY_COUNT(sSearchMenu_BgTemplate));
            SetBgTilemapBuffer(3, AllocZeroed(BG_SCREEN_SIZE));
            SetBgTilemapBuffer(2, AllocZeroed(BG_SCREEN_SIZE));
            SetBgTilemapBuffer(1, AllocZeroed(BG_SCREEN_SIZE));
            SetBgTilemapBuffer(0, AllocZeroed(BG_SCREEN_SIZE));
            InitWindows(sSearchMenu_WindowTemplate);
            DeactivateAllTextPrinters();
            PutWindowTilemap(0);
            DecompressAndLoadBgGfxUsingHeap(3, gPokedexSearchMenu_Gfx, 0x2000, 0, 0);

            if (!IsNationalPokedexEnabled())
                CopyToBgTilemapBuffer(3, gPokedexSearchMenuHoenn_Tilemap, 0, 0);
            else
                CopyToBgTilemapBuffer(3, gPokedexSearchMenuNational_Tilemap, 0, 0);
            LoadPalette(gPokedexSearchMenu_Pal + 1, BG_PLTT_ID(0) + 1, PLTT_SIZEOF(4 * 16 - 1));
            gMain.state = 1;
        }
        break;
    case 1:
        LoadCompressedSpriteSheet(sInterfaceSpriteSheet);
        LoadSpritePalettes(sInterfaceSpritePalette);
        CreateSearchParameterScrollArrows(taskId);
        for (i = 0; i < NUM_TASK_DATA; i++)
            gTasks[taskId].data[i] = 0;
        SetDefaultSearchModeAndOrder(taskId);
        HighlightSelectedSearchTopBarItem(SEARCH_TOPBAR_SEARCH);
        PrintSelectedSearchParameters(taskId);
        CopyWindowToVram(0, COPYWIN_FULL);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        CopyBgTilemapBufferToVram(3);
        gMain.state++;
        break;
    case 2:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    case 3:
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        HideBg(0);
        ShowBg(1);
        ShowBg(2);
        ShowBg(3);
        gMain.state++;
        break;
    case 4:
        if (!gPaletteFade.active)
        {
            gTasks[taskId].func = Task_SwitchToSearchMenuTopBar;
            gMain.state = 0;
        }
        break;
    }
}

static void FreeSearchWindowAndBgBuffers(void)
{
    void *tilemapBuffer;

    FreeAllWindowBuffers();
    tilemapBuffer = GetBgTilemapBuffer(0);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(1);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(2);
    if (tilemapBuffer)
        Free(tilemapBuffer);
    tilemapBuffer = GetBgTilemapBuffer(3);
    if (tilemapBuffer)
        Free(tilemapBuffer);
}

static void Task_SwitchToSearchMenuTopBar(u8 taskId)
{
    HighlightSelectedSearchTopBarItem(gTasks[taskId].tTopBarItem);
    PrintSelectedSearchParameters(taskId);
    CopyWindowToVram(0, COPYWIN_GFX);
    CopyBgTilemapBufferToVram(3);
    gTasks[taskId].func = Task_HandleSearchTopBarInput;
}

static void Task_HandleSearchTopBarInput(u8 taskId)
{
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_PC_OFF);
        gTasks[taskId].func = Task_ExitSearch;
        return;
    }
    if (JOY_NEW(A_BUTTON) || (JOY_NEW(DPAD_DOWN) && gTasks[taskId].tTopBarItem != SEARCH_TOPBAR_CANCEL))
    {
        switch (gTasks[taskId].tTopBarItem)
        {
        case SEARCH_TOPBAR_SEARCH:
            PlaySE(SE_PIN);
            gTasks[taskId].tMenuItem = SEARCH_TYPE_LEFT;
            gTasks[taskId].func = Task_SwitchToSearchMenu;
            break;
        case SEARCH_TOPBAR_SHIFT:
            PlaySE(SE_PIN);
            gTasks[taskId].tMenuItem = SEARCH_ORDER;
            gTasks[taskId].func = Task_SwitchToSearchMenu;
            break;
        case SEARCH_TOPBAR_CANCEL:
            PlaySE(SE_PC_OFF);
            gTasks[taskId].func = Task_ExitSearch;
            break;
        }
        return;
    }
    if (JOY_NEW(DPAD_LEFT) && gTasks[taskId].tTopBarItem > SEARCH_TOPBAR_SEARCH)
    {
        PlaySE(SE_DEX_PAGE);
        gTasks[taskId].tTopBarItem--;
        HighlightSelectedSearchTopBarItem(gTasks[taskId].tTopBarItem);
        CopyWindowToVram(0, COPYWIN_GFX);
        CopyBgTilemapBufferToVram(3);
    }
    if (JOY_NEW(DPAD_RIGHT) && gTasks[taskId].tTopBarItem < SEARCH_TOPBAR_CANCEL)
    {
        PlaySE(SE_DEX_PAGE);
        gTasks[taskId].tTopBarItem++;
        HighlightSelectedSearchTopBarItem(gTasks[taskId].tTopBarItem);
        CopyWindowToVram(0, COPYWIN_GFX);
        CopyBgTilemapBufferToVram(3);
    }
}

static void Task_SwitchToSearchMenu(u8 taskId)
{
    HighlightSelectedSearchMenuItem(gTasks[taskId].tTopBarItem, gTasks[taskId].tMenuItem);
    PrintSelectedSearchParameters(taskId);
    CopyWindowToVram(0, COPYWIN_GFX);
    CopyBgTilemapBufferToVram(3);
    gTasks[taskId].func = Task_HandleSearchMenuInput;
}

// Input for main search menu
static void Task_HandleSearchMenuInput(u8 taskId)
{
    const u8 (*movementMap)[4];

    if (gTasks[taskId].tTopBarItem != SEARCH_TOPBAR_SEARCH)
    {
        if (!IsNationalPokedexEnabled())
            movementMap = sSearchMovementMap_ShiftHoennDex;
        else
            movementMap = sSearchMovementMap_ShiftNatDex;
    }
    else
    {
        if (!IsNationalPokedexEnabled())
            movementMap = sSearchMovementMap_SearchHoennDex;
        else
            movementMap = sSearchMovementMap_SearchNatDex;
    }

    if (JOY_NEW(B_BUTTON)
        || (JOY_NEW(DPAD_UP) && (gTasks[taskId].tMenuItem == SEARCH_TYPE_LEFT
                             || (gTasks[taskId].tMenuItem == SEARCH_ORDER && gTasks[taskId].tTopBarItem == SEARCH_TOPBAR_SHIFT))))
    {
        PlaySE(SE_BALL);
        SetDefaultSearchModeAndOrder(taskId);
        gTasks[taskId].func = Task_SwitchToSearchMenuTopBar;
        return;
    }
    if (JOY_NEW(A_BUTTON))
    {
        if (gTasks[taskId].tMenuItem == SEARCH_OK)
        {
            if (gTasks[taskId].tTopBarItem != SEARCH_TOPBAR_SEARCH)
            {
                sPokeBallRotation = POKEBALL_ROTATION_TOP;
                sPokedexView->pokeBallRotationBackup = POKEBALL_ROTATION_TOP;
                sLastSelectedPokemon = 0;
                sPokedexView->selectedPokemonBackup = 0;
                gSaveBlock2Ptr->pokedex.mode = GetSearchModeSelection(taskId, SEARCH_MODE);
                if (!IsNationalPokedexEnabled())
                    gSaveBlock2Ptr->pokedex.mode = DEX_MODE_HOENN;
                sPokedexView->dexModeBackup = gSaveBlock2Ptr->pokedex.mode;
                gSaveBlock2Ptr->pokedex.order = GetSearchModeSelection(taskId, SEARCH_ORDER);
                sPokedexView->dexOrderBackup = gSaveBlock2Ptr->pokedex.order;
                PlaySE(SE_PC_OFF);
                gTasks[taskId].func = Task_ExitSearch;
            }
            else
            {
                EraseAndPrintSearchTextBox(gText_SearchingPleaseWait);
                gTasks[taskId].func = Task_StartPokedexSearch;
                PlaySE(SE_DEX_SEARCH);
                CopyWindowToVram(0, COPYWIN_GFX);
            }
        }
        else
        {
            PlaySE(SE_PIN);
            gTasks[taskId].func = Task_SelectSearchMenuItem;
        }
        return;
    }

    if (JOY_NEW(DPAD_LEFT) && movementMap[gTasks[taskId].tMenuItem][0] != 0xFF)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].tMenuItem = movementMap[gTasks[taskId].tMenuItem][0];
        HighlightSelectedSearchMenuItem(gTasks[taskId].tTopBarItem, gTasks[taskId].tMenuItem);
        CopyWindowToVram(0, COPYWIN_GFX);
        CopyBgTilemapBufferToVram(3);
    }
    if (JOY_NEW(DPAD_RIGHT) && movementMap[gTasks[taskId].tMenuItem][1] != 0xFF)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].tMenuItem = movementMap[gTasks[taskId].tMenuItem][1];
        HighlightSelectedSearchMenuItem(gTasks[taskId].tTopBarItem, gTasks[taskId].tMenuItem);
        CopyWindowToVram(0, COPYWIN_GFX);
        CopyBgTilemapBufferToVram(3);
    }
    if (JOY_NEW(DPAD_UP) && movementMap[gTasks[taskId].tMenuItem][2] != 0xFF)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].tMenuItem = movementMap[gTasks[taskId].tMenuItem][2];
        HighlightSelectedSearchMenuItem(gTasks[taskId].tTopBarItem, gTasks[taskId].tMenuItem);
        CopyWindowToVram(0, COPYWIN_GFX);
        CopyBgTilemapBufferToVram(3);
    }
    if (JOY_NEW(DPAD_DOWN) && movementMap[gTasks[taskId].tMenuItem][3] != 0xFF)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].tMenuItem = movementMap[gTasks[taskId].tMenuItem][3];
        HighlightSelectedSearchMenuItem(gTasks[taskId].tTopBarItem, gTasks[taskId].tMenuItem);
        CopyWindowToVram(0, COPYWIN_GFX);
        CopyBgTilemapBufferToVram(3);
    }
}

static void Task_StartPokedexSearch(u8 taskId)
{
    u32 dexMode = GetSearchModeSelection(taskId, SEARCH_MODE);
    u32 order = GetSearchModeSelection(taskId, SEARCH_ORDER);
    //u8 abcGroup = GetSearchModeSelection(taskId, SEARCH_NAME);
    //u8 bodyColor = GetSearchModeSelection(taskId, SEARCH_COLOR);
    u32 type1 = GetSearchModeSelection(taskId, SEARCH_TYPE_LEFT);
    u32 type2 = GetSearchModeSelection(taskId, SEARCH_TYPE_RIGHT);
    u32 eggGroup1 = GetSearchModeSelection(taskId, SEARCH_GROUP_LEFT);
    u32 eggGroup2 = GetSearchModeSelection(taskId, SEARCH_GROUP_RIGHT);
    u32 ability = GetSearchModeSelection(taskId, SEARCH_ABILITY);
    u32 move = GetSearchModeSelection(taskId, SEARCH_MOVE);
    u32 moveType = GetSearchModeSelection(taskId, SEARCH_MOVETYPE);

    DoPokedexSearch(dexMode, order, type1, type2, eggGroup1, eggGroup2, ability, move, moveType);
    gTasks[taskId].func = Task_WaitAndCompleteSearch;
}

static void Task_WaitAndCompleteSearch(u8 taskId)
{
    if (!IsSEPlaying())
    {
        if (sPokedexView->pokemonListCount != 0)
        {
            PlaySE(SE_SUCCESS);
            EraseAndPrintSearchTextBox(gText_SearchCompleted);
        }
        else
        {
            PlaySE(SE_FAILURE);
            EraseAndPrintSearchTextBox(gText_NoMatchingPkmnWereFound);
        }
        gTasks[taskId].func = Task_SearchCompleteWaitForInput;
        CopyWindowToVram(0, COPYWIN_GFX);
    }
}

static void Task_SearchCompleteWaitForInput(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        if (sPokedexView->pokemonListCount != 0)
        {
            // Return to dex list and show search results
            sPokedexView->screenSwitchState = 1;
            sPokedexView->dexMode = GetSearchModeSelection(taskId, SEARCH_MODE);
            sPokedexView->dexOrder = GetSearchModeSelection(taskId, SEARCH_ORDER);
            gTasks[taskId].func = Task_ExitSearch;
            PlaySE(SE_PC_OFF);
        }
        else
        {
            gTasks[taskId].func = Task_SwitchToSearchMenu;
            PlaySE(SE_BALL);
        }
    }
}

static void Task_SelectSearchMenuItem(u8 taskId)
{
    u32 menuItem;
    u16 *cursorPos;
    u16 *scrollOffset;

    DrawOrEraseSearchParameterBox(FALSE);
    menuItem = gTasks[taskId].tMenuItem;
    cursorPos = &gTasks[taskId].data[sSearchOptions[menuItem].taskDataCursorPos];
    scrollOffset = &gTasks[taskId].data[sSearchOptions[menuItem].taskDataScrollOffset];
    gTasks[taskId].tCursorPos = *cursorPos;
    gTasks[taskId].tScrollOffset = *scrollOffset;
    PrintSearchParameterText(taskId);
    PrintSelectorArrow(*cursorPos);
    gTasks[taskId].func = Task_HandleSearchParameterInput;
    CopyWindowToVram(0, COPYWIN_GFX);
    CopyBgTilemapBufferToVram(3);
}

// Input for scrolling parameter box in right column
static void Task_HandleSearchParameterInput(u8 taskId)
{
    u32 menuItem;
    const struct SearchOptionText *texts;
    u16 *cursorPos;
    u16 *scrollOffset;
    u32 maxOption;
    bool32 moved;

    menuItem = gTasks[taskId].tMenuItem;
    texts = sSearchOptions[menuItem].texts;
    cursorPos = &gTasks[taskId].data[sSearchOptions[menuItem].taskDataCursorPos];
    scrollOffset = &gTasks[taskId].data[sSearchOptions[menuItem].taskDataScrollOffset];
    maxOption = sSearchOptions[menuItem].numOptions - 1;
    if (JOY_NEW(A_BUTTON))
    {
        if ((menuItem == SEARCH_ABILITY || menuItem == SEARCH_MOVE) && *cursorPos + *scrollOffset != 0)
        {
            PlaySE(SE_PIN);
            gTasks[taskId].tMenuItem = *cursorPos + *scrollOffset + SEARCH_COUNT;
            if (menuItem == SEARCH_MOVE)
                gTasks[taskId].tMenuItem += SEARCH_MOVE_BUG - SEARCH_ABILITY_ABC;
            menuItem = gTasks[taskId].tMenuItem;
            gTasks[taskId].data[sSearchOptions[menuItem].taskDataCursorPos] = 0;
            gTasks[taskId].data[sSearchOptions[menuItem].taskDataScrollOffset] = 0;
            gTasks[taskId].func = Task_SelectSearchMenuItem;
            CopyWindowToVram(0, COPYWIN_GFX);
            CopyBgTilemapBufferToVram(3);
            return;
        }
        else
        {
            if (menuItem > SEARCH_COUNT && menuItem < SEARCH_MOVE_BUG)
                gTasks[taskId].tMenuItem = SEARCH_ABILITY;
            else if (menuItem >= SEARCH_MOVE_BUG)
                gTasks[taskId].tMenuItem = SEARCH_MOVE;
            PlaySE(SE_PIN);
            ClearSearchParameterBoxText();
            DrawOrEraseSearchParameterBox(TRUE);
            gTasks[taskId].func = Task_SwitchToSearchMenu;
            CopyWindowToVram(0, COPYWIN_GFX);
            CopyBgTilemapBufferToVram(3);
            return;
        }
    }
    if (JOY_NEW(B_BUTTON))
    {
        if (menuItem > SEARCH_COUNT)
        {
            PlaySE(SE_BALL);
            *cursorPos = gTasks[taskId].tCursorPos;
            *scrollOffset = gTasks[taskId].tScrollOffset;
            if (menuItem < SEARCH_MOVE_BUG)
                gTasks[taskId].tMenuItem = SEARCH_ABILITY;
            else
                gTasks[taskId].tMenuItem = SEARCH_MOVE;
            gTasks[taskId].func = Task_SelectSearchMenuItem;
            CopyWindowToVram(0, COPYWIN_GFX);
            CopyBgTilemapBufferToVram(3);
            return;
        }
        else
        {
            PlaySE(SE_BALL);
            ClearSearchParameterBoxText();
            DrawOrEraseSearchParameterBox(TRUE);
            if (menuItem == SEARCH_ABILITY || menuItem == SEARCH_MOVE)
            {
                gTasks[taskId].data[sSearchOptions[menuItem].taskDataCursorPos] = 0;
                gTasks[taskId].data[sSearchOptions[menuItem].taskDataScrollOffset] = 0;
            }
            else
            {
                *cursorPos = gTasks[taskId].tCursorPos;
                *scrollOffset = gTasks[taskId].tScrollOffset;
            }
            gTasks[taskId].func = Task_SwitchToSearchMenu;
            CopyWindowToVram(0, COPYWIN_GFX);
            CopyBgTilemapBufferToVram(3);
            return;
        }
    }
    moved = FALSE;
    if (JOY_REPEAT(DPAD_UP))
    {
        if (*cursorPos != 0)
        {
            // Move cursor up
            EraseSelectorArrow(*cursorPos);
            (*cursorPos)--;
            PrintSelectorArrow(*cursorPos);
            moved = TRUE;
        }
        else if (*scrollOffset != 0)
        {
            // Scroll up
            (*scrollOffset)--;
            PrintSearchParameterText(taskId);
            PrintSelectorArrow(*cursorPos);
            moved = TRUE;
        }
        if (moved)
        {
            PlaySE(SE_SELECT);
            EraseAndPrintSearchTextBox(texts[*cursorPos + *scrollOffset].description);
            CopyWindowToVram(0, COPYWIN_GFX);
        }
        return;
    }
    if (JOY_REPEAT(DPAD_DOWN))
    {
        if (*cursorPos < MAX_SEARCH_PARAM_CURSOR_POS && *cursorPos < maxOption)
        {
            // Move cursor down
            EraseSelectorArrow(*cursorPos);
            (*cursorPos)++;
            PrintSelectorArrow(*cursorPos);
            moved = TRUE;
        }
        else if (maxOption > MAX_SEARCH_PARAM_CURSOR_POS && *scrollOffset < maxOption - MAX_SEARCH_PARAM_CURSOR_POS)
        {
            // Scroll down
            (*scrollOffset)++;
            PrintSearchParameterText(taskId);
            PrintSelectorArrow(5);
            moved = TRUE;
        }
        if (moved)
        {
            PlaySE(SE_SELECT);
            EraseAndPrintSearchTextBox(texts[*cursorPos + *scrollOffset].description);
            CopyWindowToVram(0, COPYWIN_GFX);
        }
        return;
    }
}

static void Task_ExitSearch(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_ExitSearchWaitForFade;
}

static void Task_ExitSearchWaitForFade(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        FreeSearchWindowAndBgBuffers();
        DestroyTask(taskId);
    }
}

void SetSearchRectHighlight(u8 flags, u8 x, u8 y, u8 width)
{
    u32 i;
    u32 temp; //should be a pointer, but does not match as one
    u32 ptr = (u32)GetBgTilemapBuffer(3); //same as above

    for (i = 0; i < width; i++)
    {
        temp = *(u16 *)(ptr + (y + 0) * 64 + (x + i) * 2);
        temp &= 0x0fff;
        temp |= (flags << 12);
        *(u16 *)(ptr + (y + 0) * 64 + (x + i) * 2) = temp;

        temp = *(u16 *)(ptr + (y + 1) * 64 + (x + i) * 2);
        temp &= 0x0fff;
        temp |= (flags << 12);
        *(u16 *)(ptr + (y + 1) * 64 + (x + i) * 2) = temp;
    }
}

#define SEARCH_BG_SEARCH                SEARCH_TOPBAR_SEARCH
#define SEARCH_BG_SHIFT                 SEARCH_TOPBAR_SHIFT
#define SEARCH_BG_CANCEL                SEARCH_TOPBAR_CANCEL
//#define SEARCH_BG_NAME                  (SEARCH_NAME + SEARCH_TOPBAR_COUNT)
//#define SEARCH_BG_COLOR                 (SEARCH_COLOR + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_GROUP_SELECTION_LEFT  (SEARCH_GROUP_LEFT + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_GROUP_SELECTION_RIGHT (SEARCH_GROUP_RIGHT + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_TYPE_SELECTION_LEFT   (SEARCH_TYPE_LEFT + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_TYPE_SELECTION_RIGHT  (SEARCH_TYPE_RIGHT + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_ABILITY               (SEARCH_ABILITY + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_MOVE                  (SEARCH_MOVE + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_MOVETYPE              (SEARCH_MOVETYPE + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_ORDER                 (SEARCH_ORDER + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_MODE                  (SEARCH_MODE + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_OK                    (SEARCH_OK + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_GROUP_TITLE           (SEARCH_COUNT + SEARCH_TOPBAR_COUNT)
#define SEARCH_BG_TYPE_TITLE            (SEARCH_COUNT + SEARCH_TOPBAR_COUNT + 1)
#define SEARCH_BG_MOVE_TITLE            (SEARCH_COUNT + SEARCH_TOPBAR_COUNT + 2)

static void DrawSearchMenuItemBgHighlight(u8 searchBg, bool8 unselected, bool8 disabled)
{
    u8 highlightFlags = (unselected & 1) | ((disabled & 1) << 1);

    switch (searchBg)
    {
    case SEARCH_BG_SEARCH:
    case SEARCH_BG_SHIFT:
    case SEARCH_BG_CANCEL:
        SetSearchRectHighlight(highlightFlags, sSearchMenuTopBarItems[searchBg].highlightX, sSearchMenuTopBarItems[searchBg].highlightY, sSearchMenuTopBarItems[searchBg].highlightWidth);
        break;
    //case SEARCH_BG_NAME:
    //case SEARCH_BG_COLOR:
    case SEARCH_BG_ABILITY:
    case SEARCH_BG_ORDER:
    case SEARCH_BG_MODE:
        SetSearchRectHighlight(highlightFlags, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgX, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgY, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgWidth);
        // fall through, draw selectionBg for above
    case SEARCH_BG_TYPE_SELECTION_LEFT:
    case SEARCH_BG_TYPE_SELECTION_RIGHT:
    case SEARCH_BG_GROUP_SELECTION_LEFT:
    case SEARCH_BG_GROUP_SELECTION_RIGHT:
    case SEARCH_BG_MOVE:
    case SEARCH_BG_MOVETYPE:
        SetSearchRectHighlight(highlightFlags, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].selectionBgX, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].selectionBgY, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].selectionBgWidth);
        break;
    case SEARCH_BG_TYPE_TITLE:
        SetSearchRectHighlight(highlightFlags, sSearchMenuItems[SEARCH_TYPE_LEFT].titleBgX, sSearchMenuItems[SEARCH_TYPE_LEFT].titleBgY, sSearchMenuItems[SEARCH_TYPE_LEFT].titleBgWidth);
        break;
    case SEARCH_BG_GROUP_TITLE:
        SetSearchRectHighlight(highlightFlags, sSearchMenuItems[SEARCH_GROUP_LEFT].titleBgX, sSearchMenuItems[SEARCH_GROUP_LEFT].titleBgY, sSearchMenuItems[SEARCH_GROUP_LEFT].titleBgWidth);
        break;
    case SEARCH_BG_MOVE_TITLE:
        SetSearchRectHighlight(highlightFlags, sSearchMenuItems[SEARCH_MOVE].titleBgX, sSearchMenuItems[SEARCH_MOVE].titleBgY, sSearchMenuItems[SEARCH_MOVE].titleBgWidth);
        break;
    case SEARCH_BG_OK:
        /*
        if (!IsNationalPokedexEnabled())
            SetSearchRectHighlight(highlightFlags, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgX, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgY - 2, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgWidth);
        else
        */
        SetSearchRectHighlight(highlightFlags, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgX, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgY, sSearchMenuItems[searchBg - SEARCH_TOPBAR_COUNT].titleBgWidth);
        break;
    }
}

static void SetInitialSearchMenuBgHighlights(u8 topBarItem)
{
    switch (topBarItem)
    {
    case SEARCH_TOPBAR_SEARCH:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_SEARCH, FALSE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_SHIFT, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_CANCEL, TRUE, FALSE);
        //DrawSearchMenuItemBgHighlight(SEARCH_BG_NAME, TRUE, FALSE);
        //DrawSearchMenuItemBgHighlight(SEARCH_BG_COLOR, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_TITLE, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_SELECTION_LEFT, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_SELECTION_RIGHT, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_TITLE, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_SELECTION_LEFT, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_SELECTION_RIGHT, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_ABILITY, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE_TITLE, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVETYPE, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_ORDER, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MODE, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_OK, TRUE, FALSE);
        break;
    case SEARCH_TOPBAR_SHIFT:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_SEARCH, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_SHIFT, FALSE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_CANCEL, TRUE, FALSE);
        //DrawSearchMenuItemBgHighlight(SEARCH_BG_NAME, TRUE, TRUE);
        //DrawSearchMenuItemBgHighlight(SEARCH_BG_COLOR, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_TITLE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_SELECTION_LEFT, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_SELECTION_RIGHT, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_TITLE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_SELECTION_LEFT, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_SELECTION_RIGHT, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_ABILITY, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE_TITLE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVETYPE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_ORDER, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MODE, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_OK, TRUE, FALSE);
        break;
    case SEARCH_TOPBAR_CANCEL:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_SEARCH, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_SHIFT, TRUE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_CANCEL, FALSE, FALSE);
        //DrawSearchMenuItemBgHighlight(SEARCH_BG_NAME, TRUE, TRUE);
        //DrawSearchMenuItemBgHighlight(SEARCH_BG_COLOR, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_TITLE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_SELECTION_LEFT, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_SELECTION_RIGHT, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_TITLE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_SELECTION_LEFT, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_SELECTION_RIGHT, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_ABILITY, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE_TITLE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVETYPE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_ORDER, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MODE, TRUE, TRUE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_OK, TRUE, TRUE);
        break;
    }
}

static void HighlightSelectedSearchTopBarItem(u8 topBarItem)
{
    SetInitialSearchMenuBgHighlights(topBarItem);
    EraseAndPrintSearchTextBox(sSearchMenuTopBarItems[topBarItem].description);
}

static void HighlightSelectedSearchMenuItem(u8 topBarItem, u8 menuItem)
{
    SetInitialSearchMenuBgHighlights(topBarItem);
    switch (menuItem)
    {
    /*
    case SEARCH_NAME:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_NAME, FALSE, FALSE);
        break;
    case SEARCH_COLOR:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_COLOR, FALSE, FALSE);
        break;
    */
    case SEARCH_TYPE_LEFT:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_TITLE, FALSE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_SELECTION_LEFT, FALSE, FALSE);
        break;
    case SEARCH_TYPE_RIGHT:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_TITLE, FALSE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_TYPE_SELECTION_RIGHT, FALSE, FALSE);
        break;
    case SEARCH_GROUP_LEFT:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_TITLE, FALSE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_SELECTION_LEFT, FALSE, FALSE);
        break;
    case SEARCH_GROUP_RIGHT:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_TITLE, FALSE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_GROUP_SELECTION_RIGHT, FALSE, FALSE);
        break;
    case SEARCH_ABILITY:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_ABILITY, FALSE, FALSE);
        break;
    case SEARCH_MOVE:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE_TITLE, FALSE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE, FALSE, FALSE);
        break;
    case SEARCH_MOVETYPE:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVE_TITLE, FALSE, FALSE);
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MOVETYPE, FALSE, FALSE);
        break;
    case SEARCH_ORDER:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_ORDER, FALSE, FALSE);
        break;
    case SEARCH_MODE:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_MODE, FALSE, FALSE);
        break;
    case SEARCH_OK:
        DrawSearchMenuItemBgHighlight(SEARCH_BG_OK, FALSE, FALSE);
        break;
    }
    EraseAndPrintSearchTextBox(sSearchMenuItems[menuItem].description);
}

// Prints the currently selected search parameters in the search menu selection boxes
static void PrintSelectedSearchParameters(u8 taskId)
{
    u16 searchParamId;
    const struct SearchOptionText *searchParamId2;
    u8 truncatedParameter[10];
    u8 i, j;

    ClearSearchMenuRect(40, 16, 96, 96);

    /*
    searchParamId = gTasks[taskId].tCursorPos_Name + gTasks[taskId].tScrollOffset_Name;
    PrintSearchText(sDexSearchNameOptions[searchParamId].title, 0x2D, 0x11); 45, 17

    searchParamId = gTasks[taskId].tCursorPos_Color + gTasks[taskId].tScrollOffset_Color;
    PrintSearchText(sDexSearchColorOptions[searchParamId].title, 0x2D, 0x21); 45, 33
    */
   
    // Type left
    searchParamId = gTasks[taskId].tCursorPos_TypeLeft + gTasks[taskId].tScrollOffset_TypeLeft;
    for (i = 0; i < 6; i++)
        truncatedParameter[i] = sDexSearchTypeOptions[searchParamId].title[i];
    truncatedParameter[i] = EOS;
    PrintSearchText(truncatedParameter, 45, 17);
    //PrintSearchText(sDexSearchTypeOptions[searchParamId].title, 45, 17);

    // Type right
    searchParamId = gTasks[taskId].tCursorPos_TypeRight + gTasks[taskId].tScrollOffset_TypeRight;
    for (i = 0; i < 6; i++)
        truncatedParameter[i] = sDexSearchTypeOptions[searchParamId].title[i];
    truncatedParameter[i] = EOS;
    PrintSearchText(truncatedParameter, 93, 17);
    //PrintSearchText(sDexSearchTypeOptions[searchParamId].title, 93, 17);

    // Group left
    searchParamId = gTasks[taskId].tCursorPos_GroupLeft + gTasks[taskId].tScrollOffset_GroupLeft;
    for (i = 0, j = 0; i < 6 && sDexSearchGroupOptions[searchParamId].title[j] != EOS; i++, j++)
    {
        if (sDexSearchGroupOptions[searchParamId].title[j] == CHAR_SPACE)
            j++;

        truncatedParameter[i] = sDexSearchGroupOptions[searchParamId].title[j];
    }
    truncatedParameter[i] = EOS;
    PrintSearchText(truncatedParameter, 45, 33);

    // Group right
    searchParamId = gTasks[taskId].tCursorPos_GroupRight + gTasks[taskId].tScrollOffset_GroupRight;
    for (i = 0, j = 0; i < 6 && sDexSearchGroupOptions[searchParamId].title[i] != EOS; i++, j++)
    {
        if (sDexSearchGroupOptions[searchParamId].title[j] == CHAR_SPACE)
            j++;

        truncatedParameter[i] = sDexSearchGroupOptions[searchParamId].title[j];
    }
    truncatedParameter[i] = EOS;
    PrintSearchText(truncatedParameter, 93, 33);

    // Ability
    if (gTasks[taskId].tCursorPos_Ability + gTasks[taskId].tScrollOffset_Ability == 0)
    {
        PrintSearchText(sDexSearchAbilityInitialOptions[0].title, 45, 49);
    }
    else
    {
        searchParamId2 = sSearchOptions[SEARCH_COUNT + gTasks[taskId].tCursorPos_Ability + gTasks[taskId].tScrollOffset_Ability].texts;
        searchParamId = gTasks[taskId].tCursorPos_Ability_2 + gTasks[taskId].tScrollOffset_Ability_2;
        PrintSearchText(searchParamId2[searchParamId].title, 45, 49);
    }

    // Move
    if (gTasks[taskId].tCursorPos_Move + gTasks[taskId].tScrollOffset_Move == 0)
    {
        PrintSearchText(sDexSearchMoveInitialOptions[0].title, 45, 65);
    }
    else
    {
        searchParamId2 = sSearchOptions[SEARCH_ABILITY_VWX + gTasks[taskId].tCursorPos_Move + gTasks[taskId].tScrollOffset_Move].texts;
        searchParamId = gTasks[taskId].tCursorPos_Move_2 + gTasks[taskId].tScrollOffset_Move_2;

        for (i = 0; i < 9 && searchParamId2[searchParamId].title[i] != EOS; i++)
            truncatedParameter[i] = searchParamId2[searchParamId].title[i];
        truncatedParameter[i] = EOS;
        PrintSearchText(truncatedParameter, 45, 65);
    }

    // Movetype
    searchParamId = gTasks[taskId].tCursorPos_MoveType + gTasks[taskId].tScrollOffset_MoveType;
    switch(searchParamId)
    {
        case MOVETYPE_ANY:
            PrintSearchText(sDexSearchMoveTypeOptions[searchParamId].title, 109, 65);
            break;
        case MOVETYPE_LEVEL_UP:
            PrintSearchText(sText_Lvl, 109, 65);
            break;
        case MOVETYPE_TMHM:
            PrintSearchText(sText_TM, 109, 65);
            break;
        case MOVETYPE_TUTOR:
            PrintSearchText(sText_Tut, 109, 65);
            break;
        case MOVETYPE_EGG_MOVE:
            PrintSearchText(sText_Egg, 109, 65);
            break;
    }

    // Order
    searchParamId = gTasks[taskId].tCursorPos_Order + gTasks[taskId].tScrollOffset_Order;
    PrintSearchText(sDexOrderOptions[searchParamId].title, 45, 81);

    // Mode
    if (IsNationalPokedexEnabled())
    {
        searchParamId = gTasks[taskId].tCursorPos_Mode + gTasks[taskId].tScrollOffset_Mode;
        for (i = 0, j = 0; i < 7 && sDexModeOptions[searchParamId].title[j] != CHAR_SPACE; i++, j++)
            truncatedParameter[i] = sDexModeOptions[searchParamId].title[j];
        truncatedParameter[i] = EOS;
        PrintSearchText(truncatedParameter, 45, 97);
    }
}

static void DrawOrEraseSearchParameterBox(bool8 erase)
{
    u32 i;
    u32 j;
    u16 *ptr = GetBgTilemapBuffer(3);

    if (!erase)
    {
        *(ptr + 0x11) = 0xC0B;
        for (i = 0x12; i < 0x1F; i++)
            *(ptr + i) = 0x80D;
        for (j = 1; j < 13; j++)
        {
            *(ptr + 0x11 + j * 32) = 0x40A;
            for (i = 0x12; i < 0x1F; i++)
                *(ptr + j * 32 + i) = 2;
        }
        *(ptr + 0x1B1) = 0x40B;
        for (i = 0x12; i < 0x1F; i++)
            *(ptr + 0x1A0 + i) = 0xD;
    }
    else
    {
        for (j = 0; j < 14; j++)
        {
            for (i = 0x11; i < 0x1E; i++)
            {
                *(ptr + j * 32 + i) = 0x4F;
            }
        }
    }
}

// Prints the currently viewable search parameter titles in the right-hand text box
// and the currently selected search parameter description in the bottom text box
static void PrintSearchParameterText(u8 taskId)
{
    const struct SearchOptionText *texts = sSearchOptions[gTasks[taskId].tMenuItem].texts;
    const u16 *cursorPos = &gTasks[taskId].data[sSearchOptions[gTasks[taskId].tMenuItem].taskDataCursorPos];
    const u16 *scrollOffset = &gTasks[taskId].data[sSearchOptions[gTasks[taskId].tMenuItem].taskDataScrollOffset];
    u32 i;
    u32 j;

    ClearSearchParameterBoxText();

    for (i = 0, j = *scrollOffset; i < MAX_SEARCH_PARAM_ON_SCREEN && texts[j].title != NULL; i++, j++)
        PrintSearchParameterTitle(i, texts[j].title);

    EraseAndPrintSearchTextBox(texts[*cursorPos + *scrollOffset].description);
}

static u16 GetSearchModeSelection(u8 taskId, u8 option)
{
    const u16 *cursorPos = &gTasks[taskId].data[sSearchOptions[option].taskDataCursorPos];
    const u16 *scrollOffset = &gTasks[taskId].data[sSearchOptions[option].taskDataScrollOffset];
    u16 id = *cursorPos + *scrollOffset;
    u8 option2;
    const u16 *cursorPos2;
    const u16 *scrollOffset2;
    u16 id2;

    switch (option)
    {
    default:
        return 0;
    case SEARCH_MODE:
        return sPokedexModes[id];
    case SEARCH_ORDER:
        return sOrderOptions[id];
    /*
    case SEARCH_NAME:
        if (id == 0)
            return 0xFF;
        else
            return id;
    case SEARCH_COLOR:
        if (id == 0)
            return 0xFF;
        else
            return id - 1;
    */
    case SEARCH_TYPE_LEFT:
    case SEARCH_TYPE_RIGHT:
        return sDexSearchTypeIds[id];
    case SEARCH_GROUP_LEFT:
    case SEARCH_GROUP_RIGHT:
        return sDexSearchGroupIds[id];
    case SEARCH_ABILITY:
        option2 = SEARCH_COUNT + id;
        cursorPos2 = &gTasks[taskId].data[sSearchOptions[option2].taskDataCursorPos];
        scrollOffset2 = &gTasks[taskId].data[sSearchOptions[option2].taskDataScrollOffset];
        id2 = *cursorPos2 + *scrollOffset2;

        switch (option2)
        {
            case SEARCH_ABILITY_ABC:
                return sDexSearchAbilityIdsABC[id2];
            case SEARCH_ABILITY_DEF:
                return sDexSearchAbilityIdsDEF[id2];
            case SEARCH_ABILITY_GHI:
                return sDexSearchAbilityIdsGHI[id2];
            case SEARCH_ABILITY_JKL:
                return sDexSearchAbilityIdsJKL[id2];
            case SEARCH_ABILITY_MNO:
                return sDexSearchAbilityIdsMNO[id2];
            case SEARCH_ABILITY_PQR:
                return sDexSearchAbilityIdsPQR[id2];
            case SEARCH_ABILITY_STU:
                return sDexSearchAbilityIdsSTU[id2];
            case SEARCH_ABILITY_VWX:
                return sDexSearchAbilityIdsVWX[id2];
            default:
                return sDexSearchAbilityIds[0];
        }
    case SEARCH_MOVE:
        option2 = SEARCH_ABILITY_VWX + id;
        cursorPos2 = &gTasks[taskId].data[sSearchOptions[option2].taskDataCursorPos];
        scrollOffset2 = &gTasks[taskId].data[sSearchOptions[option2].taskDataScrollOffset];
        id2 = *cursorPos2 + *scrollOffset2;

        switch (option2)
        {
            case SEARCH_MOVE_BUG:
                return sDexSearchMoveIdsBug[id2];
            case SEARCH_MOVE_DARK:
                return sDexSearchMoveIdsDark[id2];
            case SEARCH_MOVE_DRAGON:
                return sDexSearchMoveIdsDragon[id2];
            case SEARCH_MOVE_ELECTRIC:
                return sDexSearchMoveIdsElectric[id2];
            case SEARCH_MOVE_FIGHTING:
                return sDexSearchMoveIdsFighting[id2];
            case SEARCH_MOVE_FIRE:
                return sDexSearchMoveIdsFire[id2];
            case SEARCH_MOVE_FLYING:
                return sDexSearchMoveIdsFlying[id2];
            case SEARCH_MOVE_GHOST:
                return sDexSearchMoveIdsGhost[id2];
            case SEARCH_MOVE_GRASS:
                return sDexSearchMoveIdsGrass[id2];
            case SEARCH_MOVE_GROUND:
                return sDexSearchMoveIdsGround[id2];
            case SEARCH_MOVE_ICE:
                return sDexSearchMoveIdsIce[id2];
            case SEARCH_MOVE_NORMAL_AE:
                return sDexSearchMoveIdsNormalAE[id2];
            case SEARCH_MOVE_NORMAL_FL:
                return sDexSearchMoveIdsNormalFL[id2];
            case SEARCH_MOVE_NORMAL_MR:
                return sDexSearchMoveIdsNormalMR[id2];
            case SEARCH_MOVE_NORMAL_S:
                return sDexSearchMoveIdsNormalS[id2];
            case SEARCH_MOVE_NORMAL_TZ:
                return sDexSearchMoveIdsNormalTZ[id2];
            case SEARCH_MOVE_POISON:
                return sDexSearchMoveIdsPoison[id2];
            case SEARCH_MOVE_PSYCHIC:
                return sDexSearchMoveIdsPsychic[id2];
            case SEARCH_MOVE_ROCK:
                return sDexSearchMoveIdsRock[id2];
            case SEARCH_MOVE_STEEL:
                return sDexSearchMoveIdsSteel[id2];
            case SEARCH_MOVE_WATER:
                return sDexSearchMoveIdsWater[id2];
            default:
                return sDexSearchMoveIds[0];
        }
    case SEARCH_MOVETYPE:
        return sDexSearchMoveTypeIds[id];
    }
}

static void SetDefaultSearchModeAndOrder(u8 taskId)
{
    u16 selected;

    switch (sPokedexView->dexModeBackup)
    {
    default:
    case DEX_MODE_HOENN:
        selected = DEX_MODE_HOENN;
        break;
    case DEX_MODE_NATIONAL:
        selected = DEX_MODE_NATIONAL;
        break;
    }
    gTasks[taskId].tCursorPos_Mode = selected;

    switch (sPokedexView->dexOrderBackup)
    {
    default:
    case ORDER_NUMERICAL:
        selected = ORDER_NUMERICAL;
        break;
    case ORDER_ALPHABETICAL:
        selected = ORDER_ALPHABETICAL;
        break;
    case ORDER_WEIGHT:
        selected = ORDER_WEIGHT;
        break;
    //case ORDER_LIGHTEST:
    //    selected = ORDER_LIGHTEST;
    //    break;
    case ORDER_HEIGHT:
        selected = ORDER_HEIGHT;
        break;
    case ORDER_HP:
        selected = ORDER_HP;
        break;
    case ORDER_ATTACK:
        selected = ORDER_ATTACK;
        break;
    case ORDER_DEFENSE:
        selected = ORDER_DEFENSE;
        break;
    case ORDER_SP_ATK:
        selected = ORDER_SP_ATK;
        break;
    case ORDER_SP_DEF:
        selected = ORDER_SP_DEF;
        break;
    case ORDER_SPEED:
        selected = ORDER_SPEED;
        break;
    case ORDER_STATS:
        selected = ORDER_STATS;
        break;
    //case ORDER_SMALLEST:
    //    selected = ORDER_SMALLEST;
    //    break;
    }
    gTasks[taskId].tCursorPos_Order = selected > MAX_SEARCH_PARAM_CURSOR_POS ? MAX_SEARCH_PARAM_CURSOR_POS : selected;
    gTasks[taskId].tScrollOffset_Order = selected - MAX_SEARCH_PARAM_CURSOR_POS > 0 ? selected - MAX_SEARCH_PARAM_CURSOR_POS : 0;
}

static bool8 SearchParamCantScrollUp(u8 taskId)
{
    u8 menuItem = gTasks[taskId].tMenuItem;
    const u16 *scrollOffset = &gTasks[taskId].data[sSearchOptions[menuItem].taskDataScrollOffset];
    u16 lastOption = sSearchOptions[menuItem].numOptions - 1;

    if (lastOption > MAX_SEARCH_PARAM_CURSOR_POS && *scrollOffset != 0)
        return FALSE;
    else
        return TRUE;
}

static bool8 SearchParamCantScrollDown(u8 taskId)
{
    u8 menuItem = gTasks[taskId].tMenuItem;
    const u16 *scrollOffset = &gTasks[taskId].data[sSearchOptions[menuItem].taskDataScrollOffset];
    u16 lastOption = sSearchOptions[menuItem].numOptions - 1;

    if (lastOption > MAX_SEARCH_PARAM_CURSOR_POS && *scrollOffset < lastOption - MAX_SEARCH_PARAM_CURSOR_POS)
        return FALSE;
    else
        return TRUE;
}

#define sTaskId      data[0]

static void SpriteCB_SearchParameterScrollArrow(struct Sprite *sprite)
{
    if (gTasks[sprite->sTaskId].func == Task_HandleSearchParameterInput)
    {
        u8 val;

        if (sprite->sIsDownArrow)
        {
            if (SearchParamCantScrollDown(sprite->sTaskId))
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
        }
        else
        {
            if (SearchParamCantScrollUp(sprite->sTaskId))
                sprite->invisible = TRUE;
            else
                sprite->invisible = FALSE;
        }
        val = sprite->data[2] + sprite->sIsDownArrow * 128;
        sprite->y2 = gSineTable[val] / 128;
        sprite->data[2] += 8;
    }
    else
    {
        sprite->invisible = TRUE;
    }
}

static void CreateSearchParameterScrollArrows(u8 taskId)
{
    u8 spriteId;

    spriteId = CreateSprite(&sScrollArrowSpriteTemplate, 184, 4, 0);
    gSprites[spriteId].sTaskId = taskId;
    gSprites[spriteId].sIsDownArrow = FALSE;
    gSprites[spriteId].callback = SpriteCB_SearchParameterScrollArrow;

    spriteId = CreateSprite(&sScrollArrowSpriteTemplate, 184, 108, 0);
    gSprites[spriteId].sTaskId = taskId;
    gSprites[spriteId].sIsDownArrow = TRUE;
    gSprites[spriteId].vFlip = TRUE;
    gSprites[spriteId].callback = SpriteCB_SearchParameterScrollArrow;
}

#undef sTaskId
#undef sIsDownArrow

static void EraseAndPrintSearchTextBox(const u8 *str)
{
    ClearSearchMenuRect(8, 120, 224, 32);
    PrintSearchText(str, 8, 121);
}

static void EraseSelectorArrow(u32 y)
{
    ClearSearchMenuRect(144, y * 16 + 8, 8, 16);
}

static void PrintSelectorArrow(u32 y)
{
    PrintSearchText(gText_SelectorArrow, 144, y * 16 + 9);
}

static void PrintSearchParameterTitle(u32 y, const u8 *str)
{
    PrintSearchText(str, 152, y * 16 + 9);
}

static void ClearSearchParameterBoxText(void)
{
    ClearSearchMenuRect(144, 8, 96, 96);
}
