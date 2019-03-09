#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NUMCARDS 10

/* Tests Smithy card */
//Description: Adds 3 cards to a players hand from their deck.

void assertCheck(int left, int right, int * numError, char * validMessage, char * errMessage) {
	if (left != right) {
		printf("ERROR: %s\n", errMessage);
		*numError += 1;
	}
	else {
		printf("PASS: %s\n", validMessage);
	}
}

int main() {
	// Initialize game state

	int seed = 100;
	int k[NUMCARDS] = { adventurer, council_room, feast, gardens, mine, remodel,
					smithy, village, baron, great_hall };
	struct gameState pre, G;
	int * bonus = NULL;
	int currentPlayer;
	int handPos;
	int numError = 0;

	printf("----------Testing Smithy Card----------\n");

	// -----------------Test Case 1-----------------------//
	assert(initializeGame(MAX_PLAYERS, k, seed, &G) == 0);		// Initialize game
	currentPlayer = whoseTurn(&G);
	handPos = G.handCount[currentPlayer];
	
	G.hand[currentPlayer][handPos] = smithy;
	G.handCount[currentPlayer]++;
	G.supplyCount[smithy]--;
	
	pre = G;									// save game state before cardEffect() calls smithy card
	assert(cardEffect(smithy, copper, silver, gold, &G, handPos, bonus) == 0);

	// Check 3 card added to hand from deck
	assertCheck(pre.handCount[currentPlayer] + 2, G.handCount[currentPlayer], &numError, "Hand count increased by 2.", "Incorrect hand count.");
	assertCheck(pre.deckCount[currentPlayer] - 3, G.deckCount[currentPlayer], &numError, "Deck count decreased by 3.", "Incorrect deck count.");
	
	// Check 3 cards should come from his own pile.
	// No state change should occur for other players.
	// No state change should occur to the victory card piles and kingdom card piles
	
	// Reverse changes from smithy card
	memcpy(G.hand[currentPlayer], pre.hand[currentPlayer], sizeof(int) * MAX_HAND);
	G.handCount[currentPlayer] = pre.handCount[currentPlayer];
	memcpy(G.deck[currentPlayer], pre.deck[currentPlayer], sizeof(int) * MAX_DECK);
	G.deckCount[currentPlayer] = pre.deckCount[currentPlayer];
	
	// Reverse changes from discardCard()
	memcpy(G.playedCards, pre.playedCards, sizeof(int) * MAX_DECK);
	G.playedCardCount = pre.playedCardCount;
	memcpy(G.discard[currentPlayer], pre.discard[currentPlayer], sizeof(int) * MAX_DECK);
	G.discardCount[currentPlayer] = pre.discardCount[currentPlayer];
	
	// Check if changes made to other players
	assertCheck(memcmp(&pre, &G, sizeof(struct gameState)), 0, &numError, "No modifications to other gameState variables", "Other gameState variables were modified");

	// Print test results
	if (numError > 0) {
		printf("\n!!!!!----------TEST FAILED: Testing Smithy----------!!!!!\n");
		printf("Number of errors: %d\n\n", numError);
	}
	else {
		printf("\n----------SUCCESS: Testing complete Smithy----------\n\n");
	}

	return 0;
}