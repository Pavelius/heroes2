#include "collection.h"
#include "crt.h"
#include "point.h"
#include "stringbuilder.h"

enum index_s : unsigned short {
	Blocked = 0xFFFF,
};
enum path_s : unsigned {
	BlockedPath = 0xFFFFFFFF
};
enum kind_s : unsigned char {
	Barbarian, Knight, Necromancer, Sorcerer, Warlock, Wizard,
	RandomKind,
};
enum ability_s : unsigned char {
	Attack, Defence, SpellPower, Knowledge,
	LuckStat, MoraleStat, Speed,
	DamageMin, DamageMax,
	HitPoints, HitPointsMax,
	Shoots, ShootsMax,
	AllAbilities, AttackDefence, SpellPowerKnowledge,
};
enum skill_s : unsigned char {
	Pathfinding, Archery, Logistics, Scounting, Diplomacy,
	Navigation, Leadership, WisdomSkill, Mysticism, Luck,
	Ballistics, EagleEye, Necromancy, Estates,
	FirstSkill = Pathfinding, LastSkill = Estates,
};
enum spell_s : unsigned char {
	FireBall, FireBlast, LightingBolt, ChainLighting, Teleport,
	Cure, MassCure, Resurrect, RessurectTrue, Haste, MassHaste, Slow, MassSlow,
	Blind, Bless, MassBless, StoneSkin, SteelSkin, Curse, MassCurse,
	HolyWord, HolyShout, Antimagic, Dispel, MassDispel, MagicArrow, Berserker,
	Armagedon, ElementalStorm, MeteorShower, Paralyze, Hypnotize, ColdRay, ColdRing,
	DisruptingRay, DeathRipple, DeathWave, DragonSlayer, BloodLust, AnimateDead, MirrorImage,
	Shield, MassShield,
	SummonEarthElemental, SummonAirElemental, SummonFireElemental, SummonWaterElemental,
	Earthquake,
	ViewMines, ViewResources, ViewArtifacts, ViewTowns, ViewHeroes, ViewAll,
	IdentifyHero, SummonBoat,
	DimensionDoor, TownGate, TownPortal, Visions,
	Haunt, SetEartGuadrdian, SetAirGuadrdian, SetFireGuadrdian, SetWaterGuadrdian,
	Stone,
	FirstSpell = FireBall, LastSpell = Stone,
};
enum resource_s : unsigned char {
	Gold, Wood, Mercury, Ore, Sulfur, Crystal, Gems,
	FirstResource = Gold, RandomResource,
};
enum luck_s : unsigned char {
	CursedLuck, AwfulLuck, BadLuck, NormalLuck, GoodLuck, GreatLuck, IrishLuck,
};
enum morale_s : unsigned char {
	TreasonMorale, AwfulMorale, PoorMorale, NormalMorale, GoodMorale, GreatMorale, BloodMorale,
};
enum speed_s : unsigned char {
	CrawlingSpeed, VerySlowSpeed, SlowSpeed, AverageSpeed, FastSpeed, VeryFastSpeed, UltraFastSpeed,
};
enum player_s : unsigned char {
	PlayerBlue, PlayerGreen, PlayerRed, PlayerYellow, PlayerOrange, PlayerPurple,
	FirstPlayer = PlayerBlue, LastPlayer = PlayerPurple,
	RandomPlayer, MultiPlayer
};
enum artifact_s : unsigned char {
	UltimateBook, UltimateSword, UltimateCloack, UltimateWand, UltimateShied, UltimateStaff, UltimateCrown, GoldenGoose,
	ArcaneNecklage, CasterBracers, MageRing, WitchesBroach,
	MedalValor, MedalCourage, MedalHonor, MedalDistinction, FizbinMesfortune,
	ThunderMace, ArmouredGauntlets, DefenderHelm, GiantFlail, Ballista, StealthShield,
	DragonSword, PowerAxe, DivineBreastPlate,
	MinorScroll, MajorScroll, SuperiorScroll, ForemostScroll,
	EndlessSackOfGold, EndlessBagOfGold, EndlessPurseOfGold,
	NomadBootsOfMobility, TravellerBootsOfMobility,
	RabbitFoot, GoldenHorseshoe, GamblerLuckCoin, fourLeafClover,
	TrueCompassMobility, SailorsAstrolableMobility,
	EvilEye, EnchantedHourglass, GoldenWatch, SkullCap, IceCloack, FireCloack, LightingHelm,
	EvercoldIcicle, EverhotLavaRock, LightingRod,
	SnakeRing,
	Ankh, BookElements, ElementalRing,
	PedantHoly, PendantFreeWill, PendantLife, PendantSerenity, PendantSeekingEye, PendantKinetic, PendantDeath,
	WandNegation, GoldenBow, Telescope, StatemanQuill, WizardHat, PowerRing, AmmoCart,
	TaxLien, HideousMask,
	EndlessPouchSulfur, EndlessPouchMercury, EndlessPouchGems, EndlessCordWood, EndlessCartOre, EndlessPouchCrystal,
	SpikedHelm, SpikedShield,
	WhitePearl, BlackPearl,
	MagicBook,
	FirstArtifact = UltimateBook, LastArtifact = MagicBook,
	NoArtifact,
};
enum building_s : unsigned char {
	Castle, ThievesGuild, Tavern, Shipyard, Well, Statue,
	LeftTurret, RightTurret, Moat,
	MarketPlace, Captain, Well2, SpecialBuilding,
	Dwelving1, Dwelving2, Dwelving3, Dwelving4, Dwelving5, Dwelving6,
	MageGuild, Tent,
	Dwelving1u, Dwelving2u, Dwelving3u, Dwelving4u, Dwelving5u, Dwelving6u, Dwelving6u2,
	MageGuild2, MageGuild3, MageGuild4, MageGuild5,
	NoBuilding,
	FirstBuilding = Castle, LastBuilding = NoBuilding,
};
enum monster_s : unsigned char {
	Peasant, Archer, Ranger, Pikeman, VeteranPikeman, Swordsman, MasterSwordsman,
	Cavalry, Champion, Paladin, Crusader,
	Goblin, Orc, OrcChief, Wolf, Ogre, OgreLord,
	Troll, WarTroll, Cyclop,
	Sprite, Dwarf, BattleDwarf, Elf, GrandElf, Druid, GreaterDruid,
	Unicorn, Phoenix,
	Centaur, Gargoyle, Griffin, Minotaur, MinotaurKing,
	Hydra, GreenDragon, RedDragon, BlackDragon,
	Halfling, Boar, IronGolem, SteelGolem, Roc,
	Mage, ArchMage, Giant, Titan,
	Skeleton, Zombie, MutantZombie, Mummy, RoyalMummy, Vampire, VampireLord,
	Lich, PowerLich, BoneDragon,
	Rogue, Nomand, Ghost, Genie, Medusa,
	EarthElement, AirElement, FireElement, WaterElement,
	MonsterRnd1, MonsterRnd2, MonsterRnd3, MonsterRnd4,
	RandomMonster,
	FirstMonster = Peasant, LastMonster = RandomMonster,
};
enum hero_s : unsigned char {
	LordKillburn, SirGallanth, Ector, Gvenneth, Tyro, Ambrose, Ruby, Maximus, Dimitry,
	Thundax, Fineous, Jojosh, CragHack, Jezebel, Jaclyn, Ergon, Tsabu, Atlas,
	Astra, Natasha, Troyan, Vatawna, Rebecca, Gem, Ariel, Carlawn, Luna,
	Arie, Alamar, Vesper, Crodo, Barok, Kastore, Agar, Falagar, Wrathmont,
	Myra, Flint, Dawn, Halon, Myrini, Wilfrey, Sarakin, Kalindra, Mandigal,
	Zom, Darlana, Zam, Ranloo, Charity, Rialdo, Roxana, Sandro, Celia,
	Roland, Corlagon, Eliza, Archibald, Halton, Bax,
	FirstHero = LordKillburn, LastGameHero = Celia, LastHero = Bax,
	CaptainBarbarian, CaptainKnight, CaptainNecromancer, CaptainSorcerer, CaptainWarlock, CaptainWizard,
	RandomHero
};
enum object_s : unsigned char {
	WaterChest, AlchemyLab, Sign, WaterBue, // 0x80-0x83
	DeadSkeleton, DemonCave, TreasureChest, FaerieRing, // 0x84-0x87
	CampFire, Fountain, Gazebo, AncientLamp, // 0x88-0x8B
	Graveyard, ArcherHouse, GoblinHut, DwarfCottage, // 0x8C-0x8F
	PeasantHut, Map0x91, Map0x92, Event, // 0x90-0x93
	DragonCity, LitghHouse, WaterWheel, Mines, // 0x95-0x97
	MonsterObject, Obelisk, Oasis, ResourceObject, // 0x98-0x9B
	Map0x9C, SawMill, Oracle, Shrine1, // 0x9C-0x9F
	ShipWreck, Map0xA1, DesertTent, CastleObject, // 0xA0-0xA3
	Teleporter, RogueCamp, Map0xA6, WhirlPool, // 0xA4-0xA7
	WindMill, ArtifactObject, Map0xAA, Boat, // 0xA8-0xAB
	RndUltimateArtifact, RndArtifact, RndResource, RndMonster, // 0xAC-0xAF
	RndTown, RndCastle, Map0xB2, RndMonster1, // 0xB0-0xB3
	RndMonster2, RndMonster3, RndMonster4, HeroObject, // 0xB4-0xB7
	Map0xB8, Map0xB9, WatchTower, SpriteHouse, // 0xB8-0xBB
	SpriteCity, Ruins, Fort, TradingPost, // 0xBC-0xBF
	AbandoneMine, ThatchedHut, StandingStones, Idol, // 0xC0-xC3
	TreeKnowledge, DoctorHut, Temple, HillFort, // 0xC4-0xC7
	HalflingHole, MercenaryCamp, Shrine2, Shrine3, // 0xC8-0xCB
	Pyramid, CityDead, Excavation, Sphinx, // 0xCC-0xCF
	Wagon, Map0xD1, ArtesianSpring, TrollBridge, // 0xD0-0xD3
	WateringHole, WitchHut, Xanadu, CentautCave, // 0xD4-0xD7
	LeanTo, WaterBoatStation, Flotsam, DeRelictShip, // 0xD8-0xDB
	ShipWreckSurvivor, Bottle, MagicWell, MagicGarden, // 0xDC-0xDF
	ObservationTower, FreemanFoundry, Map0xE2, Map0xE3, // 0xE0-0xE3
	Map0xE4, Map0xE5, Map0xE6, Map0xE7, // 0xE4-0xE7
	Map0xE8, Reefs, Map0xEA, Map0xEB, // 0xE8-0xEB
	Mermaid, Sirens, MagiHut, MagiEye, // 0xEC-0xEF
	AlchemyTower, Stables, Arena, BarrowMounds, // 0xF0-0xF3
	RndArtifact1, RndArtifact2, RndArtifact3, Barrier, // 0xF4-0xF7
	TravellerTent, Map0xF9, Map0xFA, Jail, // 0xF8-0xFB
	FireAltar, AirAltar, EarthAltar, WaterAltar, // 0xFC-0xFF
	FirstObject = WaterChest, LastObject = WaterAltar,
	// Standart landscape objects
	Brush, Cactus, Cliff, Crack, Flowers, Hill, Hole, Lake, Mountains, Mushrooms, Rock, Trees, Volcano,
	Stream, Road, StreamDelta, Stumps, DiggingHole,
	EmpthyObject,
};
enum tag_s : unsigned char {
	Air, Cold, Dragon, Earth, Fire, Lighting, Water, Undead,
	Friendly, Hostile,
	Area, LargeArea, Mass, SafeCenter,
	Damage, Enchantment, Summon,
	Fly, Twice, Stealth, MeleeArcher, Wide,
};
enum action_s : unsigned char {
	Wait, Warn, Move, PalmFace, AttackAction, FlyAction, Shoot, Damaged, Dead, Killed,
};
enum map_flag_s : unsigned char {
	AttackTile, BlockedTile, ActionTile,
};
enum spell_type_s : unsigned char {
	AnySpell,
	CombatSpell, AdventureSpell
};
enum variant_s : unsigned char {
	NoVariant,
	Ability, Artifact, CastleVar, Hero, Index, Landscape, Object, Monster, Moveable,
	Player, Resource, Skill, Spell, Stat, Tag,
};
enum activity_s : unsigned char {
	NotAllowed,
	Human, Computer, ComputerOnly
};
enum level_s : unsigned char {
	LevelEasy, LevelNormal, LevelHard, LevelExpert,
};
enum difficult_s : unsigned char {
	EasyDifficulty, NormalDifficulty, HardDifficulty, VeryHardDifficulty, ImpossibleDifficulty
};
enum size_s : unsigned char {
	NoSize,
	SmallSize, MediumSize, LargeSize, XLargeSize,
};
enum loss_s : unsigned char {
	LossNone, LossHero, LossTown, LossTime
};
enum direction_s : unsigned char {
	Left, LeftUp, Up, RightUp, Right, RightDown, Down, LeftDown,
};
enum armysize_s {
	Few, Several, Pack, Lots, Horde, Throng, Swarm, Zounds, Legion
};
enum landscape_s : unsigned char {
	Beach, Desert, Dirt, Grass, Lava, Sea, Snow, Swamp, Waste,
};
enum castle_flag_s : unsigned char {
	AlreadyMoved,
};
class heroi;
struct variant {
	variant_s				type;
	union {
		ability_s			ability;
		artifact_s			artifact;
		hero_s				hero;
		landscape_s			landscape;
		skill_s				skill;
		spell_s				spell;
		resource_s			resource;
		tag_s				tag;
		object_s			object;
		monster_s			monster;
		unsigned char		value;
	};
	constexpr variant() : type(NoVariant), value(0) {}
	constexpr variant(variant_s t, unsigned char v) : type(t), value(v) {}
	constexpr variant(ability_s v) : type(Ability), ability(v) {}
	constexpr variant(artifact_s v) : type(Artifact), artifact(v) {}
	constexpr variant(hero_s v) : type(Hero), hero(v) {}
	constexpr variant(landscape_s v) : type(Landscape), landscape(v) {}
	constexpr variant(object_s v) : type(Object), object(v) {}
	constexpr variant(monster_s v) : type(Monster), monster(v) {}
	constexpr variant(resource_s v) : type(Resource), resource(v) {}
	constexpr variant(skill_s v) : type(Skill), skill(v) {}
	constexpr variant(spell_s v) : type(Spell), spell(v) {}
	constexpr variant(tag_s v) : type(Tag), tag(v) {}
	constexpr bool operator==(const variant& e) const { return type == e.type && value == e.value; }
	constexpr explicit operator bool() const { return type != NoVariant; }
};
struct resourcei {
	const char*				name;
	const char*				nameof;
};
struct armysizei {
	const char*				name;
	int						count;
	static const armysizei* find(int value);
	static armysize_s		get(int value);
};
struct costi {
	int						data[Gems + 1];
	bool operator==(const costi& e) const;
	bool operator<=(const costi& e) const;
	bool operator>=(const costi& e) const;
	void operator-=(const costi& e);
	costi operator*(int v);
	void					clear();
	void					correct();
	int						get(resource_s v) const { return data[v]; }
	int						mindiv(const costi& e) const;
	void					paint(int x, int y) const;
};
struct namei {
	char					name[32];
	constexpr operator bool() const { return name[0] != 0; }
	const char*				getname() const { return name; }
	void					setname(const char* v) { zcpy(name, v, sizeof(name) - 1); }
};
struct leveli {
	const char*				name;
};
struct speedi {
	const char*				name;
};
struct moralei {
	const char*				name;
};
struct lucki {
	const char*				name;
};
struct difficulti {
	const char*				name;
	difficult_s				opposition;
	costi					resources;
	int						rating;
};
struct squadi {
	monster_s				unit;
	unsigned short			count;
	constexpr explicit operator bool() const { return count != 0; }
	bool					canupgrade() const { return getupgrade() != 0; }
	void					clear();
	int						get(ability_s v) const;
	int						get(ability_s v, const heroi* hero) const;
	monster_s				getupgrade() const;
	bool					is(tag_s v) const;
	void					information(const heroi* hero) { show(hero, true, false, false); }
	void					paint(int x, int y, const heroi* hero = 0, bool allow_change = true) const;
	void					show(const heroi* hero, bool info_mode, bool allow_dismiss, bool allow_upgrade);
};
struct armyi {
	squadi					units[5];
	squadi*					add(monster_s v, short unsigned count);
	const squadi*			find(monster_s v) const;
	bool					is(monster_s v) const { return find(v) != 0; }
	bool					is(tag_s v) const;
	void					paint(int x, int y, const heroi* hero = 0, bool allow_change = true, bool clean_current_unit = true) const;
	void					paintsmall(const rect& rc, bool show_count, bool show_text) const;
};
class playeri : public namei {
	kind_s					kind;
	activity_s				activity;
	costi					resources;
	hero_s					heroes[2];
public:
	constexpr explicit operator bool() const { return activity == Human || activity == Computer; }
	void					adventure();
	void					clear();
	static void				endturn();
	static void				initialize();
	int						getadventurers() const;
	activity_s				getactivity() const { return activity; }
	int						getbuildings(building_s v) const;
	int						getcastles() const;
	int						getheroes() const;
	heroi*					gethire(int index) const;
	player_s				getid() const { return player_s(this - bsmeta<playeri>::elements); }
	kind_s					getkind() const { return kind; }
	int						getmarket() const;
	static int				getrate(resource_s resf, resource_s rest, int markets);
	costi&					getresources() { return resources; }
	int						getspies() const;
	int						gettowns() const;
	void					marketplace();
	static void				prepare(difficult_s difficult);
	void					quickmessage(const costi& cost, const char* format, ...);
	static heroi*			randomhire(kind_s kind, kind_s excude_kind);
	heroi*					randomhire(int index) const;
	bool					recruit(monster_s unit, int& count, int maximum);
	void					set(activity_s v) { activity = v; }
	void					set(kind_s v) { kind = v; }
	void					sethire(int index);
	void					setup(difficult_s id);
	void					tavern();
	void					tooltips(const char* format, ...);
	void					thieves();
	void					trade(resource_s rs, resource_s rt, int count, int market_count);
};
struct abilityi {
	const char*				name;
};
struct kindi {
	const char*				name;
	const char*				name_abbr;
	unsigned char			abilities[4];
	skill_s					skills[2];
	monster_s				units[2];
	spell_s					spell;
};
struct landscapei {
	const char*				name;
};
struct skilli {
	const char*				id;
	const char*				name;
};
struct artifacti {
	int						effect;
	variant					type;
	int						level;
	const char*				name;
	const char*				description;
	const char*				text;
};
class spellbooki {
	static constexpr unsigned size = sizeof(unsigned);
	unsigned				data[3];
public:
	constexpr bool			is(spell_s v) const { return (data[v / size] & (1 << (v % 32))) != 0; }
	void					remove(spell_s v) { data[v / size] &= ~(1 << (v % 32)); }
	void					set(spell_s v) { data[v / size] |= (1 << (v % 32)); }
};
struct moveablei {
	short unsigned			index;
	variant					element;
	short unsigned			value;
	player_s				player;
	unsigned char			value2;
	explicit constexpr operator bool() const { return index != Blocked; }
	void					blockpath(unsigned* path) const;
	void					clear();
};
class heroi : public namei, public armyi {
	kind_s					kind;
	unsigned char			level;
	short unsigned			spell_points;
	short unsigned			move_points;
	short unsigned			index;
	short unsigned			index_move;
	unsigned char			portrait;
	player_s				player;
	unsigned				experience;
	artifact_s				artifacts[14];
	unsigned char			abilities[Knowledge + 1];
	unsigned char			skills[LastSkill + 1];
	spellbooki				spellbook;
	direction_s				direction;
	static void				open_artifact();
public:
	void					add(artifact_s id);
	void					add(monster_s id, short unsigned count) { armyi::add(id, count); }
	void					clear();
	static const costi		cost;
	static heroi*			find(short unsigned index);
	playeri*				getplayer() const;
	int						get(ability_s v) const;
	int						get(skill_s v) const { return skills[v]; }
	int						getcost(spell_s v) const;
	direction_s				getdirection() const { return direction; }
	hero_s					getid() const { return hero_s(this - bsmeta<heroi>::elements); }
	kind_s					getkind() const;
	unsigned char			getportrait() const { return portrait; }
	short unsigned			getpos() const { return index; }
	static void				initialize();
	void					input(const playeri* player) const;
	bool					is(spell_s v) const { return spellbook.is(v); }
	bool					isadventure() const { return index != Blocked; }
	static unsigned			select(heroi** result, heroi** result_maximum, const playeri* player, kind_s kind, kind_s kind_exclude, bool include_special = false);
	void					set(direction_s v) { direction = v; }
	void					set(skill_s id, int v) { skills[id] = v; }
	void					set(spell_s id) { spellbook.set(id); }
	void					set(playeri* v) { if(v) player = v->getid(); else player = RandomPlayer; }
	void					set(player_s v) { player = v; }
	void					setportrait(unsigned char i) { portrait = i; }
	void					setpos(short unsigned i) { index = i; }
	void					show(bool allow_change = true) const;
	void					showbook(spell_type_s mode);
};
class castlei : public namei, public armyi {
	cflags<building_s>		buildings;
	spellbooki				mageguild;
	kind_s					kind;
	player_s				player;
	short unsigned			index;
	short unsigned			population[6];
	building_s				getupgraded(building_s v) const;
	void					paint_panel(int x, int y, const heroi* hero) const;
	void					paint_panorama(int x, int y) const;
	void					paint_name() const;
	void					paint_monster(int x, int y, int height, int width, int level);
	cflags<castle_flag_s, unsigned char> flags;
public:
	void					add(monster_s id, unsigned short count) { armyi::add(id, count); }
	void					build();
	bool					build(building_s building, bool confirm);
	void					clear();
	static castlei*			find(const playeri* player, castlei* first = 0);
	static building_s		getbase(building_s v);
	static const costi&		getcost(building_s v, kind_s k);
	static const char*		getdescription(building_s v, kind_s k);
	static building_s		getdowngrade(building_s v);
	static unsigned char	getframe(building_s v);
	int						getgrowth(building_s building) const;
	kind_s					getkind() const { return kind; }
	static monster_s		getmonster(building_s building, kind_s kind);
	playeri*				getplayer() const { return bsmeta<playeri>::elements + player; }
	short unsigned			getpos() const { return index; }
	static cflags<building_s> getprereqisit(building_s v, kind_s k);
	static int				getstatueincome() { return 250; }
	static int				gettavernmorale() { return 1; }
	static int				getwellgrow() { return 2; }
	static int				getwell2grow() { return 8; }
	static building_s		getupgrade(building_s v);
	static building_s		getupgrade(building_s v, kind_s k);
	static void				initialize();
	void					input(const playeri* player) const;
	constexpr int			is(building_s v) const { return buildings.is(v); }
	bool					is(monster_s v) const { return armyi::is(v); }
	bool					is(castle_flag_s v) const { return flags.is(v); }
	bool					isallow(monster_s v) const;
	bool					iscoastal() const;
	static void				information(building_s v, kind_s k);
	void					paint(const heroi* hero) const;
	static void				paint(int x, int y, landscape_s tile, kind_s race, bool castle, bool shadow);
	void					random(bool castle);
	void					recruit(building_s building);
	void					set(castle_flag_s v) { flags.add(v); }
	void					set(const playeri* v) { player = (v ? v->getid() : RandomPlayer); }
	void					set(player_s v) { player = v; }
	void					set(building_s v) { buildings.add(v); }
	void					set(kind_s v) { kind = v; }
	void					setpos(short unsigned v) { index = v; }
	void					show();
	void					well();
	void					remove(building_s v) { buildings.remove(v); }
	void					remove(castle_flag_s v) { flags.remove(v); }
	void					growth();
};
struct spelli {
	const char*				name;
	unsigned char			points;
	unsigned char			level;
	unsigned char			portrait;
	int						power;
	cflags<tag_s>			tags;
	spell_type_s			type;
	const char*				description;
};
struct monsteri {
	kind_s					type;
	unsigned char			level;
	unsigned char			animtype;
	unsigned char			attack;
	unsigned char			defence;
	unsigned char			damageMin;
	unsigned char			damageMax;
	unsigned short			hp;
	unsigned char			speed;
	unsigned char			grown;
	unsigned char			shoots;
	cflags<tag_s>			tags;
	const char*				name;
	const char*				multiname;
	costi					cost;
	building_s				building;
	monster_s				upgrade;
	int						rating;
	//
	constexpr bool			isarcher() const { return shoots != 0; }
	monster_s				getid() const;
};
struct gamei {
	char					file[32];
	char					name[16];
	char					description[143];
	size_s					size;
	unsigned char			wins;
	loss_s					loss;
	activity_s				types[6];
	kind_s					races[6];
	level_s					level;
	difficult_s				difficult;
	bool					start_hero;
	//
	operator bool() const { return file[0] != 0; }
	bool					choose();
	void					clear();
	int						getplayers() const;
	bool					isallow(int index) const { return types[index] != NotAllowed; }
	static bool				isresource(unsigned char object);
	bool					load(const char* filename);
	static void				newgame();
	void					prepare();
	bool					setupmap();
private:
	void					updatebase();
};
struct objecti {
	const char*				name;
};
struct hightscore {
	char					name[32];
	char					map[32];
	int						days;
	int						score;
	monster_s				monster;
	static void				show();
};
struct rumori {
	char					text[128];
	void					settext(const char* v) { zcpy(text, v, sizeof(text) - 1); }
};
struct variantcol {
	variant					element;
	int						count;
};
struct pvar : variant {
	union {
		castlei*			castle;
		heroi*				hero;
		playeri*			player;
		squadi*				squad;
		moveablei*			moveable;
		int					value;
	};
	constexpr pvar() : variant(), value(0) {}
	template<class T> constexpr pvar(const T v) : variant(v) {}
	constexpr pvar(castlei* v) : variant(CastleVar, 0), castle(v) {}
	constexpr pvar(heroi* v) : variant(Hero, 0), hero(v) {}
	constexpr pvar(playeri* v) : variant(Player, 0), player(v) {}
	constexpr pvar(moveablei* v) : variant(Moveable, 0), moveable(v) {}
	constexpr bool operator==(const pvar& e) const { return type == e.type && value == e.value; }
	constexpr explicit operator bool() const { return type != NoVariant; }
	void					clear() { type = NoVariant; variant::value = 0; value = 0; }
};
class generator {
	unsigned char			artifacts[LastArtifact + 1];
	unsigned char			monsters[WaterElement + 1];
	unsigned				castle_index;
public:
	generator();
	artifact_s				add(artifact_s v) { artifacts[v]++; return v; }
	monster_s				add(monster_s v) { monsters[v]++; return v; }
	artifact_s				artifact(int level = 0);
	const char*				castlename();
	monster_s				monster(int level = 0);
	resource_s				resource();
};
class string : public stringbuilder {
	char					buffer[512];
public:
	string() : stringbuilder(buffer) {}
	void					addh(const char* format, ...);
	void					addi(variant v, int value);
};
namespace map {
extern point				camera;
extern unsigned				day;
extern unsigned char		flags[256 * 256];
extern unsigned	char		height;
extern unsigned	char		obelisc_count;
extern unsigned	char		roads[256 * 256];
extern unsigned short		tiles[256 * 256];
extern unsigned char		width;
//
void						clear();
unsigned					getcost(short unsigned index);
unsigned					getcost(short unsigned index, direction_s direct, unsigned pathfinding);
direction_s					getdir(short unsigned from, short unsigned to);
inline unsigned				getmonth() { return day / (7 * 4); }
short unsigned*				getpath();
unsigned					getpathcount();
landscape_s					gettile(short unsigned index);
inline unsigned				getweek() { return day / 7; }
inline unsigned				getweekday() { return day % 7; }
inline unsigned				getmonthweek() { return getweek() % 4; }
inline int					i2x(short unsigned i) { return i % 256; }
inline int					i2y(short unsigned i) { return i >> 8; }
bool						is(short unsigned index, map_flag_s v);
bool						isinteract(object_s v);
bool						ispathable(short unsigned index);
inline unsigned short		m2i(int x, int y) { return (y << 8) + x; }
void						set(short unsigned index, map_flag_s v);
void						setcamera(short unsigned index);
short unsigned				to(short unsigned i, direction_s d);
direction_s					to(direction_s f, direction_s d);
void						walk(short unsigned start, short unsigned goal);
void						wave(short unsigned start, int skill, int ship_master, const playeri* player);
}
const char*					getstr(building_s id, kind_s kind);
DECLENUM(ability);
DECLENUM(artifact);
DECLENUM(difficult);
DECLENUM(skill);
DECLENUM(spell);
DECLENUM(kind);
DECLENUM(landscape);
DECLENUM(level);
DECLENUM(luck);
DECLENUM(monster);
DECLENUM(morale);
DECLENUM(object);
DECLENUM(resource);
DECLENUM(speed);