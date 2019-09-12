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
	MovePoints, SpellPoints,
	LuckStat, MoraleStat, Speed,
	DamageMin, DamageMax,
	HitPoints, HitPointsMax,
	Shoots, ShootsMax,
	Experience,
	AllAbilities, AttackDefence, SpellPowerKnowledge,
};
enum skill_s : unsigned char {
	Pathfinding, Archery, Logistics, Scounting, Diplomacy,
	Navigation, Leadership, Wisdow, Mysticism, Luck,
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
	Wait, Move, PalmFace, AttackAction, FlyAction, Shoot, Damaged, Dead, Killed, Cast,
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
	Ability, Artifact, Building, CastleVar, Hero, Index, Landscape, Object, Monster, Moveable,
	Player, Resource, Skill, Spell, Stat, Tag, Variant,
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
enum object_flag_s : unsigned char {
	Used, OneUse, AllowComputer,
};
enum interact_s : unsigned char {
	NoCase,
	TreasureCase, TreasureArtifact, TreasureCost,
	QuestArtifact, FightArtifact, GuardSoldier, BuyArtifact,
	JoinDwelling, IncreaseAbility,
	CaptureObject, LearnAbility, LookAround,
};
enum reaction_s : unsigned char {
	ReactionHostile, ReactionIndifferent, ReactionFriendly,
};
enum object_use_s : unsigned char {
	NoUse, NoBlock,
	SingleUse, PlayerUse, PlayerOwned, HeroUse,
	Dwelling, WeekBonus, ContentUse,
	LearnUse, SpecialUse,
};
typedef cflags<player_s, unsigned char> playerf;
typedef cflags<object_flag_s, unsigned char> objectf;
class heroi;
struct pvar;
struct shapei;
struct variantcol;
struct variant {
	variant_s				type;
	union {
		ability_s			ability;
		artifact_s			artifact;
		building_s			building;
		hero_s				hero;
		landscape_s			landscape;
		player_s			player;
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
	constexpr variant(building_s v) : type(Building), building(v) {}
	constexpr variant(hero_s v) : type(Hero), hero(v) {}
	constexpr variant(landscape_s v) : type(Landscape), landscape(v) {}
	constexpr variant(object_s v) : type(Object), object(v) {}
	constexpr variant(player_s v) : type(Player), player(v) {}
	constexpr variant(monster_s v) : type(Monster), monster(v) {}
	constexpr variant(resource_s v) : type(Resource), resource(v) {}
	constexpr variant(skill_s v) : type(Skill), skill(v) {}
	constexpr variant(spell_s v) : type(Spell), spell(v) {}
	constexpr variant(tag_s v) : type(Tag), tag(v) {}
	constexpr bool operator==(const variant& e) const { return type == e.type && value == e.value; }
	constexpr explicit operator bool() const { return type != NoVariant; }
	const char*				getname() const;
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
	void operator+=(const costi& e);
	costi operator*(int v);
	void					add(resource_s id, int v) { data[id] += v; correct(); }
	void					clear();
	void					correct();
	int						get(resource_s v) const { return data[v]; }
	costi					getonly(resource_s id) const;
	int						mindiv(const costi& e) const;
	void					paint(int x, int y) const;
};
struct namei {
	const char*				name;
	constexpr operator bool() const { return name != 0; }
	const char*				getname() const { return name; }
	void					setname(const char* v) { name = szdup(v); }
};
struct positioni {
	short unsigned			index;
	short unsigned			getpos() const { return index; }
	void					setpos(short unsigned v) { index = v; }
};
struct mapinfoi {
	short unsigned			index;
	char					text[126];
	static const mapinfoi*	find(short unsigned index);
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
	unsigned				getstrenght() const;
	monster_s				getupgrade() const;
	bool					is(tag_s v) const;
	void					information(const heroi* hero) { show(hero, true, false, false); }
	void					paint(int x, int y, const heroi* hero = 0, bool allow_change = true) const;
	void					show(const heroi* hero, bool info_mode, bool allow_dismiss, bool allow_upgrade);
};
struct armyi {
	squadi					units[5];
	squadi*					add(monster_s v, short unsigned count);
	void					clear();
	squadi*					getsquad(monster_s v) const;
	squadi*					getslowest() const;
	unsigned				getstrenght() const;
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
	static bool				confirm(const char* format);
	static void				endturn();
	static void				initialize();
	void					gainprofit();
	int						getadventurers() const;
	activity_s				getactivity() const { return activity; }
	int						getbuildings(building_s v) const;
	int						getbuildings(building_s v, kind_s kind) const;
	int						getcastles() const;
	heroi*					gethero(heroi* current = 0) const;
	int						getheroes() const;
	heroi*					gethire(int index) const;
	player_s				getid() const { return player_s(this - bsmeta<playeri>::elements); }
	kind_s					getkind() const { return kind; }
	int						getmarket() const;
	int						getmines(resource_s id) const;
	static int				getmineincome(resource_s id);
	costi					getprofit() const;
	static int				getrate(resource_s resf, resource_s rest, int markets);
	costi&					getresources() { return resources; }
	int						getspies() const;
	int						gettowns() const;
	void					hire(heroi* hero);
	void					marketplace();
	static void				message(const char* format);
	static void				prepare(difficult_s difficult);
	void					quickmessage(const char* format);
	static heroi*			randomhire(kind_s kind, kind_s excude_kind);
	heroi*					randomhire(int index) const;
	bool					recruit(monster_s unit, int& count, int maximum);
	void					refresh();
	void					set(activity_s v) { activity = v; }
	void					set(kind_s v) { kind = v; }
	void					sethire(int index);
	void					setup(difficult_s id);
	void					tavern();
	static void				tooltips(const char* format);
	void					thieves();
	void					trade(resource_s rs, resource_s rt, int count, int market_count);
};
struct abilityi {
	const char*				name;
};
struct kindi {
	const char*				name;
	const char*				name_abbr;
	unsigned short			abilities[4];
	skill_s					skills[2];
	monster_s				units[2];
	char					ability_chance[Knowledge + 1];
	char					ability_chance_high[Knowledge + 1];
	char					skill_chance[LastSkill + 1];
	spell_s					spell;
	ability_s				getrandomability(int level) const;
	variant					getrandomskill(cflags<skill_s> exclude) const;
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
struct shapei {
	unsigned char			count;
	point					size;
	point					points[24];
	unsigned char			animation[24];
	unsigned char			content[24]; // 0 - shadow, 1 - passable, 2 - blocked
	unsigned char			indecies[24];
	point					offset;
	unsigned char			zero;
	unsigned char			initialized;
	bool					is(short unsigned start, short unsigned index) const;
};
class generator {
	unsigned char			artifacts[LastArtifact + 1];
	unsigned char			skills[LastSkill + 1];
	unsigned char			spells[LastSpell + 1];
	unsigned char			monsters[WaterElement + 1];
	unsigned				castle_index;
public:
	generator() { memset(this, 0, sizeof(*this)); }
	artifact_s				add(artifact_s v) { artifacts[v]++; return v; }
	monster_s				add(monster_s v) { monsters[v]++; return v; }
	static artifact_s		any_artifact(int level = 0);
	artifact_s				artifact(int level = 0);
	const char*				castlename();
	monster_s				monster(int level = 0);
	static resource_s		resource();
	skill_s					skill();
	spell_s					spell(int level);
};
class moveablei : public positioni {
	object_s				type;
	playerf					player;
	reaction_s				reaction;
	unsigned char			subtype;
	short unsigned			count;
	short unsigned			drawobj;
public:
	explicit constexpr operator bool() const { return index != Blocked; }
	void					blockpath(unsigned* path) const;
	void					clear();
	static moveablei*		find(short unsigned i);
	static moveablei*		find(short unsigned index, object_s type);
	static moveablei*		findnear(short unsigned i, object_s type);
	artifact_s				getartifact() const { return artifact_s(subtype); }
	short unsigned			getcount() const { return count; }
	short unsigned			getdraw() const { return drawobj; }
	interact_s				getinteract() const;
	unsigned char			getframe() const { return subtype; }
	monster_s				getmonster() const { return monster_s(subtype); }
	player_s				getplayer() const;
	reaction_s				getreaction() const { return reaction; }
	resource_s				getresource() const { return resource_s(subtype); }
	const shapei&			getshape() const;
	spell_s					getspell() const { return spell_s(subtype); }
	skill_s					getskill() const { return skill_s(subtype); }
	object_s				gettype() const { return type; }
	bool					is(player_s v) const { return player.is(v); }
	bool					is(object_s v) const { return type==v; }
	bool					isplayer() const { return player.data != 0; }
	bool					issingleuse() const;
	void					set(artifact_s v) { subtype = v; }
	void					set(monster_s v) { subtype = v; }
	void					set(object_s v) { type = v; }
	void					set(player_s v) { player.add(v); }
	void					set(reaction_s v) { reaction = v; }
	void					set(resource_s v) { subtype = v; }
	void					set(spell_s v) { subtype = v; }
	void					set(skill_s v) { subtype = v; }
	void					setcount(short unsigned v) { count = v; }
	void					setdraw(short unsigned v) { drawobj = v; }
	void					setframe(unsigned char v) { subtype = v; }
	void					setowner(player_s v) { player.clear(); player.add(v); }
	void					setup(generator& generate);
};
class heroi : public namei, public armyi, public positioni {
	kind_s					kind;
	unsigned char			level;
	short unsigned			index_move;
	unsigned char			portrait;
	player_s				player;
	unsigned				experience;
	artifact_s				artifacts[14];
	unsigned short			abilities[SpellPoints + 1];
	unsigned char			skills[LastSkill + 1];
	unsigned char			visited[64];
	spellbooki				spellbook;
	direction_s				direction;
	static void				open_artifact();
	int						getbonus(ability_s v) const;
public:
	void					add(artifact_s id);
	void					add(const costi& v);
	void					add(const variantcol& v);
	void					add(monster_s id, short unsigned count) { armyi::add(id, count); }
	void					addexperience(unsigned count, bool interactive = true);
	int						ask(const char* format);
	int						ask(const char* format, const variantcol* source);
	bool					battle(moveablei& enemy);
	void					battlemove();
	bool					buymagicbook();
	void					clear();
	void					choose();
	static const costi		cost;
	void					disappear() const;
	static heroi*			find(short unsigned index);
	void					focusing() const;
	void					gain(resource_s type, unsigned short count);
	void					gainmine(const char* text, resource_s mine);
	playeri*				getplayer() const;
	int						get(ability_s v) const;
	int						get(skill_s v) const { return skills[v]; }
	unsigned				get(artifact_s* source, unsigned count_maximum);
	int						getcost(spell_s v) const;
	unsigned				getcost(short unsigned from, short unsigned to) const;
	direction_s				getdirection() const { return direction; }
	hero_s					getid() const { return hero_s(this - bsmeta<heroi>::elements); }
	short unsigned			getmove() const { return index_move; }
	int						getmpmax() const;
	kind_s					getkind() const;
	int						getlevel() const { return level; }
	unsigned				getlearn(variantcol* skill, unsigned count) const;
	unsigned char			getportrait() const { return portrait; }
	int						getspmax() const;
	int						getsprefresh() const;
	int						getskillscount() const;
	void					moveto();
	static void				initialize();
	void					input(const playeri* player) const;
	bool					interact(moveablei& object);
	bool					interact(moveablei& object, interact_s type, variantcol v1, variantcol v2, const char* text, const char* fail);
	bool					interact(short unsigned index, const pvar& object);
	bool					is(artifact_s v) const;
	bool					is(spell_s v) const { return spellbook.is(v); }
	bool					is(skill_s v) const { return get(v)>0; }
	bool					isadventure() const { return index != Blocked; }
	bool					isvisited(const moveablei& object) const;
	static int				learn(const char* format, const variantcol* v1, unsigned count);
	void					levelup(bool interactive);
	void					message(const char* format);
	void					refresh();
	static unsigned			select(heroi** result, heroi** result_maximum, const playeri* player, kind_s kind, kind_s kind_exclude, bool include_special = false);
	void					set(ability_s id, int v);
	void					set(direction_s v) { direction = v; }
	void					set(skill_s id, int v) { skills[id] = v; }
	void					set(spell_s id) { spellbook.set(id); }
	void					set(playeri* v) { if(v) player = v->getid(); else player = RandomPlayer; }
	void					set(player_s v) { player = v; }
	void					setmove(short unsigned i) { index_move = i; }
	void					setportrait(unsigned char i) { portrait = i; }
	void					setup_battle(heroi* defender);
	void					setvisit(unsigned short index);
	void					show(bool allow_change = true) const;
	void					showbook(spell_type_s mode);
};
class castlei : public namei, public armyi, public positioni {
	cflags<building_s>		buildings;
	spellbooki				mageguild;
	kind_s					kind;
	player_s				player;
	short unsigned			population[6];
	building_s				getupgraded(building_s v) const;
	void					paint_panel(int x, int y, const heroi* hero) const;
	void					paint_panorama(int x, int y) const;
	void					paint_name() const;
	void					paint_monster(int x, int y, int height, int width, int level);
	objectf					flags;
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
	static int				getdungeonincome() { return 500; }
	static unsigned char	getframe(building_s v);
	int						getgrowth(building_s building) const;
	kind_s					getkind() const { return kind; }
	static monster_s		getmonster(building_s building, kind_s kind);
	playeri*				getplayer() const { return bsmeta<playeri>::elements + player; }
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
	bool					is(object_flag_s v) const { return flags.is(v); }
	bool					isallow(monster_s v) const;
	bool					iscoastal() const;
	static bool				isdwelling(building_s v) { return (v >= Dwelving1 && v <= Dwelving6) || (v >= Dwelving1u && v <= Dwelving6u2); }
	static void				information(building_s v, kind_s k);
	void					paint() const;
	static void				paint(int x, int y, landscape_s tile, kind_s race, bool castle, bool shadow);
	void					random(bool castle);
	void					recruit(building_s building);
	void					refresh();
	void					set(object_flag_s v) { flags.add(v); }
	void					set(const playeri* v) { player = (v ? v->getid() : RandomPlayer); }
	void					set(player_s v) { player = v; }
	void					set(building_s v) { buildings.add(v); }
	void					set(kind_s v) { kind = v; }
	void					show();
	void					well();
	void					remove(building_s v) { buildings.remove(v); }
	void					remove(object_flag_s v) { flags.remove(v); }
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
	unsigned				rating;
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
	static const int		cost_magic_book = 500;
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
	void					update_castle_spells();
	void					update_map();
	void					update_monsters();
};
struct interacti {
	interact_s				id;
	const char*				text;
	const char*				fail;
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
struct variantcol : variant {
	int						count;
	int						format;
	constexpr variantcol() : variant(), count(0), format(0) {}
	constexpr variantcol(const variant& element) : variant(element), count(0), format(0) {}
	constexpr variantcol(const variant& element, int c1) : variant(element), count(c1), format(0) {}
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
	constexpr pvar(moveablei* v) : variant(Moveable, 0), moveable(v) {}
	constexpr bool operator==(const pvar& e) const { return type == e.type && value == e.value; }
	constexpr explicit operator bool() const { return type != NoVariant; }
	void					clear() { type = NoVariant; variant::value = 0; value = 0; }
};
struct casei {
	typedef void(*caseproc)(moveablei& m, casei& e);
	unsigned short			chance;
	interact_s				type;
	variantcol				variants[2];
	const char*				text;
	caseproc				initialize;
};
struct objecti {
	const char*				name;
	interact_s				type;
	variantcol				param;
	const char*				text;
	const char*				fail;
	const aref<casei>		actions;
	constexpr objecti(const char* name) : name(name), type(NoCase), param(), text(0), fail(0), actions() {}
	constexpr objecti(const char* name, interact_s type, variantcol param, const char* text = 0, const char* fail = 0) : name(name), type(type), param(param), text(text), fail(fail), actions() {}
	constexpr objecti(const char* name, const aref<casei>& actions, const char* text = 0, const char* fail = 0) : name(name), type(NoCase), param(), text(text), fail(fail), actions(actions) {}
	bool					isvisitable() const;
};
class string : public stringbuilder {
	char					buffer[512];
public:
	string() : stringbuilder(buffer) {}
	void					addh(const char* format, ...);
	void					addi(variant v, int value = 0, int format = 0);
	void					addi(const costi& v);
	void					addi(const variantcol& v);
	void					addt(const costi& v);
	static const char*		parse(const char* p, variantcol* source, unsigned& count);
};
class eventi : public namei, public positioni {
	playerf					players;
	objectf					flags;
	artifact_s				artifact;
	costi					resources;
public:
	void					clear();
	bool					is(player_s v) { return players.is(v); }
	bool					is(object_flag_s v) { return flags.is(v); }
	artifact_s				getartifact() { return artifact; }
	costi&					getresources() { return resources; }
	void					set(artifact_s v) { artifact = v; }
	void					set(player_s v) { players.add(v); }
	void					set(object_flag_s v) { flags.add(v); }
	void					remove(player_s v) { players.remove(v); }
	void					remove(object_flag_s v) { flags.remove(v); }
};
class dayeventi : public namei {
	playerf					players;
	objectf					flags;
	artifact_s				artifact;
	costi					resources;
	short unsigned			first, subsequenced;
public:
	void					clear();
	bool					is(player_s v) { return players.is(v); }
	bool					is(object_flag_s v) { return flags.is(v); }
	artifact_s				getartifact() { return artifact; }
	costi&					getresources() { return resources; }
	void					set(artifact_s v) { artifact = v; }
	void					set(player_s v) { players.add(v); }
	void					set(object_flag_s v) { flags.add(v); }
	void					setfirst(short unsigned v) { first = v; }
	void					setsubsequenced(short unsigned v) { subsequenced = v; }
	void					remove(player_s v) { players.remove(v); }
	void					remove(object_flag_s v) { flags.remove(v); }
};
struct answeri {
	const char*				answers[10];
	const char*				getanswer(int i) const { return i<int(sizeof(answers) / sizeof(answers[0])) ? answers[i] : ""; }
	void					setanswer(int i, const char* v) { if(i < int(sizeof(answers) / sizeof(answers[0]))) answers[i] = szdup(v); }
};
class sphinxi : public namei, public positioni, public answeri {
	costi					resources;
public:
	costi & getresources() { return resources; }
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
void						clearpath();
pvar						find(short unsigned index, bool rich_find = false);
unsigned					getcost(short unsigned index);
unsigned					getcost(short unsigned index, direction_s direct, unsigned pathfinding);
unsigned					getcost(short unsigned from, short unsigned to, unsigned pathfinding);
direction_s					getdir(short unsigned from, short unsigned to);
inline unsigned				getmonth() { return day / (7 * 4); }
short unsigned*				getpath();
int							getpathcount();
landscape_s					gettile(short unsigned index);
inline unsigned				getweek() { return day / 7; }
inline unsigned				getweekday() { return day % 7; }
inline unsigned				getmonthweek() { return getweek() % 4; }
short unsigned				getvisit(short unsigned index);
inline int					i2x(short unsigned i) { return i % 256; }
inline int					i2y(short unsigned i) { return i >> 8; }
bool						is(short unsigned index, map_flag_s v);
bool						isinteract(object_s v);
bool						ispathable(short unsigned index);
inline unsigned short		m2i(int x, int y) { return (y << 8) + x; }
void						removestep();
void						route(short unsigned goal);
void						set(short unsigned index, map_flag_s v);
void						setcamera(short unsigned index);
short unsigned				to(short unsigned i, direction_s d);
direction_s					to(direction_s f, direction_s d);
void						wave(short unsigned start, int skill, int ship_master);
}
namespace battle {
namespace setting {
extern bool					movement, cursor, distance, grid, index;
}
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