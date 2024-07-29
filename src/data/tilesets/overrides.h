static const u16 sTilesetPalOverride_General01_05[] = INCBIN_U16("data/tilesets/primary/general/override_palettes/01_05.gbapal");

static const u16 sTilesetPalOverride_Petalburg06_08_09[] = INCBIN_U16("data/tilesets/secondary/petalburg/override_palettes/06_08_09.gbapal");
static const u16 sTilesetPalOverride_Petalburg12[] = INCBIN_U16("data/tilesets/secondary/petalburg/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Rustboro06[] = INCBIN_U16("data/tilesets/secondary/rustboro/override_palettes/06.gbapal");
static const u16 sTilesetPalOverride_Rustboro08[] = INCBIN_U16("data/tilesets/secondary/rustboro/override_palettes/08.gbapal");
static const u16 sTilesetPalOverride_Rustboro12[] = INCBIN_U16("data/tilesets/secondary/rustboro/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Dewford12[] = INCBIN_U16("data/tilesets/secondary/dewford/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Slateport06[] = INCBIN_U16("data/tilesets/secondary/slateport/override_palettes/06.gbapal");
static const u16 sTilesetPalOverride_Slateport08[] = INCBIN_U16("data/tilesets/secondary/slateport/override_palettes/08.gbapal");
static const u16 sTilesetPalOverride_Slateport10[] = INCBIN_U16("data/tilesets/secondary/slateport/override_palettes/10.gbapal");
static const u16 sTilesetPalOverride_Slateport12[] = INCBIN_U16("data/tilesets/secondary/slateport/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Mauville07[] = INCBIN_U16("data/tilesets/secondary/mauville/override_palettes/07.gbapal");
static const u16 sTilesetPalOverride_Mauville10[] = INCBIN_U16("data/tilesets/secondary/mauville/override_palettes/10.gbapal");
static const u16 sTilesetPalOverride_Mauville11[] = INCBIN_U16("data/tilesets/secondary/mauville/override_palettes/11.gbapal");
static const u16 sTilesetPalOverride_Mauville12[] = INCBIN_U16("data/tilesets/secondary/mauville/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Lavaridge09[] = INCBIN_U16("data/tilesets/secondary/lavaridge/override_palettes/09.gbapal");
static const u16 sTilesetPalOverride_Lavaridge12[] = INCBIN_U16("data/tilesets/secondary/lavaridge/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Fallarbor08[] = INCBIN_U16("data/tilesets/secondary/fallarbor/override_palettes/08.gbapal");
static const u16 sTilesetPalOverride_Fallarbor12[] = INCBIN_U16("data/tilesets/secondary/fallarbor/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Fortree07[] = INCBIN_U16("data/tilesets/secondary/fortree/override_palettes/07.gbapal");
static const u16 sTilesetPalOverride_Fortree12[] = INCBIN_U16("data/tilesets/secondary/fortree/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Lilycove09[] = INCBIN_U16("data/tilesets/secondary/lilycove/override_palettes/09.gbapal");
static const u16 sTilesetPalOverride_Lilycove11[] = INCBIN_U16("data/tilesets/secondary/lilycove/override_palettes/11.gbapal");
static const u16 sTilesetPalOverride_Lilycove12[] = INCBIN_U16("data/tilesets/secondary/lilycove/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Pacifidlog09[] = INCBIN_U16("data/tilesets/secondary/pacifidlog/override_palettes/09.gbapal");
static const u16 sTilesetPalOverride_Pacifidlog12[] = INCBIN_U16("data/tilesets/secondary/pacifidlog/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Mossdeep06[] = INCBIN_U16("data/tilesets/secondary/mossdeep/override_palettes/06.gbapal");
static const u16 sTilesetPalOverride_Mossdeep09[] = INCBIN_U16("data/tilesets/secondary/mossdeep/override_palettes/09.gbapal");
static const u16 sTilesetPalOverride_Mossdeep12[] = INCBIN_U16("data/tilesets/secondary/mossdeep/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_Sootopolis06[] = INCBIN_U16("data/tilesets/secondary/sootopolis/override_palettes/06.gbapal");
static const u16 sTilesetPalOverride_Sootopolis12[] = INCBIN_U16("data/tilesets/secondary/sootopolis/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_EverGrande08[] = INCBIN_U16("data/tilesets/secondary/ever_grande/override_palettes/08.gbapal");
static const u16 sTilesetPalOverride_EverGrande12[] = INCBIN_U16("data/tilesets/secondary/ever_grande/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_BattleFrontierOutsideWest12[] = INCBIN_U16("data/tilesets/secondary/battle_frontier_outside_west/override_palettes/12.gbapal");

static const u16 sTilesetPalOverride_BattleFrontierOutsideEast12[] = INCBIN_U16("data/tilesets/secondary/battle_frontier_outside_east/override_palettes/12.gbapal");

#define OVERRIDES_END { .slot = 0xFF, }

const struct PaletteOverride gTilesetPalOverrides_General[] =
{
    {
        .slot = 1,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_General01_05,
    },
    {
        .slot = 5,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_General01_05,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Petalburg[] =
{
    {
        .slot = 6,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Petalburg06_08_09,
    },
    {
        .slot = 8,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Petalburg06_08_09,
    },
    {
        .slot = 9,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Petalburg06_08_09,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Petalburg12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Rustboro[] =
{
    {
        .slot = 6,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Rustboro06,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Rustboro12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Dewford[] =
{
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Dewford12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Slateport[] =
{
    {
        .slot = 6,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Slateport06,
    },
    {
        .slot = 8,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Slateport08,
    },
    {
        .slot = 10,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Slateport10,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Slateport12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Mauville[] =
{
    {
        .slot = 7,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Mauville07,
    },
    {
        .slot = 10,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Mauville10,
    },
    {
        .slot = 11,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Mauville11,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Mauville12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Lavaridge[] =
{
    {
        .slot = 9,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Lavaridge09,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Lavaridge12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Fallarbor[] =
{
    {
        .slot = 8,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Fallarbor08,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Fallarbor12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Fortree[] =
{
    {
        .slot = 7,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Fortree07,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Fortree12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Lilycove[] =
{
    {
        .slot = 9,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Lilycove09,
    },
    {
        .slot = 11,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Lilycove11,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Lilycove12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Pacifidlog[] =
{
    {
        .slot = 9,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Pacifidlog09,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Pacifidlog12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Mossdeep[] =
{
    {
        .slot = 6,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Mossdeep06,
    },
    {
        .slot = 9,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Mossdeep09,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Mossdeep12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_Sootopolis[] =
{
    {
        .slot = 6,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Sootopolis06,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_Sootopolis12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_EverGrande[] =
{
    {
        .slot = 8,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_EverGrande08,
    },
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_EverGrande12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_BattleFrontierOutsideWest[] =
{
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_BattleFrontierOutsideWest12,
    },
    OVERRIDES_END
};

const struct PaletteOverride gTilesetPalOverrides_BattleFrontierOutsideEast[] =
{
    {
        .slot = 12,
        .startHour = HOUR_NIGHT,
        .endHour = HOUR_MORNING,
        .palette = sTilesetPalOverride_BattleFrontierOutsideEast12,
    },
    OVERRIDES_END
};