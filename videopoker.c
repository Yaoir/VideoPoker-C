#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>

/* Library functions */

int strcmp(const char *s1, const char *s2);
size_t strlen(const char *s);
long int random(void);
void srandom(unsigned int seed);
time_t time(time_t *t);

/* Appearing later in this file */

void play(void);
void setgame(int game);

/* Software version */

#define VERSION "videopoker 1.0"

/* Debugging compile-time option (Later, it will go in Makefile) */

#define DEBUG

/* values for unicode */

#define UNICODE_TTY   0
#define UNICODE_OFF   1
#define UNICODE_SUITS 2
#define UNICODE_CARDS 3

int unicode = UNICODE_SUITS; /* unicode is true if unicode output is enabled.  */

int hands = 0;	/* number of hands played */

/* the card typedef, for holding infomation about the deck of cards */

typedef struct
{
	int index;	/* cards value, minus 1 */
	char *sym;	/* text appearance */
	wchar_t *uc;	/* Unicode value for the card */
	int suit;	/* card's suit (see just below) */
	int gone;	/* true if it's been dealt */
} card;

/* The number of cards in the hand */

#define CARDS 5

/* The number of cards in the deck */

#define CARDSINDECK 52

#define CLUBS	0
#define DIAMONDS 1
#define HEARTS	2
#define SPADES	3

/* one-character suit designations */

#define NUMSUITS 4
const char *suitname[NUMSUITS] = { "C", "D", "H", "S" };

#define TWO 1
#define THREE 2
#define FOUR 3
#define FIVE 4
#define SIX 5
#define SEVEN 6
#define EIGHT 7
#define NINE 8
#define TEN 9	/* needed for recognizing royal flush, or tens or better (TEN), or jacks or better (JACK) */
#define JACK 10 /* needed for recognizing royal flush, or tens or better (TEN), or jacks or better (JACK) */
#define QUEEN 11
#define KING 12
#define ACE 13	/* needed for recognizing Ace-low straight (Ace, 2, 3, 4, 5) */

/* The standard deck of 52 cards */

card deck[CARDSINDECK] =
{
/*	index, card, Unicode, suit, gone */
	{ TWO,   " 2", L"\U0001F0D2", CLUBS, 0 },
	{ THREE, " 3", L"\U0001F0D3", CLUBS, 0 },
	{ FOUR,  " 4", L"\U0001F0D4", CLUBS, 0 },
	{ FIVE,  " 5", L"\U0001F0D5", CLUBS, 0 },
	{ SIX,   " 6", L"\U0001F0D6", CLUBS, 0 },
	{ SEVEN, " 7", L"\U0001F0D7", CLUBS, 0 },
	{ EIGHT, " 8", L"\U0001F0D8", CLUBS, 0 },
	{ NINE,  " 9", L"\U0001F0D9", CLUBS, 0 },
	{ TEN,   "10", L"\U0001F0Da", CLUBS, 0 },
	{ JACK,  " J", L"\U0001F0Db", CLUBS, 0 },
	{ QUEEN, " Q", L"\U0001F0Dd", CLUBS, 0 },
	{ KING,  " K", L"\U0001F0De", CLUBS, 0 },
	{ ACE,   " A", L"\U0001F0D1", CLUBS, 0 },

	{ TWO,   " 2", L"\U0001F0C2", DIAMONDS, 0 },
	{ THREE, " 3", L"\U0001F0C3", DIAMONDS, 0 },
	{ FOUR,  " 4", L"\U0001F0C4", DIAMONDS, 0 },
	{ FIVE,  " 5", L"\U0001F0C5", DIAMONDS, 0 },
	{ SIX,   " 6", L"\U0001F0C6", DIAMONDS, 0 },
	{ SEVEN, " 7", L"\U0001F0C7", DIAMONDS, 0 },
	{ EIGHT, " 8", L"\U0001F0C8", DIAMONDS, 0 },
	{ NINE,  " 9", L"\U0001F0C9", DIAMONDS, 0 },
	{ TEN,   "10", L"\U0001F0Ca", DIAMONDS, 0 },
	{ JACK,  " J", L"\U0001F0Cb", DIAMONDS, 0 },
	{ QUEEN, " Q", L"\U0001F0Cd", DIAMONDS, 0 },
	{ KING,  " K", L"\U0001F0Ce", DIAMONDS, 0 },
	{  ACE,  " A", L"\U0001F0C1", DIAMONDS, 0 },

	{ TWO,   " 2", L"\U0001F0B2", HEARTS, 0 },
	{ THREE, " 3", L"\U0001F0B3", HEARTS, 0 },
	{ FOUR,  " 4", L"\U0001F0B4", HEARTS, 0 },
	{ FIVE,  " 5", L"\U0001F0B5", HEARTS, 0 },
	{ SIX,   " 6", L"\U0001F0B6", HEARTS, 0 },
	{ SEVEN, " 7", L"\U0001F0B7", HEARTS, 0 },
	{ EIGHT, " 8", L"\U0001F0B8", HEARTS, 0 },
	{ NINE,  " 9", L"\U0001F0B9", HEARTS, 0 },
	{ TEN,   "10", L"\U0001F0Ba", HEARTS, 0 },
	{ JACK,  " J", L"\U0001F0Bb", HEARTS, 0 },
	{ QUEEN, " Q", L"\U0001F0Bd", HEARTS, 0 },
	{ KING,  " K", L"\U0001F0Be", HEARTS, 0 },
	{ ACE,   " A", L"\U0001F0B1", HEARTS, 0 },

	{ TWO,   " 2", L"\U0001F0A2", SPADES, 0 },
	{ THREE, " 3", L"\U0001F0A3", SPADES, 0 },
	{ FOUR,  " 4", L"\U0001F0A4", SPADES, 0 },
	{ FIVE,  " 5", L"\U0001F0A5", SPADES, 0 },
	{ SIX,   " 6", L"\U0001F0A6", SPADES, 0 },
	{ SEVEN, " 7", L"\U0001F0A7", SPADES, 0 },
	{ EIGHT, " 8", L"\U0001F0A8", SPADES, 0 },
	{ NINE,  " 9", L"\U0001F0A9", SPADES, 0 },
	{ TEN,   "10", L"\U0001F0Aa", SPADES, 0 },
	{ JACK,  " J", L"\U0001F0Ab", SPADES, 0 },
	{ QUEEN, " Q", L"\U0001F0Ad", SPADES, 0 },
	{ KING,  " K", L"\U0001F0Ae", SPADES, 0 },
	{ ACE,   " A", L"\U0001F0A1", SPADES, 0 },
};

/* The hand. It holds five cards. */

card hand[5];
card shand[5];	/* sorted hand, for internal use when recognizing winners */

/* keys used to select kept cards */

#define NUMKEYS	5
char keys[NUMKEYS] = { ' ', 'j', 'k', 'l', ';', };

/* number of chips held */

#define INITCHIPS 1000
int score = INITCHIPS;

/* minimum and maximum swing of score during this game */

int score_low = INITCHIPS;
int score_high = INITCHIPS;

/* The games starts with a bet of 10, the minimum allowed */

#define INITMINBET 10

int minbet = INITMINBET;
int bet = INITMINBET;

/* number of chips or groups of 10 chips bet */

int betmultiplier = 1;

/* Options */

/* -b (Bold): print in boldface */

int boldface = 0;

/* -mh (Mark Held): Mark cards that are held */

int markheld = 0;

/* -q (Quiet): Don't print banner or final report */

int quiet = 0;

/* Sanity check: check that there are no duplicate cards in hand */

int check_for_dupes(void)
{
	int i, j;

	for(i = 0; i < CARDS; i++)
	{
		for(j = i+1; j < CARDS; j++)
		{
			if(hand[i].index == hand[j].index
			&& hand[i].suit  == hand[j].suit ) return 0;
		}
	}

	return 1;
}

/*
	Some ANSI Terminal escape codes:
	ESC[38;5; then one of (0m = black, 1m = red, 2m = green, 3m = yellow,
			 4m = blue, 5m = magenta, 6m = cyan, 7m = white)
	ESC[1m = bold, ESC[0m = reset all attributes
*/

/* The below are part of the escape codes listed above. Do not change the values. */
#define BLACK	0
#define RED	1
#define GREEN	2
#define YELLOW	3
#define BLUE	4
#define MAGENTA	5
#define CYAN	6
#define WHITE	7

void color(int color)
{
	if(unicode == UNICODE_TTY) return;
	wprintf(L"\033[38;5;%dm",color);
}

void ANSIbold(void)
{
	if(unicode == UNICODE_TTY) return;
	wprintf(L"\033[1m");
}

void ANSIreset(void)
{
	if(unicode == UNICODE_TTY) return;
	wprintf(L"\033[0m");
	if(boldface) ANSIbold();
}

/*
	Display the hand
	This is where the Unicode output setting (-u<N> option) takes effect,
	so there are three different ways it can display the cards.
*/

void showhand(void)
{
	int i;
	/* Unicode characters for the suits */
	const wchar_t *spade = L"\u2660";
	const wchar_t *heart = L"\u2665";
	const wchar_t *diamond = L"\u2666";
	const wchar_t *club = L"\u2663";

	/* Method 1: Unicode Card Faces (-u3 option), which requires only one line  */

	if(unicode == UNICODE_CARDS) /* print the Unicode card faces */
	{
		for(i = 0; i < CARDS; i++)
		{
			switch(hand[i].suit)
			{
				case DIAMONDS:
				case HEARTS:
					color(RED);
//					wprintf(L"\033[38;5;1m%ls\033[0m", hand[i].uc);
					wprintf(L"%ls ", hand[i].uc);
					ANSIreset();
					break;
				case CLUBS:
				case SPADES:
					wprintf(L"%ls ", hand[i].uc);
					break;
			}
		}
		/* print a space to separate output from user input */
		wprintf(L" ");
		return;
	}

	/* Method 2: Two Line Output for -u0/-u1/-u2 options, requires 2 lines */

	if(boldface) ANSIbold();

	/* First Line of output: show card values */

	for(i = 0; i < CARDS; i++)
	{
		switch(hand[i].suit)
		{
			case DIAMONDS:
			case HEARTS:
				/* print in red */
				color(RED);
//				wprintf(L"\033[38;5;1m%s\033[0m ", hand[i].sym);
				wprintf(L"%s", hand[i].sym);
				ANSIreset();
				wprintf(L" ");
				break;
			case CLUBS:
			case SPADES:
				/* print in default text color */
				wprintf(L"%s ", hand[i].sym);
				break;
		}
	}

	wprintf(L"\n");

	for(i = 0; i < CARDS; i++)
	{
		if(unicode == UNICODE_SUITS)
		{
			/* Unicode method */
			switch(hand[i].suit)
			{
				case DIAMONDS:
					/* print in red */
					wprintf(L" ");
					color(RED);
//					wprintf(L"\033[38;5;1m %ls\033[0m ", diamond);
					wprintf(L"%ls", diamond);
					ANSIreset();
					wprintf(L" ");
					break;
				case HEARTS:
					wprintf(L" ");
					color(RED);
				//	wprintf(L"\033[38;5;1m %ls\033[0m ", heart);
					wprintf(L"%ls", heart);
					ANSIreset();
					wprintf(L" ");
					break;
				case CLUBS:
					/* print in default text color */
					wprintf(L" %ls ", club);
					break;
				case SPADES:
					wprintf(L" %ls ", spade);
					break;
			}
		}
		else
		{
			/* ASCII method */
			switch(hand[i].suit)
			{
				case DIAMONDS:
				case HEARTS:
					/* print H or D in red */
					wprintf(L" ");
					color(RED);
//					wprintf(L" \033[38;5;1m%s\033[0m ", suitname[hand[i].suit]);
					wprintf(L"%s", suitname[hand[i].suit]);
					ANSIreset();
					wprintf(L" ");
					break;
				case CLUBS:
				case SPADES:
					/* print S or C in default text color */
					wprintf(L" %s ", suitname[hand[i].suit]);
					break;
			}
		}
	}

	/* print a space to separate output from user input */
	wprintf(L" ");
#ifdef DEBUG
	if(check_for_dupes() == 0) wprintf(L"\n!!! DUPLICATE CARD !!!\n\n");
#endif
}

#define AllAmerican		 0
#define TensOrBetter		 1
#define BonusPoker		 2
#define DoubleBonus		 3
#define DoubleBonusBonus	 4
#define JacksOrBetter		 5	/* default */
#define JacksOrBetter95		 6
#define JacksOrBetter86		 7
#define JacksOrBetter85		 8
#define JacksOrBetter75		 9
#define JacksOrBetter65		 10
/* If you add another game, increment NUMGAMES: */
#define NUMGAMES 11

/*
	The game in play. Default is Jacks or Better,
	which is coded into initialization of static data
*/

int game = JacksOrBetter;

const char *gamenames[NUMGAMES] =
{
	"All American",
	"Tens or Better",
	"Bonus Poker",
	"Double Bonus",
	"Double Bonus Bonus",
	"Jacks or Better",
	"9/5 Jacks or Better",
	"8/6 Jacks or Better",
	"8/5 Jacks or Better",
	"7/5 Jacks or Better",
	"6/5 Jacks or Better"
};

// TODO: the #defines, gamenames[], and option strings can be put in an array of structures:
// typedef struct {} gameinfo;
// gameinfo game[] = { ... };  (etc.)
// Then change badgame(), option handling in Main(), and setgame()

/* Error message for -g option. Also a way to display the list of games */

void badgame(void)
{
	wprintf(L"Video Poker: -g option is missing valid game name.\n");
	wprintf(L"Available games are:\n");
	wprintf(L"aa   - All American\n");
	wprintf(L"10s  - Tens or Better\n");
#if 0
	wprintf(L"bon  - Bonus Poker\n");
	wprintf(L"db   - Double Bonus\n");
	wprintf(L"dbb  - Double Bonus Bonus\n");
#endif
	wprintf(L"jb95 - 9/5 Jacks or Better\n");
	wprintf(L"jb86 - 8/6 Jacks or Better\n");
	wprintf(L"jb85 - 8/5 Jacks or Better\n");
	wprintf(L"jb75 - 7/5 Jacks or Better\n");
	wprintf(L"jb65 - 6/5 Jacks or Better\n");
	exit(0);
}

/* Initialize, Handle arguments, enter loop */

int main(int argc, char **argv)
{
	int i, cnt;
	char **argp;
	char *arg;

	/* initialize random number generator */
	srandom((unsigned int)time(NULL));

	/* initialize for unicode output in showhand() */
	if(unicode) setlocale(LC_CTYPE, "en_US.UTF-8");

	/* process arguments */

	for(cnt = argc, argp = argv; cnt > 1;)
	{
		/* -b (Bold) */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'b'
		&& argp[1][2] == '\0')
		{
			if(cnt < 2) badgame();

			boldface = 1;

			/* advance to next argument */
			argp += 1;
			cnt -= 1;
			continue;
		}

		/* -b1 (Bet 1 Chip) */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'b'
		&& argp[1][2] == '1'
		&& argp[1][3] == '\0')
		{
			if(cnt < 2) badgame();

			/* set minimum bet */
			minbet = bet = 1;

			/* advance to next argument */
			argp += 1;
			cnt -= 1;
			continue;
		}

		/* -g <name>  Choose Game */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'g'
		&& argp[1][2] == '\0')
		{
			if(cnt < 3) badgame();
			arg = argp[2];

			if     ( ! strcmp(arg,"jb95")) game = JacksOrBetter95;
			else if( ! strcmp(arg,"jb86")) game = JacksOrBetter86;
			else if( ! strcmp(arg,"jb85")) game = JacksOrBetter85;
			else if( ! strcmp(arg,"jb75")) game = JacksOrBetter75;
			else if( ! strcmp(arg,"jb65")) game = JacksOrBetter65;
			else if( ! strcmp(arg,"aa"))   game = AllAmerican;
			else if( ! strcmp(arg,"10s"))  game = TensOrBetter;
#if 0
			else if( ! strcmp(arg,"bon"))  game = BonusPoker;
			else if( ! strcmp(arg,"db"))   game = DoubleBonus;
			else if( ! strcmp(arg,"dbb"))  game = DoubleBonusBonus;
#endif
			else badgame();

			setgame(game);

			/* advance to next argument */
			argp += 2;
			cnt -= 2;
			continue;
		}

		/* -is (Initial Score) */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'i'
		&& argp[1][2] == 's'
		&& argp[1][3] == '\0')
		{
			if(cnt < 3) badgame();

			score = atoi(argp[2]);

			if(score <= 0 || score > 100000)
			{
				wprintf(L"Video Poker: bad number given with the -is option.\n");
				exit(1);
			}

			if(score%10 != 0) minbet = bet = 1;

			/* advance to next argument */
			argp += 2;
			cnt -= 2;
			continue;
		}

		/* -k <5-char-string>  Redefine input keys (default is " jkl;" */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'k'
		&& argp[1][2] == '\0')
		{
			if(cnt < 3) badgame();
			arg = argp[2];

			if(strlen(arg) != NUMKEYS)
			{
				// complain and exit
				wprintf(L"Video Poker: the string given with the -k option is the wrong length.\n");
				exit(1);
			}

			/* copy the string into keys[] */
			for(i = 0; i < NUMKEYS; ++i)
			{
				if(arg[i] == 'q' || arg[i] == 'e')
				{
					wprintf(L"Video Poker: for the -k option, the string may not contain q or e.\n");
					exit(1);
				}
				keys[i] = (char) arg[i];
			}

			/* advance to next argument */
			argp += 2;
			cnt -= 2;
			continue;
		}

		/* -mh (Mark Held) */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'm'
		&& argp[1][2] == 'h'
		&& argp[1][3] == '\0')
		{
			if(cnt < 2) badgame();

			/* turn on Mark Held flag */
			markheld = 1;

			/* advance to next argument */
			argp += 1;
			cnt -= 1;
			continue;
		}

		/* -q (Quiet) */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'q'
		&& argp[1][2] == '\0')
		{
			if(cnt < 2) badgame();

			/* turn on Quiet flag */
			quiet = 1;

			/* advance to next argument */
			argp += 1;
			cnt -= 1;
			continue;
		}

		/* -u<n>  Unicode Output */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'u'
		&& strlen(argp[1]) > 2)
		{
			switch(argp[1][2])
			{
				case '0':
					unicode = UNICODE_TTY;
					break;
				case '1':
					unicode = UNICODE_OFF;
					break;
				case '2':
					unicode = UNICODE_SUITS;
					break;
				case '3':
					unicode = UNICODE_CARDS;
					break;
				default:
					wprintf(L"Video Poker: digit %d given with -u option is out of range.\n",argp[1][2]-'0');
					exit(1);
					break;
			}
			/* advance to next argument */
			++argp;
			--cnt;
			continue;
		}

		/* -v (Version) */

		if(argp[1][0] == '-'
		&& argp[1][1] == 'v'
		&& argp[1][2] == '\0')
		{
			if(cnt < 2) badgame();

			wprintf(L"%s\n",VERSION);
			exit(0);
		}

		/* unrecognized option */
		wprintf(L"Video Poker: the %s option was not recognized\n",argp[1]);
		exit(1);
	}

	/* Before starting play, print the name of the game in green */

	if( ! quiet)
	{
		wprintf(L"\n ");
		color(GREEN);
		ANSIbold();
		wprintf(L"%s",gamenames[game]);
		ANSIreset();
		wprintf(L"\n\n");
	}

	for(;;) play();
	return 1;
}

/* Functions that recognize winning hands */

/*
	Flush:
	returns 1 if the sorted hand is a flush
*/

int flush(void)
{
	if( shand[0].suit == shand[1].suit
	&&  shand[1].suit == shand[2].suit
	&&  shand[2].suit == shand[3].suit
	&&  shand[3].suit == shand[4].suit ) return 1;

	return 0;
}

/*
	Straight:
	returns 1 if the sorted hand is a straight
*/

int straight(void)
{
	if( shand[1].index == shand[0].index + 1
	&&  shand[2].index == shand[1].index + 1
	&&  shand[3].index == shand[2].index + 1
	&&  shand[4].index == shand[3].index + 1 ) return 1;

	/* Ace low straight: Ace, 2, 3, 4, 5 */

	if( shand[4].index == ACE
	&&  shand[0].index == TWO
	&&  shand[1].index == THREE
	&&  shand[2].index == FOUR
	&&  shand[3].index == FIVE ) return 1;

	return 0;
}

/*
	Four of a kind:
	the middle 3 all match, and the first or last matches those
*/

int four(void)
{
	if( (shand[1].index == shand[2].index
	&&   shand[2].index == shand[3].index )
	&& ( shand[0].index == shand[2].index
	||   shand[4].index == shand[2].index) ) return 1;

	return 0;
}

/*
	Full house:
	3 of a kind and a pair
*/

int full(void)
{
	if( shand[0].index == shand[1].index
	&& (shand[2].index == shand[3].index
	&&  shand[3].index == shand[4].index) ) return 1;

	if( shand[3].index == shand[4].index
	&& (shand[0].index == shand[1].index
	&&  shand[1].index == shand[2].index) ) return 1;

	return 0;
}

/*
	Three of a kind:
	it can appear 3 ways
*/

int three(void)
{
	if(shand[0].index == shand[1].index
	&& shand[1].index == shand[2].index) return 1;

	if(shand[1].index == shand[2].index
	&& shand[2].index == shand[3].index) return 1;

	if(shand[2].index == shand[3].index
	&& shand[3].index == shand[4].index) return 1;

	return 0;
}

/*
	Two pair:
	it can appear in 3 ways
*/

int twopair(void)
{
	if( ((shand[0].index == shand[1].index) && (shand[2].index == shand[3].index))
	||  ((shand[0].index == shand[1].index) && (shand[3].index == shand[4].index))
	||  ((shand[1].index == shand[2].index) && (shand[3].index == shand[4].index))) return 1;

	return 0;
}

/*
	Two of a kind (pair), jacks or better
	or if the game is Tens or Better, 10s or better.
*/

int two(void)
{
	int min = JACK;

	if(game == TensOrBetter) min = TEN;

	if(shand[0].index == shand[1].index && shand[1].index >= min) return 1;
	if(shand[1].index == shand[2].index && shand[2].index >= min) return 1;
	if(shand[2].index == shand[3].index && shand[3].index >= min) return 1;
	if(shand[3].index == shand[4].index && shand[4].index >= min) return 1;

	return 0;
}

/*
	This bunch of defines is used to index into
	paytable[] and handname[], so don't change the numbers.
*/

#define ROYAL	0
#define STRFL	1
#define	FOURK	2
#define FULL	3
#define FLUSH	4
#define STR	5
#define THREEK	6
#define TWOPAIR	7
#define PAIR	8
#define NOTHING 9

/* the number of the above: */
#define NUMHANDTYPES 10

int paytable[NUMHANDTYPES] =
{
	800,	/* royal flush: 800 */
	50,	/* straight flush: 50 */
	25,	/* 4 of a kind: 25 */
	9,	/* full house: 9 */
	6,	/* flush: 6 */
	4,	/* straight: 4 */
	3,	/* 3 of a kind: 3 */
	2,	/* two pair: 2 */
	1,	/* jacks or better: 1 */
	0	/* nothing */
};

const char *handname[NUMHANDTYPES] =
{
	"Royal Flush    ",
	"Straight Flush ",
	"Four of a Kind ",
	"Full House     ",
	"Flush          ",
	"Straight       ",
	"Three of a Kind",
	"Two Pair       ",
	"Pair           ",
	"Nothing        ",
};

#define INVALID 100	/* higher than any valid card index */

/* returns type of hand */

int recognize(void)
{
	int i, j, f;
	int min = INVALID;
	card tmp[CARDS];
	int st = 0, fl = 0;

	/* Sort hand into sorted hand (shand) */

	/* make copy of hand */
	for(i = 0; i < CARDS; i++) tmp[i] = hand[i];

	for(i = 0; i < CARDS; i++)
	{
		/* put lowest card in hand into next place in shand */

		for(j = 0; j < CARDS; j++)
			if(tmp[j].index <= min)
			{
				min = tmp[j].index;
				f = j;
			}

		shand[i] = tmp[f];
		tmp[f].index = INVALID;	/* larger than any card */
		min = INVALID;
	}

	/* royal and straight flushes, strait, and flush */

	fl = flush();
	st = straight();

	if(st && fl && shand[0].index == TEN) return ROYAL;
	if(st && fl) return STRFL;
	if(four()) return FOURK;
	if(full()) return FULL;
	if(fl) return FLUSH;
	if(st) return STR;
	if(three()) return THREEK;
	if(twopair()) return TWOPAIR;
	if(two()) return PAIR;

	/* Nothing */

	return NOTHING;
}

/* The loop */

void play(void)
{
	int i, c, crd;
	int hold[CARDS];
	int digit;

	/* initialize deck */
	for(i = 0; i < CARDSINDECK; i++) deck[i].gone = 0;

	/* initialize hold[] */
	for(i = 0; i < CARDS; i++) hold[i] = 0;

	score -= bet;

#if 0
	/* To test Ace-low straights,
	   substitute this for the for loop below */
	hand[0] = deck[0]; deck[0].gone = 1;
	hand[1] = deck[1]; deck[1].gone = 1;
	hand[2] = deck[2]; deck[2].gone = 1;
	hand[3] = deck[3]; deck[3].gone = 1;
	hand[4] = deck[12]; deck[12].gone = 1;
#endif

	for(i = 0; i < CARDS; i++)
	{
		/* find a card not already dealt */
		do crd = random()%CARDSINDECK;
		while(deck[crd].gone);

		deck[crd].gone = 1;
		hand[i] = deck[crd];
	}


	showhand();

	/* get cards to hold, and replace others */

	while((c = getchar()) != '\n')
	{
		if(c == 'q' || c == 'e')
		{
			boldface = 0;
			ANSIreset();
			if( ! quiet)
			{
				wprintf(L"\nYou quit with %d chips after playing %d hands.\n",score+bet,hands);
				wprintf(L"Range: %d - %d\n", score_low, score_high);
			}
			exit(0);
		}

		if(c == 'b')	/* Change the bet. Only 1, 2, 3, 4, and 5 are allowed. */
		{
			digit = getchar();
			if(digit <= '1' || digit >= '6') ungetc(digit,stdin);
			else
			{
				betmultiplier = digit - '0';
				bet = betmultiplier * minbet;
			}
			continue;
		}

		for(i = 0; i < NUMKEYS; ++i)
		{
			if(keys[i] == c)
			{
				/* flip bit to hold/discard it */
				hold[i] ^= 1;
			}
		}
	}

	/* Optional Line: mark held cards */

	if(markheld == 1)
	{
		for(i = 0; i < CARDS; i++)
		{
			wprintf(L"%s ", hold[i] ? " +" : "  ");
		}
		wprintf(L"\n");
	}

	/* replace cards not held */

	for(i = 0; i < CARDS; i++)
	{
		if( ! hold[i])
		{
			do crd = random()%CARDSINDECK;
			while(deck[crd].gone);

			deck[crd].gone = 1;
			hand[i] = deck[crd];
		}
	}

	/* print final hand */

	showhand();

	/* recognize and score hand */

	i = recognize();

	score += paytable[i] * bet;

	wprintf(L"%s  ",handname[i]);
	wprintf(L"%d\n\n",score);

	++hands;

	if(score < score_low)  score_low  = score;
	if(score > score_high) score_high = score;

	if(score < bet)
	{
		while(score < bet && betmultiplier > 1)
		{
			--betmultiplier;
			bet = minbet * betmultiplier;
		}

		if(score < bet)
		{
			boldface = 0;
			ANSIreset();
			if( ! quiet)
			{
				wprintf(L"You ran out of chips after playing %d hands.\n", hands);
				if(score_high > INITCHIPS) wprintf(L"At one point, you had %d chips.\n", score_high);
			}
			exit(0);
		}
		else
		{
			wprintf(L"You are low on chips. Your bet has been reduced to %d.\n\n", bet);
		}
	}
}

/* do the work for the -g option */

void setgame(int game)
{
	switch(game)
	{
		case JacksOrBetter95:
			paytable[FLUSH] = 5;
			break;
		case JacksOrBetter86:
			paytable[FULL] = 8;
			break;
		case JacksOrBetter85:
			paytable[FULL] = 8;
			paytable[FLUSH] = 5;
			break;
		case JacksOrBetter75:
			paytable[FULL] = 7;
			paytable[FLUSH] = 5;
			break;
		case JacksOrBetter65:
			paytable[FULL] = 6;
			paytable[FLUSH] = 5;
			break;
		case AllAmerican:
			paytable[FULL] = 8;
			paytable[FLUSH] = 8;
			paytable[STR] = 8;
			paytable[PAIR] = 1;
			break;
		case TensOrBetter:
			/* pay table same as JacksOrBetter65 */
			paytable[FULL] = 6;
			paytable[FLUSH] = 5;
			break;
#if 0
		case BonusPoker:
			wprintf(L"Bonus Poker is unimplemented in this version.\n");
			exit(0);
			break;
		case DoubleBonus:
			wprintf(L"Double Bonus is unimplemented in this version.\n");
			exit(0);
			break;
		case DoubleBonusBonus:
			wprintf(L"Double Bonus Bonus is unimplemented in this version.\n");
			exit(0);
			break;
#endif
		default:
			break;
	}
}
