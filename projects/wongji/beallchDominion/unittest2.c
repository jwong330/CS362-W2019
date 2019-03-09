#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define NUMCARDS 10

/* Tests numHandCards() */
// Description:Getter for gameState handCount

int main() {
	int seed = 100;
	int k[NUMCARDS] = { adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall };
	int cardCount = 1;
	struct gameState G;
	
	printf("----------Testing numHandCards()----------\n");

	// Black Box Test
	// -----------------Test Case 1-----------------------//
	printf("----------Test Case 1 - different card count in all player hands----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
	// set up handcount for each player
	for (int player = 0; player < MAX_PLAYERS; player++) {
		G.handCount[player] = cardCount++;
	}
	cardCount = 1;		// reset card count
		
	// cycle through each player by setting gamestate to current player flag
	for (int player = 0; player < MAX_PLAYERS; player++) {
		G.whoseTurn = player;
		assert(numHandCards(&G) == cardCount);
		cardCount++;
	}

	// -----------------Test Case 2-----------------------//
	printf("----------Test Case 2 - negative card count in all player hands----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);	// Reset game
	cardCount = -5;
	for (int player = 0; player < MAX_PLAYERS; player++) {
		G.handCount[player] = cardCount--;
	}
	cardCount = -5;		// reset card count
		
	// cycle through each player by setting gamestate to current player flag
	for (int player = 0; player < MAX_PLAYERS; player++) {
		G.whoseTurn = player;
		assert(numHandCards(&G) == cardCount);
		cardCount--;
	}
	

	// WB
	// -----------------Test Case 3-----------------------//
	printf("----------Test Case 3 - max card count in all player hands----------\n");
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Reset game
	for (int player = 0; player < MAX_PLAYERS; player++) {
		G.handCount[player] = MAX_HAND;
	}

	// cycle through each player by setting gamestate to current player flag
	for (int player = 0; player < MAX_PLAYERS; player++) {
		G.whoseTurn = player;
		assert(numHandCards(&G) == MAX_HAND);
	}

	printf("\n----------SUCCESS: Testing complete numHandCards()----------\n\n");

	return 0;
}