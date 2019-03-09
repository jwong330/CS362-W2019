#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define NUMCARDS 10

/* Tests supplyCount() */
// Description: Getter for gameState returning the supply count of a specific card


int main() {
	int seed = 100;
	int numPlayer = 2;
	int k[NUMCARDS] = { adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall };
	struct gameState G;
	printf("----------Testing supplyCount()----------\n");

	// Black Box Test
	// -----------------Test Case 1-----------------------//
	printf("----------Test Case 1 - supply count for 2 players----------\n");
	// Initialize game state for 2 players
	assert(initializeGame(numPlayer, k, seed, &G) == 0);
	// check counts for each card
	for (int card = 0; card < NUMCARDS; card++) {
		switch (k[card]) {
			case gardens:	// fallthrough
			case great_hall: 
				assert(supplyCount(k[card], &G) == 8);
				break;
			default: 
				assert(supplyCount(k[card], &G) == 10);
				break;
		}
	}

	// -----------------Test Case 2-----------------------//
	printf("----------Test Case 2 - supply count for 4 players----------\n");
	// Initialize game state for 4 players
	numPlayer = 4;
	assert(initializeGame(numPlayer, k, seed, &G) == 0);
	// check counts for each card
	for (int card = 0; card < NUMCARDS; card++) {
		switch (k[card]) {
		case gardens:	// fallthrough
		case great_hall:
			assert(supplyCount(k[card], &G) == 12);
			break;
		default:
			assert(supplyCount(k[card], &G) == 10);
			break;
		}
	}

	// -----------------Test Case 3-----------------------//
	printf("----------Test Case 3 - supply count for cards not in play----------\n");
	// checks for cards not in play after great hall in kingdom list
	for (int card = minion; card <= treasure_map; card++) {
		assert(supplyCount(card, &G) == -1);
	}

	printf("\n----------SUCCESS: Testing complete supplyCount()----------\n\n");

	return 0;
}